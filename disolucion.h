#ifndef DISOLUCION_H
#define DISOLUCION_H

#include "estructuras.h"
#include "jugador.h"

int debe_disolverse(struct comuna* comuna);
	/* Dice si la comuna cruzo un umbral este dia.
		Entrada: la comuna, con necesidad y satisfaccion ya calculadas
		Salida: 1 si necesidad > UMBRAL_NECESIDAD o satisfaccion < UMBRAL_SATISFACCION */

int disolver_comuna(struct lista_comunas* sociedad, struct nodo_lista_comunas* nodo);
	/* Saca la comuna del circulo. La gente se parte entre la anterior y la siguiente.
		Los bienes no se reparten. No llama a eliminar_comuna.
		Entrada: la sociedad y el nodo de la comuna
		Salida: 1 si salio del circulo, 0 si no habia a donde mandar a la gente */

int disolver_marcadas(struct lista_comunas* sociedad, struct jugador* jugador);
	/* Disuelve las comunas ajenas que cruzaron un umbral. Las del jugador se quedan.
		Entrada: la sociedad y el jugador
		Salida: 0 si alguna comuna del jugador estaba marcada, 1 si la partida sigue */

#endif
