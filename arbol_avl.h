#ifndef ARBOL_AVL_H
#define ARBOL_AVL_H
 
#include "estructuras.h"
 
/* Buca el titulo de un libro en el avl
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
 
 
/* Funcion que inserta un nodo en el arbol avl
 * Entradas: arbol, nombre del nodo, ruta 
 * Salidas: 0 si fallo, 1 si exito */
int insertar_avl(struct arbol_avl* arbol, struct string* nombre, const char* ruta);








#endif
