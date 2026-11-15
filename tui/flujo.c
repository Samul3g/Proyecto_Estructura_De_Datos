#include "flujo.h"

#include "tui.h"
#include "input.h"
#include "widgets.h"
#include "ascii_art.h"
#include "dialogos.h"
#include "main.h"
#include "constantes.h"
#include "utils.h"

#include <stdio.h>
#include <string.h>

static void copiar_nombre(struct string* origen, char* destino, int tope) {
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
	if (i == 0) {
		snprintf(destino, (size_t)tope, "(sin nombre)");
	}
}

static void esperar_continuar(void) {
	fflush(stdout);
	for (;;) {
		struct tui_tecla tecla = tui_leer_tecla();
		if (tecla.codigo == TECLA_ENTER || tecla.codigo == TECLA_ESC || tecla.codigo == TECLA_Q) {
			return;
		}
	}
}

static struct nodo_recurso* servicio_nacimiento(struct persona* persona) {
	struct nodo_lista_recursos* nodo;
	if (persona == NULL || persona->servicios == NULL) {
		return NULL;
	}
	nodo = persona->servicios->inicio;
	if (nodo == NULL) {
		return NULL;
	}
	while (nodo->siguiente != NULL) {
		nodo = nodo->siguiente;
	}
	return nodo->recurso;
}

static struct nodo_recurso* primera_carencia(struct comuna* comuna) {
	struct nodo_lista_recursos* nodo;
	if (comuna == NULL || comuna->bienes == NULL) {
		return NULL;
	}
	nodo = comuna->bienes->inicio;
	while (nodo != NULL) {
		if (nodo->recurso->necesidad && nodo->recurso->cantidad == 0) {
			return nodo->recurso;
		}
		nodo = nodo->siguiente;
	}
	return NULL;
}

static struct nodo_lista_recursos* recurso_en(struct lista_recursos* lista, int indice) {
	struct nodo_lista_recursos* nodo = NULL;
	if (lista == NULL || indice < 0) {
		return NULL;
	}
	nodo = lista->inicio;
	while (nodo != NULL && indice > 0) {
		nodo = nodo->siguiente;
		indice--;
	}
	return nodo;
}

static struct persona* persona_en(struct lista_personas* lista, int indice) {
	struct nodo_lista_personas* nodo;
	if (lista == NULL || indice < 0) {
		return NULL;
	}
	nodo = lista->inicio;
	while (nodo != NULL && indice > 0) {
		nodo = nodo->siguiente;
		indice--;
	}
	if (nodo == NULL) {
		return NULL;
	}
	return nodo->persona;
}

static struct persona* persona_azar(struct comuna* comuna) {
	int cantidad;
	if (comuna == NULL || comuna->personas == NULL) {
		return NULL;
	}
	cantidad = largo_personas(comuna->personas);
	if (cantidad <= 0) {
		return NULL;
	}
	return persona_en(comuna->personas, aleatorio(0, cantidad - 1));
}

static int hay_algo(struct comuna* comuna);

static int llenar_comunas(struct lista_comunas* lista, struct comuna** salida, int tope) {
	struct nodo_lista_comunas* nodo;
	int cantidad = 0;
	if (lista == NULL || lista->inicio == NULL || tope <= 0) {
		return 0;
	}
	nodo = lista->inicio;
	do {
		salida[cantidad] = nodo->comuna;
		cantidad++;
		nodo = nodo->siguiente;
	} while (nodo != lista->inicio && cantidad < tope);
	return cantidad;
}

static void linea_carencia(struct comuna* comuna, int indice) {
	static int cache_indice = -2;
	static char cache_recurso[80];
	static char cache_frase[180];
	struct nodo_recurso* falta = primera_carencia(comuna);
	char recurso[80];
	if (falta == NULL) {
		return;
	}
	copiar_nombre(falta->nombre, recurso, (int)sizeof(recurso));
	if (indice != cache_indice || strcmp(recurso, cache_recurso) != 0) {
		snprintf(cache_frase, sizeof(cache_frase), "%s", dialogo_queja(recurso));
		snprintf(cache_recurso, sizeof(cache_recurso), "%s", recurso);
		cache_indice = indice;
	}
	printf("  %s%s%s\n", TUI_ROJO, cache_frase, TUI_RESET);
}

static void pantalla_intro(void) {
	tui_cls();
	ascii_logo();
	printf("\n            %sDelegad@ de la provincia%s\n\n", TUI_TITULO, TUI_RESET);
	tui_escribir(
		"  Eres delegad@ de una provincia de tres comunas.\n"
		"  No firmas decretos. Escuchas, repartes y sostienes\n"
		"  lo que es de todas.\n"
		"\n"
		"  Cada dia alguien pide un tema para leer, alguien\n"
		"  sugiere un regalo, y alguien se queja si el almacen\n"
		"  amanece vacio.\n"
		"\n"
		"  Tu periodo dura 25 dias. Si una comuna tuya se vacia\n"
		"  de satisfaccion o se llena de necesidad, el caos\n"
		"  te saca del cargo.\n"
		"\n"
	);
	printf("  %sPaula Flores Solano y Samuel Ureña Gonzalez%s\n", TUI_DIM, TUI_RESET);
	widget_ayuda("Enter para seguir");
	esperar_continuar();
}

