#ifndef ESTRUCTURAS_H
#define ESTRUCTURAS_H

#include <stdbool.h>

struct entrada_lista_archivo {
	struct string* nombre;
	int indice;
	struct entrada_lista_archivo* siguiente;
};

struct lista_archivo {
	struct entrada_lista_archivo* inicio;
	struct entrada_lista_archivo* ultimo;
};

struct lista_archivo* crear_lista_archivo();
	/* Crea una lista de archivo
		Entrada: ninguna
		Salida: la lista creada */

void liberar_lista_archivo(struct lista_archivo* lista);
	/* Libera una lista de archivo
		Entrada: la lista
		Salida: la lista queda liberada */

struct entrada_lista_archivo* crear_entrada_lista_archivo(struct string* nombre, int indice);
	/* Crea una entrada de lista de archivo
		Entrada: el nombre y el indice
		Salida: la entrada creada */

void liberar_entrada_lista_archivo(struct entrada_lista_archivo* entrada);
	/* Libera una entrada de lista de archivo
		Entrada: la entrada
		Salida: la entrada queda liberada */

struct entrada_lista_archivo* buscar_entrada_lista_archivo(struct lista_archivo* lista, int indice);
	/* Busca una entrada de lista de archivo por indice
		Entrada: la lista y el indice
		Salida: la entrada si existe, NULL si no */

void agregar_entrada_lista_archivo(struct lista_archivo* lista, struct entrada_lista_archivo* entrada);
	/* Agrega una entrada de lista de archivo a la lista
		Entrada: la lista y la entrada
		Salida: la entrada queda agregada a la lista */

void eliminar_entrada_lista_archivo(struct lista_archivo* lista, struct string* nombre);
	/* Elimina una entrada de lista de archivo por nombre
		Entrada: la lista y el nombre
		Salida: la entrada queda eliminada de la lista */

void barajar_lista_archivo(struct lista_archivo* lista);
	/* Baraja la lista de archivo
		Entrada: la lista
		Salida: la lista barajada */

struct string {
	struct char_array* inicio;
};

struct char_array {
	char caracter;
	struct char_array* siguiente;
};
struct nodo_recurso {
    struct string* nombre;
    bool necesidad;
    int maximo;
    int relevancia;
    int cantidad;
};

struct nodo_lista_recursos {
    struct nodo_recurso* recurso;
    struct nodo_lista_recursos* siguiente;
    struct nodo_lista_recursos* anterior;
};

struct lista_recursos {
    struct nodo_lista_recursos* inicio;
};

struct comuna {
    struct string* nombre;
    struct lista_recursos* bienes;
    struct lista_recursos* servicios;
	int satisfaccion;
	int necesidad;
};

struct nodo_lista_comunas {
    struct comuna* comuna;
    struct nodo_lista_comunas* siguiente;
    struct nodo_lista_comunas* anterior;
};

struct lista_comunas {
    struct nodo_lista_comunas* inicio;
};

struct string* crear_string(char* cadena);
	/* Crea un string con la cadena
		Entrada: la cadena
		Salida: el string creado */

void liberar_string(struct string* string);
	/* Libera un string y su cadena
		Entrada: el string
		Salida: la memoria del string queda libre. La cadena tiene que haber salido de malloc */

struct char_array* crear_char_array(char caracter);
	/* Crea un char_array con el caracter
		Entrada: el caracter
		Salida: el char_array creado */

void liberar_char_array(struct char_array* char_array);
	/* Libera un char_array y su cadena
		Entrada: el char_array
		Salida: la memoria del char_array queda libre. La cadena tiene que haber salido de malloc */
		
struct nodo_recurso* crear_nodo_recurso(struct string* nombre, bool necesidad, int maximo, int relevancia, int cantidad);
	/* Crea un recurso con nombre, necesidad, maximo, relevancia y cantidad
		Entrada: nombre, necesidad, maximo, relevancia y cantidad
		Salida: el nodo del recurso creado */

		
struct nodo_lista_recursos* crear_nodo_lista_recursos(struct nodo_recurso* recurso);
	/* Arma el nodo de la lista doble a partir de un recurso
		Entrada: el recurso que va dentro del nodo
		Salida: el nodo de la lista, con anterior y siguiente vacios */

