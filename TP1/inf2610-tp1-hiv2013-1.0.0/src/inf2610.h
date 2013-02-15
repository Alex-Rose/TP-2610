/*
 *   inf2610.h - Ce fichier contient les structures et constantes
 *               n�cessaires � la r�alisation du TP
 *
 *   Copyright 2013        Rapha�l Beamonte <raphael.beamonte@polymtl.ca>
 *
 */

#ifndef INF2610_H
#define INF2610_H

// D�commenter pour passer en mode DEBUG
//#define DEBUG

// D�commenter pour activer les parties de code des BONUS
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
 * La structure Instruction utilis�e pour les communications depuis
 * le processeur central vers les coprocesseurs, et depuis le processeur
 * central et les coprocesseurs vers le p�riph�rique.
 */
typedef struct _Instruction {
	int coprocesseur;	// Coprocesseur qui a trait� l'instruction
	char operation;		// Op�ration ('M', 'D', 'A', 'S', ou 'O')
	int valeur1;		// Premi�re valeur de l'op�ration
	int valeur2;		// Seconde valeur de l'op�ration
	double resultat;	// R�sultat obtenu
	_Instruction(){		// La structure est initialis�e en mettant � 0 chaque variable
		coprocesseur = operation = valeur1 = valeur2 = resultat = 0;
	}
} Instruction;

#endif /* fin INF2610_H */
