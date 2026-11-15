#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include "estructuras.h"
#include "biblioteca.h"
#include "arbol_avl.h"
#include "arbol_chaflaneado.h" 
#include "utils.h"

/* Revisa que un archivo sea .txt
 * Entradas: nombre del archivo
 * Salidas: 1 si termina en .txt, 0 si no */
static int es_txt(const char* nombre){
	
	size_t largo = strlen(nombre);
	
	if(strcmp(nombre + largo - 4, ".txt") == 0){
		return 1;
	}
	
	return 0;
}


/* Mete al avl un nodo por cada .txt
 * Entradas: la biblioteca
 * Salidas: 1 si leyo el directorio, 0 si no */
int guardar_libros_avl(struct biblioteca* biblioteca){
	
	if(bilbioteca == NULL || biblioteca->ruta_corpus == NULL){
		return 0;
	}

	DIR* directorio = opendir(bilbioteca->ruta_corpus);
	
	if(directorio == NULL){
		return 0;
	}
	
	struct dirent* entrada = readdir(directorio);
	char ruta[4096];
	
	while{
		
		if(es_txt(entrada->d_name)){
			snprintf(ruta, sizeof(ruta), "%s/%s", bilbioteca->ruta_corpus, entrada->d_name);
			
			struct string* nombre = crear_string(entrada->d_name);
			
			if(insertar_avl(biblioteca->libros, nombre, ruta)){
				bilbioteca->cantidad_libros++;
			
			}else{
				liberar_string(nombre); //si no se inserto
			}
		}
		
		entrada = readdir(directorio);
	}
	
	closedir(directorio);
	
	
	return 1;
	
}

/* Crea la biblioteca e indexa los libros en el avl
 * Entradas: ruta del directorio con los .txt
 * Salidas: la bilbioteca, NULL si la ruta esta vacia o no se pudo abrir el directorio */
struct biblioteca* crear_bilbioteca(char* ruta_corpus){
	
	if(ruta_corpus == NULL){
		return NULL:
	}
	
	struct biblioteca* bilbioteca = calloc(1, sizeof(struct biblioteca));
	
	biblioteca->ruta_corpus = calloc(1, strlen(ruta_corpus));
	
	biblioteca->ruta_corpus = ruta_corpus;
	
	biblioteca->libros = crear_arbol_avl();
	biblioteca->busquedas = crear_arbol_chaflaneado();
	biblioteca->cantidad_libros = 0l
	
	int guardar_libros = guardar_libros_avl(biblioteca);
	
	if(guardar_libros == 0){
		return NULL;
	}
	
	return biblioteca;
}

/* Libera la biblioteca y todod sus componentes
 * Entradas: la biblioteca
 * Salidas: nada */
void liberar_biblioteca(struct biblioteca* biblioteca){
	 
	if (biblioteca == NULL) {
		return;
	}
 
	liberar_arbol_avl(biblioteca->libros);
	liberar_arbol_chaflaneado(biblioteca->busquedas);
	free(biblioteca->ruta_corpus);
	free(biblioteca);
	
}


/* Busca un libro en la biblioteca
 * Entradas: biblioteca, nombre del libro
 * Salidas: nodo del avl del libro*/
struct nodo_arbol_avl* buscar_libro_nombre(struct biblioteca* biblioteca, struct string* nombre){
	
	if(biblioteca == NULL){
		return NULL;
	}
	
	return buscar_avl(biblioteca->libros, nombre);
}



/* Recorre la lista de comunas y les asigna la biblioteca a cada una
 * Entradas: lista comunas, bilbioteca
 * Salidas: 1 si hubo exito, cero si no */
int asignar_biblioteca_sociedad(struct lista_comunas* sociedad, struct biblioteca* biblioteca){
	
	if(sociedad == NULL || biblioteca == NULL){
		return 0;
	}
	
	int asignadas = 0;
	
	struct nodo_lista_comunas inicio = sociedad->inicio;
	struct nodo_lista_comunas actual = sociedad->inicio;
	
	actual->biblioteca = biblioteca;
	actual = actual->siguiente;
	asignadas++;
	
	while(actual != inicio){
		actual->biblioteca = biblioteca;
		asignadas++;
		actual = actual->siguiente;
	}
	
	return asignadas;
}



























