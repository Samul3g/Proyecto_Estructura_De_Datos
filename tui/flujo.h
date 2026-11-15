#ifndef FLUJO_H
#define FLUJO_H

#include "estructuras.h"
#include "jugador.h"
#include "biblioteca.h"
#include "simulacion.h"

/* Intro y configuracion con teclado. Sin TTY, usa el dialogo de texto.
	Entrada: ninguna
	Salida: las variables globales de la partida quedan definidas */
void tui_preparar_partida(void);

/* Victoria si la partida siguio, derrota si una comuna del jugador cayo.
	Entrada: el valor que devolvio el ultimo dia, y el turno en que salio el ciclo
	Salida: la pantalla de cierre */
void tui_cerrar_partida(int sigue, int turno);

/* Tres vecinos, uno por comuna del jugador. No cambia el estado.
	Entrada: el jugador
	Salida: las tres voces en pantalla */
void tui_ciudadanos_dia(struct jugador* jugador);

/* Rota las comunas del jugador hasta que elige regalar.
	Entrada: el jugador y el turno (desde 1)
	Salida: vuelve cuando la accion es regalar */
void tui_hub(struct jugador* jugador, int turno);

/* Regalo con la misma logica que el intercambio de texto.
	Entrada: la sociedad y el jugador
	Salida: 1 si el regalo quedo resuelto, 0 si se vuelve al hub */
int tui_regalo_jugador(struct lista_comunas* sociedad, struct jugador* jugador);

/* Lectura con carrusel y una linea de texto.
	Entrada: el jugador y la biblioteca
	Salida: 1 si el turno de lectura termino, 0 si la busqueda no pudo hacerse */
int tui_lectura_jugador(struct jugador* jugador, struct biblioteca* biblioteca);

/* Pasa la lista de regalos ajenos. Pausa si el destino es del jugador.
	Entrada: el jugador y el registro del dia
	Salida: la lista en pantalla */
void tui_montaje_autonomo(struct jugador* jugador, struct registro_regalos* registro);

/* Pantalla del evento, antes de aplicar la emergencia o la prosperidad.
	Entrada: 0 ninguno, 1 emergencia, 2 prosperidad, y la comuna
	Salida: el dibujo */
void tui_evento(int tipo, struct comuna* comuna);

/* Una frase al cerrar el dia.
	Entrada: el jugador
	Salida: la frase en pantalla */
void tui_linea_postdia(struct jugador* jugador);

/* Mensaje breve que espera Enter.
	Entrada: el texto
	Salida: la pausa */
void tui_aviso(const char* mensaje);

#endif
