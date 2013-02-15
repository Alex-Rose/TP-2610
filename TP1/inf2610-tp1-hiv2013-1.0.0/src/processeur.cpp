/*
 *   processeur.cpp - Le processeur central
 *
 *   Binôme:
 *      - Prénom & Nom, Matricule
 *      - Prénom & Nom, Matricule
 *
 *   TP1 - Hiver 2013
 */

#include "inf2610.h"
#include <sys/stat.h>
#include <errno.h>
#include <string.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <libgen.h>
#include <unistd.h>
#include <fstream>
#include <iostream>
#include <vector>
#include <fcntl.h>

using namespace std;


// Exécution du processeur
int main(int argc, char** argv) {
	// On vérifie les arguments

	// On créé la fifo avec laquelle les autres processus pourront communiquer avec le processeur

	// On créé la fifo avec laquelle les coprocesseurs pourront communiquer avec le périphérique

	// Création des coprocesseurs

	// Création du périphérique

	// On entre dans le cycle
	//	Lecture et traitement d'une ligne en mémoire
	//	On lit le fifo s'il y a un message

	// On demande aux coprocesseurs et au périphérique d'entrée/sortie de se terminer

	// On attend qu'ils se terminent

	// On libère les ressources allouées

	return 0;
}