static void pantalla_config(void) {
	const char* etiquetas[4] = {"Comunas", "Bienes", "Servicios", "Personas"};
	int valores[4] = {3, 1, 1, 1};
	int minimos[4] = {3, 1, 1, 1};
	int maximos[4] = {50, 20, 20, 500};
	int campo = 0;

	for (;;) {
		struct tui_tecla tecla;
		tui_cls();
		widget_titulo("Configuracion");
		printf("\n");
		for (int i = 0; i < 4; i++) {
			const char* marca = (i == campo) ? TUI_TITULO : TUI_DIM;
			printf("   %s%-12s  [ %3d ]%s   %s%d-%d%s\n",
				marca, etiquetas[i], valores[i], TUI_RESET, TUI_DIM, minimos[i], maximos[i], TUI_RESET);
		}
		widget_ayuda("arriba/abajo cambia el campo    izquierda/derecha cambia el valor    Enter confirma");
		fflush(stdout);
		tecla = tui_leer_tecla();
		if (tecla.codigo == TECLA_DERECHA && valores[campo] < maximos[campo]) {
			valores[campo]++;
		} else if (tecla.codigo == TECLA_IZQUIERDA && valores[campo] > minimos[campo]) {
			valores[campo]--;
		} else if (tecla.codigo == TECLA_ARRIBA) {
			campo = (campo + 3) % 4;
		} else if (tecla.codigo == TECLA_ABAJO || tecla.codigo == TECLA_TAB) {
			campo = (campo + 1) % 4;
		} else if (tecla.codigo == TECLA_ENTER) {
			break;
		}
	}

	cantidad_comunas = valores[0];
	bienes = valores[1];
	servicios = valores[2];
	personas = valores[3];
}

/* Intro y configuracion con teclado. Sin TTY, usa el dialogo de texto.
	Entrada: ninguna
	Salida: las variables globales de la partida quedan definidas */
void tui_preparar_partida(void) {
	if (!tui_es_tty()) {
		configuracion_inicial();
		return;
	}
	pantalla_intro();
	pantalla_config();
}

/* Mensaje breve que espera Enter.
	Entrada: el texto
	Salida: la pausa */
void tui_aviso(const char* mensaje) {
	if (!tui_es_tty()) {
		printf("%s\n", mensaje != NULL ? mensaje : "");
		return;
	}
	tui_cls();
	widget_titulo("Aviso");
	printf("\n  %s\n", mensaje != NULL ? mensaje : "");
	widget_ayuda("Enter");
	esperar_continuar();
}

static void ficha_persona(struct persona* persona) {
	char nombre[96];
	char servicio[96];
	struct nodo_recurso* oficio = servicio_nacimiento(persona);
	copiar_nombre(persona != NULL ? persona->nombre : NULL, nombre, (int)sizeof(nombre));
	if (oficio == NULL) {
		snprintf(servicio, sizeof(servicio), "sin servicio");
	} else {
		copiar_nombre(oficio->nombre, servicio, (int)sizeof(servicio));
	}
	ascii_npc(oficio != NULL ? servicio : NULL);
	printf("\n  %s%s%s\n", TUI_NEGRITA, nombre, TUI_RESET);
	printf("  %sservicio de nacimiento:%s %s\n", TUI_DIM, TUI_RESET, servicio);
}

static void pantalla_ciudadano(struct comuna* comuna, int rol) {
	char comuna_nombre[80];
	char servicio[96];
	char frase[180];
	struct persona* persona = persona_azar(comuna);
	struct nodo_recurso* oficio;
	copiar_nombre(comuna != NULL ? comuna->nombre : NULL, comuna_nombre, (int)sizeof(comuna_nombre));
	tui_cls();
	widget_titulo("Ciudadanos del dia");
	printf("\n  %s%s%s\n\n", TUI_CIAN, comuna_nombre, TUI_RESET);
	if (persona == NULL) {
		printf("  Esta comuna amanece en silencio.\n");
		widget_ayuda("Enter");
		esperar_continuar();
		return;
	}
	oficio = servicio_nacimiento(persona);
	copiar_nombre(oficio != NULL ? oficio->nombre : NULL, servicio, (int)sizeof(servicio));
	ficha_persona(persona);
	if (rol == 0) {
		snprintf(frase, sizeof(frase), "%s", dialogo_pedido_lectura(oficio != NULL ? servicio : NULL));
	} else if (rol == 1) {
		struct nodo_lista_recursos* nodo = (comuna != NULL && comuna->bienes != NULL) ? comuna->bienes->inicio : NULL;
		struct nodo_recurso* bien = NULL;
		char bien_nombre[80];
		while (nodo != NULL) {
			if (bien == NULL || nodo->recurso->cantidad > 0) {
				bien = nodo->recurso;
				if (nodo->recurso->cantidad > 0) {
					break;
				}
			}
			nodo = nodo->siguiente;
		}
		copiar_nombre(bien != NULL ? bien->nombre : NULL, bien_nombre, (int)sizeof(bien_nombre));
		snprintf(frase, sizeof(frase), "%s", dialogo_sugerencia(bien != NULL ? bien_nombre : NULL));
	} else {
		struct nodo_recurso* falta = primera_carencia(comuna);
		if (falta != NULL) {
			char recurso[80];
			copiar_nombre(falta->nombre, recurso, (int)sizeof(recurso));
			snprintf(frase, sizeof(frase), "%s", dialogo_queja_de(oficio != NULL ? servicio : NULL, recurso));
		} else {
			snprintf(frase, sizeof(frase), "%s", dialogo_meme());
		}
	}
	printf("\n  \"%s\"\n", frase);
	widget_ayuda("Enter");
	esperar_continuar();
}

/* Tres vecinos, uno por comuna del jugador. No cambia el estado.
	Entrada: el jugador
	Salida: las tres voces en pantalla */
void tui_ciudadanos_dia(struct jugador* jugador) {
	if (!tui_es_tty() || jugador == NULL) {
		return;
	}
	for (int i = 0; i < jugador->cantidad; i++) {
		pantalla_ciudadano(jugador->comunas[i], i % 3);
	}
}

