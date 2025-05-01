#ifndef COMPORTAMIENTOAUXILIAR_H
#define COMPORTAMIENTOAUXILIAR_H

#include <chrono>
#include <time.h>
#include <thread>

#include "comportamientos/comportamiento.hpp"

class ComportamientoAuxiliar : public Comportamiento
{

public:
    ComportamientoAuxiliar(unsigned int size = 0) : Comportamiento(size)
    {
        // Inicializar Variables de Estado Niveles 0,1,4
        ultimaAccion = IDLE;
        tieneZapatillas = false;
        giro45Izqda = 0;
        matrizRecorridos = vector<vector<int>>(size,vector<int>(size,0));
        avanzado  = false;
		tam = size;
		vieneDeDerechas = false;
    }
    ComportamientoAuxiliar(std::vector<std::vector<unsigned char>> mapaR, std::vector<std::vector<unsigned char>> mapaC) : Comportamiento(mapaR,mapaC)
    {
        // Inicializar Variables de Estado Niveles 2,3
    }
    ComportamientoAuxiliar(const ComportamientoAuxiliar &comport) : Comportamiento(comport) {}
    ~ComportamientoAuxiliar() {}

    Action think(Sensores sensores);

    int interact(Action accion, int valor);

    Action ComportamientoAuxiliarNivel_0(Sensores sensores);
    Action ComportamientoAuxiliarNivel_1(Sensores sensores);
    Action ComportamientoAuxiliarNivel_2(Sensores sensores);
    Action ComportamientoAuxiliarNivel_3(Sensores sensores);
    Action ComportamientoAuxiliarNivel_4(Sensores sensores);

private:
    // Definir Variables de Estado

    Action ultimaAccion;

    //disponibilidad de zapatillas
    bool tieneZapatillas;

    // almacena la realizacion de giro a la izquierda
    int giro45Izqda;

    vector<vector<int>> matrizRecorridos;
    bool avanzado;
	int tam;
	bool vieneDeDerechas;

    void actualizarEstado(const Sensores & sensores);

    Action tomarDecisionNivel_0(const Sensores & sensores);

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
int indiceDetras(int f, int c, Orientacion o);
int indiceIAtras(int f, int c, Orientacion o);
int indiceDrAtras(int f, int c, Orientacion o);


};

#endif