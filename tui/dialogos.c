#include "dialogos.h"
#include "tui.h"

#include <stdio.h>
#include <string.h>
#include "utils.h"

#define PROF_TOPE 40
#define PROF_LINEAS 12
#define PROF_ANCHO 80
#define PROF_FRASES 8
#define PROF_LARGO 160

struct profesion {
	char nombre[PROF_ANCHO];
	char sprite[PROF_LINEAS][PROF_ANCHO];
	int nsprite;
	char frases[PROF_FRASES][PROF_LARGO];
	int nfrases;
	char quejas[PROF_FRASES][PROF_LARGO];
	int nquejas;
	char lectura[PROF_FRASES][PROF_LARGO];
	int nlectura;
	char despedida[PROF_FRASES][PROF_LARGO];
	int ndespedida;
};

static struct profesion profesiones[PROF_TOPE];
static int profesiones_cantidad = 0;
static int profesiones_listo = 0;
static char dialogo_buffer[PROF_LARGO];

static void quitar_salto(char* linea) {
	size_t n = strlen(linea);
	while (n > 0 && (linea[n - 1] == '\n' || linea[n - 1] == '\r')) {
		linea[--n] = '\0';
	}
}

static char* saltar_espacio(char* linea) {
	while (*linea == ' ' || *linea == '\t') {
		linea++;
	}
	return linea;
}

static int es_separador(const char* linea) {
	linea = saltar_espacio((char*)linea);
	if (linea[0] != 'n' || linea[1] != '/') {
		return 0;
	}
	linea += 2;
	while (*linea == ' ' || *linea == '\t') {
		linea++;
	}
	return *linea == '\0';
}

static int es_encabezado(const char* linea, const char* titulo) {
	size_t n = strlen(titulo);
	linea = saltar_espacio((char*)linea);
	return strncmp(linea, titulo, n) == 0 && linea[n] == '\0';
}

static void agregar_texto(char destino[][PROF_LARGO], int* cantidad, const char* linea) {
	if (*cantidad >= PROF_FRASES || linea[0] == '\0') {
		return;
	}
	snprintf(destino[*cantidad], PROF_LARGO, "%s", linea);
	(*cantidad)++;
}

static void agregar_sprite(struct profesion* actual, const char* linea) {
	if (actual->nsprite >= PROF_LINEAS) {
		return;
	}
	snprintf(actual->sprite[actual->nsprite], PROF_ANCHO, "%s", linea);
	actual->nsprite++;
}

static void limpiar_profesion(struct profesion* actual) {
	memset(actual, 0, sizeof(*actual));
}

static void guardar_profesion(struct profesion* actual) {
	if (actual->nombre[0] == '\0' || profesiones_cantidad >= PROF_TOPE) {
		return;
	}
	profesiones[profesiones_cantidad] = *actual;
	profesiones_cantidad++;
}

static void placeholder_profesion(void) {
	profesiones_cantidad = 1;
	limpiar_profesion(&profesiones[0]);
	snprintf(profesiones[0].nombre, PROF_ANCHO, "placeholder");
	snprintf(profesiones[0].sprite[0], PROF_ANCHO, "   .---.");
	snprintf(profesiones[0].sprite[1], PROF_ANCHO, "  ( o_O )");
	snprintf(profesiones[0].sprite[2], PROF_ANCHO, "   |   |");
	snprintf(profesiones[0].sprite[3], PROF_ANCHO, "  /|   |\\");
	profesiones[0].nsprite = 4;
	snprintf(profesiones[0].frases[0], PROF_LARGO, "Estoy en lo mio, que tambien es de todas.");
	snprintf(profesiones[0].frases[1], PROF_LARGO, "El dia sigue si la comuna sigue.");
	profesiones[0].nfrases = 2;
	snprintf(profesiones[0].quejas[0], PROF_LARGO, "Se acabo %%s. El barrio ya lo noto.");
	snprintf(profesiones[0].quejas[1], PROF_LARGO, "Sin %%s no hay oficio hoy.");
	profesiones[0].nquejas = 2;
	snprintf(profesiones[0].lectura[0], PROF_LARGO, "Hoy quiero leer algo de este oficio. Busca un texto.");
	snprintf(profesiones[0].lectura[1], PROF_LARGO, "Traeme un texto que sirva para trabajar.");
	profesiones[0].nlectura = 2;
	snprintf(profesiones[0].despedida[0], PROF_LARGO, "Me voy. El oficio se viene conmigo.");
	profesiones[0].ndespedida = 1;
}

