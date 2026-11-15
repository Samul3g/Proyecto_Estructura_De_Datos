#include <stdio.h>
#include <string.h>
#include "estructuras.h"
#include "manejo_recursos.h"
#include "indices.h"
#include "utils.h"
#include "simulacion.h"
#include "jugador.h"
#include "disolucion.h"
#include "constantes.h"
#include "tui/tui.h"
#include "tui/flujo.h"


static void nombre_cstr(struct string* origen, char* destino, int tope) {
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

/* Deja el registro vacio.
	Entrada: el registro
	Salida: cantidad en 0 */
void registro_regalos_iniciar(struct registro_regalos* registro) {
	if (registro == NULL) {
		return;
	}
	registro->cantidad = 0;
}

/* Anota un regalo autonomo si todavia hay espacio.
	Entrada: el registro, las comunas y el texto ya armado
	Salida: el registro crece en uno, o se queda igual si esta lleno o falta un puntero */
void registro_regalos_agregar(struct registro_regalos* registro, struct comuna* origen, struct comuna* destino, const char* mensaje) {
	struct regalo_log* entrada;
	if (registro == NULL || mensaje == NULL || registro->cantidad >= TOPE_REGALOS_LOG) {
		return;
	}
	entrada = &registro->entradas[registro->cantidad];
	entrada->origen = origen;
	entrada->destino = destino;
	snprintf(entrada->mensaje, sizeof(entrada->mensaje), "%s", mensaje);
	registro->cantidad++;
}

int registrar_regalo (struct comuna* origen, struct comuna* destino) {
	if (origen == NULL || destino == NULL) {
		return 0;
	}
	origen->regalos_dados++;
	destino->regalos_recibidos++;
	return 1;
}

/* Funcion que devuelve una comuna aleatoria de la lista de comunas
	Entrada: sociedad
	Salida: comuna
*/
struct comuna* comuna_random(struct lista_comunas* sociedad){
    int largo = largo_comuna(sociedad);
    int comuna = aleatorio(1, largo);
    struct nodo_lista_comunas* actual = sociedad->inicio;
    while(comuna > 1){
        actual = actual->siguiente;
        comuna--;
    }
    return actual->comuna;
}

/* Funcion que realiza un paso de dia en la simulacion
	Entrada: sociedad y jugador
	Salida: 1 si la partida sigue, 0 si una comuna del jugador cruzo un umbral */
static int paso_dia_texto(struct lista_comunas* sociedad, struct jugador* jugador, struct biblioteca* biblioteca){
    int valido = 0;
    valido = usuario_intercambio(sociedad, jugador);
    if(valido == 0){
        printf("\033[31m  No se pudo realizar el intercambio.\033[0m\n");
    }
    valido = intercambio_autonomo(sociedad, jugador);
    if(valido == 0){
        printf("\033[31m  No se pudo realizar el intercambio autonomo.\033[0m\n");
    }
    valido = lectura_jugador(jugador, biblioteca);
    if(valido == 0){
        printf("\033[31m  No se pudo realizar la lectura.\033[0m\n");
    }
    valido = reduccion_bienes_servicios(sociedad);
    if(valido == 0){
        printf("\033[31m  No se pudo reducir los bienes.\033[0m\n");
    }
    int evento = aleatorio(1, 10);
    if(evento < 3){
        struct comuna* afectada = comuna_random(sociedad);
        printf("\n\033[31m  Evento: Emergencia en \033[1m");
        imprimirString(afectada->nombre);
        printf("\033[0m\n");
        valido = emergencia(afectada);
        if(valido == 0){
            printf("\033[31m  No se pudo realizar el evento.\033[0m\n");
        }
    }else if(evento < 8){
        struct comuna* afectada = comuna_random(sociedad);
        printf("\n\033[32m  Evento: Prosperidad en \033[1m");
        imprimirString(afectada->nombre);
        printf("\033[0m\n");
        valido = prosperidad(afectada);
        if(valido == 0){
            printf("\033[31m  No se pudo realizar el evento.\033[0m\n");
        }
    }else{
        printf("\n\033[2m  Evento: Ninguno\033[0m\n");
    }

    valido = calcular_indice_sociedad(sociedad);
    if(valido == 0){
        printf("\033[31m  No se pudo calcular el indice de la sociedad.\033[0m\n");
    }

    valido = disolver_marcadas(sociedad, jugador);
    return valido;
}

/* Funcion que realiza un paso de dia en la simulacion
	Entrada: sociedad, jugador, biblioteca y el numero de turno
	Salida: 1 si la partida sigue, 0 si una comuna del jugador cruzo un umbral */
int paso_dia(struct lista_comunas* sociedad, struct jugador* jugador, struct biblioteca* biblioteca, int turno){
	int valido = 0;
	int evento = 0;
	if (!tui_es_tty()) {
		return paso_dia_texto(sociedad, jugador, biblioteca);
	}

	tui_ciudadanos_dia(jugador);
	while (valido == 0) {
		tui_hub(jugador, turno);
		valido = tui_regalo_jugador(sociedad, jugador);
	}

	valido = tui_lectura_jugador(jugador, biblioteca);
	if (valido == 0) {
		tui_aviso("No se pudo realizar la lectura.");
	}

	valido = intercambio_autonomo(sociedad, jugador);
	if (valido == 0) {
		tui_aviso("No se pudo realizar el intercambio autonomo.");
	}

	valido = reduccion_bienes_servicios(sociedad);
	if (valido == 0) {
		tui_aviso("No se pudo reducir los bienes.");
	}

	evento = aleatorio(1, 10);
	if (evento < 3) {
		struct comuna* afectada = comuna_random(sociedad);
		tui_evento(1, afectada);
		valido = emergencia(afectada);
		if (valido == 0) {
			tui_aviso("No se pudo realizar el evento.");
		}
	} else if (evento < 8) {
		struct comuna* afectada = comuna_random(sociedad);
		tui_evento(2, afectada);
		valido = prosperidad(afectada);
		if (valido == 0) {
			tui_aviso("No se pudo realizar el evento.");
		}
	} else {
		tui_evento(0, NULL);
	}

	valido = calcular_indice_sociedad(sociedad);
	if (valido == 0) {
		tui_aviso("No se pudo calcular el indice de la sociedad.");
	}

	tui_linea_postdia(jugador);
	valido = disolver_marcadas(sociedad, jugador);
	tui_msleep(800);
	return valido;
}


/* Funcion que reduce la cantidad de bienes de cada comuna
	Entrada: sociedad
	Salida: 1 (true/exito), 0 si la sociedad es NULL
*/
int reduccion_bienes_servicios(struct lista_comunas* sociedad){
    if(sociedad == NULL){
        return 0;
    }
    if(sociedad->inicio == NULL){
        return 1;
    }

    struct nodo_lista_comunas* inicio = sociedad->inicio;
    struct nodo_lista_comunas* actual = inicio;

    do {
        struct lista_recursos* lista = actual->comuna->bienes;
        if(lista != NULL){
            struct nodo_lista_recursos* bienes = lista->inicio;
            while(bienes != NULL){
                bienes->recurso->cantidad = (int)(bienes->recurso->cantidad * 0.9);
                bienes = bienes->siguiente;
            }
        }
        actual = actual->siguiente;
    } while(actual != inicio);

    return 1;
}

/* Elige un bien al azar de la comuna
	Entrada: comuna
	Salida: el nodo elegido, o NULL si esa lista esta vacia
*/
struct nodo_lista_recursos* bien_aleatorio(struct comuna* comuna){
	if(comuna == NULL || comuna->bienes == NULL){
		return NULL;
	}

	int largo = largo_recurso(comuna->bienes);
	if(largo == 0){
		return NULL;
	}

	int i = aleatorio(0, largo - 1);
	struct nodo_lista_recursos* actual = comuna->bienes->inicio;
	while(i > 0){
		actual = actual->siguiente;
		i--;
	}
	return actual;
}

/* Funcion que realiza la emergencia de un bien en una comuna
	Entrada: comuna
	Salida: 1 (true/exito)
*/
int emergencia(struct comuna* comuna){
	struct nodo_lista_recursos* actual = bien_aleatorio(comuna);
	if(actual == NULL || actual->recurso->cantidad == 0){
		return 0;
	}

	int baja = aleatorio(1, actual->recurso->cantidad);
	actual->recurso->cantidad = actual->recurso->cantidad - baja;
	return 1;
}

/* Funcion que realiza la prosperidad de un bien en una comuna
	Entrada: comuna
	Salida: 1 (true/exito)
*/
int prosperidad(struct comuna* comuna){
	struct nodo_lista_recursos* actual = bien_aleatorio(comuna);
	if(actual == NULL){
		return 0;
	}

    if(actual->recurso->cantidad == 0){
        actual->recurso->cantidad = 1;
        return 1;
    }
	int sube = aleatorio(1, actual->recurso->cantidad);
	actual->recurso->cantidad = actual->recurso->cantidad + sube;
	if(actual->recurso->cantidad > actual->recurso->maximo){
		actual->recurso->cantidad = actual->recurso->maximo;
	}
	return 1;
}


/* Pide un entero dentro del rango. Si escriben texto, lo descarta y vuelve a preguntar.
	Entrada: la pregunta, el minimo y el maximo
	Salida: el entero, o -1 si se acabo la entrada */
int pedir_numero(const char* pregunta, int minimo, int maximo){
	int valor = 0;
	printf("%s", pregunta);
	int estado = leer_entero(&valor);
	while(estado <= 0 || valor < minimo || valor > maximo){
		if(estado < 0){
			return -1;
		}
		printf("\033[31m  Numero invalido, fuera del rango.\033[0m\n");
		printf("%s", pregunta);
		estado = leer_entero(&valor);
	}
	return valor;
}

/* Recorre la lista y dice si algun recurso tiene cantidad mayor que 0
	Entrada: la lista de recursos
	Salida: 1 si hay algo para regalar, 0 si no */
int hay_cantidad_disponible(struct lista_recursos* lista){
	if(lista == NULL){
		return 0;
	}
	struct nodo_lista_recursos* actual = lista->inicio;
	while(actual != NULL){
		if(actual->recurso->cantidad > 0){
			return 1;
		}
		actual = actual->siguiente;
	}
	return 0;
}

/* Imprime las personas numeradas
	Entrada: la lista de personas
	Salida: la lista impresa */
void imprimir_personas(struct lista_personas* lista){
	if(lista == NULL){
		return;
	}

	printf("\n\033[33m  Personas\033[0m\n");
	printf("\033[33m  ────────────────────────────────────────\033[0m\n");

	struct nodo_lista_personas* actual = lista->inicio;
	int numero = 1;
	while(actual != NULL){
		printf("  \033[33m┌\033[0m \033[1m%d\033[0m  ", numero);
		imprimirString(actual->persona->nombre);
		printf("\n");
		actual = actual->siguiente;
		numero++;
	}
}

/* Devuelve la persona en esa posicion, contando desde 1
	Entrada: la lista y el numero
	Salida: la persona, o NULL si no esta */
struct persona* persona_por_numero(struct lista_personas* lista, int numero){
	if(lista == NULL){
		return NULL;
	}

	int i = 1;
	struct nodo_lista_personas* actual = lista->inicio;
	while(actual != NULL){
		if(i == numero){
			return actual->persona;
		}
		actual = actual->siguiente;
		i++;
	}
	return NULL;
}

/* Devuelve la comuna en esa posicion, contando desde 1
	Entrada: la lista y el numero
	Salida: la comuna, o NULL si no esta */
static struct comuna* comuna_por_numero(struct lista_comunas* lista, int numero){
	if(lista == NULL || lista->inicio == NULL || numero < 1){
		return NULL;
	}

	int i = 1;
	struct nodo_lista_comunas* actual = lista->inicio;
	do {
		if(i == numero){
			return actual->comuna;
		}
		actual = actual->siguiente;
		i++;
	} while(actual != lista->inicio);
	return NULL;
}

/* Pasa una cantidad de un bien y anota el regalo. No imprime.
	Entrada: quien da, quien recibe, el recurso y la cantidad
	Salida: 1 si el bien quedo movido, 0 si no se pudo */
int regalo_bien_jugador(struct comuna* origen, struct comuna* destino, struct nodo_recurso* recurso, int cantidad) {
	int exito;
	if (origen == NULL || destino == NULL || recurso == NULL || origen == destino) {
		return 0;
	}
	if (cantidad < 1 || cantidad > recurso->cantidad) {
		return 0;
	}
	exito = aumento_bien(recurso->nombre, cantidad, destino);
	if (exito) {
		exito = reduccion_bien(recurso->nombre, cantidad, origen);
	}
	if (exito) {
		registrar_regalo(origen, destino);
	}
	return exito;
}

/* Muda a la persona y anota el regalo. No imprime.
	Entrada: quien da, quien recibe y la persona
	Salida: 1 si la persona quedo en el destino, 0 si no se pudo */
int regalo_persona_jugador(struct comuna* origen, struct comuna* destino, struct persona* persona) {
	int exito;
	if (origen == NULL || destino == NULL || persona == NULL || origen == destino) {
		return 0;
	}
	if (origen->personas == NULL || destino->personas == NULL) {
		return 0;
	}
	exito = mover_persona(origen->personas, destino->personas, persona);
	if (exito) {
		registrar_regalo(origen, destino);
	}
	return exito;
}

/*Funcion que pide al usuario la comuna que regala y recibe y el bien o la persona a regalar y luego da el regalo.
 * Solo las comunas del jugador pueden dar.
 * Entradas: lista de comunas y el jugador
 * Salidas: 1 para exito, 0 para fracaso*/
static int usuario_intercambio_texto(struct lista_comunas* lista_comunas, struct jugador* jugador){
	if(lista_comunas == NULL || jugador == NULL || jugador->cantidad < 1){
			return 0;
	}

	int largoLista = largo_comuna(lista_comunas);
	if(largoLista < 2){
		printf("\033[33m  Hay una sola comuna, no hay otra a quien regalar.\033[0m\n");
		return 1;
	}

	printf("\n\033[33m  Regalo\033[0m\n");
	imprimir_comunas_jugador(jugador);
	int n_da = pedir_numero("\033[36m  ›\033[0m Elija el numero de tu comuna que va a dar el regalo: ", 1, jugador->cantidad);
	if(n_da < 0){
		return 0;
	}
	struct comuna* comunaDa = jugador->comunas[n_da - 1];

	printf("\n");
	imprimir_lista_comunas(lista_comunas);

	int comunaRecibidora = 0;
	printf("\033[36m  ›\033[0m Elija el numero de la comuna que va a recibir el regalo: ");
	int estado = leer_entero(&comunaRecibidora);
	struct comuna* comunaRecibe = comuna_por_numero(lista_comunas, comunaRecibidora);

	while(estado <= 0 || comunaRecibe == NULL || comunaRecibe == comunaDa){
		if(estado < 0){
			return 0;
		}
		printf("\033[31m  Numero invalido, fuera del rango o igual a la comuna que da el regalo.\033[0m\n");
		printf("\033[36m  ›\033[0m Elija el numero de la comuna que va a recibir el regalo: ");
		estado = leer_entero(&comunaRecibidora);
		comunaRecibe = comuna_por_numero(lista_comunas, comunaRecibidora);
	}
	
	//Pedir si dar bien o mudar una persona (el servicio se va con ella)
	int regalo = pedir_numero("\n\033[36m  ›\033[0m Elija el numero de lo que quiere regalar:\n    \033[1;33m1\033[0m  Bien\n    \033[1;33m2\033[0m  Servicio\n\033[36m  ›\033[0m ", 1, 2);
	if(regalo < 0){
		return 0;
	}

	if(regalo == 2){
		int largo_gente = 0;
		if(comunaDa->personas != NULL){
			largo_gente = largo_personas(comunaDa->personas);
		}
		imprimir_personas(comunaDa->personas);
		if(largo_gente == 0){
			printf("\033[33m  No hay personas para mudar.\033[0m\n");
			return 1;
		}

		int n_persona = pedir_numero("\033[36m  ›\033[0m Ingrese el numero de la persona que se muda: ", 1, largo_gente);
		if(n_persona < 0){
			return 0;
		}

		struct persona* persona_elegida = persona_por_numero(comunaDa->personas, n_persona);
		int exito = mover_persona(comunaDa->personas, comunaRecibe->personas, persona_elegida);
		if(exito){
			registrar_regalo(comunaDa, comunaRecibe);
			printf("\n\033[32m  La persona se mudo y el regalo quedo hecho.\033[0m\n");
		}
		return exito;
	}

	//conseguir lista de bienes
	struct lista_recursos* lista_dador = comunaDa->bienes;

	int n_recurso = 0;
	int largo_recu = largo_recurso(lista_dador);

	imprimir_recurso(lista_dador);

	if(hay_cantidad_disponible(lista_dador) == 0){
		printf("\033[33m  No hay cantidad disponible para regalar.\033[0m\n");
		return 1;
	}

	struct nodo_recurso* recurso_dar = NULL;
	while(recurso_dar == NULL){
		n_recurso = pedir_numero("\033[36m  ›\033[0m Ingrese el numero del recurso que desea regalar: ", 1, largo_recu);
		if(n_recurso < 0){
			return 0;
		}

		int i = 1;
		struct nodo_lista_recursos* rec_actual = lista_dador->inicio;
		while(rec_actual != NULL){
			if(i == n_recurso){
				recurso_dar = rec_actual->recurso;
				break;
			}
			rec_actual = rec_actual->siguiente;
			i++;
		}

		if(recurso_dar != NULL && recurso_dar->cantidad <= 0){
			printf("\033[31m  Ese recurso esta en 0. Elija otro.\033[0m\n");
			recurso_dar = NULL;
		}
	}

	//Preguntar cantidad a regalar del recurso
	int cant_regalar = pedir_numero("\033[36m  ›\033[0m Ingrese la cantidad del recurso que desea regalar: ", 1, recurso_dar->cantidad);
	if(cant_regalar < 0){
		return 0;
	}

	int exito = aumento_bien(recurso_dar->nombre, cant_regalar, comunaRecibe);
	if(exito){
		exito = reduccion_bien(recurso_dar->nombre, cant_regalar, comunaDa);
	}

	if(exito){
		registrar_regalo(comunaDa, comunaRecibe);
		printf("\n\033[32m  El regalo quedo hecho.\033[0m\n");
	}
	return exito;
}

int usuario_intercambio(struct lista_comunas* lista_comunas, struct jugador* jugador){
	if (tui_es_tty()) {
		return tui_regalo_jugador(lista_comunas, jugador);
	}
	return usuario_intercambio_texto(lista_comunas, jugador);
}

/* Peso para recibir un regalo autonomo.
	Si todavia no ha recibido nada, el peso es 1.
	Entrada: la comuna
	Salida: regalos_dados / regalos_recibidos, o 1 si recibidos es 0 */
static float peso_recibir(struct comuna* comuna){
	if(comuna->regalos_recibidos == 0){
		return 1.0f;
	}
	return (float)comuna->regalos_dados / (float)comuna->regalos_recibidos;
}

/* Elige quien recibe, con mas chance si dados/recibidos es mas alto.
	Si todos los pesos quedan en 0, sale parejo entre las demas.
	Entrada: la sociedad y quien da
	Salida: otra comuna, o NULL si no hay otra */
static struct comuna* elegir_receptora(struct lista_comunas* sociedad, struct comuna* origen){
	struct nodo_lista_comunas* inicio = sociedad->inicio;
	struct nodo_lista_comunas* actual = inicio;
	float suma = 0.0f;
	int otras = 0;

	do {
		if(actual->comuna != origen){
			otras++;
			suma += peso_recibir(actual->comuna);
		}
		actual = actual->siguiente;
	} while(actual != inicio);

	if(otras == 0){
		return NULL;
	}

	if(suma <= 0.0f){
		int elegido = aleatorio(1, otras);
		actual = inicio;
		do {
			if(actual->comuna != origen){
				elegido--;
				if(elegido == 0){
					return actual->comuna;
				}
			}
			actual = actual->siguiente;
		} while(actual != inicio);
		return NULL;
	}

	float avance = (aleatorio(0, 999999) / 1000000.0f) * suma;
	float acumulado = 0.0f;
	struct comuna* ultima = NULL;
	actual = inicio;
	do {
		if(actual->comuna != origen){
			acumulado += peso_recibir(actual->comuna);
			ultima = actual->comuna;
			if(acumulado > avance){
				return actual->comuna;
			}
		}
		actual = actual->siguiente;
	} while(actual != inicio);
	return ultima;
}

/* Regala un bien con stock. La cantidad sale al azar.
	Entrada: quien da y quien recibe
	Salida: 1 si el bien paso, 0 si no habia stock o el movimiento fallo */
static int regalar_bien_autonomo(struct comuna* origen, struct comuna* destino, struct registro_regalos* registro){
	if(origen == NULL || origen->bienes == NULL){
		return 0;
	}

	int disponibles = 0;
	struct nodo_lista_recursos* nodo = origen->bienes->inicio;
	while(nodo != NULL){
		if(nodo->recurso->cantidad > 0){
			disponibles++;
		}
		nodo = nodo->siguiente;
	}
	if(disponibles == 0){
		return 0;
	}

	int cual = aleatorio(1, disponibles);
	nodo = origen->bienes->inicio;
	struct nodo_recurso* recurso = NULL;
	while(nodo != NULL){
		if(nodo->recurso->cantidad > 0){
			cual--;
			if(cual == 0){
				recurso = nodo->recurso;
				break;
			}
		}
		nodo = nodo->siguiente;
	}
	if(recurso == NULL){
		return 0;
	}

	int cantidad = aleatorio(1, recurso->cantidad);
	int exito = aumento_bien(recurso->nombre, cantidad, destino);
	if(exito){
		exito = reduccion_bien(recurso->nombre, cantidad, origen);
	}
	if(exito){
		char nombre_origen[64];
		char nombre_recurso[64];
		char nombre_destino[64];
		char mensaje[256];
		nombre_cstr(origen->nombre, nombre_origen, (int)sizeof(nombre_origen));
		nombre_cstr(recurso->nombre, nombre_recurso, (int)sizeof(nombre_recurso));
		nombre_cstr(destino->nombre, nombre_destino, (int)sizeof(nombre_destino));
		snprintf(mensaje, sizeof(mensaje), "\033[32m  Regalo autonomo:\033[0m %s da %d de %s a %s.",
			nombre_origen, cantidad, nombre_recurso, nombre_destino);
		registro_regalos_agregar(registro, origen, destino, mensaje);
	}
	return exito;
}

/* Muda una persona al azar.
	Entrada: quien da y quien recibe
	Salida: 1 si la persona se mudo, 0 si no habia gente */
static int regalar_persona_autonoma(struct comuna* origen, struct comuna* destino, struct registro_regalos* registro){
	if(origen == NULL || origen->personas == NULL || destino == NULL || destino->personas == NULL){
		return 0;
	}

	int n = largo_personas(origen->personas);
	if(n == 0){
		return 0;
	}

	struct persona* persona = persona_por_numero(origen->personas, aleatorio(1, n));
	int exito = mover_persona(origen->personas, destino->personas, persona);
	if(exito){
		char nombre_origen[64];
		char nombre_persona[64];
		char nombre_destino[64];
		char mensaje[256];
		nombre_cstr(origen->nombre, nombre_origen, (int)sizeof(nombre_origen));
		nombre_cstr(persona->nombre, nombre_persona, (int)sizeof(nombre_persona));
		nombre_cstr(destino->nombre, nombre_destino, (int)sizeof(nombre_destino));
		snprintf(mensaje, sizeof(mensaje), "\033[32m  Regalo autonomo:\033[0m %s muda a %s hacia %s.",
			nombre_origen, nombre_persona, nombre_destino);
		registro_regalos_agregar(registro, origen, destino, mensaje);
	}
	return exito;
}

/* Una comuna intenta dar. Primero sale el tipo (bien o mudanza); si no puede, prueba el otro.
	Entrada: la sociedad y quien da
	Salida: 1 si regalo, 0 si no */
static int intentar_regalo_autonomo(struct lista_comunas* sociedad, struct comuna* origen, struct registro_regalos* registro){
	struct comuna* destino = elegir_receptora(sociedad, origen);
	if(destino == NULL){
		return 0;
	}

	int primero_bien = aleatorio(1, 2) == 1;
	int exito = 0;
	if(primero_bien){
		exito = regalar_bien_autonomo(origen, destino, registro);
		if(exito == 0){
			exito = regalar_persona_autonoma(origen, destino, registro);
		}
	}else{
		exito = regalar_persona_autonoma(origen, destino, registro);
		if(exito == 0){
			exito = regalar_bien_autonomo(origen, destino, registro);
		}
	}

	if(exito){
		registrar_regalo(origen, destino);
	}
	return exito;
}

/* Cada comuna que no es del jugador puede regalar un bien o mudar una persona.
	Las del jugador pueden recibir, no dar.
	Entrada: la sociedad y el jugador
	Salida: 1 si recorrio la sociedad, 0 si la sociedad es NULL */
int intercambio_autonomo(struct lista_comunas* sociedad, struct jugador* jugador){
	if(sociedad == NULL || sociedad->inicio == NULL){
		return 0;
	}

	struct registro_regalos registro;
	registro_regalos_iniciar(&registro);

	struct nodo_lista_comunas* inicio = sociedad->inicio;
	struct nodo_lista_comunas* actual = inicio;
	do {
		if(es_comuna_jugador(jugador, actual->comuna) == 0){
			if(aleatorio(1, 100) <= PROBABILIDAD_DAR){
				intentar_regalo_autonomo(sociedad, actual->comuna, &registro);
			}
		}
		actual = actual->siguiente;
	} while(actual != inicio);

	if (tui_es_tty()) {
		tui_montaje_autonomo(jugador, &registro);
	} else {
		for (int i = 0; i < registro.cantidad; i++) {
			printf("%s\n", registro.entradas[i].mensaje);
		}
	}

	return 1;
}
