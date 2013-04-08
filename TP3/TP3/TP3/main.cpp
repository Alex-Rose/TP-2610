/*
   * Fichier : main.cpp
   * Auteurs : Alexandre Rose, Anael Maubant
   * Date : 4 avril 2012
   * Description : 
*/
#include <iostream>
#include <fstream>
#include <Windows.h>
#include <iomanip>

#include "grille.h"
#include "game.h"

#define println(a) std::cout<<a<<std::endl;

//Imprimer la grille
void printGrille(grille_t g)
{
   for(int i = 0; i < g.size_; i++)
   {
      for (int j = 0; j < g.size_; j++)
         std::cout<<std::setw(3)<<g[i][j];
      std::cout<<std::endl;
   }
}

//Attribuer des valeurs pseudo random
void populateGrid(grille &g, int max)
{
   max++; //Dans mon livre a moi, les bornes sont incluses
   for(int i = 0; i < g.size_; i++)
   {
      for (int j = 0; j < g.size_; j++)
         g[i][j] = rand() % max ;
   }
}

//Methode de test
DWORD WINAPI startGame(LPVOID)
{
   Game* game = new Game();
   game->init(3, 10);
   game->startGame();
   
   std::ofstream f("results.txt", std::ios::app);
   f<<game->getResults().c_str()<<std::endl;
   f.close();
   delete game;
   return 0;
}

int main()
{  
   //Procedure normale pour une game
  /* Game* game = new Game();
   game->init(3, 10);
   game->startGame();
   
   delete game;*/

   //Procedure pour tester vraiment beaucoup de games en meme temps
   HANDLE threads[2000];
   DWORD id[2000];
   for (int i = 0; i < 200; i++)
   {
      threads[i] = CreateThread(NULL, 0, startGame, NULL, 0, &id[i]);
   }


   int x;
   std::cin>>x;
   return 0;
}