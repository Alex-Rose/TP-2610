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
DWORD WINAPI startGame(LPVOID arg)
{
   //std::cout<<"Game "<<(int)arg<<" started"<<std::endl;

   Game* game = new Game();
   game->init(5, 20);
   game->startGame();
   
   std::ofstream f("results.txt", std::ios::app);
   f<<game->getResults().c_str()<<std::endl;
   game->writeResults(f);
   f.close();
   delete game;

   std::cout<<"Game "<<(int)arg<<" over"<<std::endl;
   return 0;
}

VOID CALLBACK MyWorkCallback(
    PTP_CALLBACK_INSTANCE Instance,
    PVOID                 Parameter,
    PTP_WORK              Work
    )
{
   UNREFERENCED_PARAMETER(Instance);
   UNREFERENCED_PARAMETER(Work);

   std::cout<<"Work Callback"<<std::endl;
   startGame(Parameter);
}




int main()
{  
   //Procedure normale pour une game
   /*Game* game = new Game();
   game->init(5, 10);
   game->startGame();
   
   delete game;*/


   /*TP_CALLBACK_ENVIRON CallBackEnviron;
   InitializeThreadpoolEnvironment(&CallBackEnviron);
   PTP_POOL pool = CreateThreadpool(NULL);

   PTP_CLEANUP_GROUP cleanupgroup = CreateThreadpoolCleanupGroup();

   SetThreadpoolCallbackPool(&CallBackEnviron, pool);

   SetThreadpoolCallbackCleanupGroup(&CallBackEnviron,
                                      cleanupgroup,
                                      NULL);


   PTP_WORK_CALLBACK workcallback = MyWorkCallback;

   PTP_WORK work = CreateThreadpoolWork(workcallback, NULL, &CallBackEnviron);



   for (int i = 0; i < 2 ; i++)
   {
      PTP_WORK work = CreateThreadpoolWork(workcallback, (void*)i, &CallBackEnviron);

      SubmitThreadpoolWork(work);
   }*/


   //Procedure pour tester vraiment beaucoup de games en meme temps
   HANDLE threads[2000];
   DWORD id[2000];
   for (int i = 0; i < 200; i++)
   {
      threads[i] = CreateThread(NULL, 0, startGame, (void*)i, 0, &id[i]);
   }


   int x;
   std::cin>>x;
   return 0;
}