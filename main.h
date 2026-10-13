#ifndef MAIN_H
#define MAIN_H

#include "estructuras.h"

extern int cantidad_comunas;
extern int bienes;
extern int servicios;
extern int personas;

/* Funcion de impresion del logo y saludo inicial
 * Entradas: nada
 * Salidas: nada
 */
void saludo_inicio();


/* Funcion que pide los valores de las comunas, bienes y servicios que el usuario pida y los coloca en variables globales
 * Entradas: nada
 * Salidas: ninguna*/
void configuracion_inicial();


/*Funcion que imprime una lista de recursos
 * Entradas: lista recursos
 * Salidas: impresion en la consola y 0 en caso de fallo, 1 en caso de exito
 */
 int imprimir_recurso(struct lista_recursos* lista);
	 
	 
/*Funcion que pide al usuario la comuna que regala y recibe y el servicio o bien a regalar y luego da el regalo
 * Entradas: Lista comunas
 * Salidas: 1 para exito, 0 para fracaso*/
int usuario_intercambio(struct lista_comunas* lista_comunas);

#endif
