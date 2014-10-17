#include "rsa.hpp"

RSA::RSA (void)
{
  p_ =0; q_=0; fi_=0; d_=0; e_=0;
}

bool RSA::cargar_fichero (char fichero[])
{
  ifstream flujo;
  flujo.open(fichero);
  if (flujo.is_open())
  {
    getline(flujo, mensaje_);
    flujo>>p_;
    if (!test_primalidad(p_))
    {
      cout<<"Error: p= "<<p_<<" no es primo"<<endl;
      flujo.close();
      return false;
    }
//     cout<<"***********************"<<endl;
    flujo>>q_;
    if (!test_primalidad(q_))
    {
      cout<<"Error: q= "<<q_<<" no es primo"<<endl;
      flujo.close();
      return false;
    }
    
    flujo>>d_;
    flujo.close();
    fi_=(p_-1)*(q_-1);
    return true;
  }
  else
    return false;
};

string RSA::get_mensaje (void)
{
  return mensaje_;
}

string RSA::get_mensaje_color (unsigned inicio, unsigned tam)
{
  string mensaje = quitar_espacios(mensaje_);
  Color::Modifier def(Color::BG_DEFAULT);
  Color::Modifier red(Color::BG_RED);
  stringstream aux;
  unsigned marca=0;
  unsigned MAX = mensaje.size();
  for (unsigned i=0; i<MAX; i++)
  {
      if ((i>=inicio)&&(marca<tam)&&(mensaje[i]!=' '))
      {
	aux<<red<<mensaje[i]<<def;
	marca++;
      }
      else
	aux<<mensaje[i];
  }
  return aux.str();
}

unsigned RSA::get_p (void)
{
  return p_;
}

unsigned RSA::get_q (void)
{
  return q_;
}

unsigned RSA::get_fi (void)
{
  return fi_;
}

unsigned RSA::get_d (void)
{
  return d_;
}

//Test de primalidad de Lehman y Peralta: calcula por probabilidad si un num es primo o no
bool RSA::test_primalidad (unsigned n)
{
  const unsigned ELEM= 5;
  unsigned v[ELEM] = {2,3,5,7,11};
  for (short i=0; (i<ELEM)&&(n!=v[i]); i++){
    if (n%v[i]==0)
      return false; //es compuesto => no es primo
    }
  
  vector<unsigned> a;
  srand (time(NULL));
  unsigned TEST;
  if (n<101) //si es =<100 tomamos su valor para evitarnos iteraciones innecesarias
    TEST= n;
  else
    TEST= 100;
  for (unsigned i=0; i<TEST; i++)
  {
    if (TEST==n)
      a.push_back(i+1);
    else
      a.push_back(rand() % (n-1)+1); //numeros aleatorios entre 1 y (p_-1)
  }

  unsigned cont=0;
  for (unsigned i=0; i<TEST; i++)
  {
    unsigned r = exp_rap(a[i], (n-1)/2, n);
    if (r==1)
      cont++;
    if ((r!=(n-1)) && (r!=1))
      return true; //es primo, no hace falta seguir haciendo iteraciones
//     cout<<r<<endl;
  }
  if (cont==TEST) //para todo i, ai=1
    return false; //es compuesto
  return true; //en otro caso, es primo
}

//Algoritmo de Euclides
bool RSA::test_coprimos (unsigned a, unsigned b)
{
  vector<int> x,z;
  if (a>b)
  {
    x.push_back(a);
    x.push_back(b);
  }
  else
  {
    x.push_back(b);
    x.push_back(a);
  }
  z.push_back(0);
  z.push_back(1);

  unsigned i=1;
  while ((x[i-1]%x[i])!=0)
  {
    x.push_back(x[i-1]%x[i]);
    z.push_back(-(x[i-1]/x[i])*z[i]+z[i-1]);
    i++;
  }
  
  cout<<"Test coprimos: alg de Euclides"<<endl;
  cout<<"x= ";
  for (unsigned i=0; i<x.size();i++)
    cout<<x[i]<<" ";
  cout<<endl;
  
  cout<<"z= ";
  for (unsigned i=0; i<z.size();i++)
    cout<<z[i]<<" ";
  cout<<endl;

  cout<<"EL mcd= "<<x[i]<<endl;
//   cout<<"EL mcd= "<<x[0]<<endl;
  if (x[i]==1)
  {
    if (z[i] < 0)
      z[i] = fi_+ z[i];
    cout<<"el inv es (e) = "<<z[i]<<endl;
    e_ = z[i];
    return true;
  }
  return false;
}



unsigned RSA::exp_rap (unsigned base, unsigned exp, unsigned m)
{
  //P1
  bitset<BITS> b(exp);
  //P2
  unsigned long long r=1;
  //P3
  for (int i=(BITS-1); i>-1; i--)
  {
      r=(r*r)%m;
      if (b[i]==1)
	r=(r*base)%m;
  }
  return r;
}


unsigned RSA::tam_bloque (void)
{
  unsigned n = p_*q_;
  short j=0;
  while (exp_rap(BASE, j, INF) < n)
//   while (pow(BASE,j)<n)
    j++;
  
  return j-1;
}


unsigned RSA::buscar (char a)
{
  for (unsigned i=0; i<BASE;i++)
  {
    if (ALFABETO[i]==a)
      return i;
  }
  return -1;
}

unsigned RSA::codificacion_numerica (string cad, unsigned tam)
{  
  unsigned r=0;
  for (unsigned i=0; i<tam; i++)
     r += buscar(cad[i])*exp_rap(BASE,tam-(i+1),INF);
  return r;
}



string RSA::quitar_espacios (string cad)
{
  string aux;
  aux.clear();
  for (unsigned i=0; i<cad.size();i++)
    if (cad[i]!=' ')
      aux+= cad[i];
  return aux;
}

void RSA::cifrado (void)
{
  Color::Modifier def(Color::BG_DEFAULT);
  Color::Modifier blue(Color::BG_BLUE);
  
  vector<unsigned> codi, cif;
  string men = quitar_espacios (mensaje_);
  
  unsigned tam = tam_bloque();
  unsigned n = p_*q_;
  cout<<"\nCIFRADO"<<endl;
  cout<<"======="<<endl;
  cout<<"n = "<<n<<endl;
  cout<<"Tam bloque= "<<tam<<endl;

  if (test_coprimos(d_,fi_))  
  {
      for (unsigned i=0; i<men.size();i++)
      {
	  string cad (men,i,tam);
	  cout<<"\n\t"<<get_mensaje_color(i,tam)<<" => "<<cad<<endl;
	  i += tam-1;
	  unsigned cod= codificacion_numerica(cad,tam);
	  codi.push_back(cod);
	  cout<<"\tBloque obtenido = "<<cod<<endl;
	  cif.push_back(exp_rap(cod, e_, n));
	  cout<<"\t"<<blue<<"Cifrado = "<<def<<cif[cif.size()-1]<<endl;
      }
   }
   else
      cout<<"\nNo existe valor inverso (e), d y fi(n) no son coprimos."<<endl;
  
  cout<<"\nBLOQUE : ";
  for (unsigned i=0; i<codi.size(); i++)
    cout<<codi[i]<<" ";
  cout<<endl;
  cout<<"CIFRADO: ";
  for (unsigned i=0; i<cif.size(); i++)
    cout<<cif[i]<<" ";
  cout<<endl;
}

