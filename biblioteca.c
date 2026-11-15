#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include "estructuras.h"
#include "biblioteca.h"
#include "arbol_avl.h"
#include "arbol_chaflaneado.h" 
#include "utils.h"
 
 
/* Mete al avl un nodo por cada .txt
 * Entradas: la biblioteca
 * Salidas: 1 si leyo el directorio, 0 si no */
int guardar_libros_avl(struct biblioteca* biblioteca){
	
	if(biblioteca == NULL || biblioteca->ruta_corpus == NULL){
		return 0;
	}
 
	DIR* directorio = opendir(biblioteca->ruta_corpus);
	
	if(directorio == NULL){
		return 0;
	}
	
	struct dirent* entrada = readdir(directorio);
	char ruta[4096];
	
	while(entrada != NULL){
		
		if(es_txt(entrada->d_name)){
			snprintf(ruta, sizeof(ruta), "%s/%s", biblioteca->ruta_corpus, entrada->d_name);
			
			struct string* nombre = crear_string(entrada->d_name);
			
			if(insertar_avl(biblioteca->libros, nombre, ruta)){
				biblioteca->cantidad_libros++;
			
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
 * Salidas: la biblioteca, NULL si la ruta esta vacia o no se pudo abrir el directorio */
struct biblioteca* crear_biblioteca(char* ruta_corpus){
	
	if(ruta_corpus == NULL){
		return NULL;
	}
	
	struct biblioteca* biblioteca = calloc(1, sizeof(struct biblioteca));
	
	if(biblioteca == NULL){
		return NULL;
	}
	
	biblioteca->ruta_corpus = calloc(1, strlen(ruta_corpus) + 1);
	
	strcpy(biblioteca->ruta_corpus, ruta_corpus);
	
	biblioteca->libros = crear_arbol_avl();
	biblioteca->busquedas = crear_arbol_chaflaneado();
	biblioteca->cantidad_libros = 0;
	
	int guardar_libros = guardar_libros_avl(biblioteca);
	
	if(guardar_libros == 0){
		liberar_biblioteca(biblioteca);
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
	
	if(biblioteca == NULL || nombre == NULL){
		return NULL;
	}
	
	return buscar_avl(biblioteca->libros, nombre);
}
 
 
 
/* Guarda en la comuna el puntero a la biblioteca compartida
 * Entradas: comuna, biblioteca
 * Salidas: 1 si guardo el puntero, 0 si alguno es NULL */
int asignar_biblioteca(struct comuna* comuna, struct biblioteca* biblioteca){
	
	if(comuna == NULL || biblioteca == NULL){
		return 0;
	}
	
	comuna->biblioteca = biblioteca;   // solo el puntero: todas comparten la misma
	
	return 1;
}
 
 
/* Recorre la lista de comunas y les asigna la biblioteca a cada una
 * Entradas: lista comunas, biblioteca
 * Salidas: cuantas comunas quedaron asignadas, 0 si alguno es NULL o no hay comunas */
int asignar_biblioteca_sociedad(struct lista_comunas* sociedad, struct biblioteca* biblioteca){
	
	if(sociedad == NULL || biblioteca == NULL || sociedad->inicio == NULL){
		return 0;
	}
	
	int asignadas = 0;
	
	struct nodo_lista_comunas* inicio = sociedad->inicio;
	struct nodo_lista_comunas* actual = sociedad->inicio;
	
	asignadas += asignar_biblioteca(actual->comuna, biblioteca);
	actual = actual->siguiente;
	
	while(actual != inicio){
		asignadas += asignar_biblioteca(actual->comuna, biblioteca);
		actual = actual->siguiente;
	}
	
	return asignadas;
}
 
 
/* Dice si una persona ya leyo un libro
 * Entrada: la persona y el nombre del libro
 * Salida: 1 si el nombre esta en persona->libros, 0 si no */
int persona_ya_leyo(struct persona* persona, struct string* nombre_libro){
	
	if(persona == NULL || nombre_libro == NULL || persona->libros == NULL){
		return 0;
	}
	
	struct entrada_lista_archivo* actual = persona->libros->inicio;
	
	while(actual != NULL){
		
		if(mismos_nombres(actual->nombre, nombre_libro)){
			return 1;
		}
		
		actual = actual->siguiente;
	}
	
	return 0;
}
 
 
/* Registra que una persona leyo un libro
 * Entrada: la persona y el nombre del libro
 * Salida: 1 si registro la lectura: agrega una copia del nombre a libros y suma libros_leidos, 0 si algun puntero es NULL */
int persona_leer_libro(struct persona* persona, struct string* nombre_libro) {
	
	if (persona == NULL || nombre_libro == NULL) {
		return 0;
	}
	
	if (persona->libros == NULL) {
		persona->libros = crear_lista_archivo();   // por si crear_persona no la creo
	}
	
 
	struct string* copia = copiar_string(nombre_libro);
	
	agregar_entrada_lista_archivo(persona->libros, crear_entrada_lista_archivo(copia, persona->libros_leidos));
	persona->libros_leidos++;
	
	return 1;
}
 
 
/* Dice si la persona todavia no tiene el servicio de una linea del catalogo
 * Entrada: la persona, la linea y cuantas personas hay en su comuna
 * Salida: 1 si no lo tiene, 0 si ya lo tiene */
static int servicio_nuevo(struct persona* persona, struct string* linea, int cantidad_personas) {
	
	struct nodo_recurso* candidato = crear_recurso_desde_linea(linea, cantidad_personas);
	int nuevo = 0;
	
	if (buscar_recurso(persona->servicios, candidato->nombre) == NULL) {
		nuevo = 1;
	}
	
	liberar_recurso(candidato);
	
	return nuevo;
}
 
 
/* Le ensena a la persona un servicio
 * Entrada: la persona, lista de servicios, cantidad de personas en su comuna
 * Salida: 1 si aprendio un servicio nuevo, 0 si no le toca (libros_leidos no es 3, 6, 9...),
 *         ya tiene todos los del catalogo, o algun puntero es NULL */
int persona_aprender_servicio(struct persona* persona, struct lista_archivo* lista_servicios, int cantidad_personas) {
	
	if (persona == NULL || lista_servicios == NULL) {
		return 0;
	}
	
	
	if (persona->libros_leidos == 0 || persona->libros_leidos % 3 != 0) {
		return 0;   // solo aprende al llegar a 3, 6, 9, etc libros
	}
	
	
	if (persona->servicios == NULL) {
		persona->servicios = crear_lista_recursos();
	}
	
	
	// 1) contar cuantos servicios de la lista todavia no tiene
	int candidatos = 0;
	struct entrada_lista_archivo* actual = lista_servicios->inicio;
	
	while (actual != NULL) {
		
		if (servicio_nuevo(persona, actual->nombre, cantidad_personas)) {
			candidatos++;
		}
		
		actual = actual->siguiente;
	}
	
	if (candidatos == 0) {
		return 0;   // ya sabe todos
	}
	
	// 2) elegir uno al azar entre esos candidatos y volver a recorrer hasta llegar a el
	int elegido = aleatorio(1, candidatos);
	actual = lista_servicios->inicio;
	
	while (actual != NULL) {
		
		if (servicio_nuevo(persona, actual->nombre, cantidad_personas)) {
			
			elegido--;
			
			if (elegido == 0) {
				
				struct nodo_recurso* servicio = crear_recurso_desde_linea(actual->nombre, cantidad_personas);
				
				servicio->cantidad = servicio->maximo;
				
				if (asignar_servicio_persona(persona, servicio)) {
					return 1;
				}
				
				liberar_recurso(servicio);   // no se pudo asignar
				return 0;
			}
		}
		
		actual = actual->siguiente;
	}
	
	return 0;
}