static void lista_bienes(struct comuna* comuna) {
	int total = (comuna != NULL && comuna->bienes != NULL) ? largo_recurso(comuna->bienes) : 0;
	int cursor = 0;
	while (1) {
		struct tui_tecla tecla;
		int inicio = 0;
		char nombre_comuna[80];
		copiar_nombre(comuna != NULL ? comuna->nombre : NULL, nombre_comuna, (int)sizeof(nombre_comuna));
		if (total > 0 && cursor >= total) {
			cursor = total - 1;
		}
		if (cursor < 0) {
			cursor = 0;
		}
		if (cursor >= 6) {
			inicio = cursor - 5;
		}
		tui_cls();
		widget_titulo("Bienes");
		printf("\n  %s\n", nombre_comuna);
		if (total == 0) {
			printf("\n  No hay bienes en esta comuna.\n");
		} else {
			struct nodo_lista_recursos* elegido = recurso_en(comuna->bienes, cursor);
			char nombre_bien[80];
			copiar_nombre(elegido != NULL ? elegido->recurso->nombre : NULL, nombre_bien, (int)sizeof(nombre_bien));
			printf("\n");
			ascii_recurso(nombre_bien);
			printf("\n     %s%s%s", TUI_NEGRITA, nombre_bien, TUI_RESET);
			if (elegido != NULL) {
				printf("   %d/%d", elegido->recurso->cantidad, elegido->recurso->maximo);
			}
			printf("\n\n");
		}
		for (int i = inicio; i < total && i < inicio + 6; i++) {
			struct nodo_lista_recursos* nodo = recurso_en(comuna->bienes, i);
			char nombre[80];
			char extra[48];
			if (nodo == NULL) {
				continue;
			}
			copiar_nombre(nodo->recurso->nombre, nombre, (int)sizeof(nombre));
			if (nodo->recurso->necesidad && nodo->recurso->cantidad == 0) {
				snprintf(extra, sizeof(extra), "   %s%d/%d  falta%s", TUI_ROJO, nodo->recurso->cantidad, nodo->recurso->maximo, TUI_RESET);
			} else {
				snprintf(extra, sizeof(extra), "   %d/%d", nodo->recurso->cantidad, nodo->recurso->maximo);
			}
			widget_fila(i == cursor, nombre, extra);
		}
		linea_carencia(comuna, 100 + cursor);
		widget_ayuda("arriba/abajo recorre    Esc vuelve    1-9 salta");
		fflush(stdout);
		tecla = tui_leer_tecla();
		if (tecla.codigo == TECLA_ESC || tecla.codigo == TECLA_Q) {
			return;
		}
		if (tecla.codigo == TECLA_ARRIBA && cursor > 0) {
			cursor--;
		} else if (tecla.codigo == TECLA_ABAJO && cursor + 1 < total) {
			cursor++;
		} else if (tecla.caracter >= '1' && tecla.caracter <= '9') {
			int salto = tecla.caracter - '1';
			if (salto < total) {
				cursor = salto;
			}
		}
	}
}

struct servicio_visto {
	struct string* nombre;
	int cuantos;
};

static int juntar_servicios(struct comuna* comuna, struct servicio_visto* vistos, int tope) {
	struct nodo_lista_personas* persona;
	int cantidad = 0;
	if (comuna == NULL || comuna->personas == NULL) {
		return 0;
	}
	persona = comuna->personas->inicio;
	while (persona != NULL) {
		struct nodo_lista_recursos* servicio = NULL;
		if (persona->persona != NULL && persona->persona->servicios != NULL) {
			servicio = persona->persona->servicios->inicio;
		}
		while (servicio != NULL) {
			int puesto = -1;
			for (int i = 0; i < cantidad; i++) {
				if (mismos_nombres(vistos[i].nombre, servicio->recurso->nombre)) {
					puesto = i;
					break;
				}
			}
			if (puesto >= 0) {
				vistos[puesto].cuantos++;
			} else if (cantidad < tope) {
				vistos[cantidad].nombre = servicio->recurso->nombre;
				vistos[cantidad].cuantos = 1;
				cantidad++;
			}
			servicio = servicio->siguiente;
		}
		persona = persona->siguiente;
	}
	return cantidad;
}

static void lista_servicios(struct comuna* comuna) {
	struct servicio_visto vistos[64];
	int total = juntar_servicios(comuna, vistos, 64);
	int cursor = 0;
	while (1) {
		struct tui_tecla tecla;
		int inicio = 0;
		if (total > 0 && cursor >= total) {
			cursor = total - 1;
		}
		if (cursor >= 6) {
			inicio = cursor - 5;
		}
		tui_cls();
		widget_titulo("Servicios");
		printf("\n");
		if (total == 0) {
			printf("  Nadie aqui porta un servicio.\n");
		} else {
			char nombre_servicio[80];
			copiar_nombre(vistos[cursor].nombre, nombre_servicio, (int)sizeof(nombre_servicio));
			ascii_recurso(nombre_servicio);
			printf("\n     %s%s%s   x%d\n\n", TUI_NEGRITA, nombre_servicio, TUI_RESET, vistos[cursor].cuantos);
		}
		for (int i = inicio; i < total && i < inicio + 6; i++) {
			char nombre[80];
			char extra[24];
			copiar_nombre(vistos[i].nombre, nombre, (int)sizeof(nombre));
			snprintf(extra, sizeof(extra), "   x%d", vistos[i].cuantos);
			widget_fila(i == cursor, nombre, extra);
		}
		widget_ayuda("arriba/abajo recorre    Esc vuelve");
		fflush(stdout);
		tecla = tui_leer_tecla();
		if (tecla.codigo == TECLA_ESC || tecla.codigo == TECLA_Q) {
			return;
		}
		if (tecla.codigo == TECLA_ARRIBA && cursor > 0) {
			cursor--;
		} else if (tecla.codigo == TECLA_ABAJO && cursor + 1 < total) {
			cursor++;
		}
	}
}

