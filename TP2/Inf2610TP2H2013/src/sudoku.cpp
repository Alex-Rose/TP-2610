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
#include <signal.h>
#include <time.h>




////////////////////////////////////////// DEFINES //////////////////////////////////////////////////
#define EXIT_SUCCESS 0
#define EXIT_FAILIURE 1

void* jouer(void* arg);
void* accueil(void* arg);
void* minuterie (void* arg);
void writeResults();
void dispose();

///////////////////////////////////// GLOBAL VARIABLES //////////////////////////////////////////////
std::queue<_MessageCJ*> file1;
std::queue<_MessageJC*> file2;
std::queue<int> nouveauxJoueurs;

pthread_mutex_t file1_lock = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t file2_lock = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t nouveauJoueurs_lock = PTHREAD_MUTEX_INITIALIZER;
sem_t file1_sem;

//condition pour la lecture
pthread_cond_t nonEmpty = PTHREAD_COND_INITIALIZER;
pthread_cond_t nonFullFile2 = PTHREAD_COND_INITIALIZER;


Joueur* joueurs[5];
std::map<int, Joueur*> listeJoueurs;
int tidCount = 3;
int playerCount = 3;
std::string pathResultat;
bool timeUp = false;
bool noOneLeft = false;

pthread_t mainThread;

//Gestionnaire de signal
void sigHandler(int arg)
{
    writeResults();
    dispose();
    exit(EXIT_SUCCESS);
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
//     int* cell = 0;
//     
//     for (int i = x + 1; i < 9; i++)
//     {
//         for (int j = y; j < 9; j++)
//         {
//             if (grid[j][i] != 0)
//                 continue;
//             cell = new int[2];
//             cell[0] = j;
//             cell[1] = i;
//             return cell;
//         }   
//     }
//     
//     for (int i = 0; i <= x; i++)
//     {
//         for (int j = 0; j < 9; j++)
//         {
//             if (i == x && j == y) //Condifitons initiales => boucle finie
//                 return cell;
//             
//             if (grid[j][i] != 0)
//                 continue;
//             cell = new int[2];
//             cell[0] = j;
//             cell[1] = i;
//             return cell;
//         }   
//     }
//     
//     return cell;
    return findEmpty(grid);
}

std::list<int> getOptions (int (&grid)[9][9], int x, int y)
{
//     if (grid[x][y] != 0)
//         std::cout<<"On est dans la colle"<<std::endl;
    
//     std::cout<<"LUKE, JE SUIS TON ["<<x<<","<<y<<"]"<<std::endl;
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
    
    for(int i = (y / 3) * 3 ; i < (y / 3) * 3 + 3; i++)
    {
        for(int j = (x / 3) * 3 ; j < (x / 3) * 3 + 3; j++)
        {
            if (grid[j][i] != 0)
                set[grid[j][i] - 1] = false;
        }
    }
    
//     std::cout<<"CHOOSE YOUR SIDE LUKE :";
    for(int i = 0; i < 9; i++)
    {
        if(set[i])
        {
            opt.push_back(i + 1);
//             std::cout<<i+1;
        }
    }
//     std::cout<<std::endl;
    
    return opt;
}

void eliminateLooser(std::map<int, Joueur*>* listeJoueurs, int tid, Joueur** joueursActifs)
{
    //pthread_cancel((*listeJoueurs->find(tid)->second->thread));
    pthread_mutex_lock(&nouveauJoueurs_lock);
    for(int i = 0; i < 5; i++)
    {
        if (joueursActifs[i] != 0 && joueursActifs[i]->tid == tid)
        {
            joueursActifs[i]->etat = "Elimine";
            joueursActifs[i] = 0;
            playerCount--;
        }
    }
    pthread_mutex_unlock(&nouveauJoueurs_lock);
}