struct lista_recursos* crear_lista_recursos();
	/* Crea la lista doble de recursos
		Entrada: ninguna
		Salida: la lista, con inicio en NULL */

struct comuna* crear_comuna(struct string* nombre);
	/* Crea una comuna con sus dos listas de inventario vacias
		Entrada: el nombre de la comuna
		Salida: la comuna, con bienes y servicios listos para usar */

struct nodo_lista_comunas* crear_nodo_lista_comunas(struct comuna* comuna);
	/* Arma el nodo del circulo a partir de una comuna
		Entrada: la comuna que va dentro del nodo
		Salida: el nodo, sin enlazar todavia */

struct lista_comunas* crear_lista_comunas();
	/* Crea la lista circular de comunas
		Entrada: ninguna
		Salida: la lista, con inicio en NULL */

void imprimit_lista_comunas(struct lista_comunas* lista_comunas);
	/* Imprime la lista circular de comunas
		Entrada: la lista circular
		Salida: la lista circular impresa */

struct nodo_recurso* buscar_recurso(struct lista_recursos* lista_recursos, struct string* nombre);
	/* Recorre la lista doble hasta encontrar un recurso por nombre
		Entrada: la lista y el nombre a buscar
		Salida: el recurso si esta, NULL si no */

struct nodo_lista_comunas* buscar_comuna(struct lista_comunas* lista_comunas, struct string* nombre);
	/* Da una vuelta al circulo buscando una comuna por nombre
		Entrada: la lista circular y el nombre
		Salida: el nodo de la comuna si esta, NULL si la lista esta vacia o no aparece */

void agregar_recurso(struct lista_recursos* lista_recursos, struct nodo_recurso* recurso);
	/* Mete un recurso al frente de la lista doble
		Entrada: la lista y el recurso
		Salida: la lista queda con ese recurso de primero */

void agregar_comuna(struct lista_comunas* lista_comunas, struct comuna* comuna);
	/* Mete una comuna en el circulo
		Entrada: la lista circular y la comuna
		Salida: la comuna queda enlazada y pasa a ser el inicio. Si era la primera, se apunta a si misma */

void eliminar_recurso(struct lista_recursos* lista_recursos, struct string* nombre);
	/* Saca un recurso de la lista doble y libera su memoria
		Entrada: la lista y el nombre del recurso
		Salida: el nodo desaparece y los vecinos quedan enlazados. Si no esta, la lista no cambia */

void eliminar_comuna(struct lista_comunas* lista_comunas, struct string* nombre);
	/* Saca una comuna del circulo y libera la comuna con sus listas
		Entrada: la lista circular y el nombre
		Salida: el circulo se cierra sin esa comuna. Si era la unica, inicio queda en NULL */

void liberar_recurso(struct nodo_recurso* recurso);
	/* Libera un recurso y su nombre
		Entrada: el recurso
		Salida: la memoria del recurso queda libre. El nombre tiene que haber salido de malloc */

void liberar_lista_recursos(struct lista_recursos* lista_recursos);
	/* Recorre la lista doble y libera cada recurso, cada nodo y la lista
		Entrada: la lista de recursos
		Salida: toda esa lista queda liberada */

void liberar_comuna(struct comuna* comuna);
	/* Libera una comuna, su nombre y las listas de bienes y servicios
		Entrada: la comuna
		Salida: la comuna y su inventario quedan liberados */

void liberar_lista_comunas(struct lista_comunas* lista_comunas);
	/* Abre el circulo y libera cada comuna, cada nodo y la lista
		Entrada: la lista circular
		Salida: todas las comunas quedan liberadas */

void liberar_estructura(struct lista_recursos* lista_recursos, struct lista_comunas* lista_comunas);
	/* Libera una lista de recursos suelta y la lista de comunas
		Entrada: la lista de recursos y la lista de comunas
		Salida: ambas quedan liberadas. No pases una lista que ya viva dentro de una comuna */

int largo_comuna(struct lista_comunas* lista);
	/* Da una vuelta al circulo y devuelve el largo
		Entrada: la lista circular
		Salida: el largo de la lista */

int largo_recurso(struct lista_recursos* lista);
	/* Da una vuelta a la lista doble y devuelve el largo
		Entrada: la lista doble
		Salida: el largo de la lista */

#endif
