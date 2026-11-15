#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "jugador.h"
#include "biblioteca.h"
#include "busqueda.h"
#include "utils.h"
#include "simulacion.h"
#include "tui/tui.h"
#include "tui/flujo.h"

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

static void copiar_informe(struct string* origen, char* destino, int tope) {
	int i = 0;
	struct char_array* actual = NULL;
	if (destino == NULL || tope <= 0) {
		return;
	}
	if (origen != NULL) {
		actual = origen->inicio;
	}
	while (actual != NULL && i < tope - 1) {
		destino[i] = actual->caracter;
		actual = actual->siguiente;
		i++;
	}
	destino[i] = '\0';
}

/* Abre el catalogo de servicios, le ensena uno a la persona y cierra el archivo.
	Entrada: la persona, cuantas personas viven en su comuna y donde dejar el nombre
	Salida: 1 si aprendio un servicio, 0 si no */
static int aprender_desde_catalogo(struct persona* persona, int cantidad_personas, char* servicio, int tope) {
	FILE* archivo = leer_archivo("servicios");
	struct lista_archivo* catalogo = generar_lista_archivo(archivo);
	fclose(archivo);

	int aprendio = persona_aprender_servicio(persona, catalogo, cantidad_personas);
	if (aprendio && servicio != NULL && persona->servicios != NULL && persona->servicios->inicio != NULL) {
		copiar_informe(persona->servicios->inicio->recurso->nombre, servicio, tope);
	}
	liberar_lista_archivo(catalogo);
	return aprendio;
}

/* Busca el texto y hace leer a quien lleva menos libros. No imprime.
	Entrada: la comuna, la biblioteca, el texto y donde dejar el informe
	Salida: 1 si el turno de lectura termino, 0 si falta un puntero o la busqueda fallo */
int ejecutar_lectura(struct comuna* comuna, struct biblioteca* biblioteca, struct string* texto, struct informe_lectura* informe) {
	struct resultado_busqueda* resultado;
	struct persona* persona = NULL;
	int leyo = 0;
	int hay_libros = 0;

	if (comuna == NULL || biblioteca == NULL || texto == NULL || informe == NULL) {
		return 0;
	}

	memset(informe, 0, sizeof(*informe));
	resultado = buscar_texto(biblioteca, texto);
	if (resultado == NULL) {
		informe->caso = LECTURA_FALLO;
		return 0;
	}

	persona = persona_con_menos_libros(comuna->personas);
	hay_libros = resultado->libros != NULL && resultado->libros->inicio != NULL;
	if (persona != NULL && hay_libros) {
		struct entrada_lista_archivo* libro = resultado->libros->inicio;
		while (libro != NULL) {
			if (persona_ya_leyo(persona, libro->nombre) == 0) {
				leyo = persona_leer_libro(persona, libro->nombre);
				if (leyo) {
					copiar_informe(persona->nombre, informe->persona, (int)sizeof(informe->persona));
					copiar_informe(libro->nombre, informe->libro, (int)sizeof(informe->libro));
					informe->libros_leidos = persona->libros_leidos;
				}
				break;
			}
			libro = libro->siguiente;
		}
	}

	if (persona == NULL) {
		informe->caso = LECTURA_SIN_PERSONA;
	} else if (hay_libros == 0) {
		informe->caso = LECTURA_SIN_LIBROS;
	} else if (leyo == 0) {
		copiar_informe(persona->nombre, informe->persona, (int)sizeof(informe->persona));
		informe->caso = LECTURA_YA_LEYO;
	} else if (persona->libros_leidos % 3 == 0) {
		int cantidad_personas = 0;
		if (comuna->personas != NULL) {
			cantidad_personas = largo_personas(comuna->personas);
		}
		if (aprender_desde_catalogo(persona, cantidad_personas, informe->servicio, (int)sizeof(informe->servicio))) {
			informe->caso = LECTURA_APRENDIO;
		} else {
			informe->caso = LECTURA_OK;
		}
	} else {
		informe->caso = LECTURA_OK;
	}

	liberar_resultado_busqueda(resultado);
	return 1;
}

static void imprimir_informe_lectura(struct informe_lectura* informe) {
	if (informe->caso == LECTURA_SIN_PERSONA) {
		printf("\033[33m  Esa comuna no tiene personas para leer.\033[0m\n");
	} else if (informe->caso == LECTURA_SIN_LIBROS) {
		printf("\033[33m  Ningun libro contiene ese texto.\033[0m\n");
	} else if (informe->caso == LECTURA_YA_LEYO) {
		printf("\033[33m  %s ya leyo todos los libros de esta busqueda.\033[0m\n", informe->persona);
	} else if (informe->caso == LECTURA_OK || informe->caso == LECTURA_APRENDIO) {
		printf("\033[32m  %s lee \033[0m%s \033[2m(%d libros)\033[0m\n",
			informe->persona, informe->libro, informe->libros_leidos);
		if (informe->caso == LECTURA_APRENDIO) {
			printf("\033[32m  Aprendio el servicio \033[0m%s.\n", informe->servicio);
		}
	}
}

static int lectura_clasica(struct jugador* jugador, struct biblioteca* biblioteca) {
	struct informe_lectura informe;
	struct string* texto;
	int n;

	if (jugador->cantidad < 1) {
		return 1;
	}

	printf("\n\033[33m  Lectura\033[0m\n");
	imprimir_comunas_jugador(jugador);
	n = pedir_numero("\033[36m  ›\033[0m Elija el numero de tu comuna que va a leer: ", 1, jugador->cantidad);
	if (n < 0) {
		return 0;
	}

	printf("\033[36m  ›\033[0m Escriba el texto a buscar: ");
	texto = linea_archivo(stdin);
	if (texto == NULL) {
		return 0;
	}
	if (ejecutar_lectura(jugador->comunas[n - 1], biblioteca, texto, &informe) == 0) {
		liberar_string(texto);
		return 0;
	}
	imprimir_informe_lectura(&informe);
	liberar_string(texto);
	return 1;
}

/* Una persona de una comuna del jugador busca texto y lee un solo libro.
	Cada 3 libros nuevos puede aprender un servicio.
	Entrada: el jugador y la biblioteca
	Salida: 1 si el turno de lectura termino, 0 si falta un puntero o la busqueda fallo */
int lectura_jugador(struct jugador* jugador, struct biblioteca* biblioteca) {
	if (jugador == NULL || biblioteca == NULL) {
		return 0;
	}
	if (tui_es_tty()) {
		return tui_lectura_jugador(jugador, biblioteca);
	}
	return lectura_clasica(jugador, biblioteca);
}
