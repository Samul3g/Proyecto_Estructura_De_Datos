#include "ascii_art.h"
#include "dialogos.h"
#include "tui.h"

#include <stdio.h>
#include <string.h>

static unsigned hash_nombre(const char* nombre) {
	unsigned hash = 5381;
	if (nombre == NULL) {
		return hash;
	}
	while (*nombre != '\0') {
		hash = hash * 33u + (unsigned char)(*nombre);
		nombre++;
	}
	return hash;
}

#define ARTE_TOPE 48
#define ARTE_LINEAS 16
#define ARTE_ANCHO 80

struct arte_bloque {
	char nombre[ARTE_ANCHO];
	char linea[ARTE_LINEAS][ARTE_ANCHO];
	int cantidad;
};

static struct arte_bloque comunas_arte[ARTE_TOPE];
static int comunas_cantidad = 0;
static struct arte_bloque recursos_arte[ARTE_TOPE];
static int recursos_cantidad = 0;
static int arte_listo = 0;

static void quitar_salto(char* linea) {
	size_t n = strlen(linea);
	while (n > 0 && (linea[n - 1] == '\n' || linea[n - 1] == '\r')) {
		linea[--n] = '\0';
	}
}

static int es_separador(const char* linea) {
	while (*linea == ' ' || *linea == '\t') {
		linea++;
	}
	if (linea[0] != 'n' || linea[1] != '/') {
		return 0;
	}
	linea += 2;
	while (*linea == ' ' || *linea == '\t') {
		linea++;
	}
	return *linea == '\0';
}

static void copiar_tope(char* destino, int tope, const char* origen) {
	int i = 0;
	if (destino == NULL || tope <= 0) {
		return;
	}
	if (origen == NULL) {
		destino[0] = '\0';
		return;
	}
	while (origen[i] != '\0' && i < tope - 1) {
		destino[i] = origen[i];
		i++;
	}
	destino[i] = '\0';
}

static void agregar_linea(struct arte_bloque* bloque_arte, const char* linea) {
	if (bloque_arte->cantidad >= ARTE_LINEAS) {
		return;
	}
	copiar_tope(bloque_arte->linea[bloque_arte->cantidad], ARTE_ANCHO, linea);
	bloque_arte->cantidad++;
}

static void guardar_bloque(struct arte_bloque* destino, int* cantidad, struct arte_bloque* actual) {
	if (actual->cantidad <= 0 || *cantidad >= ARTE_TOPE) {
		actual->cantidad = 0;
		actual->nombre[0] = '\0';
		return;
	}
	destino[*cantidad] = *actual;
	(*cantidad)++;
	actual->cantidad = 0;
	actual->nombre[0] = '\0';
}

static void placeholder_comuna(void) {
	static const char* lineas[] = {
		"      \\ | /",
		"    --- * ---",
		"      / | \\",
		"   ___|~~~|___",
		"  | placeholder |",
		"  |___|___|___|"
	};
	comunas_cantidad = 1;
	comunas_arte[0].cantidad = 6;
	comunas_arte[0].nombre[0] = '\0';
	for (int i = 0; i < 6; i++) {
		snprintf(comunas_arte[0].linea[i], ARTE_ANCHO, "%s", lineas[i]);
	}
}

static void placeholder_recurso(void) {
	static const char* lineas[] = {
		"   [?]",
		"  |___|",
		"   | |",
		"  /   \\"
	};
	recursos_cantidad = 1;
	snprintf(recursos_arte[0].nombre, ARTE_ANCHO, "placeholder");
	recursos_arte[0].cantidad = 4;
	for (int i = 0; i < 4; i++) {
		snprintf(recursos_arte[0].linea[i], ARTE_ANCHO, "%s", lineas[i]);
	}
}

static void cargar_comunas(FILE* archivo) {
	struct arte_bloque actual;
	char linea[160];
	actual.cantidad = 0;
	actual.nombre[0] = '\0';
	while (fgets(linea, (int)sizeof(linea), archivo) != NULL) {
		quitar_salto(linea);
		if (es_separador(linea)) {
			guardar_bloque(comunas_arte, &comunas_cantidad, &actual);
			continue;
		}
		if (linea[0] == '\0') {
			continue;
		}
		agregar_linea(&actual, linea);
	}
	guardar_bloque(comunas_arte, &comunas_cantidad, &actual);
}

static void cargar_recursos(FILE* archivo) {
	struct arte_bloque actual;
	char linea[160];
	int tiene_nombre = 0;
	actual.cantidad = 0;
	actual.nombre[0] = '\0';
	while (fgets(linea, (int)sizeof(linea), archivo) != NULL) {
		quitar_salto(linea);
		if (es_separador(linea)) {
			guardar_bloque(recursos_arte, &recursos_cantidad, &actual);
			tiene_nombre = 0;
			continue;
		}
		if (linea[0] == '\0') {
			continue;
		}
		if (!tiene_nombre) {
			copiar_tope(actual.nombre, ARTE_ANCHO, linea);
			tiene_nombre = 1;
			continue;
		}
		agregar_linea(&actual, linea);
	}
	guardar_bloque(recursos_arte, &recursos_cantidad, &actual);
}

