#ifndef COMPORTAMIENTORESCATADOR_H
#define COMPORTAMIENTORESCATADOR_H

#include <chrono>
#include <time.h>
#include <thread>

#include "comportamientos/comportamiento.hpp"

struct DFC{
	int df;
	int dc;
};

static DFC calcularDiferencias(int indice, Orientacion orientacion);
/**
* vector base para las diferencias para los arrays de observaciones
* para las orientaciones norte, sur, ese y oeste
*/
 static vector<DFC> difBase = {
 {0, 0},
 {-1, -1},
 {-1, 0},
 {-1, 1},
 {-2, -2},
 {-2, -1},
 {-2, 0},
 {-2, 1},
 {-2, 2},
 {-3, -3},
 {-3, -2},
 {-3, -1},
 {-3, 0},
 {-3, 1},
 {-3, 2},
 {-3, 3}
};

/**
* vector base para las diferencias para los arrays de observaciones
* para las orientaciones combinadas
*/
static vector<DFC> difCombinadas = {
 {0, 0},
 {-1, 0},
 {-1, 1},
 {0, 1},
 {-2, 0},
 {-2, 1},
 {-2, 2},
 {-1, 2},
 {0, 2},
 {-3, 0},
 {-3, 1},
 {-3, 2},
 {-3, 3},
 {-2, 3},
 {-1, 3},
 {0, 3}
};


static DFC calcularDiferencias(int indice, Orientacion orientacion){
 DFC resultado = {0,0};
 switch(orientacion){
 	case norte :
 		resultado = difBase[indice];
 		break;
 	case sur :
 		resultado.df = difBase[indice].df * (-1);
 		resultado.dc = difBase[indice].dc * (-1);
 		break;
 	case este :
 		resultado.df = difBase[indice].dc;
 		resultado.dc = difBase[indice].df * (-1);
 		break;
 	case oeste :
 		resultado.df = difBase[indice].dc * (-1);
 		resultado.dc = difBase[indice].df;
 		break;
 	case noreste :
 		resultado= difCombinadas[indice];
 		break;
 	case sureste:
 		resultado.df = difCombinadas[indice].dc;
 		resultado.dc = difCombinadas[indice].df * (-1);
 		break;
 	case noroeste:
 		resultado.df = difCombinadas[indice].dc * (-1);
 		resultado.dc = difCombinadas[indice].df;
 		break;
 	case suroeste:
 		resultado.df = difCombinadas[indice].df * (-1);
 		resultado.dc = difCombinadas[indice].dc * (-1);
 	break;
 }

 // se devuelve el resultado
 return resultado;
}

/**
 * metodo para rellenar el mapa de resultado o el de cotas
 * @param sensores
 * @param mapa
 * @param superficie flag booleano para indicar si se rellena el
 * mapa de superficie o el de cota
 */
 static void rellenarMapa(const Sensores & sensores,
 vector< vector< unsigned char> > & mapa, bool superficie){
 // se consideran las 16 posiciones del vector de informacion
 int filaDest = 0, columnaDest = 0;

 // inicialmente asumimos que se copia el vector de superficie
 vector<unsigned char> vectorInfo = sensores.superficie;

 // en caso contrario, se reasigna al de cotas
 if(!superficie){
 vectorInfo = sensores.cota;
 }

 // se calculan las diferencias y se hace la copia sobre el mapa
 // pasado como argumento
 DFC diferencias;
 for(int i=0; i < 16; i++){
 	diferencias = calcularDiferencias(i, sensores.rumbo);
 	filaDest = sensores.posF + diferencias.df;
 	columnaDest = sensores.posC + diferencias.dc;
 	mapa[filaDest][columnaDest] = vectorInfo[i];
 }
}




class ComportamientoRescatador : public Comportamiento
{

public:
  ComportamientoRescatador(unsigned int size = 0) : Comportamiento(size)
  {
    // Inicializar Variables de Estado Niveles 0,1,4
    ultimaAccion = IDLE;
    tieneZapatillas = false;
    giro45Izqda = 0;
	matrizRecorridos = vector<vector<int>>(size,vector<int>(size,0));
	avanzado  = true;
	tam = size;
	bool dandoMediaVuelta = false;
	int pasosMediaVuelta = 0;

  }
  ComportamientoRescatador(std::vector<std::vector<unsigned char>> mapaR, std::vector<std::vector<unsigned char>> mapaC) : Comportamiento(mapaR,mapaC)
  {
    // Inicializar Variables de Estado Niveles 2,3
  }
  ComportamientoRescatador(const ComportamientoRescatador &comport) : Comportamiento(comport) {}
  ~ComportamientoRescatador() {}

  Action think(Sensores sensores);

  int interact(Action accion, int valor);

  Action ComportamientoRescatadorNivel_0(Sensores sensores);
  Action ComportamientoRescatadorNivel_1(Sensores sensores);
  Action ComportamientoRescatadorNivel_2(Sensores sensores);
  Action ComportamientoRescatadorNivel_3(Sensores sensores);
  Action ComportamientoRescatadorNivel_4(Sensores sensores);

private:
  // Variables de Estado

  // ultima accion realizada
Action ultimaAccion;

  // para recordar si se tienen las zapatillas
bool tieneZapatillas;

  //para controlar el giro de 45 a la izquierda
int giro45Izqda;
vector<vector<int>> matrizRecorridos;

bool avanzado;
int tam;
bool dandoMediaVuelta;
int pasosMediaVuelta;

bool giro90grados;
  /**
 * metodo para actualiza el estado en base al contenido
 * de los sensores
 * @param sensores
*/
  void actualizarEstado(const Sensores & sensores);

  /**
 * metodo para la toma de decisiones
 * @param sensores
 * @return
 */
  Action tomarDecisionNivel_0(const Sensores & sensores);

 /**
 * metodo para determinar el indice de la casilla mas interesante
 * de las que son accesibles directamente : frente, izquierda o derecha
 * @param sensores
 * @return
 */
 int indiceCasillaMasInteresante(const Sensores & sensores);

 char viablePorAltura(int indice, const Sensores & sensores);


void vecesRecorridas(int f, int c);

bool tieneMasIndiceIzquierda(int f, int c, Orientacion orientacion);
bool tieneMasIndiceFrenteI(int f, int c, Orientacion orientacion);
bool tieneMasIndiceFrenteD(int f, int c, Orientacion orientacion);

int indiceFrente(int f, int c, Orientacion o);
int indiceIzquierda(int f, int c, Orientacion o);
int indiceDerecha(int f, int c, Orientacion o);
void mostrarMatriz();
};

#endif