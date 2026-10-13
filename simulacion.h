#ifndef SIMULACION_H
#define SIMULACION_H

int emergencia(struct comuna* comuna);
/* Funcion que realiza la emergencia de un bien o un servicio en una comuna
	Entrada: comuna
	Salida: 1 (true/exito)
*/

int prosperidad(struct comuna* comuna);
/* Funcion que realiza la prosperidad de un bien o un servicio en una comuna
	Entrada: comuna
	Salida: 1 (true/exito)
*/


int usuario_intercambio(struct lista_comunas* lista_comunas);
/*Funcion que pide al usuario la comuna que regala y recibe y el servicio o bien a regalar y luego da el regalo
 * Entradas: Lista comunas
 * Salidas: 1 para exito, 0 para fracaso*/
 
int reduccion_bienes_servicios(struct lista_comunas* sociedad);
/* Funcion que reduce la cantidad de bienes y servicios en una comuna
	Entrada: sociedad
	Salida: 1 (true/exito)
*/

int calcular_indice_sociedad(struct lista_comunas* sociedad);
/* Funcion que calcula el indice de la sociedad
	Entrada: sociedad
	Salida: 1 (true/exito)
*/

struct comuna* comuna_random(struct lista_comunas* sociedad);
/* Funcion que devuelve una comuna aleatoria de la lista de comunas
	Entrada: sociedad
	Salida: comuna
*/

int paso_dia(struct lista_comunas* sociedad);
/* Funcion que realiza un paso de dia en la simulacion
	Entrada: sociedad
	Salida: 1 (true/exito)
*/

struct nodo_lista_recursos* recurso_aleatorio(struct comuna* comuna);
/* Funcion que elige un bien o un servicio al azar de la comuna
	Entrada: comuna
	Salida: el nodo elegido, o NULL si esa lista esta vacia
*/
 
#endif
