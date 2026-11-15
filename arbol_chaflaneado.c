#include <stdio.h>
#include <stdlib.h>
#include "estructuras.h"
#include "arbol_chaflaneado.h"
#include "utils.h"

/* Imprime el nombre de un paso y el nodo que sube, solo si mostrar_pasos esta activo
 * Entrada: el arbol, el nombre del paso y el nodo
 * Salida: impresion en consola */
static void anunciar_paso(struct arbol_chaflaneado* arbol, const char* paso, struct nodo_chaflaneado* x) {
	if (arbol->mostrar_pasos) {
		printf("    %s con \"", paso);
		imprimirString(x->texto);
		printf("\"\n");
	}
}

/* Rotacion basica: sube x un nivel, por encima de su padre
 * Si x es hijo izquierdo es una rotacion a la derecha; si es hijo derecho, a la izquierda.
 * Entrada: el arbol y el nodo x (no puede ser la raiz)
 * Salida: nada (x queda donde estaba su padre) */
static void rotar(struct arbol_chaflaneado* arbol, struct nodo_chaflaneado* x) {
	
	struct nodo_chaflaneado* p = x->padre;
	struct nodo_chaflaneado* a = p->padre;
 
	if (x == p->izquierdo) {
		
		p->izquierdo = x->derecho; 
		
		if(x->derecho != NULL){
				x->derecho->padre = p;
		}
		
		x->derecho = p;
	
	} else {
		
		p->derecho = x->izquierdo;
		
		if(x->izquierdo != NULL){
			x->izquierdo->padre = p;
		}
		
		x->izquierdo = p;
	}
	
	p->padre = x;
	x->padre = a;
 
	//poner x donde estaba su padre
	if (a == NULL) {
		arbol->raiz = x;
	
	} else if (a->izquierdo == p) {
		a->izquierdo = x;
	
	} else {
		a->derecho = x;
	}
}

/* Zig: el padre de x es la raiz. Una sola rotacion.
 * Entrada: el arbol y el nodo
 * Salida: x queda en la raiz */
static void zig(struct arbol_chaflaneado* arbol, struct nodo_chaflaneado* x) {
	
	anunciar_paso(arbol, "zig", x);
	rotar(arbol, x);

}


/* Zig-zig: x y su padre: los dos izquierdos o los dos derechos
 * Entrada: el arbol y el nodo
 * Salida: nada (x sube dos niveles) */
static void zig_zig(struct arbol_chaflaneado* arbol, struct nodo_chaflaneado* x) {
	
	anunciar_paso(arbol, "zig-zig", x);
	rotar(arbol, x->padre);
	rotar(arbol, x);

}


/* Zig-zag: x y su padre son hijos de lados distintos.
 * Entrada: el arbol y el nodo
 * Salida: nada (x sube dos niveles) */
static void zig_zag(struct arbol_chaflaneado* arbol, struct nodo_chaflaneado* x) {
	
	anunciar_paso(arbol, "zig-zag", x);
	rotar(arbol, x);
	rotar(arbol, x);


}


/* Chaflanea: sube x hasta la raiz con las funciones zig, zig-zig y zig-zag
 * Entrada: el arbol y el nodo
 * Salida: nada (x queda en la raiz) */
static void chaflanear(struct arbol_chaflaneado* arbol, struct nodo_chaflaneado* x) {
	
	while (x != arbol->raiz) {
	
		struct nodo_chaflaneado* p = x->padre;
		struct nodo_chaflaneado* a = p->padre;
		
		if (a == NULL) {
			
			zig(arbol, x);
		
		} else if ((x == p->izquierdo && p == a->izquierdo) || (x == p->derecho && p == a->derecho)) {
			
			zig_zig(arbol, x);
		
		} else {
			
			zig_zag(arbol, x);
		}
	}
}


/* Baja por el arbol buscando el texto
 * Entrada: el arbol, el texto y donde guardar el ultimo nodo visitado
 * Salida: el nodo si esta, NULL si no (en ultimo queda el ultimo nodo visitado) */
static struct nodo_chaflaneado* bajar(struct arbol_chaflaneado* arbol, struct string* texto, struct nodo_chaflaneado** ultimo) {
	
	struct nodo_chaflaneado* actual = arbol->raiz;
	
	*ultimo = NULL;
	
	while (actual != NULL) {
		
		*ultimo = actual;
		
		int cmp = comparar_strings(texto, actual->texto);
		
		if (cmp == 0) {
			return actual;
		}
		
		if (cmp < 0) {
			
			actual = actual->izquierdo;
		
		} else {
			
			actual = actual->derecho;
		}
	}
	return NULL;
}


/* Libera un nodo, sus higos y sus sub-arboles
 * Entrada: el nodo
 * Salida: nada (la memoria queda libre) */
static void liberar_nodos_chaflaneado(struct nodo_chaflaneado* nodo) {
	
	if (nodo == NULL) {
		return;
	}
	
	liberar_nodos_chaflaneado(nodo->izquierdo);
	liberar_nodos_chaflaneado(nodo->derecho);
	liberar_string(nodo->texto);
	
	if (nodo->libros != NULL) {
		liberar_lista_archivo(nodo->libros);
	}
	
	free(nodo);
}


