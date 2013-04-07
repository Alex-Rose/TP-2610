/*
   * Fichier : Game.cpp
   * Auteurs : Alexandre Rose, Anael Maubant
   * Date : 4 avril 2012
   * Description : Classe game (partie)
*/

#include "game.h"

Game::Game()
{
   nbPlayers_ = 0;
   grid_ = 0;
   players_ = 0;
}

Game::~Game()
{
   delete grid_;
   delete []players_;
}

void Game::init(int nbPlayers, int gridSize)
{
   grid_ = new grille(gridSize);
   nbPlayers_ = nbPlayers;

   players_ = new Player*[nbPlayers];

   for(int i = 0; i < nbPlayers; i++)
      players_[i] = new Player();
    
}

void Game::startGame()
{
   for(int i = 0; i < nbPlayers_; i++)
      players_[i]->startPlayingAsync();
}