/**************************************************************************************************
* file: sudoku.cpp
* description: 
* author(s): 
* date: 
* update: 
***************************************************************************************************/

////////////////////////////////////////// INCLUDE //////////////////////////////////////////////////
#include "entete.h"
#include <iostream>
#include <unistd.h>
#include <stdio.h>
#include <signal.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <semaphore.h>
#include <pthread.h>
#include <fstream>
#include <errno.h>
#include <queue>
#include <list>
#include <utility>
#include <map>
#include <algorithm>



////////////////////////////////////////// DEFINES //////////////////////////////////////////////////
#define EXIT_SUCCESS 0
#define EXIT_FAILIURE 1

void* jouer(void* arg);
void* accueil(void* arg);
void* minuterie (void* arg);

///////////////////////////////////// GLOBAL VARIABLES //////////////////////////////////////////////
std::queue<_MessageCJ*> file1;
std::queue<_MessageJC*> file2;
pthread_mutex_t file1_lock = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t file2_lock = PTHREAD_MUTEX_INITIALIZER;


//condition pour la lecture
pthread_cond_t nonEmpty = PTHREAD_COND_INITIALIZER;
pthread_cond_t nonFullFile2 = PTHREAD_COND_INITIALIZER;


int tidCount = 3;

//Gestionnaire de signal
void sigHandler(int arg)
{

}

/**************************************** MAIN *****************************************************/
void loadGrid(std::string path, int (&grid)[9][9])
{
    std::ifstream file;
    file.open(path.c_str());
    for (int i = 0; i < 9; i++)
    {
        for (int j = 0; j < 9; j++)
        {
            file >> grid[j][i];
        }
    }
    file.close();
}

void printGrid(int (&grid)[9][9])
{
    for (int i = 0; i < 9; i++)
    {
        for (int j = 0; j < 9; j++)
        {
            std::cout << grid[j][i];
        }
        std::cout<<std::endl;
    }
}

int* findEmpty(int (&grid)[9][9])
{
    int* cell = 0;
    
    for (int i = 0; i < 9; i++)
    {
        for (int j = 0; j < 9; j++)
        {
            if (grid[j][i] != 0)
                continue;
            cell = new int[2];
            cell[0] = j;
            cell[1] = i;
            return cell;
        }   
    }
    
    return cell;
}

int* findEmpty(int (&grid)[9][9], int x, int y)
{
    int* cell = 0;
    
    for (int i = x; i < 9; i++)
    {
        for (int j = y + 1; j < 9; j++)
        {
            if (grid[j][i] != 0)
                continue;
            cell = new int[2];
            cell[0] = j;
            cell[1] = i;
            return cell;
        }   
    }
    
    for (int i = 0; i <= x; i++)
    {
        for (int j = 0; j < 9; j++)
        {
            if (i == x && j == y) //Condifitons initiales => boucle finie
                return cell;
            
            if (grid[j][i] != 0)
                continue;
            cell = new int[2];
            cell[0] = j;
            cell[1] = i;
            return cell;
        }   
    }
    
    return cell;
}

std::list<int> getOptions (int (&grid)[9][9], int x, int y)
{
    std::list<int> opt;
    
    bool set[9];
    
    for (int i = 0; i < 9; i++)
        set[i] = true;
    
    for(int i = 0; i < 9; i++)
    {
        if (grid[x][i] != 0)
            set[grid[x][i] - 1] = false;
    }
    
    for(int i = 0; i < 9; i++)
    {
        if (grid[i][y] != 0)
            set[grid[i][y] - 1] = false;
    }
    
    for(int i = (x / 3) * 3 ; i < (x / 3) * 3 + 3; i++)
    {
        for(int j = (x / 3) * 3 ; j < (x / 3) * 3 + 3; j++)
        {
            if (grid[i][j] != 0)
                set[grid[i][j] - 1] = false;
        }
    }
    
    for(int i = 0; i < 9; i++)
    {
        if(set[i])
            opt.push_back(i + 1);
    }
    
    return opt;
}

void eliminateLooser(std::map<int, Joueur*>* listeJoueurs, int tid, Joueur** joueursActifs)
{
    pthread_cancel(listeJoueurs->find(tid)->second->thread);
    
    for(int i = 0; i < 5; i++)
    {
        if (joueursActifs[i] != 0 && joueursActifs[i]->tid == tid)
            joueursActifs[i] = 0;
    }
}

