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

//Un callback peut etre appele par le thread pool le moment venu
VOID CALLBACK MyWorkCallback(PTP_CALLBACK_INSTANCE Instance, PVOID Parameter, PTP_WORK Work)
{
   UNREFERENCED_PARAMETER(Instance);
   UNREFERENCED_PARAMETER(Work);

   std::cout<<"Work Callback"<<std::endl;
   startGame(Parameter);
}




int main()
{  

   std::cout<<"Combien de joueurs? ";
   int nb, dim; 
   std::cin>>nb;
   std::cout<<"Dimension de la grille? ";
   std::cin>>dim;


   Game* game = new Game();
   game->init(nb, dim);
   game->startGame();

   delete game;


   //Un essai en utilisant le thread pool de WinAPI
   //Ce bloc initialise un thread pool et tous les trucs que j'ai pas encore compris, puis 
   //Submit des work item (a la maniere de QueueUserWorkItem en C#/.NET 4.0)
   //Source http://msdn.microsoft.com/en-us/library/windows/desktop/ms686980(v=vs.85).aspx
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



   for (int i = 0; i < 10 ; i++)
   {
      PTP_WORK work = CreateThreadpoolWork(workcallback, (void*)i, &CallBackEnviron);

      SubmitThreadpoolWork(work);
   }*/


   //Procedure pour tester vraiment beaucoup de games en meme temps (define NO_OUT sinon sa ecrit 4ever)
   /*HANDLE threads[2000];
   DWORD id[2000];
   for (int i = 0; i < 200; i++)
   {
      threads[i] = CreateThread(NULL, 0, startGame, (void*)i, 0, &id[i]);
   }*/


   //Un arret avant de fermer le programme
   int x;
   std::cin>>x;
   return 0;
}