#ifndef CONSTANTES_H
#define CONSTANTES_H

/* Necesidad alta: la comuna esta vacia, las vecinas estan mal o los regalos estan desbalanceados.
	Satisfaccion baja: el barrio tiene la necesidad local por encima de 75. */
#define UMBRAL_NECESIDAD 90
#define UMBRAL_SATISFACCION 25

#define MAX_COMUNAS_JUGADOR 3
#define TOPE_TURNOS 25

/* Cada comuna que no es del jugador tira una vez por dia. */
#define PROBABILIDAD_DAR 80

#define RUTA_CORPUS "corpus_prueba"

#endif
