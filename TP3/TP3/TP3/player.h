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