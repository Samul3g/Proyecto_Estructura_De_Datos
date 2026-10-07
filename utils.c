#include <stdio.h>
#include <stdlib.h>
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




