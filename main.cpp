#include <iostream>
#include <array>
#include <chrono>

constexpr int n = 3;
constexpr int N = n * n;
int boucle{0}, total{0};

//using Grille = std::vector<std::vector<int>>;
using Grille = std::array<std::array<int, N>, N>;

//Liste des fonctions
bool rien_carre_nxn(Grille &grille, int k, int i, int j) noexcept;
bool rien_ligne_colonne(Grille &grille, int k, int i, int c) noexcept;
void singleton(Grille &grille) noexcept;
bool backtrack(Grille &grille, int pos) noexcept;
void solver(Grille &grille) noexcept;

void afficher(Grille &grille)
{
  std::cout << "+-----+-----+-----+\n";
  for (int x = 0; x < N; x++)
  {
    std::cout << "|";
    for (int y = 0; y < N; y++)
    {
      std::cout << grille [x][y];
      ((y + 1) % n) ? std::cout << " " : std::cout << "|";
    }
    std::cout << '\n';
    if (!((x + 1) % n))
    {
      std::cout << "+-----+-----+-----+\n";
    }
  }
  std::cout << std::endl;
}
int main()
{

  Grille grille
  {
    0,0,0,0,0,2,0,0,0,
    0,0,3,0,0,0,8,0,0,
    1,0,0,9,6,0,0,7,0,
    0,0,0,5,0,0,0,0,0,
    0,9,0,0,0,0,0,0,7,
    6,0,0,1,4,0,0,9,0,
    2,0,0,0,0,7,0,0,0,
    0,0,0,0,0,5,0,4,0,
    0,0,1,4,2,0,0,0,3
  };

  auto const taille{grille.size()}; std::cout << "nombre de lignes -> " << taille << "\n";
  auto const a{sizeof(grille) / sizeof(int)}; std::cout << "sudoku de taille " << a << "\n";
  std::cout << "Grille\n";
  afficher(grille);

  solver(grille);

  return 0;
}

