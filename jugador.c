#include <stdio.h>
#include <stdlib.h>
#include "jugador.h"
#include "biblioteca.h"
#include "busqueda.h"
#include "utils.h"
#include "simulacion.h"

/* Toma las primeras comunas de la lista, en el orden en que se imprimen.
	Entrada: la sociedad
	Salida: el jugador, o NULL si no hay al menos MAX_COMUNAS_JUGADOR comunas */
struct jugador* crear_jugador(struct lista_comunas* sociedad) {
	if (sociedad == NULL || sociedad->inicio == NULL) {
		return NULL;
	}
	if (largo_comuna(sociedad) < MAX_COMUNAS_JUGADOR) {
		return NULL;
	}

	struct jugador* jugador = calloc(1, sizeof(struct jugador));
	if (jugador == NULL) {
		return NULL;
	}

	struct nodo_lista_comunas* nodo = sociedad->inicio;
	for (int i = 0; i < MAX_COMUNAS_JUGADOR; i++) {
		jugador->comunas[i] = nodo->comuna;
		nodo = nodo->siguiente;
	}
	jugador->cantidad = MAX_COMUNAS_JUGADOR;
	return jugador;
}

/* Libera el struct del jugador. Las comunas siguen siendo de la sociedad.
	Entrada: el jugador
	Salida: el struct queda liberado */
void liberar_jugador(struct jugador* jugador) {
	free(jugador);
}

/* Dice si esa comuna es una de las del jugador
	Entrada: el jugador y la comuna
	Salida: 1 si es suya, 0 si no */
int es_comuna_jugador(struct jugador* jugador, struct comuna* comuna) {
	if (jugador == NULL || comuna == NULL) {
		return 0;
	}
	for (int i = 0; i < jugador->cantidad; i++) {
		if (jugador->comunas[i] == comuna) {
			return 1;
		}
	}
	return 0;
}

/* Imprime las comunas del jugador numeradas desde 1
	Entrada: el jugador
	Salida: la lista impresa */
void imprimir_comunas_jugador(struct jugador* jugador) {
	if (jugador == NULL) {
		return;
	}

	printf("\033[33m  Tus comunas\033[0m\n");
	printf("\033[33m  ────────────────────────────────────────\033[0m\n");
	for (int i = 0; i < jugador->cantidad; i++) {
		printf("  \033[33m┌\033[0m \033[1m%d\033[0m  ", i + 1);
		imprimirString(jugador->comunas[i]->nombre);
		printf("\n  \033[33m│\033[0m \033[33mnecesidad\033[0m %.1f    \033[32msatisfaccion\033[0m %.1f\n",
			jugador->comunas[i]->necesidad, jugador->comunas[i]->satisfaccion);
	}
}

/* La primera persona con menos libros leidos. Si empatan, se queda la de mas adelante.
	Entrada: la lista de personas de la comuna
	Salida: esa persona, o NULL si no hay nadie */
static struct persona* persona_con_menos_libros(struct lista_personas* lista) {
	if (lista == NULL || lista->inicio == NULL) {
		return NULL;
	}

	struct persona* elegida = lista->inicio->persona;
	struct nodo_lista_personas* actual = lista->inicio->siguiente;
	while (actual != NULL) {
		if (actual->persona->libros_leidos < elegida->libros_leidos) {
			elegida = actual->persona;
		}
		actual = actual->siguiente;
	}
	return elegida;
}

/* Abre el catalogo de servicios, le ensena uno a la persona y cierra el archivo.
	Entrada: la persona y cuantas personas viven en su comuna
	Salida: 1 si aprendio un servicio, 0 si no */
static int aprender_desde_catalogo(struct persona* persona, int cantidad_personas) {
	FILE* archivo = leer_archivo("servicios");
	struct lista_archivo* catalogo = generar_lista_archivo(archivo);
	fclose(archivo);

	int aprendio = persona_aprender_servicio(persona, catalogo, cantidad_personas);
	if (aprendio && persona->servicios != NULL && persona->servicios->inicio != NULL) {
		printf("\033[32m  Aprendio el servicio \033[0m");
		imprimirString(persona->servicios->inicio->recurso->nombre);
		printf(".\n");
	}
	liberar_lista_archivo(catalogo);
	return aprendio;
}

/* Una persona de una comuna del jugador busca texto y lee un solo libro.
	Cada 3 libros nuevos puede aprender un servicio.
	Entrada: el jugador y la biblioteca
	Salida: 1 si el turno de lectura termino, 0 si falta un puntero o la busqueda fallo */
int lectura_jugador(struct jugador* jugador, struct biblioteca* biblioteca) {
	if (jugador == NULL || biblioteca == NULL) {
		return 0;
	}
	if (jugador->cantidad < 1) {
		return 1;
	}

	printf("\n\033[33m  Lectura\033[0m\n");
	imprimir_comunas_jugador(jugador);
	int n = pedir_numero("\033[36m  ›\033[0m Elija el numero de tu comuna que va a leer: ", 1, jugador->cantidad);
	if (n < 0) {
		return 0;
	}

	struct comuna* comuna = jugador->comunas[n - 1];
	printf("\033[36m  ›\033[0m Escriba el texto a buscar: ");
	struct string* texto = linea_archivo(stdin);
	if (texto == NULL) {
		return 0;
	}

	struct resultado_busqueda* resultado = buscar_texto(biblioteca, texto);
	if (resultado == NULL) {
		liberar_string(texto);
		return 0;
	}

	struct persona* persona = NULL;
	if (comuna != NULL) {
		persona = persona_con_menos_libros(comuna->personas);
	}

	int leyo = 0;
	if (persona != NULL && resultado->libros != NULL) {
		struct entrada_lista_archivo* libro = resultado->libros->inicio;
		while (libro != NULL) {
			if (persona_ya_leyo(persona, libro->nombre) == 0) {
				leyo = persona_leer_libro(persona, libro->nombre);
				if (leyo) {
					printf("\033[32m  ");
					imprimirString(persona->nombre);
					printf(" lee \033[0m");
					imprimirString(libro->nombre);
					printf(" \033[2m(%d libros)\033[0m\n", persona->libros_leidos);
				}
				break;
			}
			libro = libro->siguiente;
		}
	}

	if (leyo == 0) {
		printf("\033[33m  Nadie leyo un libro nuevo en esta busqueda.\033[0m\n");
	} else if (persona->libros_leidos % 3 == 0) {
		int cantidad_personas = 0;
		if (comuna->personas != NULL) {
			cantidad_personas = largo_personas(comuna->personas);
		}
		aprender_desde_catalogo(persona, cantidad_personas);
	}

	liberar_resultado_busqueda(resultado);
	liberar_string(texto);
	return 1;
}
