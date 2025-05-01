#include "../Comportamientos_Jugador/rescatador.hpp"
#include "motorlib/util.h"
#include "cstdlib"
#include "ctime"
#include "iostream"

Action ComportamientoRescatador::think(Sensores sensores)
{
	Action accion = IDLE;

	switch (sensores.nivel)
	{
	case 0:
		accion = ComportamientoRescatadorNivel_0 (sensores);
		//std::cout<<"\npos jugador = "<<sensores.posF<<","<<sensores.posC<<"\n"<<" orientacion: "<<sensores.rumbo<<"\n";
		break;
	case 1:
		// accion = ComportamientoRescatadorNivel_1 (sensores);
		break;
	case 2:
		// accion = ComportamientoRescatadorNivel_2 (sensores);
		break;
	case 3:
		// accion = ComportamientoRescatadorNivel_3 (sensores);
		break;
	case 4:
		// accion = ComportamientoRescatadorNivel_4 (sensores);
		break;
	}

	return accion;
}

int ComportamientoRescatador::interact(Action accion, int valor)
{
	return 0;
}

Action ComportamientoRescatador::ComportamientoRescatadorNivel_0(Sensores sensores)
{
	// El comportamiento de seguir un camino hasta encontrar un puesto base.
    // se actualizan las variables de estado
    actualizarEstado(sensores);

    // se toma la decicison
    return tomarDecisionNivel_0(sensores);
}

Action ComportamientoRescatador::ComportamientoRescatadorNivel_1(Sensores sensores)
{
}

Action ComportamientoRescatador::ComportamientoRescatadorNivel_2(Sensores sensores)
{
}

Action ComportamientoRescatador::ComportamientoRescatadorNivel_3(Sensores sensores)
{
}

Action ComportamientoRescatador::ComportamientoRescatadorNivel_4(Sensores sensores)
{
}


//******* metodos privados ****/


/**
 * metodo para actualiza el estado en base al contenido
 * de los sensores
 * @param sensores
 */
void ComportamientoRescatador::actualizarEstado(const Sensores & sensores){
  //almacenar la informacion sobre las zapatillas
    if(sensores.superficie[0] == 'D' ){
      tieneZapatillas = true;
     }

	//se rellena el mapa resultado y el de cotas
	rellenarMapa(sensores, mapaResultado, true);
	rellenarMapa(sensores, mapaCotas, false);
}

/**
 * metodo para la toma de decisiones
 * @param sensores
 * @return
 */
Action ComportamientoRescatador::tomarDecisionNivel_0(const Sensores & sensores){
  Action accion = IDLE;

  // el juego termina si el agente llega a una casilla que
  // representa el puesto base
  if(sensores.superficie[0] == 'X'){
    	accion = IDLE;
  }else if (dandoMediaVuelta) {
  	if (pasosMediaVuelta < 2) {
  		accion = TURN_L;
  		pasosMediaVuelta++;
  	} else {
  		// Terminamos la media vuelta
  		dandoMediaVuelta = true;
  		pasosMediaVuelta = 0;
  	}
  }else if(giro45Izqda != 0){
  	// si estaba intentado girar 45 a la izquierda, completar la accion
  	accion = TURN_SR;
  	giro45Izqda--;
  }else {
    int indice = indiceCasillaMasInteresante(sensores);
    switch (indice){
      case 2: accion = WALK;
		avanzado = true;
        break;

      case 1: accion = TURN_L;
		giro45Izqda = 1;
		avanzado = false;
        break;

      case 3: accion = TURN_SR;
		avanzado = false;
        break;

      case 0: accion = TURN_L;
		avanzado = false;
        break;

    }
  }


  // almacenar la ultima accion
  ultimaAccion = accion;

	vecesRecorridas(sensores.posF, sensores.posC);
  // se devuelve la accion
  return accion;
}


