#ifndef ASCII_ART_H
#define ASCII_ART_H

/* Silueta solarpunk. El nombre elige una de seis plantillas.
	Entrada: el nombre de la comuna
	Salida: el dibujo */
void ascii_comuna(const char* nombre);

/* Sprite de la profesion, leido de `profesiones`.
	Entrada: el nombre del servicio, o NULL
	Salida: el sprite, o el placeholder */
void ascii_npc(const char* servicio);

/* Dibujo de un bien o un servicio, leido de `servicios_ascii`.
	Entrada: el nombre del recurso
	Salida: su dibujo, o el placeholder si el nombre no esta */
void ascii_recurso(const char* nombre);

/* Dibujo de tormenta o de sol.
	Entrada: 1 si es emergencia, 0 si es prosperidad
	Salida: el dibujo */
void ascii_evento(int emergencia);

/* Dibujo de cierre, distinto si el periodo se cumplio.
	Entrada: 1 si es victoria
	Salida: el dibujo */
void ascii_final(int victoria);

/* Sol y techos del arranque.
	Entrada: ninguna
	Salida: el logo */
void ascii_logo(void);

#endif
