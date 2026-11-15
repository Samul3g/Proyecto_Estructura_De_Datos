#ifndef BIBLIOTECA_H
#define BIBLIOTECA_H
 
#include "estructuras.h"
#include "arbol_avl.h"
#include "arbol_chaflaneado.h"
 
struct biblioteca{
	char* ruta_corpus;
	struct arbol_avl* libros;
	struct arbol_chaflaneado* busquedas;
	int cantidad_libros;
};
 
/* Mete al avl un nodo por cada .txt
 * Entradas: la biblioteca
 * Salidas: 1 si leyo el directorio, 0 si no */
int guardar_libros_avl(struct biblioteca* biblioteca);
 
 
/* Crea la biblioteca e indexa los libros en el avl
 * Entradas: ruta del directorio con los .txt
 * Salidas: la bilbioteca, NULL si la ruta esta vacia o no se pudo abrir el directorio */
struct biblioteca* crear_biblioteca(char* ruta_corpus);
 
/* Libera la biblioteca y todod sus componentes
 * Entradas: la biblioteca
 * Salidas: nada */
void liberar_biblioteca(struct biblioteca* biblioteca);
 
/* Busca un libro en la biblioteca por nombre (coincidencia exacta)
 * Entradas: biblioteca, nombre del libro
 * Salidas: nodo del avl del libro, NULL si no esta */
struct nodo_arbol_avl* buscar_libro_nombre(struct biblioteca* biblioteca, struct string* nombre);
 
/* Guarda en la comuna el puntero a la biblioteca compartida
 * Entradas: comuna, biblioteca
 * Salidas: 1 si guardo el puntero, 0 si alguno es NULL */
int asignar_biblioteca(struct comuna* comuna, struct biblioteca* biblioteca);
 
/* Recorre la lista de comunas y les asigna la biblioteca a cada una
 * Entradas: lista comunas, bilbioteca
 * Salidas: cuantas comunas quedaron asignadas, 0 si alguno es NULL o no hay comunas */
int asignar_biblioteca_sociedad(struct lista_comunas* sociedad, struct biblioteca* biblioteca);
 
/* Dice si una persona ya leyo un libro
 * Entrada: la persona y el nombre del libro
 * Salida: 1 si el nombre esta en persona->libros, 0 si no */
int persona_ya_leyo(struct persona* persona, struct string* nombre_libro);
 
/* Registra que una persona leyo un libro
 * Entrada: la persona y el nombre del libro
 * Salida: 1 si registro la lectura: agrega una copia del nombre a libros y suma libros_leidos, 0 si algun puntero es NULL */
int persona_leer_libro(struct persona* persona, struct string* nombre_libro);

 
/* Le ensena a la persona un servicio
 * Entrada: la persona, lista de servicios, cantidad de personas en su comuna
 * Salida: 1 si aprendio un servicio nuevo, 0 si no le toca, ya tiene todos los de la lista, o algun puntero es NULL */
int persona_aprender_servicio(struct persona* persona, struct lista_archivo* lista_servicios, int cantidad_personas);
 

#endif
 
