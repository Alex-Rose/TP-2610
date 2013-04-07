/*
   * Fichier : Game.cpp
   * Auteurs : Alexandre Rose, Anael Maubant
   * Date : 4 avril 2012
   * Description : Classe game (partie)
*/

#include "game.h"
#include <iostream>
#include <fstream>

Game::Game()
{
   nbPlayers_ = 0;
   grid_ = 0;
   mirroir_ = 0;
   players_ = 0;
   sem_ = 0;
   remaining_ = 0;
   gridMutex_ = 0;
   events_ = 0;
   authorizePlay_ = 0;
}

Game::~Game()
{
   delete grid_;
   delete mirroir_;
   delete []players_;
   delete []sem_;
   delete []events_;
   delete []authorizePlay_;
}

void Game::init(int nbPlayers, int gridSize)
{
   grid_ = new grille(gridSize);
   mirroir_ = new grille(gridSize);
   remaining_ = gridSize * gridSize;
   nbPlayers_ = nbPlayers;
   sem_ = new HANDLE[nbPlayers];
   events_ = new HANDLE[nbPlayers];
   authorizePlay_ = new HANDLE[nbPlayers];
   gridMutex_ = CreateMutex(NULL, false, NULL);

   players_ = new Player*[nbPlayers];

   for(int i = 0; i < nbPlayers; i++)
   {
      sem_[i] = CreateSemaphore(NULL, 0, 1, NULL);
      events_[i] = CreateEvent(NULL, false, false, NULL);
      authorizePlay_[i] = CreateMutex(NULL, false, NULL);
      players_[i] = new Player(events_[i], mirroir_, gridMutex_, authorizePlay_[i]);
   }

    
}

void Game::startGame()
{
   //Demarrer les threads
   for(int i = 0; i < nbPlayers_; i++)
      players_[i]->startPlayingAsync();

   populateGrid(10);
   //Condition pour continuer a jouer (genre s'il reste des cases cachees)
   while(remaining_ > 0)
      loop();

   system("cls");
   printGrille(*mirroir_);

   //Afficher les resultats
   for (int i = 0; i < nbPlayers_; i++)
   {
      std::cout<<"Le joueur "<<i<<" a obtenu "<<players_[i]->score<<" points."<<std::endl;
   }


   //Liberer les joueurs, leur dire de rentrer au vestiaire prendre une douche
   for(int i = 0; i < nbPlayers_; i++)
   {
      //Dabord laisser une chance au thread de se terminer normalement
      SetEvent(events_[i]);
   }

   Sleep(1000); //Donner un delais max pour que les threads se terminent
   for(int i = 0; i < nbPlayers_; i++)
   {
      DWORD result = WaitForSingleObject( players_[i]->getThread(), 0);
      // Si le thread est encore actif, le tuer de maniere violente.
      if (result != WAIT_OBJECT_0) 
         players_[i]->stopPlaying();
   }

}

// Boucle de jeu
void Game::loop()
{
   system("cls");
   printGrille(*mirroir_);
   std::cout<<std::endl;
   for(int i = 0; i < nbPlayers_; i++)
   {
      std::cout<<"Joueur "<<i<<" : "<<players_[i]->score<<" point(s)"<<std::endl;
   }
   //Je donne le tour a chaque joueur a tour de role
   for(int i = 0; i < nbPlayers_; i++)
   {
      if (remaining_ <= 0)
         return;

      
      // Pour activer un joueur, lui donner la semaphore
      //ReleaseSemaphore(sem_[i], 1, NULL);
      ReleaseMutex(authorizePlay_[i]);
      SetEvent(events_[i]);
      
      
      //Attend 2 secondes pour la reponse
      //Sleep(2000);
      //DWORD res = WaitForSingleObject(sem_[i], 2000); //Race condition si on met le sleep dans le wait?
      HANDLE h[2];
      //h[0] = sem_[i];
      h[0] = events_[i];
      h[1] = gridMutex_;
      //DWORD res = WaitForMultipleObjects(2, h, true, 2000); //Race condition si on met le sleep dans le wait?
      DWORD res = WaitForSingleObject(events_[i], 2000); //Race condition si on met le sleep dans le wait?
      //DWORD res = SignalObjectAndWait(events_[i], gridMutex_, 2000, FALSE); //Race condition si on met le sleep dans le wait?
      
      std::pair<int, int> c;
      switch(res)
      {
      case WAIT_OBJECT_0:
         WaitForSingleObject(gridMutex_, INFINITE); 
         c = players_[i]->choice;
         if (c.first == -1 || c.second == -1)
            continue;
#if DEBUG
         std::cout<<"Game accepts "<<c.first<<","<<c.second<<std::endl;
#endif
         (*mirroir_)[c.first][c.second] =  (*grid_)[c.first][c.second];
         mirroir_->remaining_.remove(c);
         players_[i]->score += (*grid_)[c.first][c.second];
         players_[i]->choice = std::pair<int, int>(-1, -1);//Pour etre sur de pas reutiliser
         remaining_--;
         ReleaseMutex(gridMutex_);
         break;
      case WAIT_TIMEOUT:
         WaitForSingleObject(authorizePlay_[i], INFINITE);
         break;
      }
      
   }
}

void Game::populateGrid(int max)
{
   srand((unsigned int)time(NULL));
   max++; //Dans mon livre a moi, les bornes sont incluses
   for(int i = 0; i < grid_->size_; i++)
   {
      for (int j = 0; j < grid_->size_; j++)
         (*grid_)[i][j] = rand() % max ;
   }
}

void Game::printGrille(grille g)
{
   for(int i = 0; i < g.size_; i++)
   {
      for (int j = 0; j < g.size_; j++)
      {
         if (g[i][j] == -1)
            std::cout<<std::setw(3)<<"X";
         else
            std::cout<<std::setw(3)<<g[i][j];
      }
      std::cout<<std::endl;
   }
}