#ifndef MANEJO_RECURSOS_H
#define MANEJO_RECURSOS_H


int reduccion_servicio(struct string* servicio, int cantidad, struct comuna* comuna);
/* Funcion que reduce los servicios que posee una comuna
	Entrada: servicio, cantidad, comuna
	Salida: 0 (false/error), 1 (true/exito) */

int reduccion_bien(struct string* bien, int cantidad, struct comuna* comuna);
/* Funcion que reduce los bienes que posee una comuna
	Entrada: bien, cantidad, comuna
	Salida: 0 (false/error), 1 (true/exito) */
int aumento_servicio(struct string* servicio, int cantidad, struct comuna* comuna);
/* Funcion que aumenta los servicios que posee una comuna
	Entrada: servicio, cantidad, comuna
	Salida: 0 (false/error), 1 (true/exito) */
int aumento_bien(struct string* bien, int cantidad, struct comuna* comuna);
/* Funcion que aumenta los bienes que posee una comuna
	Entrada: bien, cantidad, comuna
	Salida: 0 (false/error), 1 (true/exito) */
#endif