void writeResults()
{
    std::ofstream file;
    file.open(pathResultat.c_str());
    
    if (timeUp)
        file<<"Partie annulee!"<<std::endl;
    else if (noOneLeft)
        file<<"Partie non terminee!"<<std::endl;
    else 
        file<<"Partie terminee!"<<std::endl;
    for (std::map<int, Joueur*>::iterator it = listeJoueurs.begin(); it != listeJoueurs.end(); it++)
    {
        file<<"Joueur "<<it->second->tid<<std::endl;
        file<<"Score :  "<<it->second->score<<std::endl;
        file<<"Nb Erreur :  "<<it->second->nbErreur<<std::endl;
        file<<"Etat final : "<<it->second->etat<<std::endl<<std::endl;
    }
}

void dispose()
{
    for (int i = 0; i < 5; i++)
        joueurs[i] = 0;
    
    for (std::map<int, Joueur*>::iterator it = listeJoueurs.begin(); it != listeJoueurs.end(); it++)
    {
        pthread_cancel((*it->second->thread));
        Joueur* j = it->second;
        //delete j;
    }
}

int nbJoueurActifs()
{
    int nb = 0;
    for(int i = 0; i < 5; i++)
    {       
        if (joueurs[i] != 0)
            nb++;
    }
    return nb;
}

//Execute par le thread principal (controleur)
int main (int argc, char **argv)
{
    if (argc != 6)
        return EXIT_FAILIURE;
	
    signal(SIGALRM, sigHandler);
    
	std::string pathGrilleVide = argv[1];
	std::string pathGrilleSolution = argv[2];
	std::string pathArrivee = argv[3];
	int tempsMax = atoi(argv[4]);
	pathResultat = argv[5];
    
    int grille[9][9];
    int solution[9][9];
    
    mainThread = pthread_self();
    
    loadGrid(pathGrilleVide, grille);
//     printGrid(grille);
    
    loadGrid(pathGrilleSolution, solution);
// 	printGrid(solution);

    for (int i = 0; i < 5; i++)
    {
        joueurs[i] == 0;
    }

    //creaation des thread joueur par defaut
    
    joueurs[0] = new Joueur();
    joueurs[1] = new Joueur();
    joueurs[2] = new Joueur();
    
    joueurs[0]->thread = new pthread_t();
    joueurs[1]->thread = new pthread_t();
    joueurs[2]->thread = new pthread_t();
    int un=1;
    int deux=2;
    int trois=3;
    pthread_create(joueurs[0]->thread, NULL, jouer, &un);
    pthread_create(joueurs[1]->thread, NULL, jouer, &deux);
    pthread_create(joueurs[2]->thread, NULL, jouer, &trois);
    
    joueurs[0]->tid = 1;
    joueurs[1]->tid = 2;
    joueurs[2]->tid = 3;
    
    joueurs[0]->etat = "Inconnu";
    joueurs[1]->etat = "Inconnu";
    joueurs[2]->etat = "Inconnu";
    
    listeJoueurs.insert(std::pair<int, Joueur*>(joueurs[0]->tid, joueurs[0]));
    listeJoueurs.insert(std::pair<int, Joueur*>(joueurs[1]->tid, joueurs[1]));
    listeJoueurs.insert(std::pair<int, Joueur*>(joueurs[2]->tid, joueurs[2]));
    
    
    // Creation des deux autres thread
    pthread_t accueil_t;
    pthread_t alarm_t;
    
    pthread_create(&accueil_t, NULL, accueil, (void*)pathArrivee.c_str());
    pthread_create(&alarm_t, NULL, minuterie, (void*)&tempsMax);
    
    sem_init(&file1_sem, 0, 0);
    
    int* empty = findEmpty(grille);
    int col = 0, ln = 0;
    do
    {
        //============================================================
        // BOUCLE POUR TROUVER LES ZERO ET LES ENVOYER DANS LA FILE 1 
        if (empty == 0)
            break;
        
        
        if (pthread_mutex_trylock(&file1_lock) == 0)   
        {
// 	  std::cout<<"je prend le mutex pour le broadcast"<<std::endl;
            if (file1.size() < 4)
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
        
                    msg->choiceList = opts;
                    file1.push(msg);
                     pthread_cond_broadcast(&nonEmpty);
// 		      std::cout<<"je broadcast "<<file1.size()<<std::endl;
                   // sem_post(&file1_sem);
                }
                
                delete empty;
                
                col = msg->colonne;
                ln = msg->ligne;
    
            }
            
            pthread_mutex_unlock( &file1_lock );
// 	    std::cout<<"je rend le mutex pour le broadcast "<<file2.size()<<std::endl;
        }

        
        //==========================
        //BOUCLE POUR LIRE LA FILE 2 
        
        if(pthread_mutex_trylock(&file2_lock) == 0)
        { 
            if (file2.size() > 0)
            {
                _MessageJC* msg = new _MessageJC((*file2.front()));
                file2.pop();
                if (listeJoueurs.find(msg->tid)->second->etat != "Elimine")
                {
//                     std::cout<<"ANS = "<<msg->choice<<" ? "<<solution[msg->colonne][msg->ligne]<<std::endl;
//                     if (msg->choice == 0)
//                         std::cout<<"LE CIEL NOUS TOMBE SUR LA TETE PAR TOUTATIS!!!"<<std::endl;
                    
                    //pthread_cond_broadcast(&nonFullFile2);
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
                        looser->nbErreur++;
                        if (looser->score <= -10)
                        {    
			  
                eliminateLooser(&listeJoueurs, msg->tid, joueurs);
                std::cout<<"Better luck next time, NOOB! "<<msg->tid<<std::endl;
                
                        }
                        
                    }
                }
                pthread_mutex_unlock(&file2_lock);
		
            }
            else
            {
                pthread_mutex_unlock(&file2_lock);
		
                sleep(1);
            }   
        }
            
        
        pthread_mutex_lock(&nouveauJoueurs_lock);
        if (nouveauxJoueurs.size() > 0)
        {
            if (nouveauxJoueurs.front() == 0)
                pthread_cancel(accueil_t);
            else if (playerCount < 5 )
            {
                
                    
                for (int i = 0; i < 5 && playerCount < 5 && nouveauxJoueurs.size() > 0; i++)
                {
                    if (joueurs[i] == 0)
                    {
                     
                        int id = nouveauxJoueurs.front();
                        if (id == 0)
                           pthread_cancel(accueil_t);
                        else
                        {
                            nouveauxJoueurs.pop();
                            joueurs[i] = listeJoueurs.find(id)->second;
                            playerCount++;
                            joueurs[i]->etat = "Inconnu";
                            pthread_create(joueurs[i]->thread, NULL, jouer, &joueurs[i]->tid);
                        }
                    }
                }
            }
        }
        pthread_mutex_unlock(&nouveauJoueurs_lock);
        
        empty = findEmpty(grille, col, ln);
        //printGrid(grille);