/* Imprime el arbol acostado
 * Entradas: nodo raiz del subarbol, nivel en que esta
 * Salidas: nada (impresion en consola) */
static void imprimir_nodo_chaflaneado(struct nodo_chaflaneado* nodo, int nivel) {
 
	if (nodo == NULL) {
		return;
	}
 
	imprimir_nodo_chaflaneado(nodo->derecho, nivel + 1);
 
	for (int i = 0; i < nivel; i++) {
		printf("        ");
	}
 
	if (nivel == 0) {
		printf("[RAIZ] ");
	}
 
	printf("\"");
	imprimirString(nodo->texto);
	printf("\"\n");
 
	imprimir_nodo_chaflaneado(nodo->izquierdo, nivel + 1);
}

// ------- FUNCIONES PRINCIPALES ----------------

/* Busca en el arbol
 * Entradas: arbol, texto, referencia a entero de encontrado o no
 * Salidas: libros */
struct lista_archivo* buscar_chaflaneado(struct arbol_chaflaneado* arbol, struct string* texto, int* encontrado) {
	
	if (encontrado != NULL) {
		*encontrado = 0;
	}
	
	if (arbol == NULL || texto == NULL || arbol->raiz == NULL) {
		return NULL;
	}
 
	struct nodo_chaflaneado* ultimo = NULL;
	struct nodo_chaflaneado* nodo = bajar(arbol, texto, &ultimo);
 
	if (nodo != NULL) {
		
		chaflanear(arbol, nodo);
		
		if (encontrado != NULL) {
			*encontrado = 1;
		}
		
		return nodo->libros;
	}
 
	chaflanear(arbol, ultimo);
	
	return NULL;
}

/* Inserta un nodo en el arbol
 * Entradas: arbol, texto, resultados
 * Salidas: 1 si inserto o ya estaba y 0 si fallo */
int insertar_chaflaneado(struct arbol_chaflaneado* arbol, struct string* texto, struct lista_archivo* resultados) {
	
	if (arbol == NULL || texto == NULL || resultados == NULL) {
		return 0;
	}
 
	struct nodo_chaflaneado* ultimo = NULL;
	struct nodo_chaflaneado* existente = bajar(arbol, texto, &ultimo);
 
	if (existente != NULL) { // ya estaba
		
		chaflanear(arbol, existente);
		
		if (existente->texto != texto) {
			liberar_string(texto);               // se conserva la clave vieja
		}
		
		if (existente->libros != resultados) {
			liberar_lista_archivo(resultados);   // se conserva la lista vieja
		}
		
		return 1;
	}
 
	struct nodo_chaflaneado* nuevo = crear_nodo_chaflaneado(texto, resultados);
 
	if(nuevo == NULL){
			return 0;
	}
	
	nuevo->padre = ultimo
	
	if (ultimo == NULL) { // arbol vacio
		
		arbol->raiz = nuevo;
	
	} else if (comparar_strings(texto, ultimo->texto) < 0) {
		
		ultimo->izquierdo = nuevo;
	
	} else {
		
		ultimo->derecho = nuevo;
	}
 
	chaflanear(arbol, nuevo);
	
	return 1;
}

/* Libera el arbol completo
 * Entradas: arbol
 * Salidas: nada */
void liberar_arbol_chaflaneado(struct arbol_chaflaneado* arbol) {
	
	if (arbol == NULL) {
		return;
	}
	
	liberar_nodos_chaflaneado(arbol->raiz);
	
	free(arbol);
}


/* Imprimel el arbol completo
 * Entradas: arbol
 * Salidas: nada (impresion en consola) */
void imprimir_arbol_chaflaneado(struct arbol_chaflaneado* arbol) {
	
	if (arbol == NULL || arbol->raiz == NULL) {
		printf("\nEl arbol esta vacio\n");
		return;
	}
	
	printf("Arbol chaflaneado imprimiendose...");
	imprimir_nodo(arbol->raiz, 0);
}


/* Imprime la clave de la raiz (para mostrar antes y despues de una busqueda)
 * Entradas: arbol
 * Salidas: nada (impresion en consola) */
void imprimir_raiz_chaflaneado(struct arbol_chaflaneado* arbol) {
 
	if (arbol == NULL || arbol->raiz == NULL) {
		printf("Arbol vacio\n");
		return;
	}
 
	printf("\"");
	imprimirString(arbol->raiz->texto);
	printf("\"\n");
}


/* Activa o desactiva la impresion de cada rotacion
 * Entradas: arbol, 1 para mostrar o 0 para no mostrar
 * Salidas: nada (el arbol queda configurado) */
void mostrar_pasos_chaflaneado(struct arbol_chaflaneado* arbol, int mostrar) {
 
	if (arbol != NULL) {
		arbol->mostrar_pasos = mostrar;
	}
}
































