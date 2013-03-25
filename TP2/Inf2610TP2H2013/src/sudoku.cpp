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



////////////////////////////////////////// DEFINES //////////////////////////////////////////////////


///////////////////////////////////// GLOBAL VARIABLES //////////////////////////////////////////////


//Gestionnaire de signal
void sigHandler(int arg)
{

}

/**************************************** MAIN *****************************************************/
//Exécutée par le thread principal (contrôleur)
int main (int argc, char **argv)
{


		
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