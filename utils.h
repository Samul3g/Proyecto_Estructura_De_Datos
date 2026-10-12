#ifndef UTILS_H
#define UTILS_H

#include <stdio.h>
#include "estructuras.h"

int mismos_nombres(struct string* a, struct string* b);
	/* Compara dos strings caracter por caracter
		Entrada: los dos strings
		Salida: 1 si son iguales, 0 si no */

int aleatorio(int minimo, int maximo);
	/* Funcion que retorna un numero pseudo aleatorio entre el minimo y el maximo dados.
	* Entradas: minimo, maximo
	* Salida: numero entero (numero_aleatorio) 
	*/

int siguiente_caracter(FILE* archivo);
	/* Obtiene el siguiente caracter del archivo
		Entrada: el archivo
		Salida: el siguiente caracter del archivo */

struct string* linea_archivo(FILE* archivo);
	/* Obtiene la linea del archivo
		Entrada: el archivo
		Salida: la linea del archivo */

struct lista_archivo* generar_lista_archivo(FILE* archivo);
	/* Genera la lista de archivos a partir de un archivo de lectura
		Entrada: el archivo
		Salida: la lista de archivos */

FILE* leer_archivo(char* nombre);
	/* Lee el archivo y lo devuelve como archivo de lectura
		Entrada: el nombre del archivo
		Salida: el archivo de lectura */

struct nodo_recurso* crear_recurso_desde_linea(struct string* linea, int cantidad_personas);
	/* Arma un recurso a partir de una linea nombre,necesidad,maximo,relevancia
		Entrada: la linea y cuantas personas viven en la comuna
		Salida: el recurso, con el maximo calculado y cantidad en 0 */

#endif
