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

#define TOPE_REGALOS_LOG 64

struct regalo_log {
	struct comuna* origen;
	struct comuna* destino;
	char mensaje[256];
};

struct registro_regalos {
	struct regalo_log entradas[TOPE_REGALOS_LOG];
	int cantidad;
};

/* Deja el registro vacio.
	Entrada: el registro
	Salida: cantidad en 0 */
void registro_regalos_iniciar(struct registro_regalos* registro);

/* Anota un regalo autonomo si todavia hay espacio.
	Entrada: el registro, las comunas y el texto ya armado
	Salida: el registro crece en uno, o se queda igual si esta lleno o falta un puntero */
void registro_regalos_agregar(struct registro_regalos* registro, struct comuna* origen, struct comuna* destino, const char* mensaje);

int pedir_numero(const char* pregunta, int minimo, int maximo);
	/* Pide un entero dentro del rango. Si escriben texto, lo descarta y vuelve a preguntar.
		Entrada: la pregunta, el minimo y el maximo
		Salida: el entero, o -1 si se acabo la entrada */

int usuario_intercambio(struct lista_comunas* lista_comunas, struct jugador* jugador);
/*Funcion que pide al usuario la comuna que regala y recibe y el bien o la persona a regalar y luego da el regalo.
 * Solo las comunas del jugador pueden dar.
 * Entradas: lista de comunas y el jugador
 * Salidas: 1 para exito, 0 para fracaso*/

int regalo_bien_jugador(struct comuna* origen, struct comuna* destino, struct nodo_recurso* recurso, int cantidad);
/* Pasa una cantidad de un bien y anota el regalo. No imprime.
	Entrada: quien da, quien recibe, el recurso y la cantidad
	Salida: 1 si el bien quedo movido, 0 si no se pudo */

int regalo_persona_jugador(struct comuna* origen, struct comuna* destino, struct persona* persona);
/* Muda a la persona y anota el regalo. No imprime.
	Entrada: quien da, quien recibe y la persona
	Salida: 1 si la persona quedo en el destino, 0 si no se pudo */

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

int paso_dia(struct lista_comunas* sociedad, struct jugador* jugador, struct biblioteca* biblioteca, int turno);
/* Funcion que realiza un paso de dia en la simulacion
	Entrada: sociedad, jugador, biblioteca y el numero de turno (desde 1)
	Salida: 1 si la partida sigue, 0 si una comuna del jugador cruzo un umbral
*/

struct nodo_lista_recursos* bien_aleatorio(struct comuna* comuna);
/* Funcion que elige un bien al azar de la comuna
	Entrada: comuna
	Salida: el nodo elegido, o NULL si esa lista esta vacia
*/
 
#endif
