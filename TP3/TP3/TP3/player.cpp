/*
   * Fichier : player.cpp
   * Auteurs : Alexandre Rose, Anael Maubant
   * Date : 4 avril 2012
   * Description : Classe game (partie)
*/
#include <iostream>
#include <fstream>
#include "player.h"


Player::Player()
{
   thread_ = 0;
   threadId_ = 0;
   event_ = 0;
   grid_ = 0;
   score = 0;
   canPlay_ = 0;
}

Player::Player(HANDLE eventH, grille* mirroir, HANDLE gridMutex, HANDLE canPlay)
{
   thread_ = 0;
   threadId_ = 0;
   event_ = eventH;
   grid_ = mirroir;
   score = 0;
   gridMutex_ = gridMutex;
   canPlay_ = canPlay;
}

Player::~Player()
{
   CloseHandle(thread_);
}

//Cree un thread et debute la loop de jeu asynchrone
void Player::startPlayingAsync()
{
   thread_ = CreateThread(NULL, 0, play_wrapper, (void*)this, 0, &threadId_);
}

//Wrapper pour partir une methode membre d'une classe dans un appel CreateThread
DWORD WINAPI Player::play_wrapper(void* o)
{
   static_cast<Player*>(o)->play(NULL);
}

//Boucle de jeu
DWORD WINAPI Player::play(LPVOID) 
{
   //Faudrait lui mettre une condition pour arreter un jour
   while(grid_->remaining_.size() > 0)
   {
      //Attend de recevoir l'event
      WaitForSingleObject(event_, INFINITE);
      //Une fois l'event recu, s'assurer qu'il reste des cases
      if (grid_->remaining_.size() <= 0)
         break;
#if DEBUG    
      std::cout<<"Play from thread "<<threadId_<<std::endl;
#endif
      //Reflexion
      Sleep(rand() % 3000);
      //Apres reflexion savoir si on peut jouer ou s'il est trop tard
      DWORD res = WaitForSingleObject(canPlay_, 0);
      if (res == WAIT_TIMEOUT)
         continue;

      //Prend le mutex de la grille. Ne pas attendre, car si on attend (cas qui ne devrait
      //pas arriver) on risque de causer des etats indefinis
      res = WaitForSingleObject(gridMutex_, 1);
      if (res == WAIT_TIMEOUT)
         continue;

      //fait un choix de case
      std::list<std::pair<int, int>>::iterator it = grid_->remaining_.begin();
      int random = rand() % grid_->remaining_.size();

      for (int i = 0; i < random; i++)
         it++;
      
      #if DEBUG
      std::ofstream f("out.txt", std::ios::app);
      f<<"Thread "<<threadId_<<" takes the mutex"<<" and asks for "<<(*it).first<<","<<(*it).second<<std::endl;
      std::cout<<"Thread "<<threadId_<<" takes the mutex"<<" and asks for "<<(*it).first<<","<<(*it).second<<std::endl;
      #endif
      choice = *it;

      //Release les mutex
      ReleaseMutex(gridMutex_);
      ReleaseMutex(canPlay_);
#if DEBUG
      f<<"Thread "<<threadId_<<" releases the mutex"<<std::endl;
      f.close();
#endif
      //Alerte le controleur qu'on a fini
      SetEvent(event_);

      //ReleaseSemaphore(sem_, 1, NULL);
   }
   //Termine le thread
   ExitThread(0);
}

void Player::stopPlaying()
{
   //Violent way if needed
   TerminateThread(thread_, 0);
}