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
void parseMessage(char* message, int len, std::queue<Instruction*> *queue, int p); 
bool executeOperation(Instruction*);

int main(int argc, char** argv) {

	// Ouverture du/des FIFOs utiles
	queue<Instruction*> *operations = new queue<Instruction*>();
		
	printf("coprocessur %d\n", getpid());
	
	char message[1000];
	int fd[2];
	fd[R] = atoi(argv[0]);
	fd[W] = atoi(argv[1]);
	int proc_nb = atoi(argv[2]);
	int nb;
	
	close(fd[W]);
	//sleep(10);


	while((nb = read(fd[R], message, 1000)) > 0)
	{
		parseMessage(message, nb, operations, proc_nb);	
		message[0] = '\0';
		nb = 0;
	}
	close(fd[R]);
	printf("poop\n");
	
	Instruction *it;
	while((it = operations->front()) != NULL)
	{
		printf("Oper: %d %c %d %d \t\t = %d\n",proc_nb,  it->operation, it->valeur1, it->valeur2, (int)it->resultat);
		operations->pop();
	}
	// Réception des instructions
	//	On traite l'opération (ou on sort de la boucle)
	//	Si c'était bien une opération, on met à jour l'instruction
	//	Si c'était bien une opération, on envoie l'instruction mise à jour au périphérique

	// On libère les ressources allouées
	while((it = operations->front()) != NULL)
	{
		delete it;
		operations->pop();
	}
	
	
	return 0;
}

void parseMessage(char* message, int len, queue<Instruction*> *queue, int proc_nb)
{
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
		ins->coprocesseur = proc_nb;
		ins->operation = op;
		ins->valeur1 = a;
		ins->valeur2 = b;
		if(executeOperation(ins))
			queue->push(ins);
		else
			delete ins;
			
		i++;
	}
}


bool executeOperation(Instruction* i)
{
	switch(i->operation)
	{
		case 'A':
			i->resultat = (double)(i->valeur1 + i->valeur2);
			break;
		case 'S':
			i->resultat = (double)(i->valeur1 - i->valeur2);
			break;
		case 'M':
			i->resultat = (double)(i->valeur1) * (double)(i->valeur2);
			break;
		case 'D':
			i->resultat = (double)(i->valeur1) / (double)(i->valeur2);
			break;
		case 'O':
			i->resultat = (double)(i->valeur1 % i->valeur2);
			break;
		default:
			return false;
	}
	return true;
}
