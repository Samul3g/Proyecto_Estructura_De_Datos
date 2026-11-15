#ifndef SIMULACION_H
#define SIMULACION_H

int registrar_regalo(struct comuna* origen, struct comuna* destino);
/* Funcion que registra un regalo entre dos comunas
	Entrada: origen y destino
	Salida: 1 (true/exito)
*/

int emergencia(struct comuna* comuna);
/* Funcion que realiza la emergencia de un bien en una comuna
	Entrada: comuna
	Salida: 1 (true/exito)
*/

int prosperidad(struct comuna* comuna);
/* Funcion que realiza la prosperidad de un bien en una comuna
	Entrada: comuna
	Salida: 1 (true/exito)
*/


struct jugador;
struct biblioteca;

int pedir_numero(const char* pregunta, int minimo, int maximo);
	/* Pide un entero dentro del rango. Si escriben texto, lo descarta y vuelve a preguntar.
		Entrada: la pregunta, el minimo y el maximo
		Salida: el entero, o -1 si se acabo la entrada */

int usuario_intercambio(struct lista_comunas* lista_comunas, struct jugador* jugador);
/*Funcion que pide al usuario la comuna que regala y recibe y el bien o la persona a regalar y luego da el regalo.
 * Solo las comunas del jugador pueden dar.
 * Entradas: lista de comunas y el jugador
 * Salidas: 1 para exito, 0 para fracaso*/

int intercambio_autonomo(struct lista_comunas* sociedad, struct jugador* jugador);
/* Cada comuna que no es del jugador puede regalar un bien o mudar una persona.
	Las del jugador pueden recibir, no dar.
	Entrada: la sociedad y el jugador
	Salida: 1 si recorrio la sociedad, 0 si la sociedad es NULL */
 
int reduccion_bienes_servicios(struct lista_comunas* sociedad);
/* Funcion que reduce la cantidad de bienes de cada comuna
	Entrada: sociedad
	Salida: 1 (true/exito), 0 si la sociedad es NULL
*/

struct comuna* comuna_random(struct lista_comunas* sociedad);
/* Funcion que devuelve una comuna aleatoria de la lista de comunas
	Entrada: sociedad
	Salida: comuna
*/

int paso_dia(struct lista_comunas* sociedad, struct jugador* jugador, struct biblioteca* biblioteca);
/* Funcion que realiza un paso de dia en la simulacion
	Entrada: sociedad, jugador y biblioteca
	Salida: 1 si la partida sigue, 0 si una comuna del jugador cruzo un umbral
*/

struct nodo_lista_recursos* bien_aleatorio(struct comuna* comuna);
/* Funcion que elige un bien al azar de la comuna
	Entrada: comuna
	Salida: el nodo elegido, o NULL si esa lista esta vacia
*/
 
#endif
