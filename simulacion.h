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
 
 
#endif
