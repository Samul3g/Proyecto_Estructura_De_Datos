#include <stdio.h>
#include <stdlib.h>
#include "disolucion.h"
#include "constantes.h"

/* Dice si la comuna cruzo un umbral este dia.
	Entrada: la comuna, con necesidad y satisfaccion ya calculadas
	Salida: 1 si necesidad > UMBRAL_NECESIDAD o satisfaccion < UMBRAL_SATISFACCION */
int debe_disolverse(struct comuna* comuna) {
	if (comuna == NULL) {
		return 0;
	}
	if (comuna->necesidad > UMBRAL_NECESIDAD) {
		return 1;
	}
	if (comuna->satisfaccion < UMBRAL_SATISFACCION) {
		return 1;
	}
	return 0;
}

/* Saca la comuna del circulo. La primera mitad de la gente va a la anterior
	y el resto a la siguiente. Si sobra una persona, se queda en la siguiente.
	Los bienes se liberan con la comuna.
	Entrada: la sociedad y el nodo de la comuna
	Salida: 1 si salio del circulo, 0 si no habia a donde mandar a la gente */
int disolver_comuna(struct lista_comunas* sociedad, struct nodo_lista_comunas* nodo) {
	if (sociedad == NULL || nodo == NULL || nodo->comuna == NULL) {
		return 0;
	}
	if (nodo->siguiente == nodo) {
		return 0;
	}

	struct nodo_lista_comunas* atras = nodo->anterior;
	struct nodo_lista_comunas* adelante = nodo->siguiente;
	struct comuna* comuna = nodo->comuna;

	printf("\n\033[31m  Se disuelve \033[1m");
	imprimirString(comuna->nombre);
	printf("\033[0m\033[31m. La gente se reparte entre las vecinas. Los bienes quedan abandonados.\033[0m\n");

	int n = 0;
	if (comuna->personas != NULL) {
		n = largo_personas(comuna->personas);
	}

	if (n > 0) {
		struct persona** gente = malloc((size_t)n * sizeof(struct persona*));
		if (gente == NULL) {
			return 0;
		}

		struct nodo_lista_personas* actual = comuna->personas->inicio;
		for (int i = 0; i < n; i++) {
			gente[i] = actual->persona;
			actual = actual->siguiente;
		}

		if (atras->comuna == adelante->comuna) {
			for (int i = 0; i < n; i++) {
				mover_persona(comuna->personas, atras->comuna->personas, gente[i]);
			}
		} else {
			int mitad = n / 2;
			for (int i = 0; i < mitad; i++) {
				mover_persona(comuna->personas, atras->comuna->personas, gente[i]);
			}
			for (int i = mitad; i < n; i++) {
				mover_persona(comuna->personas, adelante->comuna->personas, gente[i]);
			}
		}
		free(gente);
	}

	atras->siguiente = adelante;
	adelante->anterior = atras;
	if (sociedad->inicio == nodo) {
		sociedad->inicio = adelante;
	}

	liberar_comuna(comuna);
	free(nodo);
	return 1;
}

/* Disuelve las comunas ajenas que cruzaron un umbral. Las del jugador se quedan.
	Entrada: la sociedad y el jugador
	Salida: 0 si alguna comuna del jugador estaba marcada, 1 si la partida sigue */
int disolver_marcadas(struct lista_comunas* sociedad, struct jugador* jugador) {
	if (sociedad == NULL || sociedad->inicio == NULL) {
		return 1;
	}

	int n = largo_comuna(sociedad);
	struct nodo_lista_comunas** ajenas = calloc((size_t)n, sizeof(struct nodo_lista_comunas*));
	if (ajenas == NULL) {
		return 0;
	}

	int marcadas = 0;
	int jugador_marcado = 0;
	struct comuna* marcadas_jugador[MAX_COMUNAS_JUGADOR];
	struct nodo_lista_comunas* actual = sociedad->inicio;

	do {
		if (debe_disolverse(actual->comuna)) {
			if (es_comuna_jugador(jugador, actual->comuna)) {
				if (jugador_marcado < MAX_COMUNAS_JUGADOR) {
					marcadas_jugador[jugador_marcado] = actual->comuna;
				}
				jugador_marcado++;
			} else {
				ajenas[marcadas] = actual;
				marcadas++;
			}
		}
		actual = actual->siguiente;
	} while (actual != sociedad->inicio);

	for (int i = 0; i < marcadas; i++) {
		disolver_comuna(sociedad, ajenas[i]);
	}
	free(ajenas);

	for (int i = 0; i < jugador_marcado && i < MAX_COMUNAS_JUGADOR; i++) {
		printf("\n\033[31m  Tu comuna \033[1m");
		imprimirString(marcadas_jugador[i]->nombre);
		printf("\033[0m\033[31m cruzo el umbral. Perdiste.\033[0m\n");
	}

	if (jugador_marcado) {
		return 0;
	}
	return 1;
}
