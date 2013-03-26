/**************************************************************************************************
* file: entete.h
* description: 
* author(s): aurel RANDOLPH (aurel.randolph@polymtl.ca)
* date: 
* update: 
***************************************************************************************************/

#ifndef entete_H
#define entete_H

#include <list>
#include <utility>
#include <pthread.h>
#include <string>

/****************************************************************************************************
 * 
 * La structure de message utilisée pour les communications du contrôleur vers  les joueurs
 * Message destiné à la file File1
 * 
 */
typedef struct _MessageCJ {
	int ligne;	    // Numéro de ligne de la case
	int colonne;    // Numéro de colonne de la case
	std::list<int> choiceList; // Liste des choix possibles
	_MessageCJ(){  // Initialisation 
		ligne= colonne= -1;
	}
} MessageCJ;

/****************************************************************************************************
 * 
 * La structure de message utilisée pour les communications des joueurs vers le contrôleur 
 * Message destiné à la file File2
 * 
 */
typedef struct _MessageJC {
	int tid;	  // identifiant du joueur
	int ligne;	  // Numéro de ligne de la case
	int colonne;  // Numéro de colonne de la case
	int choice;   // Choix du joueur
	_MessageJC(){ // Initialisation 
		tid=ligne= colonne=choice= -1;
	}
} MessageJC;

typedef struct _Joueur {
    int tid;
    int score;
    int nbErreur;
    std::string etat;
    pthread_t thread;
    _Joueur(){
        etat = "Attente";
        tid= -1;
        score=nbErreur=thread = 0;
    }
} Joueur;
#endif /* fin entete_H*/