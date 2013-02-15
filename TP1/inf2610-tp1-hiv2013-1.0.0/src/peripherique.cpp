/*
 *   peripherique.cpp - Le périphérique d'entrée/sortie
 *
 *   Binôme:
 *        - Prénom & Nom, Matricule
 *        - Prénom & Nom, Matricule
 *
 *   TP1 - Hiver 2013
 */

#include "inf2610.h"
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <fstream>
#include <sstream>

using namespace std;

/**
 * La méthode journal permet de générer, à partir d'une instruction
 * donnée, le message de journalisation correspondant.
 */
const char * journal(Instruction instruction) {
	return "Eh, I have something to write here but.. Eh !";
}

// Méthode principale du périphérique
int main(int argc, char** argv) {

	// Ouverture du/des FIFO

	// Ouverture du fichier journal

	// On lit les instructions tant qu'on en reçoit
	//	Si c'est bien une instruction/opération/statistique, on passe par la fonction
	//	journal pour retourner le texte à écrire dans le journal
	//	Sinon, on quitte

	// On libère les ressources allouées

	return 0;
}


