#include "../Comportamientos_Jugador/auxiliar.hpp"
#include <iostream>
#include "motorlib/util.h"
#include "rescatador.hpp"

Action ComportamientoAuxiliar::think(Sensores sensores)
{
	Action accion = IDLE;

	switch (sensores.nivel)
	{
	case 0:
		accion = ComportamientoAuxiliarNivel_0 (sensores);
		break;
	case 1:
		// accion = ComportamientoAuxiliarNivel_1 (sensores);
		break;
	case 2:
		// accion = ComportamientoAuxiliarNivel_2 (sensores);
		break;
	case 3:
		// accion = ComportamientoAuxiliarNivel_3 (sensores);
		break;
	case 4:
		// accion = ComportamientoAuxiliarNivel_4 (sensores);
		break;
	}

	return accion;
}

int ComportamientoAuxiliar::interact(Action accion, int valor)
{
	return 0;
}

Action ComportamientoAuxiliar::ComportamientoAuxiliarNivel_0(Sensores sensores)
{
	// El comportamiento de seguir un camino hasta encontrar un puesto base.
	// se actualizan las variables de estado
	actualizarEstado(sensores);

// se rellena el mapa resultado y el de cotas
	rellenarMapa(sensores, mapaResultado, true);
	rellenarMapa(sensores, mapaCotas, false);

	// se toma la decicison
	return tomarDecisionNivel_0(sensores);

}

Action ComportamientoAuxiliar::ComportamientoAuxiliarNivel_1(Sensores sensores)
{
}

Action ComportamientoAuxiliar::ComportamientoAuxiliarNivel_2(Sensores sensores)
{
}

Action ComportamientoAuxiliar::ComportamientoAuxiliarNivel_3(Sensores sensores)
{
}

Action ComportamientoAuxiliar::ComportamientoAuxiliarNivel_4(Sensores sensores)
{
}

void ComportamientoAuxiliar::actualizarEstado(const Sensores & sensores){
	if(sensores.superficie[0] == 'D'){
		tieneZapatillas = true;
	}
}

Action ComportamientoAuxiliar::tomarDecisionNivel_0(const Sensores & sensores){
	Action accion = IDLE;

	if(sensores.rumbo == noroeste || sensores.rumbo == suroeste) vieneDeDerechas = true;
	else if(sensores.rumbo == noreste || sensores.rumbo == sureste) vieneDeDerechas = false;
	else vieneDeDerechas = false;

//comprobamos que no ha habido colisiones anteriormente
	if(sensores.agentes[2] != '_' ){
		accion = TURN_SR;
		return accion;
	}
	// el juego termina si el agente llega a una casilla que
	// representa el puesto base
	if(sensores.superficie[0] == 'X'){
		accion = IDLE;
	}
	else if(giro45Izqda != 0){
		// si estaba intentado girar 45 a la izquierda, completar la accion
		accion = TURN_SR;
		giro45Izqda--;
	}
	else {
		int indice = indiceCasillaMasInteresante(sensores);
		switch (indice){
			case 2:
				accion = WALK;
				avanzado  = true;
				break;

			case 1: accion = TURN_SR;
				giro45Izqda = 6;
				avanzado = false;
				break;

			case 3: accion = TURN_SR;
				avanzado = false;
				break;

			case 0: accion = TURN_SR;
				//if(vieneDeDerechas) giro45Izqda = 4;
				avanzado  = false;
				break;

			case 4: accion = TURN_SR;
			giro45Izqda = 4;
			break;
		}
	}
	// almacenar la ultima accion
	ultimaAccion = accion;

vecesRecorridas(sensores.posF, sensores.posC);
	// se devuelve la accion
	return accion;
}

int ComportamientoAuxiliar::indiceCasillaMasInteresante(const Sensores & sensores){
	char frente = viablePorAltura(2, sensores);
	char izquierda = viablePorAltura(1, sensores);
	char derecha = viablePorAltura(3, sensores);

	int indice = 0;

	/*if(sensores.agentes[1] != '_' || sensores.agentes[2]  != '_'  || sensores.agentes[3]  != '_' ){
		int iA = indiceDetras(sensores.posF, sensores.posC, sensores.rumbo);
		int iID = indiceIAtras(sensores.posF, sensores.posC, sensores.rumbo);
		int iDrD= indiceDrAtras(sensores.posF, sensores.posC, sensores.rumbo);

		if( iA <= iID ) indice = 4;
		else if( iA <= iDrD) indice = 4;
		else if( iID <= iDrD) indice = 3;
		else indice = 1;
		std::cout<<"\nMe doy la vuelta porque hay un agente cerca\n";
	}else if(frente == 'X' && sensores.agentes[2] !='_' ){
		indice = 4;
	}else if( frente == 'X'){
		 indice = 2;
	}else if(izquierda == 'X' && sensores.agentes[1] != '_'){
		indice = 4;
	}else if(izquierda == 'X'){
		indice = 1;
	}else if(derecha == 'X' && sensores.agentes[3] != '_' ){
		indice = 4;
	}else if (derecha == 'X'){
		indice = 3;
	}*/

	if(frente == 'X' && sensores.agentes[2] !='r' ){
		indice = 2;
		std::cout<<"\nMe meto porque no hay nadie metido en esta casilla\n";
	}else if(izquierda == 'X' && sensores.agentes[1] != 'r'){
		indice = 1;
		std::cout<<"\nMe meto porque no hay nadie metido en esta casilla izquierda\n";
		std::cout<<sensores.agentes[1];
	}else if(derecha == 'X' && sensores.agentes[3] != 'r' ){
		indice = 4;
	}else indice = 0;


if (indice == 0) {
	int iF = indiceFrente(sensores.posF, sensores.posC, sensores.rumbo);
	int iI = indiceIzquierda(sensores.posF, sensores.posC, sensores.rumbo);
	int iD = indiceDerecha(sensores.posF, sensores.posC, sensores.rumbo);
	int iA = indiceDetras(sensores.posF, sensores.posC, sensores.rumbo);
	int iID = indiceIAtras(sensores.posF, sensores.posC, sensores.rumbo);
	int iDrD= indiceDrAtras(sensores.posF, sensores.posC, sensores.rumbo);

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
	else if( indice == 0){
		if( iA <= iID ) indice = 4;
		else if( iA <= iDrD) indice = 4;
		else if( iID <= iDrD) indice = 3;
		else indice = 1;
	}
}

//mostrarMatriz();
//std::cout<<"\n\n\n\n";
	std::cout<<"\n El indice es: "<<indice<<endl;
	return indice;
}

