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

int parseLine(char* message, int* len);
void parseInstruction(char* message, int len, Instruction *ins);
Instruction* countStats(char* s, int* op, int* in);


// Exécution du processeur
int main(int argc, char** argv) {
	
	int nbCo = 3;
	int co_nb_oper[nbCo];
	int co_nb_interrupt[nbCo];
	
	// On vérifie les arguments

	// On créé la fifo avec laquelle les autres processus pourront communiquer avec le processeur	
	if(mkfifo(PROC_COPROC, 0666) != 0)
	{
		printf("Impossible de créer le tube");
		unlink(PROC_COPROC);
		//if(mkfifo(PROC_COPROC, 0666) != 0)
			exit (1);
	}
	
	int fd_co = open(PROC_COPROC, O_RDONLY | O_NDELAY);
	
	// On créé la fifo avec laquelle les coprocesseurs pourront communiquer avec le périphérique
	if(mkfifo(CO_LOG, 0666) != 0)
	{
		printf("Impossible de créer le tube");
		unlink(CO_LOG);
		//if(mkfifo(CO_LOG, 0666) != 0)
			exit (1);
	}
	
	// Création des coprocesseurs
	int p_pid = getpid();
	printf("proc pid : %d \n", p_pid);
	
	int fd[nbCo][2];
	int* co_pid = new int[nbCo];
	for (int i = 0; i < nbCo; i++)
	{
		pipe(fd[i]);
		//close(fd[i][R]);
		co_pid[i] = fork();
		if (co_pid[i] != 0)
			continue;
		char r[33];
		char w[33];
		char p[10];
		sprintf(r, "%d", (fd[i][R]));
		sprintf(w, "%d", (fd[i][W]));
		sprintf(p, "%d", i);
		execl("/home/alexrose/Documents/TP-2610/TP1/inf2610-tp1-hiv2013-1.0.0/src/coprocesseur", r, w, p);
	}
	
	
	if (getpid() == p_pid)
		printf("child list is : %d, %d, %d\n", co_pid[0], co_pid[1], co_pid[2]);
		
	
	// Création du périphérique
	int per_pid = fork();
	if (per_pid == 0)
	{
		execl("src/peripherique", NULL);
	}
	
	// On entre dans le cycle
	//	Lecture et traitement d'une ligne en mémoire
	//	On lit le fifo s'il y a un message
	fstream file("/home/alexrose/Documents/TP-2610/TP1/inf2610-tp1-hiv2013-1.0.0/memoire/memoire.txt", ios::in);
	
	int proc;
	
//	sleep(10);
	for(int i = 0; i < nbCo; i++)
		close(fd[i][R]);
		
	char temp[64];
	while(!file.eof())
	{
		char s[50];
		file.getline(s, 50);
		
		int len = -1;
		while(s[++len] != '\0' && len < 50);
		
		Instruction ins;
		
		parseInstruction(s, len, &ins);
		
		write(fd[ins.coprocesseur - 1][W], &ins, sizeof(Instruction));
		
		char c;
		int n;
		int k = 0;
		//printf("\n");
		pair<int, int> pa;
		while ((n = read(fd_co, &pa, sizeof(pair<int, int>))) > 0)
		//while ((n = read(fd_co, &c, 1)) > 0)
		{
			//if(c == '\0')
			//{
			//	temp[k] = c;
			//	Instruction* stat = countStats(temp, co_nb_oper, co_nb_interrupt);
			//	if (stat != NULL)
			//	{
			//		write(fd[stat->coprocesseur - 1][W], stat, sizeof(Instruction));
			//		delete stat;
			//	}
			//	temp[0] = '\0';
			//	k = 0;
			//}
			//else
			//{
			//	temp[k] = c;
			//	k++;
			//}
			Instruction stat;
			stat.coprocesseur = pa.first;
			stat.resultat = pa.second;
			stat.operation = 'Z';
			write(fd[stat.coprocesseur - 1][W], &stat, sizeof(Instruction));
		}
		sleep(1);
	}
		
	
	// On demande aux coprocesseurs et au périphérique d'entrée/sortie de se terminer
	for(int i = 0; i < nbCo; i++)
	{
		Instruction ins;
		ins.coprocesseur = 0;
		ins.operation = '0';
		ins.valeur1 = 0;
		ins.valeur2 = 0;
		write(fd[i][W], &ins, sizeof(Instruction));
	}
	// On attend qu'ils se terminent

	// On libère les ressources allouées
	
	close(fd_co);
	
	file.close();
	for(int i = 0; i < nbCo; i++)
		close(fd[i][W]);
	unlink(PROC_COPROC);
	unlink(CO_LOG);
	printf("GAME OVER!\n");
	return 0;
}

void parseInstruction(char* message, int len, Instruction *ins)
{
	int i = 0;
	char c_proc[10];
	while(message[i] != ' ')
	{
		c_proc[i] = message[i];
		i++;
	}
	i++;
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
	
	ins->coprocesseur = atoi(c_proc);
	ins->operation = op;
	ins->valeur1 = a;
	ins->valeur2 = b;		
}

int parseLine(char* message, int* len)
{
	int i = 0;
	char c_proc[10];
	while(message[i] != ' ')
	{
		c_proc[i] = message[i];
		i++;
	}
	i++;
	int j;
	for(j = 0; i < *len; j++)
	{
		message[j] = message[i++];
	}
	(*len) -= (i - j);
	message[j] = '\0';
	return atoi(c_proc);
}

Instruction* countStats(char* s, int* nb_op, int* nb_interupt)
{
	int a, b, i = 0;
	char c_a[8], c_b[8];
	while(s[i] != ' ' && i < 4)
	{
		if (s[i] == '\0')
			return NULL;
			
		c_a[i] = s[i];
		i++;
	}
	if (i == 4)
		return NULL;
		
	c_a[++i] = '\0';
	int max = 4;
	while(s[i] != '\0' && max > 0)
	{
		c_b[i] = s[i];
		i++;
		max--;
	}
	if (max == 0)
		return NULL;
		
	c_b[++i] = '\0';
	
	a = atoi(c_a);
	b = atoi(c_b);
	
	if(a == 0)
		return NULL;
	
	nb_op[a] += b;
	nb_interupt[a]++;
	
	Instruction* ins = new Instruction();
	ins->coprocesseur = a;
	ins->operation = 'Z';
	ins->resultat = (double)nb_op[a] / (double)nb_interupt[a];
	
	return ins;
}
