#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "utils.h"

/* Compara dos strings caracter por caracter
	Entrada: los dos strings
	Salida: 1 si son iguales, 0 si no */
	int mismos_nombres(struct string* a, struct string* b) {
		struct char_array* actual_a = a->inicio;
		struct char_array* actual_b = b->inicio;
		while (actual_a != NULL && actual_b != NULL) {
			if (actual_a->caracter != actual_b->caracter) {
				return 0;
			}
			actual_a = actual_a->siguiente;
			actual_b = actual_b->siguiente;
		}
		return actual_a == NULL && actual_b == NULL;
	}

/* Retorna un numero pseudo aleatorio entre el minimo y el maximo dados.
	Entrada: el minimo y el maximo
	Salida: el numero pseudo aleatorio */
int aleatorio(int minimo, int maximo) {
	static int inicializado = 0;
	if (!inicializado) {
		srand(time(NULL));
		inicializado = 1;
	}
	int numero_aleatorio = minimo + rand() % (maximo - minimo + 1);
	return numero_aleatorio;
}

/* Lee un entero de la entrada. Si no es un numero, descarta el resto de la linea.
	Entrada: donde guardar el entero
	Salida: 1 si leyo un numero, 0 si no era un numero, -1 si se acabo la entrada */
int leer_entero(int* destino) {
	int leido = scanf("%d", destino);
	if (leido == EOF) {
		return -1;
	}

	int c = getchar();
	while (c != '\n' && c != EOF) {
		c = getchar();
	}

	if (leido != 1) {
		return 0;
	}
	return 1;
}

/* Obtiene el siguiente caracter del archivo
	Entrada: el archivo
	Salida: el siguiente caracter del archivo */
int siguiente_caracter(FILE* archivo) {
	return fgetc(archivo);
}

/* Obtiene la linea del archivo
	Entrada: el archivo
	Salida: la linea del archivo */
struct string* linea_archivo(FILE* archivo) {
	if (archivo == NULL) {
		return NULL;
	}

	struct string* linea = crear_string("");
	struct char_array* actual = NULL;
	int c = siguiente_caracter(archivo);
	while (c != '\n' && c != EOF) {
		if (actual == NULL) {
			actual = crear_char_array(c);
			linea->inicio = actual;
		} else {
			actual->siguiente = crear_char_array(c);
			actual = actual->siguiente;
		}
		c = siguiente_caracter(archivo);
	}
	if (c == EOF && linea->inicio == NULL) {
		liberar_string(linea);
		return NULL;
	}
	return linea;
}

/* Genera la lista de archivos a partir de un archivo de lectura
	Entrada: el archivo
	Salida: la lista de archivos */
struct lista_archivo* generar_lista_archivo(FILE* archivo) {
	if (archivo == NULL) {
		return NULL;
	}

	struct lista_archivo* lista = crear_lista_archivo();
	struct string* linea = linea_archivo(archivo);
	int indice = 0;
	while (linea != NULL) {
		agregar_entrada_lista_archivo(lista, crear_entrada_lista_archivo(linea, indice));
		indice++;
		linea = linea_archivo(archivo);
	}
	return lista;
}

/* Lee el archivo y lo devuelve como archivo de lectura
	Entrada: el nombre del archivo
	Salida: el archivo de lectura */
FILE* leer_archivo(char* nombre) {
	FILE* archivo = fopen(nombre, "r");
	if (archivo == NULL) {
		printf("\033[31mError al abrir el archivo %s\033[0m\n", nombre);
		exit(1);
	}
	return archivo;
}

/* Arma un recurso a partir de una linea nombre,necesidad,maximo,relevancia
	Entrada: la linea y cuantas personas viven en la comuna
	Salida: el recurso, con el maximo calculado y cantidad en 0 */
struct nodo_recurso* crear_recurso_desde_linea(struct string* linea, int cantidad_personas) {
	bool es_necesidad = 1;
	int maximo_int = 0;
	int relevancia_int = 0;

	struct char_array* c = linea->inicio;
	struct string* nombre = crear_string("");

	nombre->inicio = crear_char_array(c->caracter);

	struct char_array* nombre_recorrer = nombre->inicio;
	c = c->siguiente;
	while (c->caracter != ',') {
		nombre_recorrer->siguiente = crear_char_array(c->caracter);
		nombre_recorrer = nombre_recorrer->siguiente;
		c = c->siguiente;
	}

	c = c->siguiente;

	char necesidad[10];
	int i = 0;

	while (c->caracter != ',') {
		necesidad[i] = c->caracter;
		i++;
		c = c->siguiente;
	}

	necesidad[i] = '\0';

	if (strcmp(necesidad, "true") == 0) {
		es_necesidad = 1;
	} else {
		es_necesidad = 0;
	}
	i = 0;

	c = c->siguiente;
	char maximo[8];
	while (c->caracter != ',') {
		maximo[i] = c->caracter;
		i++;
		c = c->siguiente;
	}
	maximo[i] = '\0';

	maximo_int = atoi(maximo) * cantidad_personas * 0.1;
	i = 0;

	c = c->siguiente;
	char relevancia[6];
	while (c != NULL) {
		relevancia[i] = c->caracter;
		i++;
		c = c->siguiente;
	}

	relevancia[i] = '\0';

	relevancia_int = atoi(relevancia);

	return crear_nodo_recurso(nombre, es_necesidad, maximo_int, relevancia_int, 0);
}