char ComportamientoAuxiliar::viablePorAltura(int indice, const Sensores &sensores){
	//buscamos el caracter de interes
	char terrenoIndice = sensores.superficie[indice];

	//determinamos la altura de la posicion actual
	int alturaActual = sensores.cota[0];

	//determinamos la altura de la posición dada por el indice
	int alturaIndice = sensores.cota[indice];

	//se calcula la diferencia
	int dif = abs(alturaIndice - alturaActual);

	if(!(dif <2 )){
		terrenoIndice = 'P';
	}
	//se devuelve el caracter
	return terrenoIndice;
}
int ComportamientoAuxiliar::indiceFrente(int f, int c, Orientacion o) {
		switch (o) {
		case norte:     return matrizRecorridos[f-1][c];
		case noreste:   return matrizRecorridos[f-1][c+1];
		case este:      return matrizRecorridos[f][c+1];
		case sureste:   return matrizRecorridos[f+1][c+1];
		case sur:  return 0;    std::cout<<"\n El indice de la casilla de abajo es: "<<matrizRecorridos[f+1][c]<<endl;
		return matrizRecorridos[f+1][c];
		case suroeste:  return matrizRecorridos[f+1][c-1];
		case oeste:     return matrizRecorridos[f][c-1];
		case noroeste:  return matrizRecorridos[f-1][c-1];
	}
	return 9999; // por seguridad
}

int ComportamientoAuxiliar::indiceIzquierda(int f, int c, Orientacion o) {
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

int ComportamientoAuxiliar::indiceDerecha(int f, int c, Orientacion o) {
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

int ComportamientoAuxiliar::indiceDrAtras(int f, int c, Orientacion o){
	 int iDr = 999;
    switch(o){
		case norte:
 			iDr = matrizRecorridos[f+1][c+1];
			break;
        case noreste:
            iDr = matrizRecorridos[f+1][c];
            break;
        case este:
            iDr = matrizRecorridos[f+1][c-1];
            break;
        case sureste:
            iDr = matrizRecorridos[f][c-1];
            break;
        case sur:
            iDr = matrizRecorridos[f-1][c-1];
            break;
        case suroeste:
            iDr = matrizRecorridos[f-1][c];
            break;
        case oeste:
            iDr = matrizRecorridos[f-1][c+1];
            break;
        case noroeste:
            iDr = matrizRecorridos[f][c+1];
            break;
    }
    return iDr;
}
int ComportamientoAuxiliar::indiceIAtras(int f, int c, Orientacion o) {
	int iI = 999;
    switch(o){
		case norte:
			iI = matrizRecorridos[f+1][c-1];
			break;
        case noreste:
            iI = matrizRecorridos[f][c-1];
            break;
        case este:
            iI = matrizRecorridos[f-1][c-1];
            break;
        case sureste:
            iI = matrizRecorridos[f-1][c];
            break;
        case sur:
            iI = matrizRecorridos[f-1][c+1];
            break;
        case suroeste:
            iI = matrizRecorridos[f][c+1];
            break;
        case oeste:
            iI = matrizRecorridos[f+1][c+1];
            break;
        case noroeste:
            iI = matrizRecorridos[f+1][c];
            break;
    }
    return iI;
}
int ComportamientoAuxiliar::indiceDetras(int f, int c, Orientacion o){
    int iD = 999;
    switch(o){
        case norte:
            iD = matrizRecorridos[f+1][c];
            break;
        case noreste:
            iD = matrizRecorridos[f+1][c-1];
            break;
        case este:
            iD = matrizRecorridos[f][c-1];
            break;
        case sureste:
            iD = matrizRecorridos[f-1][c-1];
            break;
        case sur:
            iD = matrizRecorridos[f-1][c];
            break;
        case suroeste:
            iD = matrizRecorridos[f-1][c+1];
            break;
        case oeste:
            iD = matrizRecorridos[f][c+1];
            break;
        case noroeste:
            iD = matrizRecorridos[f+1][c+1];
            break;
    }

    return iD;
}


void ComportamientoAuxiliar::vecesRecorridas(int f, int c){
	if (avanzado) {
		matrizRecorridos[f][c]++;
	}
}

void ComportamientoAuxiliar::mostrarMatriz(){
	  for (int i = 0; i < tam; i++) {
        for (int j = 0; j < tam; j++) {
            cout << matrizRecorridos[i][j] << " ";
        }
        cout << endl;
    }
}

