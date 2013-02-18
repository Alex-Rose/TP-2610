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
#include <cstdio>

using namespace std;

/**
 * La méthode journal permet de générer, à partir d'une instruction
 * donnée, le message de journalisation correspondant.
 */
const char * journal(Instruction instruction) {
	
	char *message = new char[512];
	switch(instruction.operation)
	{
		case 'Z':
			sprintf(message, "[stat]Le processeur %d a traite une moyenne %f d'operations par cycle.\n"
			, instruction.coprocesseur, instruction.resultat);
			break;
		case 'I':
			sprintf(message, "Le coprocesseur %d a recu une interruption.\n", instruction.coprocesseur);
			break;
		case 'A':
		case 'S':
		case 'M':
		case 'D':
		case 'O':
		default:
			sprintf(message, "Le coprocesseur %d a traite l'operation %c de %d de %d et a obtenu %f comme resultat\n", 
			instruction.coprocesseur, instruction.operation, instruction.valeur1, instruction.valeur2,
			instruction.resultat);
			break;
	}
	const char *msg = message;
	delete message;
	return msg;
	
	
}

bool executeOperation(Instruction* i);

// Méthode principale du périphérique
int main(int argc, char** argv) {
	printf("Peripherique ouvert : %d\n", getpid());
	// Ouverture du/des FIFO
	int fd = open(CO_LOG, O_RDONLY);
	
	// Ouverture du fichier journal
	FILE *log = fopen("./journaldemontreal", "w");
	//printf("sent log %d path %s\n", argc, argv[2]);
	// On lit les instructions tant qu'on en reçoit
	//	Si c'est bien une instruction/opération/statistique, on passe par la fonction
	//	journal pour retourner le texte à écrire dans le journal
	//	Sinon, on quitte

	Instruction ins;

	int n;
	while ((n = read(fd, &ins, sizeof(Instruction))) > 0)
	{
		const char *message = journal(ins);
		fputs(message, log);
		//delete message;
	}
	printf("Peripherique ferme\n");

	// On libère les ressources allouées
	fclose(log);
	close(fd);
	return 0;
}


