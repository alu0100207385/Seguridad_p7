#include <vector>
#include <cstdlib>
#include <bitset>
#include <sstream>

#include "mias.hpp"

using namespace std;

const unsigned BITS = 32;
const unsigned BASE = 26;
const unsigned int INF = 429496729;
const char ALFABETO[] = {'A','B','C','D','E','F','G','H','I','J',
			 'K','L','M','N','O','P','Q','R','S','T',
			 'U','V','W','Y','Z'};

class RSA
{
  private:
    unsigned p_, q_, fi_, d_;
    int long e_;
    string mensaje_;
    bool test_primalidad (unsigned n);
    unsigned exp_rap (unsigned base, unsigned exp, unsigned m);
    unsigned tam_bloque (void);
    unsigned buscar (char a);
    unsigned codificacion_numerica (string cad, unsigned tam);
    string quitar_espacios (string cad);
    string get_mensaje_color (unsigned inicio, unsigned tam);
    
  public:
    RSA();
    inline ~RSA(){};
    bool cargar_fichero (char fichero[]);
    string get_mensaje (void);
    unsigned get_p (void);
    unsigned get_q (void);
    unsigned get_fi (void);
    unsigned get_d (void);
    bool test_coprimos (unsigned a, unsigned b);
    void cifrado (void);
};