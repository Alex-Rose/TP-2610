/*
   * Fichier : player.cpp
   * Auteurs : Alexandre Rose, Anael Maubant
   * Date : 4 avril 2012
   * Description : Classe game (partie)
*/
#include <iostream>

#include "player.h"


Player::Player()
{
   thread_ = 0;
   threadId_ = 0;
   sem_ = 0;
}

Player::Player(HANDLE mutex)
{
   thread_ = 0;
   threadId_ = 0;
   sem_ = mutex;
}

Player::~Player()
{
   CloseHandle(thread_);
}
void Player::startPlayingAsync()
{
   thread_ = CreateThread(NULL, 0, play_wrapper, (void*)this, 0, &threadId_);
}

DWORD WINAPI Player::play_wrapper(void* o)
{
   static_cast<Player*>(o)->play(NULL);
   return 0;
}

DWORD WINAPI Player::play(LPVOID arg)
{
   //Faudrait lui mettre une condition pour arreter un jour
   while(true)
   {
      WaitForSingleObject(sem_, INFINITE);
      std::cout<<"Play from thread "<<threadId_<<std::endl;
      Sleep(rand() % 2000);
      ReleaseSemaphore(sem_, 1, NULL);
   }
   return 0;
}