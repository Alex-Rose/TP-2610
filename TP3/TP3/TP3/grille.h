/*
   * Fichier : grille.h
   * Auteurs : Alexandre Rose, Anael Maubant
   * Date : 4 avril 2012
   * Description : structure représentant la grille
*/
#ifndef _GRILLE_
#define _GRILLE_
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
      //for(int i = 0; i < size_; i++)
      //   delete &ligne_[i];
      delete []ligne_;
      ligne_ = 0;
      size_ = 0;
   }

   void setup(int size)
   {
      size_ = size;
      ligne_ = new int[size];
      for(int i = 0; i < size; i++)
         ligne_[i] = 0;
   }

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

   grille_t(int size)
   {
      size_ = size;

      grid_ = new ligne_t*[size];
     
      for (int i = 0; i < size; i++)
      {
         grid_[i] = new ligne_t(size);
         //grid_[i]->setup(size);
      }
   }

   grille_t(const grille_t &obj)
   {
      size_ = obj.size_;

      grid_ = new ligne_t*[size_];
      for(int i = 0; i < size_; i++)
      {
         //grid_[i].setup(size_);
         grid_[i] = new ligne_t(obj[i]);
      }
   }

   ~grille_t()
   {
      if (grid_ == 0)
         return;
      for (int i = 0; i < size_; i++)
         delete grid_[i];
      grid_ = 0;
   }

   ligne_t& operator[](const int index)
   {
      return *grid_[index];
   }

   ligne_t& operator[](const int index) const
   {
      return *grid_[index];
   }


} grille;

#endif