static void cargar_arte(void) {
	FILE* archivo;
	if (arte_listo) {
		return;
	}
	arte_listo = 1;
	archivo = fopen("sprites_comunas", "r");
	if (archivo != NULL) {
		cargar_comunas(archivo);
		fclose(archivo);
	}
	if (comunas_cantidad == 0) {
		placeholder_comuna();
	}
	archivo = fopen("servicios_ascii", "r");
	if (archivo != NULL) {
		cargar_recursos(archivo);
		fclose(archivo);
	}
	if (recursos_cantidad == 0) {
		placeholder_recurso();
	}
}

static void imprimir_bloque(const char* color, struct arte_bloque* bloque_arte) {
	for (int i = 0; i < bloque_arte->cantidad; i++) {
		printf("        %s%s%s\n", color, bloque_arte->linea[i], TUI_RESET);
	}
}

static void bloque(const char* color, const char* const* lineas, int cantidad) {
	for (int i = 0; i < cantidad; i++) {
		printf("        %s%s%s\n", color, lineas[i], TUI_RESET);
	}
}

/* Sol y techos del arranque.
	Entrada: ninguna
	Salida: el logo */
void ascii_logo(void) {
	static const char* sol[] = {
		"\\ | /",
		"--- * ---",
		"/ | \\"
	};
	static const char* techos[] = {
		"/\\        /\\        /\\",
		"/  \\  /\\  /  \\  /\\  /  \\",
		"/    \\/  \\/    \\/  \\/    \\",
		"/______\\____\\____/____\\_____\\"
	};
	bloque(TUI_AMARILLO, sol, 3);
	printf("\n");
	bloque(TUI_VERDE, techos, 4);
}

/* Silueta solarpunk. El nombre elige una plantilla de `sprites_comunas`.
	Entrada: el nombre de la comuna
	Salida: el dibujo */
void ascii_comuna(const char* nombre) {
	int cual;
	cargar_arte();
	if (comunas_cantidad <= 0) {
		return;
	}
	cual = (int)(hash_nombre(nombre) % (unsigned)comunas_cantidad);
	imprimir_bloque(TUI_VERDE, &comunas_arte[cual]);
}

/* Sprite de la profesion, leido de `profesiones`.
	Entrada: el nombre del servicio, o NULL
	Salida: el sprite, o el placeholder */
void ascii_npc(const char* servicio) {
	dialogo_imprimir_sprite(servicio);
}

/* Dibujo de un bien o un servicio, leido de `servicios_ascii`.
	Entrada: el nombre del recurso
	Salida: su dibujo, o el placeholder si el nombre no esta */
void ascii_recurso(const char* nombre) {
	int elegido = 0;
	int hay_placeholder = 0;
	cargar_arte();
	if (recursos_cantidad <= 0) {
		return;
	}
	for (int i = 0; i < recursos_cantidad; i++) {
		if (strcmp(recursos_arte[i].nombre, "placeholder") == 0) {
			elegido = i;
			hay_placeholder = 1;
		}
		if (nombre != NULL && strcmp(recursos_arte[i].nombre, nombre) == 0) {
			imprimir_bloque(TUI_AMARILLO, &recursos_arte[i]);
			return;
		}
	}
	if (!hay_placeholder) {
		elegido = 0;
	}
	imprimir_bloque(TUI_AMARILLO, &recursos_arte[elegido]);
}

/* Dibujo de tormenta o de sol.
	Entrada: 1 si es emergencia, 0 si es prosperidad
	Salida: el dibujo */
void ascii_evento(int emergencia) {
	if (emergencia) {
		static const char* tormenta[] = {
			"     _  _  _",
			"   _( \\/ )_  /",
			"  (  nube  ) /",
			"   \\______/ /",
			"     / / /",
			"    /  v  v"
		};
		bloque(TUI_ROJO, tormenta, 6);
		return;
	}
	static const char* sol[] = {
		"      \\ | /",
		"    --- * ---",
		"      / | \\",
		"    brotes +++",
		"      \\| |/",
		"       \\|/"
	};
	bloque(TUI_VERDE, sol, 6);
}

/* Dibujo de cierre, distinto si el periodo se cumplio.
	Entrada: 1 si es victoria
	Salida: el dibujo */
void ascii_final(int victoria) {
	ascii_logo();
	if (victoria) {
		printf("\n            %speriodo cumplido%s\n", TUI_VERDE, TUI_RESET);
	} else {
		printf("\n            %sprovincia en caos%s\n", TUI_ROJO, TUI_RESET);
	}
}
