#ifndef JUGADOR_H
#define JUGADOR_H

#include "estructuras.h"
#include "constantes.h"

struct jugador {
	struct comuna* comunas[MAX_COMUNAS_JUGADOR];
	int cantidad;
};

struct jugador* crear_jugador(struct lista_comunas* sociedad);
	/* Toma las primeras comunas de la lista, en el orden en que se imprimen.
		Entrada: la sociedad
		Salida: el jugador, o NULL si no hay al menos MAX_COMUNAS_JUGADOR comunas */

void liberar_jugador(struct jugador* jugador);
	/* Libera el struct del jugador. Las comunas siguen siendo de la sociedad.
		Entrada: el jugador
		Salida: el struct queda liberado */

int es_comuna_jugador(struct jugador* jugador, struct comuna* comuna);
	/* Dice si esa comuna es una de las del jugador
		Entrada: el jugador y la comuna
		Salida: 1 si es suya, 0 si no */

void imprimir_comunas_jugador(struct jugador* jugador);
	/* Imprime las comunas del jugador numeradas desde 1
		Entrada: el jugador
		Salida: la lista impresa */

struct biblioteca;

#define LECTURA_SIN_PERSONA 0
#define LECTURA_SIN_LIBROS 1
#define LECTURA_YA_LEYO 2
#define LECTURA_OK 3
#define LECTURA_APRENDIO 4
#define LECTURA_FALLO 5

struct informe_lectura {
	int caso;
	char persona[96];
	char libro[128];
	char servicio[96];
	int libros_leidos;
};

int ejecutar_lectura(struct comuna* comuna, struct biblioteca* biblioteca, struct string* texto, struct informe_lectura* informe);
	/* Busca el texto y hace leer a quien lleva menos libros. No imprime.
		Entrada: la comuna, la biblioteca, el texto y donde dejar el informe
		Salida: 1 si el turno de lectura termino, 0 si falta un puntero o la busqueda fallo */

int lectura_jugador(struct jugador* jugador, struct biblioteca* biblioteca);
	/* Una persona de una comuna del jugador busca texto y lee un libro.
		Cada 3 libros nuevos puede aprender un servicio.
		Entrada: el jugador y la biblioteca
		Salida: 1 si el turno de lectura termino, 0 si falta un puntero o la busqueda fallo */

#endif
