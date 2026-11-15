#ifndef BUSQUEDA_H
#define BUSQUEDA_H

#include "estructuras.h"
#include "biblioteca.h"

struct resultado_busqueda{
	struct lista_archivo* libros;
	int en_cache;
};

/* Lee cada .txt del directorio y guarda los que contienen el texto
 * Entrada: la ruta del directorio y el texto
 * Salida: la lista de nombres (vacia si ninguno lo contiene), NULL si el texto esta vacio o no se pudo leer el directorio */
struct lista_archivo* buscar_texto_en_archivos(const char* ruta_corpus, struct string* texto);


/* Busca un texto, primero en el arbol chaflaneado y si no esta, en los archivos
 * Entrada: la biblioteca y el texto
 * Salida: el resultado,NULL si la biblioteca o el texto son NULL, el texto esta vacio, o no se pudieron leer los archivos */
struct resultado_busqueda* buscar_texto(struct biblioteca* biblioteca, struct string* texto);


/* Libera el struct del resultado
 * Entrada: el resultado
 * Salida: nada */
void liberar_resultado_busqueda(struct resultado_busqueda* resultado);


#endif
