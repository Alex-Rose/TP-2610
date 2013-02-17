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
		sprintf(r, "%d", (fd[i][R]));
		sprintf(w, "%d", (fd[i][W]));
		execl("/home/alexrose/Documents/TP-2610/TP1/inf2610-tp1-hiv2013-1.0.0/src/coprocesseur", r, w);
	}
	
	
	if (getpid() == p_pid)
		printf("child list is : %d, %d, %d\n", co_pid[0], co_pid[1], co_pid[2]);
		

//	close(fd[2][R]);
//	write(fd[2][W], "nothing", strlen("nothing") + 1);
//	close(fd[2][W]);
//	
//	close(fd[1][R]);
//	write(fd[1][W], "nothing", strlen("nothing") + 1);
//	close(fd[1][W]);
//	
//	sleep(3);
//	close(fd[0][R]);
//	write(fd[0][W], "esad", strlen("esad") + 1);
//	sleep(2);
//	write(fd[0][W], "die bitch", strlen("die bitch") + 1);
//	close(fd[0][W]);
	       
	
	// Création du périphérique

	// On entre dans le cycle
	//	Lecture et traitement d'une ligne en mémoire
	//	On lit le fifo s'il y a un message
	fstream file("/home/alexrose/Documents/TP-2610/TP1/inf2610-tp1-hiv2013-1.0.0/memoire/memoire.txt", ios::in);
	
	int proc, a, b;
	char op;
	
	while(!file.eof())
	{
		file>>proc>>op>>a>>b;
		file.seekg(1, ios::cur);
		char s[50];
		sprintf(s, "%c %d %d\n", op, a, b);
		int len = -1;
		while(s[++len] != '0' && len < 50);
		
		write(fd[proc][W], s, len);
		
	}
	// On demande aux coprocesseurs et au périphérique d'entrée/sortie de se terminer

	// On attend qu'ils se terminent

	// On libère les ressources allouées
	file.close();
	for(int i = 0; i < nbCo; i++)
		close(fd[i][W]);
	printf("GAME OVER!\n");
	return 0;
}



