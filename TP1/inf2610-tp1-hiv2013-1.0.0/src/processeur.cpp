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

// Exécution du processeur
int main(int argc, char** argv) {
	// On vérifie les arguments

	// On créé la fifo avec laquelle les autres processus pourront communiquer avec le processeur
	if(mkfifo(PROC_COPROC, 0666) != 0)
	{
		printf("Impossible de créer le tube");
		unlink(PROC_COPROC);
		exit (1);
	}
	
	// On créé la fifo avec laquelle les coprocesseurs pourront communiquer avec le périphérique

	int nbCo = 3;
	
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

	// On entre dans le cycle
	//	Lecture et traitement d'une ligne en mémoire
	//	On lit le fifo s'il y a un message
	fstream file("/home/alexrose/Documents/TP-2610/TP1/inf2610-tp1-hiv2013-1.0.0/memoire/memoire.txt", ios::in);
	
	int proc;
	while(!file.eof())
	{
		char s[50];
		file.getline(s, 50);
		
		int len = -1;
		while(s[++len] != '\0' && len < 50);
		
		proc = parseLine(s, &len);
		
		write(fd[proc - 1][W], s, len + 1);
		
		int fd_co = open(PROC_COPROC, O_RDONLY | O_NDELAY);
		char c;
		int n;
		fcntl(fd_co,F_SETFL,0);
		while ((n = read(fd_co, &c, 1)) > 0)
		{
			printf("%c", c);
		}

		close(fd_co);
		
		sleep(1);
	}
	
	
	// On demande aux coprocesseurs et au périphérique d'entrée/sortie de se terminer

	// On attend qu'ils se terminent

	// On libère les ressources allouées
	file.close();
	for(int i = 0; i < nbCo; i++)
		close(fd[i][W]);
	unlink(PROC_COPROC);
	printf("GAME OVER!\n");
	return 0;
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

