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
#include <list>



////////////////////////////////////////// DEFINES //////////////////////////////////////////////////
#define EXIT_SUCCESS 0
#define EXIT_FAILIURE 1

void* jouer(void* arg);
void* accueil(void* arg);
void* minuterie (void* arg);

///////////////////////////////////// GLOBAL VARIABLES //////////////////////////////////////////////
std::queue<_MessageCJ*> file1();

//Gestionnaire de signal
void sigHandler(int arg)
{

}

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

//Exécutée par le thread principal (contrôleur)
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

    //creaation des thread joueur par defaut
    pthread_t joueurs[5];
    joueurs[0] = pthread_t();
    joueurs[1] = pthread_t();
    joueurs[2] = pthread_t();
    
    pthread_create(&joueurs[0], NULL, jouer, NULL);
    pthread_create(&joueurs[1], NULL, jouer, NULL);
    pthread_create(&joueurs[2], NULL, jouer, NULL);
    
    // Creation des deux autres thread
    pthread_t accueil_t;
    pthread_t alarm_t;
    
    pthread_create(&accueil_t, NULL, accueil, NULL);
    pthread_create(&alarm_t, NULL, minuterie, NULL);
    
  return EXIT_SUCCESS;
  
}
////////////////////////////////////////// THREAD FUNCTIONS //////////////////////////////////////////////////

/**************************************** thread_Alarm*****************************************************/
//Function exécutée par le thread_Alarm
void* minuterie (void* arg){
//Dormir pendant la durée maximale requise

//Envoyer un sIGALRM au thread principal

}
/**************************************** thread_Accueil*****************************************************/
//Function exécutée par le thread_Accueil
void* accueil(void* arg){


}
/**************************************** thread_Joueur*****************************************************/
//Function exécutée par le thread_Joueur
void* jouer(void* arg){
    
}
