/*
   * Fichier : player.h
   * Auteurs : Alexandre Rose, Anael Maubant
   * Date : 4 avril 2012
   * Description : Classe game (partie)
*/
#ifndef _PLAYER_
#define _PLAYER_
#include <Windows.h>
#include "grille.h"

//Classe qui represonte un joueur
//La classe joueur possede le handle du thread sur lequel s'execureta
//la loop
class Player
{
public:
   Player();

   Player(HANDLE sem_, grille*, HANDLE mutex_, HANDLE canPlay);

   ~Player();

   void startPlayingAsync();

   void stopPlaying();

   std::pair<int, int> choice;

   int score;

   HANDLE getThread() { return thread_;}
private:
   HANDLE thread_;
   HANDLE event_;
   HANDLE gridMutex_;
   HANDLE canPlay_;

   DWORD WINAPI play(LPVOID arg);

   // THIS IS VERY #YOLO
   // more info : http://stackoverflow.com/questions/7170269/c-thread-in-member-function
   static DWORD WINAPI play_wrapper(LPVOID arg);

   DWORD threadId_;

   grille* grid_;

};
#endif