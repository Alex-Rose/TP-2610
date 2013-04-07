/*
   * Fichier : main.cpp
   * Auteurs : Alexandre Rose, Anael Maubant
   * Date : 4 avril 2012
   * Description : 
*/
#include <iostream>
#include <Windows.h>
#include <iomanip>

#include "grille.h"
#include "game.h"

#define println(a) std::cout<<a<<std::endl;

void printGrille(grille_t g)
{
   for(int i = 0; i < g.size_; i++)
   {
      for (int j = 0; j < g.size_; j++)
         std::cout<<std::setw(3)<<g[i][j];
      std::cout<<std::endl;
   }
}

void populateGrid(grille &g, int max)
{
   max++; //Dans mon livre a moi, les bornes sont incluses
   for(int i = 0; i < g.size_; i++)
   {
      for (int j = 0; j < g.size_; j++)
         g[i][j] = rand() % max ;
   }
}

int main()
{
   grille g(10);
   g[0][0] = 1;
   g[1][0] = 2;
   g[1][1] = 3;
   g[9][9] = 99;

  // printGrille(g);

   grille* a = new grille(10);
   populateGrid(*a, 10);
   printGrille(*a);
   
   delete a;
   
   Game* game = new Game();
   game->init(3, 10);
   game->startGame();
   
   delete game;

   int x;
   std::cin>>x;
   return 0;
}