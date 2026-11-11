#ifndef ARBOL_AVL_H
#define ARBOL_AVL_H

/* Calcula el factor de balance: altura izquierda - altura derecha
	Entrada: el nodo
	Salida: el factor */
int calcular_factor_balance(struct nodo_arbol_avl* nodo);

/* Actualiza la altura de un nodo 
 * Entradas: nodo
 * Salida: ninguna */
static void actualizar_altura(struct nodo_arbol_avl* nodo);

/* Hace una rotacion hacia la derecha de un nodo
 * Entrada: nodo arbol avl
 * Salida: nueva raiz del subarbol */
struct nodo_arbol_avl* rotar_derecha(struct nodo_arbol_avl* y);

/* Hace una rotacion hacia la izquierda de un nodo
 * Entrada: nodo arbol avl
 * Salida: nueva raiz del subarbol */
struct nodo_arbol_avl* rotar_izquierda(struct nodo_arbol_avl* x);

/* Revisa el balance de un nodo y aplica la rotacion que haga falta
 * Entrada: el nodo
 * Salida: la nueva raiz de ese subarbol */
static struct nodo_arbol_avl* balancear(struct nodo_arbol_avl* nodo);

/* Insercion recursiva. 
 * Entradas:nodo, nombre, ruta, resultado
 * Salida: la raiz (posiblemente nueva) del subarbol */
static struct nodo_arbol_avl* insertar_nodo(struct nodo_avl* nodo, struct string* nombre, const char* ruta, int* resultado);

/* Libera un nodo y todos sus hijos
 * Entradas: nodo raiz del arbol o del subarbol
 * Salidas: nada */
static void liberar_nodos(struct nodo_arbol_avl* nodo);

/* Imprime el arbol o subarbol a partir de su nodo raiz
 * Entradas: nodo raiz, nivel en qu esta la raiz
 * Salidas: nada */
void imprimir_nodo(struct nodo_arbol_avl* nodo, int nivel);

/* Funcion que inserta un nodo en el arbol avl
 * Entradas: arbol, nombre del nodo, ruta 
 * Salidas: 0 si fallo, 1 si exito */
int insertar_avl(struct arbol_avl* arbol, struct string* nombre, const char* ruta);

/* Bucas el titulo de un libro en el avl
 * Entradas: arbol, nombre del libro 
 * Salidas: nodo del arbol o NULL si no lo encontro */
struct nodo_arbol_avl* buscar_avl(struct arbol_avl* arbol, struct string* nombre);

/* Libera el arbol avl completo
 * Entradas: arbol
 * Salidas: nada */
void liberar_arbol_avl(struct arbol_avl* arbol);

/* Imprime el arbol completo
 * Entradas: arbol avl
 * Salidas: nada (imprime en consola) */
void imprimir_arbol_avl(struct arbol_avl* arbol);



#endif