static void lista_personas_tui(struct comuna* comuna) {
	int total = (comuna != NULL && comuna->personas != NULL) ? largo_personas(comuna->personas) : 0;
	int cursor = 0;
	int hablando = 0;
	char cita[180];
	char meme[180];
	cita[0] = '\0';
	meme[0] = '\0';

	while (1) {
		struct tui_tecla tecla;
		struct persona* elegida = persona_en(comuna != NULL ? comuna->personas : NULL, cursor);
		int inicio = 0;
		int ventana = 8;
		if (total > 0 && cursor >= total) {
			cursor = total - 1;
		}
		if (cursor >= ventana) {
			inicio = cursor - (ventana - 1);
		}
		tui_cls();
		widget_titulo("Personas");
		printf("\n");
		if (elegida != NULL) {
			ficha_persona(elegida);
		} else {
			printf("  No hay personas en esta comuna.\n");
		}
		if (hablando && cita[0] != '\0') {
			printf("\n  \"%s\"\n", cita);
			if (meme[0] != '\0') {
				printf("  %s%s%s\n", TUI_DIM, meme, TUI_RESET);
			}
		}
		printf("\n");
		for (int i = inicio; i < total && i < inicio + ventana; i++) {
			struct persona* persona = persona_en(comuna->personas, i);
			struct nodo_recurso* oficio = servicio_nacimiento(persona);
			char nombre[80];
			char extra[80];
			char servicio[64];
			copiar_nombre(persona != NULL ? persona->nombre : NULL, nombre, (int)sizeof(nombre));
			copiar_nombre(oficio != NULL ? oficio->nombre : NULL, servicio, (int)sizeof(servicio));
			snprintf(extra, sizeof(extra), "   %s", servicio);
			widget_fila(i == cursor, nombre, extra);
		}
		widget_ayuda(hablando ? "Enter cierra el dialogo    Esc vuelve" : "Enter hablar    arriba/abajo    Esc vuelve");
		fflush(stdout);
		tecla = tui_leer_tecla();
		if (tecla.codigo == TECLA_ESC || tecla.codigo == TECLA_Q) {
			if (hablando) {
				hablando = 0;
				continue;
			}
			return;
		}
		if (hablando && tecla.codigo == TECLA_ENTER) {
			hablando = 0;
			continue;
		}
		if (tecla.codigo == TECLA_ARRIBA && cursor > 0) {
			cursor--;
			hablando = 0;
		} else if (tecla.codigo == TECLA_ABAJO && cursor + 1 < total) {
			cursor++;
			hablando = 0;
		} else if (tecla.codigo == TECLA_ENTER && elegida != NULL) {
			struct nodo_recurso* oficio = servicio_nacimiento(elegida);
			char servicio[96];
			copiar_nombre(oficio != NULL ? oficio->nombre : NULL, servicio, (int)sizeof(servicio));
			snprintf(cita, sizeof(cita), "%s", dialogo_de_servicio(servicio));
			meme[0] = '\0';
			if (aleatorio(0, 2) == 0) {
				snprintf(meme, sizeof(meme), "%s", dialogo_meme());
			}
			hablando = 1;
		}
	}
}

static void inspeccionar(struct comuna* comuna) {
	int menu = 0;
	const char* opciones[3] = {"Bienes", "Servicios", "Personas"};
	while (1) {
		struct tui_tecla tecla;
		char nombre[80];
		copiar_nombre(comuna != NULL ? comuna->nombre : NULL, nombre, (int)sizeof(nombre));
		tui_cls();
		ascii_comuna(nombre);
		printf("\n");
		widget_carrusel(nombre, comuna != NULL ? comuna->satisfaccion : 0, comuna != NULL ? comuna->necesidad : 0);
		widget_barra("Satisfaccion", comuna != NULL ? comuna->satisfaccion : 0, TUI_VERDE);
		widget_barra("Necesidad", comuna != NULL ? comuna->necesidad : 0, TUI_AMARILLO);
		linea_carencia(comuna, menu);
		printf("\n");
		for (int i = 0; i < 3; i++) {
			widget_fila(i == menu, opciones[i], "");
		}
		widget_ayuda("arriba/abajo elige lista    Enter entra    Esc vuelve al hub");
		fflush(stdout);
		tecla = tui_leer_tecla();
		if (tecla.codigo == TECLA_ESC || tecla.codigo == TECLA_Q) {
			return;
		}
		if (tecla.codigo == TECLA_ARRIBA) {
			menu = (menu + 2) % 3;
		} else if (tecla.codigo == TECLA_ABAJO || tecla.codigo == TECLA_TAB) {
			menu = (menu + 1) % 3;
		} else if (tecla.caracter >= '1' && tecla.caracter <= '3') {
			menu = tecla.caracter - '1';
		} else if (tecla.codigo == TECLA_ENTER) {
			if (menu == 0) {
				lista_bienes(comuna);
			} else if (menu == 1) {
				lista_servicios(comuna);
			} else {
				lista_personas_tui(comuna);
			}
		}
	}
}

/* Rota las comunas del jugador hasta que elige regalar.
	Entrada: el jugador y el turno (desde 1)
	Salida: vuelve cuando la accion es regalar */
