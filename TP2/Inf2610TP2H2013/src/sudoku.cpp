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
//Ex�cut�e par le thread principal (contr�leur)
int main (int argc, char **argv)
{


		
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

}