//Execute par le thread principal (controleur)
int main (int argc, char **argv)
{
    if (argc != 6)
        return EXIT_FAILIURE;
	
	std::string pathGrilleVide = argv[1];
	std::string pathGrilleSolution = argv[2];
	std::string pathArrivee = argv[3];
	std::string tempsMax = argv[4];
	std::string pathResultat = argv[5];
    
    int grille[9][9];
    int solution[9][9];
    
    loadGrid(pathGrilleVide, grille);
//     printGrid(grille);
    
    loadGrid(pathGrilleSolution, solution);
// 	printGrid(solution);

    //creaation des thread joueur par defaut
    Joueur* joueurs[5];
    
    joueurs[0] = new Joueur();
    joueurs[1] = new Joueur();
    joueurs[2] = new Joueur();
    
    joueurs[0]->thread = pthread_t();
    joueurs[1]->thread = pthread_t();
    joueurs[2]->thread = pthread_t();
    
    pthread_create(&joueurs[0]->thread, NULL, jouer, (void*)1);
    pthread_create(&joueurs[1]->thread, NULL, jouer, (void*)2);
    pthread_create(&joueurs[2]->thread, NULL, jouer, (void*)3);
    
    joueurs[0]->tid = 1;
    joueurs[1]->tid = 2;
    joueurs[2]->tid = 3;
    
    
    std::map<int, Joueur*> listeJoueurs;
    listeJoueurs.insert(std::pair<int, Joueur*>(joueurs[0]->tid, joueurs[0]));
    listeJoueurs.insert(std::pair<int, Joueur*>(joueurs[1]->tid, joueurs[0]));
    listeJoueurs.insert(std::pair<int, Joueur*>(joueurs[2]->tid, joueurs[0]));
    
    
    // Creation des deux autres thread
    pthread_t accueil_t;
    pthread_t alarm_t;
    
    pthread_create(&accueil_t, NULL, accueil, NULL);
    pthread_create(&alarm_t, NULL, minuterie, NULL);
    
    int* empty = findEmpty(grille);
    
    do
    {
        //============================================================
        // BOUCLE POUR TROUVER LES ZERO ET LES ENVOYER DANS LA FILE 1 
        if (empty == 0)
            break;
        
        
        if (pthread_mutex_trylock(&file1_lock) == 0 && file1.size() < 4)
        {
            MessageCJ* msg = new MessageCJ();
            msg->colonne = empty[0];
            msg->ligne = empty[1];
        
            bool duplicate = false;
            std::queue<MessageCJ*> temp;
            
            while(!file1.empty())
            {
                temp.push(new MessageCJ((*file1.front())));
                file1.pop();
            }
            
            while(!temp.empty())
            {
                MessageCJ* tmpMsg = new MessageCJ((*temp.front()));
                file1.push(tmpMsg);
                temp.pop();
                if (tmpMsg->colonne == msg->colonne && tmpMsg->ligne == msg->ligne)
                    duplicate = true;
                
            }
            
            if (!duplicate)
            {
                std::list<int> opts = getOptions(grille, msg->colonne, msg->ligne);
    
                for(std::list<int>::iterator it = opts.begin(); it != opts.end(); it++)
                {
                  //  std::cout<<*it<<std::endl;
                }
                
                file1.push(msg);
                pthread_cond_broadcast(&nonEmpty);
		std::cout<<"je viens de broadcast"<<std::endl;
            }
            pthread_mutex_unlock( &file1_lock );
            
            delete empty;
            empty = findEmpty(grille, msg->colonne, msg->ligne);
//             std::cout<<"Finding more empty cells"<<std::endl;
        }
        else
        {
//             std::cout<<"Queue 1 is full!"<<std::endl;
        }
        
        //==========================
        //BOUCLE POUR LIRE LA FILE 2 
        
        if(pthread_mutex_trylock(&file2_lock) == 0)
        {
            if (file2.size() > 0)
            {
                _MessageJC* msg = new _MessageJC((*file2.front()));
                file2.pop();
                
                if (solution[msg->colonne][msg->ligne] == msg->choice)
                {
                    //If win!
                    grille[msg->colonne][msg->ligne] = msg->choice;
                    Joueur* vainqueur = listeJoueurs.find(msg->tid)->second;
                    vainqueur->score++;
                }
                else
                {
                    //if noob!
                    Joueur* looser = listeJoueurs.find(msg->tid)->second;
                    looser->score--;
                    if (looser->score <= -10)
                        eliminateLooser(&listeJoueurs, msg->tid, joueurs);
                    std::cout<<"Better luck next time, NOOB!"<<std::endl;
                    
                }
            }
            else
            {
                sleep(1);
            }   
        }
        
        
    }while (empty != 0);
    
    
  return EXIT_SUCCESS;
  
}
////////////////////////////////////////// THREAD FUNCTIONS //////////////////////////////////////////////////

/**************************************** thread_Alarm*****************************************************/
//Function execute par le thread_Alarm
void* minuterie (void* arg){
//Dormir pendant la dur�ee maximale requise

//Envoyer un sIGALRM au thread principal

}
/**************************************** thread_Accueil*****************************************************/
//Function execute par le thread_Accueil
void* accueil(void* arg){


}
/**************************************** thread_Joueur*****************************************************/
//Function execute par le thread_Joueur
void* jouer(void* arg){
  
 
  MessageCJ* currentMessage;
  std::map<std::pair<int,int>,std::list<int> > alreadyTry;
  std::list<int>::iterator findIter;
  
  
  while(1){
    
    
     //lecture dans la file 1
   pthread_mutex_lock(&file1_lock);
   std::cout <<"mutex bloque"<<std::endl;
    
    while(file1.size()==0){
      std::cout<<"test dans ta maman "<<file1.size()<<std::endl;
      pthread_cond_wait(&nonEmpty,&file1_lock);
    }
   // 
    currentMessage=file1.front();
    std::pair<int,int> currentPair(currentMessage->ligne,currentMessage->colonne);
    
    std::cout<<"je suis "<< currentPair.first<<" "<<currentPair.second<<std::endl;
    file1.pop();
    
    

     
    
   // file1.pop();
    
    pthread_mutex_unlock(&file1_lock);
    
    std::cout<<"le mutex a ete debloque"<<std::endl;
    
    
    
    
    //ecriture du resultat sur la file 2
// pthread_mutex_lock(&file2_lock);
    
    //while(file2.size()>5)
     // pthread_cond_wait(&nonFullFile2, &file2_lock);
    
    
    //pthread_mutex_unlock(&file2_lock);
    
    
    
    
    
    
  }
  
  

}


