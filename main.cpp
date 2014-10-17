//ALUMNO: Aaron Socas Gaspar
//Seguridad curso 2013-2014
//Practica 7: RSA

#include "rsa.hpp"

int main (int argc, char* argv[])
{
  
  if (argc==2)
  {
    RSA c;
    if (c.cargar_fichero(argv[1]))
    {
      cout<<"Fichero "<<argv[1]<<" cargado con exito"<<endl;
      cout<<"Mensaje= "<<c.get_mensaje()<<endl;
      cout<<"p= "<<c.get_p()<<", q= "<<c.get_q()<<", fi(n)= "<<c.get_fi()<<", d= "<<c.get_d()<<endl;
//       c.test_coprimos(c.get_d(),c.get_fi());
//       c.test_coprimos(12,19);
      c.cifrado();
    }
    else
    {
	cout<<"Error en la carga del fichero, compruebe nombre y/o ruta y formato del mismo"<<endl;
	return 2;
    }
  }
  else
  {
      cout<<"Error de argumentos. Introduzca ./rsa nombre_fichero"<<endl;
      return 1;
  }
  return 0;
}
