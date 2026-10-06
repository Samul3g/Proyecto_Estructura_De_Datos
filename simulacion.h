#ifndef SIMULACION_H
#define SIMULACION_H

int regalo_bien(struct comuna* comuna_dar, struct comuna* comuna_recibir, char* bien, int cantidad);
/* Funcion que realiza el regalo de un bien entre dos comunas
	Entrada: comuna_dar, comuna_recibir, bien, cantidad
	Salida: 1 (true/exito)
*/

int regalo_servicio(struct comuna* comuna_dar, struct comuna* comuna_recibir, char* servicio, int cantidad);
	/* Funcion que realiza el regalo de un servicio entre dos comunas
		Entrada: comuna_dar, comuna_recibir, servicio, cantidad
		Salida: 1 (true/exito)
	*/
#endif