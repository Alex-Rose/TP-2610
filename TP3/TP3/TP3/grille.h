/*
   * Fichier : grille.h
   * Auteurs : Alexandre Rose, Anael Maubant
   * Date : 4 avril 2012
   * Description : structure représentant la grille
*/
#ifndef _GRILLE_
#define _GRILLE_

#include <iostream>
#include <iomanip> 
#include <list>

//Ligne (array de int)
struct ligne_t
{
   int size_;
   int* ligne_;

   ligne_t()
   {
      size_ = 0;
      ligne_ = 0;
   }

   ligne_t(int size)
   {
      setup(size);
   }

   ligne_t(const ligne_t &obj)
   {
      size_ = obj.size_;
      ligne_ = new int[size_];
      for(int i = 0; i < size_; i++) 
         ligne_[i] = obj[i];
      
   }

   ~ligne_t()
   {
      //delete le tableau de int
      delete []ligne_;
      ligne_ = 0;
      size_ = 0;
   }

   //Initialise les valeurs a -1 (non decouvert)
   void setup(int size)
   {
      size_ = size;
      ligne_ = new int[size];
      for(int i = 0; i < size; i++)
         ligne_[i] = -1;
   }

   //Acesseur
   int& operator[](const int index)
   {
      return ligne_[index];
   }

     // Faut la version const pour lorsquon on accede depuis une reference
   int& operator[](const int index) const
   {
      return ligne_[index];
   }

};


// GRILLE
typedef struct grille_t 
{
   int size_;
   ligne_t** grid_;
   std::list<std::pair<int, int> > remaining_;

   grille_t(int size)
   {
      size_ = size;
      
      //Tableau de lignes
      grid_ = new ligne_t*[size];

      //Ajouter aux possibilites restantes chaques cases
      for (int i = 0; i < size; i++)
      {
         for (int j = 0; j < size; j++)
            remaining_.insert(remaining_.end(), std::pair<int, int>(i, j));
      }

      //Creer les lignes
      for (int i = 0; i < size; i++)
      {
         grid_[i] = new ligne_t(size);
      }
   }

   //Constructeur de copie
   grille_t(const grille_t &obj)
   {
      size_ = obj.size_;

      grid_ = new ligne_t*[size_];
      for(int i = 0; i < size_; i++)
      {
         //grid_[i].setup(size_);
         grid_[i] = new ligne_t(obj[i]);
      }

      for (int i = 0; i < size_; i++)
      {
         for (int j = 0; j < size_; j++)
            if ((*this)[i][j] == -1)
               remaining_.insert(remaining_.end(), std::pair<int, int>(i, j));
      }
   }

   //Destructeur
   ~grille_t()
   {
      if (grid_ == 0)
         return;
      for (int i = 0; i < size_; i++)
         delete grid_[i];
      grid_ = 0;
   }

   //Accesseur
   ligne_t& operator[](const int index)
   {
      return *grid_[index];
   }

   //Accesseur const
   ligne_t& operator[](const int index) const
   {
      return *grid_[index];
   }

} grille;

#endif