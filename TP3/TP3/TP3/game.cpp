/*
   * Fichier : Game.cpp
   * Auteurs : Alexandre Rose, Anael Maubant
   * Date : 4 avril 2012
   * Description : Classe game (partie)
*/

#include "game.h"
#include <iostream>

Game::Game()
{
   nbPlayers_ = 0;
   grid_ = 0;
   players_ = 0;
   sem_ = 0;
   remaining_ = 0;
}

Game::~Game()
{
   delete grid_;
   delete []players_;
   delete []sem_;
}

void Game::init(int nbPlayers, int gridSize)
{
   grid_ = new grille(gridSize);
   remaining_ = gridSize * gridSize;
   nbPlayers_ = nbPlayers;
   sem_ = new HANDLE[nbPlayers];

   players_ = new Player*[nbPlayers];

   for(int i = 0; i < nbPlayers; i++)
   {
      sem_[i] = CreateSemaphore(NULL, 0, 1, NULL);
      players_[i] = new Player(sem_[i]);
   }

    
}

void Game::startGame()
{
   for(int i = 0; i < nbPlayers_; i++)
      players_[i]->startPlayingAsync();

   //Condition pour continuer a jouer (genre s'il reste des cases cachees)
   while(remaining_ > 0)
      loop();

   //Liberer les joueurs, leur dire de rentrer au vestiaire prendre une douche

   //Afficher les resultats
}

// Boucle de jeu
void Game::loop()
{
   //Je donne le tour a chaque joueur a tours de role
   for(int i = 0; i < nbPlayers_; i++)
   {
      if (remaining_ <= 0)
         return;

      std::cout<<"Joueur "<<i<<std::endl;
      // Pour activer un joueur, lui donner la semaphore
      ReleaseSemaphore(sem_[i], 1, NULL);
      
      
      //Attend 2 secondes pour la reponse
      //Sleep(2000);
      DWORD res = WaitForSingleObject(sem_[i], 2000); //Race condition si on met le sleep dans le wait?
      switch(res)
      {
      case WAIT_OBJECT_0:
         remaining_--;
         break;
      case WAIT_TIMEOUT:
         break;
      }
   }
}