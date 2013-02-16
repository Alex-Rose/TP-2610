/*
 *   coprocesseur.cpp - Les coprocesseurs
 *
 *   Binôme:
 *      - Prénom & Nom, Matricule
 *      - Prénom & Nom, Matricule
 *
 *   TP1 - Hiver 2013
 */

#include "inf2610.h"
#include <unistd.h>
#include <signal.h>
#include <ctime>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>

using namespace std;

int main(int argc, char** argv) {

	// Ouverture du/des FIFOs utiles	
	printf("coprocessur %d\n", getpid());
	

	char message[100];
	int fd[2];
	fd[R] = atoi(argv[0]);
	fd[W] = atoi(argv[1]);
	int nb;
	
	while((nb = read(fd[R], message, 100)) > 0)
	{
	close(fd[W]);
	printf("Message (%d) : %s\n", nb, message); 
	
}
	close(fd[R]);
	
	
	
	// Réception des instructions
	//	On traite l'opération (ou on sort de la boucle)
	//	Si c'était bien une opération, on met à jour l'instruction
	//	Si c'était bien une opération, on envoie l'instruction mise à jour au périphérique

	// On libère les ressources allouées

	return 0;
}