//         std::cout<<std::endl;
    }while (empty != 0 && nbJoueurActifs() > 0);
    
    // Si on se rend ici, soit tous les joueurs sont morts, soit on a fini la grille
    int max = 0;
    for (std::map<int, Joueur*>::iterator it = listeJoueurs.begin(); it != listeJoueurs.end(); it++)
    {
        
        if (it->second->etat == "Inconnu")
        {
            if (it->second->score > max)
                max = it->second->score;
        }
    }
    
    for (std::map<int, Joueur*>::iterator it = listeJoueurs.begin(); it != listeJoueurs.end(); it++)
    {
        
        if (it->second->etat == "Inconnu")
        {
            if (it->second->score == max)
                it->second->etat = "Vainqueur";
            else
                it->second->etat = "Perdant";
        }
    }
    
    
    pthread_cancel(accueil_t);
    pthread_cancel(alarm_t);
    
    writeResults();
    dispose();
    
    return EXIT_SUCCESS;
  
}
////////////////////////////////////////// THREAD FUNCTIONS //////////////////////////////////////////////////

/**************************************** thread_Alarm*****************************************************/
//Function execute par le thread_Alarm
void* minuterie (void* arg){

    sleep((*(int*)arg));

    pthread_kill(mainThread, SIGALRM);
}
/**************************************** thread_Accueil*****************************************************/
//Function execute par le thread_Accueil
void* accueil(void* arg){
    std::ifstream file;
    file.open((char*)arg);
    
    int delay;
    while(!file.eof())
    {
        file>>delay;
        sleep(delay);
        
        pthread_mutex_lock(&nouveauJoueurs_lock);
        Joueur* j = new Joueur();
        j->thread = new pthread_t();
        tidCount++;
        j->tid = tidCount;
        listeJoueurs.insert(std::pair<int, Joueur*>(j->tid, j));
        nouveauxJoueurs.push(j->tid);
//         std::cout<<"Player added with id : "<<j->tid<<std::endl;
        pthread_mutex_unlock(&nouveauJoueurs_lock);
    }
    
    pthread_mutex_lock(&nouveauJoueurs_lock);
    nouveauxJoueurs.push(0);
    pthread_mutex_unlock(&nouveauJoueurs_lock);
    
    pthread_exit(EXIT_SUCCESS);

}
/**************************************** thread_Joueur*****************************************************/

