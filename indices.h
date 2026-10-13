#ifndef INDICES_H
#define INDICES_H

#include "estructuras.h"

/* Funcion que calcula que tan vacio esta el recurso en la comuna
	Entrada: nodo lista recurso
	Salida: float entre 0 y 1. 0 si no hay nada, 1 si esta lleno */
float carencia(struct nodo_lista_recursos* nodo);

/* Funcion que va calculando las sumatorias del numerador y del denominador
	Entrada: la lista de recursos
	Salida: 0 si recorrio la lista, -1 si la lista es nula */
float sumatoria_recursos(struct lista_recursos* lista);

/* Funcion que calcula el indice de necesidad de una comuna
	Entrada: la comuna
	Salida: el indice de necesidad */
float calcular_necesidad(struct comuna* comuna);

/* Funcion que calcula la satisfaccion de una comuna
	Entrada: el nodo de la lista de comunas
	Salida: el indice de satisfaccion */
float calcular_satisfaccion(struct nodo_lista_comunas* nodo_lista_comunas);

#endif
