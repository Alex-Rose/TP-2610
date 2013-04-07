/*
   * Fichier : game.h
   * Auteurs : Alexandre Rose, Anael Maubant
   * Date : 4 avril 2012
   * Description : Classe game (partie)
*/
#ifndef _GAME_
#define _GAME_
#include "grille.h"
#include "player.h"
#include <Windows.h>

class Game
{
public:
   Game();

   ~Game();

   void init(int nbPlayers, int gridSize);

   void startGame();

   void loop();

private:

   int nbPlayers_;

   int remaining_;

   grille* grid_;

   Player** players_;

   HANDLE* sem_;
};

#endif