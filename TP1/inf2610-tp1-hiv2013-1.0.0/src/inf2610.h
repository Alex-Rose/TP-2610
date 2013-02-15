/*
 *   inf2610.h - Ce fichier contient les structures et constantes
 *               nécessaires à la réalisation du TP
 *
 *   Copyright 2013        Raphaël Beamonte <raphael.beamonte@polymtl.ca>
 *
 */

#ifndef INF2610_H
#define INF2610_H

// Décommenter pour passer en mode DEBUG
//#define DEBUG

// Décommenter pour activer les parties de code des BONUS
//#define BONUS1
//#define BONUS2

// Pour les tubes anonymes
#define R 0
#define W 1

// Le nombre minimum de secondes entre deux interruptions
#define MIN_INT_USEC	200
// Le nombre maximum de secondes entre deux interruptions
#define MAX_INT_USEC	700

/*
 * La structure Instruction utilisée pour les communications depuis
 * le processeur central vers les coprocesseurs, et depuis le processeur
 * central et les coprocesseurs vers le périphérique.
 */
typedef struct _Instruction {
	int coprocesseur;	// Coprocesseur qui a traité l'instruction
	char operation;		// Opération ('M', 'D', 'A', 'S', ou 'O')
	int valeur1;		// Première valeur de l'opération
	int valeur2;		// Seconde valeur de l'opération
	double resultat;	// Résultat obtenu
	_Instruction(){		// La structure est initialisée en mettant à 0 chaque variable
		coprocesseur = operation = valeur1 = valeur2 = resultat = 0;
	}
} Instruction;

#endif /* fin INF2610_H */