bool backtrack(Grille &grille, int pos) noexcept
{
  if (pos == N * N)
  {
    return true;
  }
  int i = pos / N, j = pos % N;

  if (grille [i][j] != 0)
  {
    return backtrack(grille, pos + 1);
  }

  for (int k{1}; k <= N; k++)
  {
    if (rien_ligne_colonne(grille, k, i, j) && rien_carre_nxn(grille, k, i, j))
    {
      grille [i][j] = k;
      if (backtrack(grille, pos + 1))
      {
        return true;
      }
    }
  }
  grille [i][j] = 0;
  return false;
}
// bloc, retourne FAUX si la valeur est trouvée, sinon VRAI
bool rien_carre_nxn(Grille &grille, int k, int l, int c) noexcept
{
  int ii = l - (l % n), jj = c - (c % n);
  for (l = ii; l < ii + n; l++)
  {
    for (c = jj; c < jj + n; c++)
    {
      if (grille [l][c] == k)
      {
        return false;
      }
    }
  }
  return true;
}
//ligne colonne retourne FAUX si la valeur est trouvée, sinon VRAI
bool rien_ligne_colonne(Grille &grille, int k, int l, int c) noexcept
{
  for (int i{0}; i < N; i++)
  {
    if (grille [l][i] == k || grille [i][c] == k)
    {
      return false;
    }
  }
  return true;
}
void singleton(Grille &grille) noexcept
{
  int singleton{0};
  do
  {
    singleton = 0;
    //carré
    for (int i{0}; i < N; i += n)
    {
      for (int j{0}; j < N; j += n)
      {
        int posI{0}, posJ{0};
        for (int k{1}; k <= N; k++)
        {
          int count = 0;
          for (int ii{i}; ii < i + n; ii++)
          {
            for (int jj{j}; jj < j + n; jj++)
            {
              if (grille [ii][jj] == 0 && rien_ligne_colonne(grille, k, ii, jj) && rien_carre_nxn(grille, k, ii, jj))
              {
                posI = ii;
                posJ = jj;
                count++;
              }
            }
          }
          if (count == 1)
          {
            grille [posI][posJ] = k;
            singleton++;
          }
        }
      }
    }
    //lignes
    for (int i{0}; i < N; i++)
    {
      int posI{0}, posJ{0};
      for (int k{1}; k <= N; k++)
      {
        int count{0};
        for (int j{0}; j < N; j++)
        {
          if (grille [i][j] == 0 && rien_ligne_colonne(grille, k, i, j) && rien_carre_nxn(grille, k, i, j))
          {
            posI = i;
            posJ = j;
            count++;
          }
        }
        if (count == 1)
        {
          grille [posI][posJ] = k;
          singleton++;
        }
      }
    }
    //colonnes
    for (int j{0}; j < N; j++)
    {
      int posI{0}, posJ{0};
      for (int k{1}; k <= N; k++)
      {
        int count{0};
        for (int i{0}; i < N; i++)
        {
          if (grille [i][j] == 0 && rien_ligne_colonne(grille, k, i, j) && rien_carre_nxn(grille, k, i, j))
          {
            posI = i;
            posJ = j;
            count++;
          }
        }
        if (count == 1)
        {
          grille [posI][posJ] = k;
          singleton++;
        }
      }
    }
    total += singleton;
    boucle++;
  } while (singleton);
}
void solver(Grille &grille) noexcept
{
  std::chrono::time_point<std::chrono::system_clock> start, end;
  start = std::chrono::system_clock::now();
  singleton(grille);

  if (backtrack(grille, 0))
  {
    end = std::chrono::system_clock::now();
    std::cout << "Total singletons trouvés -> " << total << "\n";
    std::cout << "Total boucle singleton -> " << boucle << "\n";
    std::cout << std::endl;
    std::cout << "Solution\n";
    afficher(grille);
    std::chrono::duration<double> duree = end - start;
    std::cout.precision(9);
    std::cout << "Durée = " << std::fixed << duree.count() << "s" << std::endl;;
  }
  else
  {
    end = std::chrono::system_clock::now();
    std::chrono::duration<double> duree = end - start;
    std::cout.precision(9);
    std::cout << "Pas de solution" << std::endl;
    std::cout << "Durée = " << std::fixed << duree.count() << "s\n";
  }
}
/*
 Grille grille
  {
    8,0,0,9,0,1,0,2,0,
    2,0,3,0,6,4,0,0,0,
    0,0,6,0,0,0,0,0,7,
    1,0,0,0,9,0,0,0,0,
    0,0,8,0,0,0,9,6,0,
    0,0,0,6,0,3,7,0,8,
    0,9,5,0,8,6,2,7,1,
    0,4,1,0,0,0,0,0,3,
    0,8,2,1,3,5,6,4,0
  };
Durée = 0.000036254s
*/
/*
Grille grille
  {
    0,0,0,0,0,6,0,0,5,
    0,0,9,0,0,0,0,0,0,
    0,0,1,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,
    7,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,
    9,0,0,0,0,0,0,0,0,
    2,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0
  };
*/
/*
  Grille grille
  {
    8,0,0,0,0,0,0,0,0,
    0,0,3,6,0,0,0,0,0,
    0,7,0,0,9,0,2,0,0,
    0,5,0,0,0,7,0,0,0,
    0,0,0,0,4,5,7,0,0,
    0,0,0,1,0,0,0,3,0,
    0,0,1,0,0,0,0,6,8,
    0,0,8,5,0,0,0,1,0,
    0,9,0,0,0,0,4,0,0
  };
Durée = 0.002794946s
*/
//grilles lentes pour backtracking
/*
  Grille grille
  {
    8,0,0,0,0,0,0,2,0,
    0,0,0,1,0,0,8,0,0,
    0,0,0,0,7,0,0,0,0,
    4,2,0,0,0,0,6,0,0,
    5,0,0,7,0,0,0,0,0,
    0,0,0,3,0,0,0,0,1,
    0,0,0,0,4,0,0,5,0,
    0,0,0,0,5,0,0,0,7,
    0,1,0,0,0,0,0,0,0
  };
  Durée = 0.000100115s
  */
  /*
   Grille grille
   {
     0,0,0,0,0,0,0,6,8,
     9,0,0,0,0,0,0,0,2,
     0,0,0,4,0,0,5,0,0,
     0,4,1,0,0,0,0,0,0,
     0,0,0,0,3,5,0,0,0,
     0,5,0,0,0,0,0,0,0,
     0,0,0,8,0,0,0,1,0,
     3,0,0,0,0,0,7,0,0,
     0,0,0,1,0,0,4,0,0
   };
    Durée = 0.000610073s
   */
   /*
   Grille grille
   {
     0,0,0,0,0,0,2,0,1,
     0,0,4,0,3,0,0,0,0,
     0,0,0,0,0,0,0,0,0,
     3,7,0,0,0,0,0,8,0,
     6,0,0,2,0,0,0,0,0,
     0,0,0,5,0,0,0,0,0,
     5,4,0,0,0,0,6,0,0,
     0,0,0,0,7,0,0,4,0,
     0,0,2,0,0,1,0,0,0
   };
    Durée = 0.000248812s
   */
   /*
Grille grille
  {
    0,0,0,0,0,0,3,6,0,
    4,3,0,0,0,0,0,0,0,
    0,0,0,5,0,0,7,0,0,
    0,0,0,7,6,0,0,0,0,
    2,0,0,0,0,0,0,0,5,
    0,0,0,0,0,0,1,0,0,
    0,0,0,0,4,2,0,0,8,
    0,7,0,0,0,0,0,4,0,
    0,0,1,0,0,0,0,0,0
  };
  Durée = 0.000309176s
   */
   /*
  Grille grille
  {
    2,0,0,4,0,0,6,0,0,
    0,0,0,5,0,0,0,1,0,
    0,0,0,0,0,0,0,0,0,
    5,0,0,0,8,0,0,0,4,
    3,0,0,0,2,0,0,0,0,
    0,0,0,0,0,0,0,8,0,
    0,0,0,7,0,5,3,0,0,
    0,0,8,0,0,0,0,0,2,
    0,1,0,0,0,0,0,0,0
  };
  Durée = 0.000125177s
   */
   /*
   Grille grille
   {
   4,0,0,0,0,1,0,7,0,
   7,6,0,0,0,0,5,0,0,
   0,0,0,0,0,0,0,6,0,
   0,0,0,1,0,0,0,2,5,
   0,0,0,5,0,0,0,0,3,
   0,0,0,7,6,4,0,0,0,
   0,5,0,0,0,8,0,0,0,
   0,0,2,0,0,0,8,0,9,
   0,0,1,0,4,0,0,0,0
   };//Durée = 0.0150565030s
   */
   /*
   Grille grille
   {
     0,0,0,3,6,0,0,0,0,
     0,2,0,0,0,0,8,0,0,
     0,0,0,1,0,0,0,0,0,
     1,0,0,0,0,0,0,6,0,
     0,0,0,0,0,2,5,0,0,
     0,0,0,0,0,0,0,0,7,
     0,0,8,0,0,0,2,4,0,
     3,0,0,7,0,0,0,0,0,
     0,0,0,0,4,0,0,0,5
   };
   Durée = 0.690193590s
   Durée = 0.507728416s
   */
   /*
   Grille grille
   {
     8,0,0,0,0,0,0,2,0,
     0,0,0,1,0,0,8,0,0,
     0,0,0,0,7,0,0,0,0,
     4,2,0,0,0,0,6,0,0,
     5,0,0,7,0,0,0,0,0,
     0,0,0,3,0,0,0,0,1,
     0,0,0,0,4,0,0,5,0,
     0,0,0,0,5,0,0,0,7,
     0,1,0,0,0,0,0,0,0
   };
   Durée = 0.000102642s
   */
   /*
   Grille grille
   {
     2,0,0,4,0,0,6,0,0,
     0,0,0,5,0,0,0,1,0,
     0,0,0,0,0,0,0,0,0,
     5,0,0,0,8,0,0,0,4,
     3,0,0,0,2,0,0,0,0,
     0,0,0,0,0,0,0,8,0,
     0,0,0,7,0,5,3,0,0,
     0,0,8,0,0,0,0,0,2,
     0,1,0,0,0,0,0,0,0
   };
    Durée = 0.000090707s
   */
   /*
   Grille grille
   {
     0,8,0,0,0,0,1,0,0,
     3,0,0,5,0,0,0,0,0,
     0,0,0,6,0,0,0,0,4,
     1,0,0,2,0,0,0,0,0,
     0,0,0,0,0,0,0,4,2,
     7,0,0,0,0,0,0,0,0,
     6,0,0,0,0,0,3,0,0,
     0,0,0,0,8,0,0,5,0,
     0,2,0,0,4,0,0,0,0
   };
    Durée = 0.000142504s
   */
   /*
   Grille grille
   {
     0,0,0,3,0,0,5,0,1,
     0,8,0,0,7,0,0,0,0,
     0,0,0,0,0,0,0,0,0,
     0,0,0,0,8,0,2,0,0,
     1,0,0,5,0,0,0,0,0,
     0,0,4,0,0,0,0,7,0,
     0,0,0,4,0,1,0,6,0,
     3,5,0,0,0,0,0,0,0,
     0,0,0,0,0,0,3,0,0
   };
   Durée = 0.000948058s
   Durée = 0.000163064s
   */
   /*
   Grille grille
   {
     0,3,7,0,8,0,0,0,9,
     0,0,6,0,0,0,0,0,0,
     0,0,0,1,0,0,8,0,0,
     6,0,0,0,3,0,0,0,0,
     0,5,3,4,0,0,0,7,0,
     1,0,0,0,0,0,0,0,5,
     0,0,0,0,0,2,0,4,0,
     0,9,8,0,1,0,0,0,7,
     5,0,0,0,0,0,0,0,0
   };
   Durée = 0.000723768s
   Durée = 0.000526556s
   */
   /*
    Grille grille
  {
    1,2,0,4,0,0,3,0,0,
    3,0,0,0,1,0,0,5,0,
    0,0,6,0,0,0,1,0,0,
    7,0,0,0,9,0,0,0,0,
    0,4,0,6,0,3,0,0,0,
    0,0,3,0,0,2,0,0,0,
    5,0,0,0,8,0,7,0,0,
    0,0,7,0,0,0,0,0,5,
    0,0,0,0,0,0,0,9,8
  };
  Durée = 0.021133548s
  */