void tui_hub(struct jugador* jugador, int turno) {
	int comuna = 0;
	int accion = 0;
	if (!tui_es_tty() || jugador == NULL || jugador->cantidad < 1) {
		return;
	}
	while (1) {
		struct tui_tecla tecla;
		struct comuna* actual = jugador->comunas[comuna];
		char nombre[80];
		char ayuda[120];
		copiar_nombre(actual != NULL ? actual->nombre : NULL, nombre, (int)sizeof(nombre));
		tui_cls();
		ascii_comuna(nombre);
		printf("\n");
		widget_carrusel(nombre, actual != NULL ? actual->satisfaccion : 0, actual != NULL ? actual->necesidad : 0);
		widget_barra("Satisfaccion", actual != NULL ? actual->satisfaccion : 0, TUI_VERDE);
		widget_barra("Necesidad", actual != NULL ? actual->necesidad : 0, TUI_AMARILLO);
		linea_carencia(actual, comuna);
		widget_acciones("Inspeccionar", "Regalar", accion);
		snprintf(ayuda, sizeof(ayuda), "izquierda/derecha rota    arriba/abajo elige    Turno %d/%d", turno, TOPE_TURNOS);
		widget_ayuda(ayuda);
		fflush(stdout);
		tecla = tui_leer_tecla();
		if (tecla.codigo == TECLA_IZQUIERDA) {
			comuna = (comuna + jugador->cantidad - 1) % jugador->cantidad;
		} else if (tecla.codigo == TECLA_DERECHA) {
			comuna = (comuna + 1) % jugador->cantidad;
		} else if (tecla.codigo == TECLA_ARRIBA || tecla.codigo == TECLA_ABAJO || tecla.codigo == TECLA_TAB) {
			accion = 1 - accion;
		} else if (tecla.caracter == '1') {
			accion = 0;
		} else if (tecla.caracter == '2') {
			accion = 1;
		} else if (tecla.codigo == TECLA_ENTER) {
			if (accion == 0) {
				inspeccionar(actual);
			} else {
				return;
			}
		}
	}
}

static int elegir_cantidad(struct nodo_recurso* recurso, int* cantidad) {
	char nombre[80];
	copiar_nombre(recurso->nombre, nombre, (int)sizeof(nombre));
	*cantidad = 1;
	while (1) {
		struct tui_tecla tecla;
		tui_cls();
		widget_titulo("Cuanto regalas");
		printf("\n");
		ascii_recurso(nombre);
		printf("\n  %s%s%s\n", TUI_NEGRITA, nombre, TUI_RESET);
		printf("\n    %s<-%s  %s%d%s  /  %d  %s->%s\n",
			TUI_CIAN, TUI_RESET, TUI_TITULO, *cantidad, TUI_RESET, recurso->cantidad, TUI_CIAN, TUI_RESET);
		widget_ayuda("izquierda/derecha cambia la cantidad    Enter confirma    Esc vuelve");
		fflush(stdout);
		tecla = tui_leer_tecla();
		if (tecla.codigo == TECLA_ESC || tecla.codigo == TECLA_Q) {
			return 0;
		}
		if ((tecla.codigo == TECLA_DERECHA || tecla.codigo == TECLA_ARRIBA) && *cantidad < recurso->cantidad) {
			(*cantidad)++;
		} else if ((tecla.codigo == TECLA_IZQUIERDA || tecla.codigo == TECLA_ABAJO) && *cantidad > 1) {
			(*cantidad)--;
		} else if (tecla.codigo == TECLA_ENTER) {
			return 1;
		}
	}
}

static void despedir_persona(struct persona* persona, const char* destino) {
	struct nodo_recurso* oficio = servicio_nacimiento(persona);
	char servicio[96];
	copiar_nombre(oficio != NULL ? oficio->nombre : NULL, servicio, (int)sizeof(servicio));
	tui_cls();
	widget_titulo("Se muda");
	printf("\n");
	ficha_persona(persona);
	printf("\n  \"%s\"\n", dialogo_despedida(oficio != NULL ? servicio : NULL));
	if (destino != NULL && destino[0] != '\0') {
		printf("\n  Se va hacia %s%s%s.\n", TUI_CIAN, destino, TUI_RESET);
	}
	widget_ayuda("Enter");
	esperar_continuar();
}

/* Regalo con la misma logica que el intercambio de texto.
	Entrada: la sociedad y el jugador
	Salida: 1 si el regalo quedo resuelto, 0 si se vuelve al hub */
