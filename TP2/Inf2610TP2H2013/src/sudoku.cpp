/**************************************************************************************************
* file: sudoku.cpp
* description: 
* author(s): 
* date: 
* update: 
***************************************************************************************************/

////////////////////////////////////////// INCLUDE //////////////////////////////////////////////////
#include "entete.h"
#include <iostream>
#include <unistd.h>
#include <stdio.h>
#include <signal.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <semaphore.h>
#include <pthread.h>
#include <fstream>
#include <errno.h>
#include <queue>



////////////////////////////////////////// DEFINES //////////////////////////////////////////////////
#define EXIT_SUCCESS 0
#define EXIT_FAILIURE 1

///////////////////////////////////// GLOBAL VARIABLES //////////////////////////////////////////////


//Gestionnaire de signal
void sigHandler(int arg)
{

}

//condition pour la lecture
pthread_cond_t nonEmpty = PTHREAD_COND_INITIALIZER;
pthread_mutex_t player_mutex = PTHREAD_MUTEX_INITIALIZER;

/**************************************** MAIN *****************************************************/
void loadGrid(std::string path, int (&grid)[9][9])
{
    std::ifstream file;
    file.open(path.c_str());
    for (int i = 0; i < 9; i++)
    {
        for (int j = 0; j < 9; j++)
        {
            file >> grid[i][j];
        }
    }
    file.close();
}

void printGrid(int (&grid)[9][9])
{
    
    for (int i = 0; i < 9; i++)
    {
        for (int j = 0; j < 9; j++)
        {
            std::cout << grid[i][j];
        }
        std::cout<<std::endl;
    }
}

//Ex�cut�e par le thread principal (contr�leur)
int main (int argc, char **argv)
{
    if (argc != 6)
        return EXIT_FAILIURE;
	
	std::string pathGrilleVide = argv[1];
	std::string pathGrilleSolution = argv[2];
	std::string pathArrivee = argv[3];
	std::string tempsMax = argv[4];
	std::string pathResultat = argv[5];
    
    int grille[9][9];
    int solution[9][9];
    
    loadGrid(pathGrilleVide, grille);
//     printGrid(grille);
    
    loadGrid(pathGrilleSolution, solution);
// 	printGrid(solution);
    
  return EXIT_SUCCESS;
  
}
////////////////////////////////////////// THREAD FUNCTIONS //////////////////////////////////////////////////

/**************************************** thread_Alarm*****************************************************/
//Function ex�cut�e par le thread_Alarm
void* minuterie (void* arg){
//Dormir pendant la dur�e maximale requise

//Envoyer un sIGALRM au thread principal

}
/**************************************** thread_Accueil*****************************************************/
//Function ex�cut�e par le thread_Accueil
void* accueil(void* arg){


}
/**************************************** thread_Joueur*****************************************************/
//Function ex�cut�e par le thread_Joueur
void* jouer(void* arg){
  
  std::queue<MessageCJ*> queue_;
  MessageCJ* currentMessage;
  
  
  while(1){
    
    pthread_mutex_lock(&player_mutex);
    
    while(queue_.size()<=0){
      pthread_cond_wait(&nonEmpty,&player_mutex);
    }
    
    currentMessage=queue_.front();
    
    //travaiol sur le message a faire ici
    
    queue_.pop();
    
    pthread_mutex_unlock(&player_mutex);
    
    
    
    
    
    
  }
  
  

}