/**
* metodo para determinar el indice de la casilla mas interesante
* de las que son accesibles directamente : frente, izquierda o derecha
* @param sensores
* @return
*/
int ComportamientoRescatador::indiceCasillaMasInteresante(const Sensores & sensores){
  char frente = viablePorAltura(2, sensores);
  char izquierda = viablePorAltura(1, sensores);
  char derecha = viablePorAltura(3, sensores);

  int indice = 0;

  if(frente == 'X' ){
    indice = 2;
	frente = 'P';
  }
  else if(izquierda == 'X'){
    indice = 1;
  }
  else if(derecha == 'X'){
    indice = 3;
  }
  else if(!tieneZapatillas){
    if(frente == 'D'){
      indice = 2;
		tieneZapatillas = true;
    }
    else if(izquierda == 'D'){
      indice = 1;
    	tieneZapatillas = true;

    }
    else if(derecha == 'D'){
      indice = 3;
    	tieneZapatillas = true;

    }
  }

	if (indice == 0) {
		int iF = indiceFrente(sensores.posF, sensores.posC, sensores.rumbo);
		int iI = indiceIzquierda(sensores.posF, sensores.posC, sensores.rumbo);
		int iD = indiceDerecha(sensores.posF, sensores.posC, sensores.rumbo);

		if (frente == 'C' && izquierda == 'C' && derecha == 'C') {
			if (iF <= iI) indice = 2;
			else if (iI <= iD) indice = 1;
			else indice = 3;
		} else if (frente == 'C' && izquierda == 'C') {
			if (iF <= iI) indice = 2;
			else indice = 1;
		} else if (frente == 'C' && derecha == 'C') {
			if (iF <= iD) indice = 2;
			else indice = 3;
		} else if (izquierda == 'C' && derecha == 'C') {
			if (iI <= iD) indice = 1;
			else indice = 3;
		} else if (frente == 'C') indice = 2;
		else if (izquierda == 'C') indice = 1;
		else if (derecha == 'C') indice = 3;
	}


  return indice;
}

char ComportamientoRescatador::viablePorAltura(int indice, const Sensores & sensores){

//buscamos el caracter de interes
char terrenoIndice = sensores.superficie[indice];

//determinamos la altura de la posicion actual
int alturaActual = sensores.cota[0];

//determinamos la altura de la posición dada por el indice
int alturaIndice = sensores.cota[indice];

//se calcula la diferencia
int dif = abs(alturaIndice - alturaActual);

//si la diferencia es menor de 2 o 2 y hay zapatillas, se devuelve
//el terreno; en caso contrario, se devuelve P. Tambien si hay otro
//agente en esa posicion
if(!(dif <2 or (tieneZapatillas and dif < 3)) or sensores.agentes[indice] != '_'){
 terrenoIndice = 'P';
}

//se devuelve el caracter
return terrenoIndice;
}
void ComportamientoRescatador::vecesRecorridas(int f, int c){
	if (avanzado) {
		matrizRecorridos[f][c]++;
	}
}

int ComportamientoRescatador::indiceFrente(int f, int c, Orientacion o) {
	switch (o) {
		case norte:     return matrizRecorridos[f-1][c];
		case noreste:   return matrizRecorridos[f-1][c+1];
		case este:      return matrizRecorridos[f][c+1];
		case sureste:   return matrizRecorridos[f+1][c+1];
		case sur:       return matrizRecorridos[f+1][c];
		case suroeste:  return matrizRecorridos[f+1][c-1];
		case oeste:     return matrizRecorridos[f][c-1];
		case noroeste:  return matrizRecorridos[f-1][c-1];
	}
	return 9999; // por seguridad
}

int ComportamientoRescatador::indiceIzquierda(int f, int c, Orientacion o) {
	switch (o) {
		case norte:    return matrizRecorridos[f-1][c-1];
		case noreste:  return matrizRecorridos[f-1][c];
		case este:     return matrizRecorridos[f-1][c+1];
		case sureste:  return matrizRecorridos[f][c+1];
		case sur:      return matrizRecorridos[f+1][c+1];
		case suroeste: return matrizRecorridos[f+1][c];
		case oeste:    return matrizRecorridos[f+1][c-1];
		case noroeste: return matrizRecorridos[f][c-1];
	}
	return 9999;
}

int ComportamientoRescatador::indiceDerecha(int f, int c, Orientacion o) {
	switch (o) {
		case norte:    return matrizRecorridos[f-1][c+1];
		case noreste:  return matrizRecorridos[f][c+1];
		case este:     return matrizRecorridos[f+1][c+1];
		case sureste:  return matrizRecorridos[f+1][c];
		case sur:      return matrizRecorridos[f+1][c-1];
		case suroeste: return matrizRecorridos[f][c-1];
		case oeste:    return matrizRecorridos[f-1][c-1];
		case noroeste: return matrizRecorridos[f-1][c];
	}
	return 9999;
}
