/*
   * Fichier : grille.h
   * Auteurs : Alexandre Rose, Anael Maubant
   * Date : 4 avril 2012
   * Description : structure représentant la grille
*/

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

   ~ligne_t()
   {
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

};

typedef struct grille_t 
{
   int size_;
   ligne_t* grid_;

   grille_t(int size)
   {
      size_ = size;

      grid_ = new ligne_t[size];
     /* grid_ = new int*[size];*/
      for (int i = 0; i < size; i++)
         grid_[i].setup(size);
   }

   ~grille_t()
   {
      delete []grid_;
      grid_ = 0;
   }

   ligne_t& operator[](const int index)
   {
      return grid_[index];
   }

} grille;

