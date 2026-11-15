#ifndef ARBOL_CHAFLANEADO_H
#define ARBOL_CHAFLANEADO_H
 
 
#include "estructuras.h"
 
 
/* Busca en el arbol
 * Entradas: arbol, texto, referencia a entero de encontrado o no
 * Salidas: libros */
struct lista_archivo* buscar_chaflaneado(struct arbol_chaflaneado* arbol, struct string* texto, int* encontrado);
 
 
/* Inserta un nodo en el arbol
 * Entradas: arbol, texto, resultados
 * Salidas: 1 si inserto o ya estaba y 0 si fallo */
int insertar_chaflaneado(struct arbol_chaflaneado* arbol, struct string* texto, struct lista_archivo* resultados);
 
 
/* Libera el arbol completo
 * Entradas: arbol
 * Salidas: nada */
void liberar_arbol_chaflaneado(struct arbol_chaflaneado* arbol);
 
 
/* Imprimel el arbol completo
 * Entradas: arbol
 * Salidas: nada (impresion en consola) */
void imprimir_arbol_chaflaneado(struct arbol_chaflaneado* arbol);
 
 
 
/* Imprime la clave de la raiz (para mostrar antes y despues de una busqueda)
 * Entradas: arbol
 * Salidas: nada (impresion en consola) */
void imprimir_raiz_chaflaneado(struct arbol_chaflaneado* arbol);
 
 
/* Activa o desactiva la impresion de cada rotacion
 * Entradas: arbol, 1 para mostrar o 0 para no mostrar
 * Salidas: nada (el arbol queda configurado) */
void mostrar_pasos_chaflaneado(struct arbol_chaflaneado* arbol, int mostrar);
 
 
 #endif
