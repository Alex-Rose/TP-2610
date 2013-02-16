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

	int fd1[2];
	int fd2[2];
	int fd3[2];
	pipe(fd1);
	pipe(fd2);
	pipe(fd3);
	
	// Création des coprocesseurs
	int p_pid = getpid();
	printf("proc pid : %d \n", p_pid);
	int p3, p2, p1;
	p1 = fork();
	if (p1 == 0)
	{
		char r[33];
		char w[33];
		sprintf(r, "%d", fd1[R]);
		sprintf(w, "%d", fd1[W]);
		execl("/home/alexrose/Documents/TP-2610/TP1/inf2610-tp1-hiv2013-1.0.0/src/coprocesseur", r, w);
	}
	if (getpid() == p_pid)
	{
		p2 = fork();
		if (p2 == 0)
		{	
			char r[33];
			char w[33];
			sprintf(r, "%d", fd2[R]);
			sprintf(w, "%d", fd2[W]);
			execl("/home/alexrose/Documents/TP-2610/TP1/inf2610-tp1-hiv2013-1.0.0/src/coprocesseur", r, w);
		}
	}
	if (getpid() == p_pid)
	{
		p3 = fork();
		if(p3 == 0)
		{	
			char r[33];
			char w[33];
			sprintf(r, "%d", fd3[R]);
			sprintf(w, "%d", fd3[W]);
			execl("/home/alexrose/Documents/TP-2610/TP1/inf2610-tp1-hiv2013-1.0.0/src/coprocesseur", r, w);
		}
	}
	if (getpid() == p_pid)
		printf("child list is : %d, %d, %d\n", p1, p2, p3);
		
	/*if(getpid() != p_pid)
	{
		char r[33];
		char w[33];
		sprintf(r, "%d", fd[R]);
		sprintf(w, "%d", fd[W]);
		
		
		execl("/home/alexrose/Documents/TP-2610/TP1/inf2610-tp1-hiv2013-1.0.0/src/coprocesseur", r, w);
	}*/
	close(fd2[R]);
	write(fd2[W], "nothing", strlen("nothing") + 1);
	close(fd2[W]);
	
	close(fd3[R]);
	write(fd3[W], "nothing", strlen("nothing") + 1);
	close(fd3[W]);
	
	sleep(3);
	close(fd1[R]);
	write(fd1[W], "esad", strlen("esad") + 1);
	sleep(2);
	write(fd1[W], "esad", strlen("esad") + 1);
	close(fd1[W]);
	       
	
	// Création du périphérique

	// On entre dans le cycle
	//	Lecture et traitement d'une ligne en mémoire
	//	On lit le fifo s'il y a un message

	// On demande aux coprocesseurs et au périphérique d'entrée/sortie de se terminer

	// On attend qu'ils se terminent

	// On libère les ressources allouées
	return 0;
}



