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

/****************************************************************************************************
 * 
 * La structure de message utilis�e pour les communications du contr�leur vers  les joueurs
 * Message destin� � la file File1
 * 
 */
typedef struct _MessageCJ {
	int ligne;	    // Num�ro de ligne de la case
	int colonne;    // Num�ro de colonne de la case
	std::list<int> choiceList; // Liste des choix possibles
	_MessageCJ(){  // Initialisation 
		ligne= colonne= -1;
	}
} MessageCJ;

/****************************************************************************************************
 * 
 * La structure de message utilis�e pour les communications des joueurs vers le contr�leur 
 * Message destin� � la file File2
 * 
 */
typedef struct _MessageJC {
	int tid;	  // identifiant du joueur
	int ligne;	  // Num�ro de ligne de la case
	int colonne;  // Num�ro de colonne de la case
	int choice;   // Choix du joueur
	_MessageJC(){ // Initialisation 
		tid=ligne= colonne=choice= -1;
	}
} MessageJC;

typedef struct _Joueur {
    int tid;
    int score;
    pthread_t thread;
    _Joueur(){
        tid= -1;
        score=thread = 0;
    }
} Joueur;
#endif /* fin entete_H*/