static void cleanup_handler(void* arg)
{
    pthread_mutex_unlock(&file1_lock);
    pthread_mutex_unlock(&file2_lock);
}

//Function execute par le thread_Joueur
void* jouer(void* arg){
  
    pthread_cleanup_push(cleanup_handler, NULL);
  MessageCJ* currentMessage;
  std::map<std::pair<int,int>,std::list<int> > alreadyTry;
  std::list<int>::iterator findIter;
  int randomNumber;
  int numberChoice;
  
  srand(time(NULL));
  
  while(1){
    
    
     //lecture dans la file 1
    pthread_mutex_lock(&file1_lock);
    
//     
     while(file1.size()==0){
      
      pthread_cond_wait(&nonEmpty,&file1_lock);
      
//       
    }
    
    //sem_wait(&file1_sem);
    //pthread_mutex_lock(&file1_lock);
   // prend le message et verifie si les valeurs sont deja tester
   
    currentMessage=file1.front();
    std::pair<int,int> currentPair(currentMessage->ligne,currentMessage->colonne);
    
 //   std::cout<<"je suis "<< currentPair.first<<" "<<currentPair.second<< "  et je suis le thread numero "<<*(int*)arg<<std::endl;
    
    for(std::list<int>::iterator it = alreadyTry[currentPair].begin();it!=alreadyTry[currentPair].end();it++)
      
    {
	currentMessage->choiceList.remove(*it);
	
      
      
      
    }
    
    
    
    // select choice of response
    if(!currentMessage->choiceList.empty())
    {
    
    randomNumber = rand()%currentMessage->choiceList.size();
    
    std::list<int>::iterator ite = currentMessage->choiceList.begin();
    
    std::advance(ite,randomNumber);
    
    alreadyTry[currentPair].push_back(*ite);
    numberChoice=*ite;
    
    for(std::list<int>::iterator it1 = currentMessage->choiceList.begin(); it1!=currentMessage->choiceList.end();it1++)
    {
      
      //std::cout<<(*it1)<<"  ";
      
      
    }
    
    
    }
    else 
    {
     // std::cout<<"je suis vide"<<std::endl;
      numberChoice=0;
      
      
      
    }
    
//     std::cout<<std::endl;
    for(std::list<int>::iterator it1 = alreadyTry[currentPair].begin(); it1!=alreadyTry[currentPair].end();it1++)
    {
      
   //   std::cout<<(*it1)<<"  ";
      
      
    }
   // std::cout<<std::endl;
   // std::cout<<std::endl;
    file1.pop();
    
    

     
    
   // file1.pop();
//     std::cout<<"je rend le lock "<<*(int*)arg<<std::endl;
    pthread_mutex_unlock(&file1_lock);
    
     sleep(2);
    
    
    
   // sleep(2);
    
//     ecriture du resultat sur la file 2
    pthread_mutex_lock(&file2_lock);
//     std::cout<<"je prend le lock de la file2"<<*(int*)arg<<std::endl;
    
    
  
    MessageJC* messageRetour= new MessageJC();
    messageRetour->ligne=currentPair.first;
    messageRetour->colonne=currentPair.second;
    messageRetour->tid=*(int*)arg;
    messageRetour->choice=numberChoice;
    file2.push(messageRetour);
//     std::cout<<"je rend le lock de la file2"<<*(int*)arg<<std::endl;
    pthread_mutex_unlock(&file2_lock);
    
    
    
  
    
  }
  
  
pthread_cleanup_pop(0);
}


