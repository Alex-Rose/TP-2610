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
bool executeOperation(Instruction*);
void signalHandler(int signum);
void sendISignal();
void informLog(Instruction);

int counter = 0;
int proc_nb = 0; 
int fd_co_proc = 0;
int fd_co_log = 0;
int main(int argc, char** argv) {

	signal(SIGALRM, &signalHandler);
	int sec = rand() % (MAX_INT_USEC - MIN_INT_USEC) + MIN_INT_USEC;
	ualarm(sec * 1000, 0);
	
	// Ouverture du/des FIFOs utiles
	fd_co_proc = open(PROC_COPROC, O_WRONLY | O_APPEND);
	fd_co_log = open(CO_LOG, O_WRONLY | O_APPEND);
		
	printf("coprocessur %d\n", getpid());
	
	char message[1000];
	int fd[2];
	fd[R] = atoi(argv[0]);
	fd[W] = atoi(argv[1]);
	proc_nb = atoi(argv[2]) + 1;
	int nb;
	
	close(fd[W]);

	// Réception des instructions
	//	On traite l'opération (ou on sort de la boucle)
	//	Si c'était bien une opération, on met à jour l'instruction
	//	Si c'était bien une opération, on envoie l'instruction mise à jour au périphérique

	Instruction *ins = new Instruction();
	while((nb = read(fd[R], ins, sizeof(Instruction))) > 0)
	{
		if(ins->operation == '0')
		{
			delete ins;
			ins = 0;
			break;
		}
		executeOperation(ins);
		informLog(*ins);
		//printf("Instruction %d %c %d\n", ins->valeur1, ins->operation, ins->valeur2); 
		//delete ins;
		counter++;
	}
	close(fd[R]);
	printf("poop\n");
	

	// On libère les ressources allouées
	close(fd_co_proc);
	close(fd_co_log);
	
	return 0;
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


void signalHandler(int signum)
{
	if ( fd_co_proc != -1 ) 
	{
		char mes[30];
		sprintf(mes, "%d %d\n", proc_nb, counter);//\n pour le besoin de la cause
		int len = -1;
		while(len < 30 && mes[++len] != '\0');
		write( fd_co_proc , mes , len + 1);
		counter = 0;
		sendISignal();
	}
	else
		printf("Ne peut pas ecrire sur le fifo\n");

	
	int sec = rand() % (MAX_INT_USEC - MIN_INT_USEC) + MIN_INT_USEC;
	ualarm(sec * 1000, 0);
}

void sendISignal()
{
	if ( fd_co_log != -1 ) 
	{
		Instruction ins;
		ins.coprocesseur = proc_nb;
		ins.operation = 'I';
		ins.valeur1 = 0;
		ins.valeur2 = 0;
		write( fd_co_log , &ins , sizeof(ins));
	}
	else
		printf("Ne peut pas ecrire sur le fifo\n");
}

void informLog(Instruction ins)
{
	if ( fd_co_log != -1 ) 
	{	
		write( fd_co_log , &ins , sizeof(Instruction));
		counter = 0;
	}
	else
		printf("Ne peut pas ecrire sur le fifo\n");
}