int tui_regalo_jugador(struct lista_comunas* sociedad, struct jugador* jugador) {
	struct comuna* anillo[TOPE_REGALOS_LOG];
	int paso = 0;
	int i_da = 0;
	int i_rec = 0;
	int tipo = 0;
	int cursor = 0;
	char error[120];
	error[0] = '\0';

	if (!tui_es_tty()) {
		return 0;
	}
	if (sociedad == NULL || jugador == NULL || jugador->cantidad < 1) {
		return 0;
	}
	if (largo_comuna(sociedad) < 2) {
		tui_aviso("Hay una sola comuna, no hay otra a quien regalar.");
		return 1;
	}

	while (1) {
		struct tui_tecla tecla;
		struct comuna* da = jugador->comunas[i_da];
		int total_anillo = llenar_comunas(sociedad, anillo, TOPE_REGALOS_LOG);
		struct comuna* recibe = NULL;
		char nombre_da[80];
		char nombre_rec[80];
		if (total_anillo < 1) {
			return 1;
		}
		if (i_rec < 0) {
			i_rec = total_anillo - 1;
		}
		if (i_rec >= total_anillo) {
			i_rec = 0;
		}
		recibe = anillo[i_rec];
		copiar_nombre(da != NULL ? da->nombre : NULL, nombre_da, (int)sizeof(nombre_da));
		copiar_nombre(recibe != NULL ? recibe->nombre : NULL, nombre_rec, (int)sizeof(nombre_rec));

		tui_cls();
		if (paso == 0) {
			widget_titulo("Quien da el regalo");
			printf("\n");
			ascii_comuna(nombre_da);
			printf("\n");
			widget_carrusel(nombre_da, da != NULL ? da->satisfaccion : 0, da != NULL ? da->necesidad : 0);
			widget_ayuda("izquierda/derecha elige tu comuna    Enter sigue    Esc vuelve al hub");
		} else if (paso == 1) {
			widget_titulo("Quien recibe");
			printf("\n");
			ascii_comuna(nombre_rec);
			printf("\n  Sale de %s%s%s\n\n", TUI_CIAN, nombre_da, TUI_RESET);
			widget_anillo(i_rec + 1, total_anillo, nombre_rec,
				recibe != NULL ? recibe->necesidad : 0,
				recibe != NULL ? recibe->satisfaccion : 0,
				es_comuna_jugador(jugador, recibe));
			if (recibe == da) {
				printf("\n  %sNo puedes regalarle a la misma comuna.%s\n", TUI_ROJO, TUI_RESET);
			}
			widget_ayuda("izquierda/derecha da la vuelta al anillo    Enter sigue    Esc vuelve");
		} else if (paso == 2) {
			widget_titulo("Que se manda");
			printf("\n  %s%s%s  ->  %s%s%s\n\n", TUI_CIAN, nombre_da, TUI_RESET, TUI_CIAN, nombre_rec, TUI_RESET);
			widget_fila(tipo == 0, "Bien", "");
			widget_fila(tipo == 1, "Persona (se lleva el servicio)", "");
			widget_ayuda("arriba/abajo elige    Enter sigue    Esc vuelve");
		} else {
			int total = 0;
			int inicio = 0;
			widget_titulo(tipo == 0 ? "Elige el bien" : "Quien se muda");
			printf("\n  %s%s%s  ->  %s%s%s\n\n", TUI_CIAN, nombre_da, TUI_RESET, TUI_CIAN, nombre_rec, TUI_RESET);
			if (tipo == 0) {
				total = (da != NULL && da->bienes != NULL) ? largo_recurso(da->bienes) : 0;
			} else {
				total = (da != NULL && da->personas != NULL) ? largo_personas(da->personas) : 0;
			}
			if (total == 0) {
				printf("  %sNo hay nada de ese tipo para mandar.%s\n", TUI_AMARILLO, TUI_RESET);
			}
			if (cursor >= total && total > 0) {
				cursor = total - 1;
			}
			if (cursor < 0) {
				cursor = 0;
			}
			if (cursor >= 8) {
				inicio = cursor - 7;
			}
			if (tipo == 0 && da != NULL) {
				struct nodo_lista_recursos* elegido = recurso_en(da->bienes, cursor);
				char nombre_bien[80];
				copiar_nombre(elegido != NULL ? elegido->recurso->nombre : NULL, nombre_bien, (int)sizeof(nombre_bien));
				ascii_recurso(nombre_bien);
				printf("\n");
			} else if (da != NULL) {
				struct persona* elegida = persona_en(da->personas, cursor);
				if (elegida != NULL) {
					ficha_persona(elegida);
					printf("\n");
				}
			}
			for (int i = inicio; i < total && i < inicio + 8; i++) {
				char nombre[80];
				char extra[40];
				extra[0] = '\0';
				if (tipo == 0) {
					struct nodo_lista_recursos* nodo = recurso_en(da->bienes, i);
					copiar_nombre(nodo != NULL ? nodo->recurso->nombre : NULL, nombre, (int)sizeof(nombre));
					if (nodo != NULL) {
						snprintf(extra, sizeof(extra), "   %d", nodo->recurso->cantidad);
					}
				} else {
					struct persona* persona = persona_en(da->personas, i);
					struct nodo_recurso* oficio = servicio_nacimiento(persona);
					copiar_nombre(persona != NULL ? persona->nombre : NULL, nombre, (int)sizeof(nombre));
					if (oficio != NULL) {
						char servicio[28];
						copiar_nombre(oficio->nombre, servicio, (int)sizeof(servicio));
						snprintf(extra, sizeof(extra), "   %s", servicio);
					}
				}
				widget_fila(i == cursor, nombre, extra);
			}
			widget_ayuda("arriba/abajo elige    Enter confirma    Esc vuelve");
		}
		if (error[0] != '\0') {
			printf("  %s%s%s\n", TUI_ROJO, error, TUI_RESET);
		}
		fflush(stdout);
		tecla = tui_leer_tecla();
		error[0] = '\0';

		if (tecla.codigo == TECLA_ESC || tecla.codigo == TECLA_Q) {
			if (paso == 0) {
				return 0;
			}
			paso--;
			cursor = 0;
			continue;
		}

		if (paso == 0) {
			if (tecla.codigo == TECLA_IZQUIERDA) {
				i_da = (i_da + jugador->cantidad - 1) % jugador->cantidad;
			} else if (tecla.codigo == TECLA_DERECHA) {
				i_da = (i_da + 1) % jugador->cantidad;
			} else if (tecla.codigo == TECLA_ENTER) {
				paso = 1;
			}
		} else if (paso == 1) {
			if (tecla.codigo == TECLA_IZQUIERDA) {
				i_rec--;
			} else if (tecla.codigo == TECLA_DERECHA) {
				i_rec++;
			} else if (tecla.codigo == TECLA_ENTER) {
				if (recibe == da) {
					snprintf(error, sizeof(error), "Esa es la comuna que esta dando.");
				} else {
					paso = 2;
				}
			}
		} else if (paso == 2) {
			if (tecla.codigo == TECLA_ARRIBA || tecla.codigo == TECLA_ABAJO || tecla.codigo == TECLA_TAB) {
				tipo = 1 - tipo;
			} else if (tecla.caracter == '1') {
				tipo = 0;
			} else if (tecla.caracter == '2') {
				tipo = 1;
			} else if (tecla.codigo == TECLA_ENTER) {
				int total = 0;
				if (tipo == 0) {
					total = (da != NULL && da->bienes != NULL) ? largo_recurso(da->bienes) : 0;
					if (total == 0 || da == NULL || hay_algo(da) == 0) {
						tui_aviso("No hay cantidad disponible para regalar.");
						return 1;
					}
				} else {
					total = (da != NULL && da->personas != NULL) ? largo_personas(da->personas) : 0;
					if (total == 0) {
						tui_aviso("No hay personas para mudar.");
						return 1;
					}
				}
				cursor = 0;
				paso = 3;
			}
		} else if (tecla.codigo == TECLA_ARRIBA && cursor > 0) {
			cursor--;
		} else if (tecla.codigo == TECLA_ABAJO) {
			int total = 0;
			if (tipo == 0 && da != NULL && da->bienes != NULL) {
				total = largo_recurso(da->bienes);
			}
			if (tipo == 1 && da != NULL && da->personas != NULL) {
				total = largo_personas(da->personas);
			}
			if (cursor + 1 < total) {
				cursor++;
			}
		} else if (tecla.codigo == TECLA_ENTER && da != NULL) {
			if (tipo == 1) {
				struct persona* persona = persona_en(da->personas, cursor);
				if (regalo_persona_jugador(da, recibe, persona)) {
					despedir_persona(persona, nombre_rec);
					return 1;
				}
				snprintf(error, sizeof(error), "No se pudo mudar a esa persona.");
			} else {
				struct nodo_lista_recursos* nodo = recurso_en(da->bienes, cursor);
				int cantidad = 1;
				if (nodo == NULL || nodo->recurso->cantidad <= 0) {
					snprintf(error, sizeof(error), "Ese recurso esta en 0. Elije otro.");
				} else if (elegir_cantidad(nodo->recurso, &cantidad)) {
					if (regalo_bien_jugador(da, recibe, nodo->recurso, cantidad)) {
						tui_aviso("El regalo quedo hecho.");
						return 1;
					}
					snprintf(error, sizeof(error), "No se pudo mover el bien.");
				}
			}
		}
	}
}

