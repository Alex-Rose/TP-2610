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

void Player::startPlayingAsync()
{
   thread_ = CreateThread(NULL, 0, play_wrapper, (void*)this, 0, &threadId_);
}

DWORD WINAPI Player::play_wrapper(void* o)
{
   static_cast<Player*>(o)->play(NULL);
   return 0;
}

DWORD WINAPI Player::play(LPVOID) 
{
   //Faudrait lui mettre une condition pour arreter un jour
   while(grid_->remaining_.size() > 0)
   {
      WaitForSingleObject(event_, INFINITE);
      if (grid_->remaining_.size() <= 0)
         break;
#if DEBUG    
      std::cout<<"Play from thread "<<threadId_<<std::endl;
#endif
      Sleep(rand() % 3000);
      DWORD res = WaitForSingleObject(canPlay_, 0);
      if (res == WAIT_TIMEOUT)
         continue;

      res = WaitForSingleObject(gridMutex_, 1);
      if (res == WAIT_TIMEOUT)
         continue;

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

      ReleaseMutex(gridMutex_);
      ReleaseMutex(canPlay_);
#if DEBUG
      f<<"Thread "<<threadId_<<" releases the mutex"<<std::endl;
      f.close();
#endif
      SetEvent(event_);

      //ReleaseSemaphore(sem_, 1, NULL);
   }
   ExitThread(0);
}

void Player::stopPlaying()
{
   
   //Violent way
   TerminateThread(thread_, 0);
}