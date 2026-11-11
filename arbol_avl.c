#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "estructuras.h"
#include "arbol_avl.h"
#include "utils.h"


/* Calcula el factor de balance: altura izquierda - altura derecha
	Entrada: el nodo
	Salida: el factor */
int calcular_factor_balance(struct nodo_arbol_avl* nodo) {
	
	if (nodo == NULL) {
		return 0;
	}
	
	return altura(nodo->izquierdo) - altura(nodo->derecho);
}

/* Actualiza la altura de un nodo 
 * Entradas: nodo
 * Salida: ninguna */
static void actualizar_altura(struct nodo_arbol_avl* nodo) {
	
	nodo->altura = 1 + mayor(altura(nodo->izquierdo), altura(nodo->derecho));

}



/* Hace una rotacion hacia la derecha de un nodo
 * Entrada: nodo arbol avl
 * Salida: nueva raiz del subarbol */
struct nodo_arbol_avl* rotar_derecha(struct nodo_arbol_avl* y) {
	
	struct nodo_arbol_avl* x = y->izquierdo;
	struct nodo_arbol_avl* b = x->derecho;
	
	x->derecho = y;
	y->izquierdo = b;
	
	actualizar_altura(y);   // primero y, porque ahora esta abajo
	actualizar_altura(x);
	
	return x;
}

/* Hace una rotacion hacia la izquierda de un nodo
 * Entrada: nodo arbol avl
 * Salida: nueva raiz del subarbol */
struct nodo_arbol_avl* rotar_izquierda(struct nodo_arbol_avl* x) {
	
	struct nodo_arbol_avl* y = x->derecho;
	struct nodo_arbol_avl* b = y->izquierdo;
	
	y->izquierdo = x;
	x->derecho = b;
	
	actualizar_altura(x);
	actualizar_altura(y);
	
	return y;
}

/* Revisa el balance de un nodo y aplica la rotacion que haga falta
 * Entrada: el nodo
 * Salida: la nueva raiz de ese subarbol */
static struct nodo_arbol_avl* balancear(struct nodo_arbol_avl* nodo) {
	
	actualizar_altura(nodo);
	
	int fb = factor_balance(nodo);
 
	if (fb > 1) { // cargado a la izquierda
		
		if (factor_balance(nodo->izquierdo) < 0) { // caso izquierda-derecha
			nodo->izquierdo = rotar_izquierda(nodo->izquierdo);
		}
		
		return rotar_derecha(nodo); //caso izquierda-izquierda
	}
	
	if (fb < -1) { // cargado a la derecha
	
		if (factor_balance(nodo->derecho) > 0) { // caso derecha-izquierda
			nodo->derecho = rotar_derecha(nodo->derecho);
		}
	
		return rotar_izquierda(nodo); // caso derecha-derecha
	}
	
	return nodo; // ya estaba balanceado
}


/* Insercion recursiva. 
 * Entradas:nodo, nombre, ruta, resultado
 * Salida: la raiz (posiblemente nueva) del subarbol */
static struct nodo_arbol_avl* insertar_nodo(struct nodo_avl* nodo, struct string* nombre, const char* ruta, int* resultado) {
	
	if (nodo == NULL) {
		
		struct nodo_arbol_avl* nuevo = crear_nodo_avl(nombre, ruta);
		
		*resultado = (nuevo != NULL);
		
		return nuevo;
	
	}
 
	int cmp = comparar_strings(nombre, nodo->nombre);
	
	if (cmp == -1) {
		
		nodo->izquierdo = insertar_nodo(nodo->izquierdo, nombre, ruta, resultado);
	
	} else if (cmp == 1) {
		
		nodo->derecho = insertar_nodo(nodo->derecho, nombre, ruta, resultado);
	
	} else {
		
		*resultado = 0; // ya existia
		return nodo;
	
	}
	
	return balancear(nodo);
	
}

/* Libera un nodo y todos sus hijos
 * Entradas: nodo raiz del arbol o del subarbol
 * Salidas: nada */
static void liberar_nodos(struct nodo_arbol_avl* nodo) {
	
	if (nodo == NULL) {
		return;
	}
	
	liberar_nodos(nodo->izquierdo);
	liberar_nodos(nodo->derecho);
	liberar_string(nodo->nombre);
	free(nodo->ruta);
	free(nodo);
}

/* Imprime el arbol o subarbol a partir de su nodo raiz
 * Entradas: nodo raiz, nivel en qu esta la raiz
 * Salidas: nada */
void imprimir_nodo(struct nodo_arbol_avl* nodo, int nivel) {
	
	if (nodo == NULL) {
		return;
	}
	
	imprimir_nodo(nodo->derecho, nivel + 1);
	
	for (int i = 0; i < nivel; i++) {
		printf("        ");
	}
	
	imprimirString(nodo->nombre);
	
	printf(" (h=%d, fb=%d)\n", nodo->altura, factor_balance(nodo));
	
	imprimir_nodo(nodo->izquierdo, nivel + 1);
}

/* Funcion que inserta un nodo en el arbol avl
 * Entradas: arbol, nombre del nodo, ruta 
 * Salidas: 0 si fallo, 1 si exito */
int insertar_avl(struct arbol_avl* arbol, struct string* nombre, const char* ruta){
	
	if (arbol == NULL || nombre == NULL || ruta == NULL) {
		return 0;
	}

	int resultado = 0;
	
	arbol->raiz = insertar_nodo(arbol->raiz, nombre, ruta, &resultado);
	
	return resultado;

}

/* Bucas el titulo de un libro en el avl
 * Entradas: arbol, nombre del libro 
 * Salidas: nodo del arbol o NULL si no lo encontro */
struct nodo_arbol_avl* buscar_avl(struct arbol_avl* arbol, struct string* nombre) {
	
	if (arbol == NULL || nombre == NULL) {
		return NULL;
	}
	
	struct nodo_arbol_avl* actual = arbol->raiz;
	
	while (actual != NULL) {
		int cmp = comparar_strings(nombre, actual->nombre);
		
		if (cmp == 0) {
			return actual;
		}
		
		if (cmp == -1) {
			actual = actual->izquierdo;
		} else {
			actual = actual->derecho;
		}
	}
	return NULL;
}

/* Libera el arbol avl completo
 * Entradas: arbol
 * Salidas: nada */
void liberar_arbol_avl(struct arbol_avl* arbol) {
	
	if (arbol == NULL) {
		return;
	}
	
	liberar_nodos(arbol->raiz);
	
	free(arbol);

}

/* Imprime el arbol completo
 * Entradas: arbol avl
 * Salidas: nada (imprime en consola) */
void imprimir_arbol_avl(struct arbol_avl* arbol) {
	
	if (arbol == NULL || arbol->raiz == NULL) {
		printf("(arbol vacio)\n");
		return;
	}
	
	printf("Arbol AVL -  h = altura, fb = factor de balance\n");
	imprimir_nodo(arbol->raiz, 0);
}
