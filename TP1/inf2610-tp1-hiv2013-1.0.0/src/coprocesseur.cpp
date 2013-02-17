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
#include <queue>

using namespace std;
void parseMessage(char* message, int len, std::queue<Instruction*> *queue); 

int main(int argc, char** argv) {

	// Ouverture du/des FIFOs utiles
	queue<Instruction*> *operations = new queue<Instruction*>();
		
	printf("coprocessur %d\n", getpid());
	

	char message[1000];
	int fd[2];
	fd[R] = atoi(argv[0]);
	fd[W] = atoi(argv[1]);
	int nb;
	
	close(fd[W]);
	while((nb = read(fd[R], message, 1000)) > 0)
	{
		parseMessage(message, nb, operations);
		printf("Message (%d) : %s\n", nb, message); 
		message[0] = '\0';
		nb = 0;
	}
	close(fd[R]);
	printf("poop\n");
	
	
	// Réception des instructions
	//	On traite l'opération (ou on sort de la boucle)
	//	Si c'était bien une opération, on met à jour l'instruction
	//	Si c'était bien une opération, on envoie l'instruction mise à jour au périphérique

	// On libère les ressources allouées

	return 0;
}

void parseMessage(char* message, int len, queue<Instruction*> *queue)
{
	printf(message);
	int i = 0;
	while(i < len && message[i] != '\0')
	{
		char op = message[i];
		i += 2;
		char c_a[10], c_b[10];
		int j = 0;
		while(message[i] != ' ')
		{
			c_a[j] = message[i];
			j++;
			i++;
		}
		j = 0;
		while(message[i] != '\0')
		{
			c_b[j] = message[i];
			j++;
			i++;
		}
		int a = atoi(c_a);
		int b = atoi(c_b);
		
		Instruction* ins = new Instruction();
		ins->coprocesseur = 69;
		ins->operation = op;
		ins->valeur1 = a;
		ins->valeur2 = b;
		queue->push(ins);
		i++;
	}
}