static int hay_algo(struct comuna* comuna) {
	struct nodo_lista_recursos* nodo;
	if (comuna == NULL || comuna->bienes == NULL) {
		return 0;
	}
	nodo = comuna->bienes->inicio;
	while (nodo != NULL) {
		if (nodo->recurso->cantidad > 0) {
			return 1;
		}
		nodo = nodo->siguiente;
	}
	return 0;
}

/* Lectura con carrusel y una linea de texto.
	Entrada: el jugador y la biblioteca
	Salida: 1 si el turno de lectura termino, 0 si la busqueda no pudo hacerse */
int tui_lectura_jugador(struct jugador* jugador, struct biblioteca* biblioteca) {
	int indice = 0;
	struct informe_lectura informe;
	struct string* texto;
	if (!tui_es_tty()) {
		return 0;
	}
	if (jugador == NULL || biblioteca == NULL) {
		return 0;
	}
	if (jugador->cantidad < 1) {
		return 1;
	}

	while (1) {
		struct tui_tecla tecla;
		struct comuna* comuna = jugador->comunas[indice];
		char nombre[80];
		copiar_nombre(comuna != NULL ? comuna->nombre : NULL, nombre, (int)sizeof(nombre));
		tui_cls();
		widget_titulo("Lectura");
		printf("\n");
		ascii_comuna(nombre);
		printf("\n");
		widget_carrusel(nombre, comuna != NULL ? comuna->satisfaccion : 0, comuna != NULL ? comuna->necesidad : 0);
		widget_ayuda("izquierda/derecha elige la comuna    Enter busca un texto");
		fflush(stdout);
		tecla = tui_leer_tecla();
		if (tecla.codigo == TECLA_IZQUIERDA) {
			indice = (indice + jugador->cantidad - 1) % jugador->cantidad;
		} else if (tecla.codigo == TECLA_DERECHA) {
			indice = (indice + 1) % jugador->cantidad;
		} else if (tecla.codigo == TECLA_ENTER) {
			break;
		}
	}

	tui_cls();
	widget_titulo("Lectura");
	printf("\n  %sTexto a buscar:%s ", TUI_CIAN, TUI_RESET);
	fflush(stdout);
	tui_modo_linea();
	texto = linea_archivo(stdin);
	tui_modo_raw();
	if (texto == NULL) {
		return 0;
	}

	if (ejecutar_lectura(jugador->comunas[indice], biblioteca, texto, &informe) == 0) {
		liberar_string(texto);
		tui_aviso("No se pudo buscar ese texto.");
		return 0;
	}
	liberar_string(texto);

	tui_cls();
	widget_titulo("Lectura");
	printf("\n");
	if (informe.caso == LECTURA_SIN_PERSONA) {
		printf("  Esa comuna no tiene personas para leer.\n");
	} else if (informe.caso == LECTURA_SIN_LIBROS) {
		printf("  Ningun libro contiene ese texto.\n");
	} else if (informe.caso == LECTURA_YA_LEYO) {
		printf("  %s%s%s ya leyo todos los libros de esta busqueda.\n", TUI_NEGRITA, informe.persona, TUI_RESET);
	} else if (informe.caso == LECTURA_OK || informe.caso == LECTURA_APRENDIO) {
		printf("  %s%s%s lee %s%s%s  %s(%d libros)%s\n",
			TUI_VERDE, informe.persona, TUI_RESET, TUI_NEGRITA, informe.libro, TUI_RESET, TUI_DIM, informe.libros_leidos, TUI_RESET);
		if (informe.caso == LECTURA_APRENDIO) {
			printf("\n  Aprendio el servicio %s%s%s.\n", TUI_TITULO, informe.servicio, TUI_RESET);
		}
	} else {
		printf("  La busqueda no dejo nada.\n");
	}
	widget_ayuda("Enter");
	esperar_continuar();
	return 1;
}