static void cargar_profesiones(void) {
	FILE* archivo;
	struct profesion actual;
	char cruda[PROF_LARGO];
	int seccion = 0;
	int abierta = 0;
	if (profesiones_listo) {
		return;
	}
	profesiones_listo = 1;
	archivo = fopen("profesiones", "r");
	if (archivo == NULL) {
		placeholder_profesion();
		return;
	}
	limpiar_profesion(&actual);
	while (fgets(cruda, (int)sizeof(cruda), archivo) != NULL) {
		char* linea;
		quitar_salto(cruda);
		linea = saltar_espacio(cruda);
		if (strncmp(linea, "Profesion:", 10) == 0 || strncmp(linea, "Profecion:", 10) == 0) {
			if (abierta) {
				guardar_profesion(&actual);
			}
			limpiar_profesion(&actual);
			linea = saltar_espacio(linea + 10);
			snprintf(actual.nombre, PROF_ANCHO, "%s", linea);
			abierta = 1;
			seccion = 1;
			continue;
		}
		if (!abierta) {
			continue;
		}
		if (es_separador(linea)) {
			seccion = 0;
			continue;
		}
		if (es_encabezado(linea, "Frases:")) {
			seccion = 2;
			continue;
		}
		if (es_encabezado(linea, "Quejas:")) {
			seccion = 3;
			continue;
		}
		if (es_encabezado(linea, "Lectura:")) {
			seccion = 4;
			continue;
		}
		if (es_encabezado(linea, "Despedida:")) {
			seccion = 5;
			continue;
		}
		if (linea[0] == '\0' && seccion != 1) {
			continue;
		}
		if (seccion == 1) {
			agregar_sprite(&actual, cruda);
		} else if (seccion == 2) {
			agregar_texto(actual.frases, &actual.nfrases, linea);
		} else if (seccion == 3) {
			agregar_texto(actual.quejas, &actual.nquejas, linea);
		} else if (seccion == 4) {
			agregar_texto(actual.lectura, &actual.nlectura, linea);
		} else if (seccion == 5) {
			agregar_texto(actual.despedida, &actual.ndespedida, linea);
		}
	}
	if (abierta) {
		guardar_profesion(&actual);
	}
	fclose(archivo);
	if (profesiones_cantidad == 0) {
		placeholder_profesion();
	}
}

static int nombre_cerca(const char* servicio, const char* clave) {
	if (servicio == NULL || clave == NULL || clave[0] == '\0') {
		return 0;
	}
	if (strcmp(servicio, clave) == 0) {
		return 2;
	}
	if (strcmp(clave, "placeholder") == 0) {
		return 0;
	}
	if (strstr(servicio, clave) != NULL || strstr(clave, servicio) != NULL) {
		return 1;
	}
	return 0;
}

static struct profesion* buscar_profesion(const char* servicio) {
	int mejor = -1;
	int puntaje = 0;
	cargar_profesiones();
	for (int i = 0; i < profesiones_cantidad; i++) {
		int ahora = nombre_cerca(servicio, profesiones[i].nombre);
		if (ahora > puntaje) {
			puntaje = ahora;
			mejor = i;
		}
	}
	if (puntaje == 0) {
		for (int i = 0; i < profesiones_cantidad; i++) {
			if (strcmp(profesiones[i].nombre, "placeholder") == 0) {
				return &profesiones[i];
			}
		}
		return &profesiones[0];
	}
	return &profesiones[mejor];
}