/* Pasa la lista de regalos ajenos. Pausa si el destino es del jugador.
	Entrada: el jugador y el registro del dia
	Salida: la lista en pantalla */
void tui_montaje_autonomo(struct jugador* jugador, struct registro_regalos* registro) {
	int cantidad;
	if (!tui_es_tty()) {
		return;
	}
	cantidad = (registro != NULL) ? registro->cantidad : 0;
	if (cantidad <= 0) {
		tui_cls();
		widget_titulo("Regalos de las otras comunas");
		printf("\n  Nadie de afuera solto un regalo hoy.\n");
		fflush(stdout);
		tui_msleep(700);
		return;
	}
	for (int i = 0; i < cantidad; i++) {
		int destacado = es_comuna_jugador(jugador, registro->entradas[i].destino);
		char origen[80];
		char destino[80];
		copiar_nombre(registro->entradas[i].origen != NULL ? registro->entradas[i].origen->nombre : NULL, origen, (int)sizeof(origen));
		copiar_nombre(registro->entradas[i].destino != NULL ? registro->entradas[i].destino->nombre : NULL, destino, (int)sizeof(destino));
		tui_cls();
		widget_titulo("Regalos de las otras comunas");
		printf("\n");
		ascii_comuna(origen);
		printf("        %s|%s\n        %sv%s\n", TUI_DIM, TUI_RESET, TUI_DIM, TUI_RESET);
		ascii_comuna(destino);
		widget_marco_abrir(destacado);
		printf("  %s\n", registro->entradas[i].mensaje);
		widget_marco_cerrar(destacado);
		printf("\n  %s%d / %d%s\n", TUI_DIM, i + 1, cantidad, TUI_RESET);
		fflush(stdout);
		tui_msleep(destacado ? 1000 : 350);
	}
}

/* Pantalla del evento, antes de aplicar la emergencia o la prosperidad.
	Entrada: 0 ninguno, 1 emergencia, 2 prosperidad, y la comuna
	Salida: el dibujo */
void tui_evento(int tipo, struct comuna* comuna) {
	char nombre[80];
	if (!tui_es_tty()) {
		return;
	}
	copiar_nombre(comuna != NULL ? comuna->nombre : NULL, nombre, (int)sizeof(nombre));
	tui_cls();
	if (tipo == 1) {
		ascii_evento(1);
		printf("\n");
		widget_titulo("Emergencia");
		printf("\n  La tormenta cae sobre %s%s%s.\n", TUI_ROJO, nombre, TUI_RESET);
		widget_ayuda("Enter");
		esperar_continuar();
		return;
	}
	if (tipo == 2) {
		ascii_evento(0);
		printf("\n");
		widget_titulo("Prosperidad");
		printf("\n  El sol se queda en %s%s%s.\n", TUI_VERDE, nombre, TUI_RESET);
		widget_ayuda("Enter");
		esperar_continuar();
		return;
	}
	widget_titulo("El dia");
	printf("\n  %sNingun evento. El tiempo pasa parejo.%s\n", TUI_DIM, TUI_RESET);
	fflush(stdout);
	tui_msleep(700);
}

/* Una frase al cerrar el dia.
	Entrada: el jugador
	Salida: la frase en pantalla */
void tui_linea_postdia(struct jugador* jugador) {
	struct comuna* comuna;
	struct persona* persona;
	char frase[180];
	if (!tui_es_tty() || jugador == NULL || jugador->cantidad < 1) {
		return;
	}
	comuna = jugador->comunas[aleatorio(0, jugador->cantidad - 1)];
	persona = persona_azar(comuna);
	snprintf(frase, sizeof(frase), "%s", dialogo_cierre_dia());
	tui_cls();
	widget_titulo("Cierra el dia");
	printf("\n");
	if (persona != NULL) {
		ficha_persona(persona);
		printf("\n  \"%s\"\n", frase);
	} else {
		printf("  \"%s\"\n", frase);
	}
	if (comuna != NULL) {
		printf("\n");
		widget_barra("Satisfaccion", comuna->satisfaccion, TUI_VERDE);
		widget_barra("Necesidad", comuna->necesidad, TUI_AMARILLO);
	}
	widget_ayuda("Enter");
	esperar_continuar();
}

/* Victoria si la partida siguio, derrota si una comuna del jugador cayo.
	Entrada: el valor que devolvio el ultimo dia, y el turno en que salio el ciclo
	Salida: la pantalla de cierre */
void tui_cerrar_partida(int sigue, int turno) {
	if (!tui_es_tty()) {
		despedida();
		return;
	}
	tui_cls();
	if (sigue == 0) {
		ascii_final(0);
		printf("\n");
		tui_escribir(
			"  Perdiste una comuna.\n"
			"  Llego el caos, la provincia se rompio\n"
			"  y te echaron del cargo.\n"
		);
	} else {
		ascii_final(1);
		printf("\n");
		tui_escribir(
			"  Termino tu periodo.\n"
			"  Mantuviste la provincia a flote.\n"
		);
		printf("\n  %s%d dias en el cargo.%s\n", TUI_DIM, turno - 1, TUI_RESET);
	}
	widget_ayuda("Enter para salir");
	esperar_continuar();
}