static const char* elegir_linea(char lineas[][PROF_LARGO], int cantidad, char respaldo[][PROF_LARGO], int nrespaldo) {
	if (cantidad <= 0) {
		lineas = respaldo;
		cantidad = nrespaldo;
	}
	if (cantidad <= 0) {
		return "El dia sigue.";
	}
	return lineas[aleatorio(0, cantidad - 1)];
}

static const char* con_dato(const char* molde, const char* dato) {
	if (molde == NULL) {
		return "";
	}
	if (dato == NULL) {
		dato = "eso";
	}
	if (strstr(molde, "%s") == NULL) {
		return molde;
	}
	snprintf(dialogo_buffer, sizeof(dialogo_buffer), molde, dato);
	return dialogo_buffer;
}

/* Una frase del oficio. Si el nombre no esta en el archivo, sale el placeholder.
	Entrada: el nombre del servicio
	Salida: la frase */
const char* dialogo_de_servicio(const char* servicio) {
	struct profesion* oficio = buscar_profesion(servicio);
	return elegir_linea(oficio->frases, oficio->nfrases, oficio->frases, oficio->nfrases);
}

/* Chiste corto, tomado de las frases del archivo.
	Entrada: ninguna
	Salida: la frase */
const char* dialogo_meme(void) {
	struct profesion* oficio = buscar_profesion(NULL);
	return elegir_linea(oficio->frases, oficio->nfrases, oficio->frases, oficio->nfrases);
}

/* Queja de un oficio. Si el texto trae %s, entra el recurso.
	Entrada: el servicio de quien habla (o NULL) y el recurso que falta
	Salida: la frase */
const char* dialogo_queja_de(const char* servicio, const char* recurso) {
	struct profesion* oficio = buscar_profesion(servicio);
	const char* molde = elegir_linea(oficio->quejas, oficio->nquejas, oficio->frases, oficio->nfrases);
	return con_dato(molde, recurso);
}

/* Queja usando el placeholder.
	Entrada: el nombre del recurso
	Salida: la frase */
const char* dialogo_queja(const char* recurso) {
	return dialogo_queja_de(NULL, recurso);
}

/* Pide que alguien busque un texto para leer.
	Entrada: el servicio de quien habla
	Salida: la frase */
const char* dialogo_pedido_lectura(const char* servicio) {
	struct profesion* oficio = buscar_profesion(servicio);
	return elegir_linea(oficio->lectura, oficio->nlectura, oficio->frases, oficio->nfrases);
}

/* Sugiere regalar un bien que sobra.
	Entrada: el nombre del bien, o NULL
	Salida: la frase */
const char* dialogo_sugerencia(const char* bien) {
	struct profesion* oficio = buscar_profesion(NULL);
	const char* molde = elegir_linea(oficio->frases, oficio->nfrases, oficio->frases, oficio->nfrases);
	return con_dato(molde, bien);
}

/* Comentario suelto al cerrar el dia.
	Entrada: ninguna
	Salida: la frase */
const char* dialogo_cierre_dia(void) {
	return dialogo_meme();
}

/* Lo que dice alguien al mudarse.
	Entrada: el servicio de nacimiento
	Salida: la frase */
const char* dialogo_despedida(const char* servicio) {
	struct profesion* oficio = buscar_profesion(servicio);
	return elegir_linea(oficio->despedida, oficio->ndespedida, oficio->frases, oficio->nfrases);
}

/* Dibuja el sprite de esa profesion.
	Entrada: el nombre del servicio, o NULL
	Salida: el dibujo en pantalla */
void dialogo_imprimir_sprite(const char* servicio) {
	struct profesion* oficio = buscar_profesion(servicio);
	if (oficio->nsprite <= 0) {
		printf("        %s( o_O )%s\n", TUI_CIAN, TUI_RESET);
		return;
	}
	for (int i = 0; i < oficio->nsprite; i++) {
		printf("        %s%s%s\n", TUI_CIAN, oficio->sprite[i], TUI_RESET);
	}
}
