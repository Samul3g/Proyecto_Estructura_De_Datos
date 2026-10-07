#include <stdio.h>
#include <stdlib.h>
#include "estructuras.h"
#include "utils.h"


struct lista_archivo* crear_lista_archivo() {
    struct lista_archivo* lista =
    calloc(1, sizeof(struct lista_archivo));
    lista->inicio = NULL;
    lista->ultimo = NULL;
    return lista;
}

/* Libera una lista de archivo
	Entrada: la lista
	Salida: la lista queda liberada */
void liberar_lista_archivo(struct lista_archivo* lista) {
    struct entrada_lista_archivo* actual = lista->inicio;
    while (actual != NULL) {
        struct entrada_lista_archivo* siguiente = actual->siguiente;
        liberar_entrada_lista_archivo(actual);
        actual = siguiente;
    }
    free(lista);
}

/* Crea una entrada de lista de archivo
	Entrada: el nombre y el indice
	Salida: la entrada creada */
struct entrada_lista_archivo* crear_entrada_lista_archivo(struct string* nombre, int indice) {
    struct entrada_lista_archivo* entrada =
    calloc(1, sizeof(struct entrada_lista_archivo));
    entrada->nombre = nombre;
    entrada->indice = indice;
    entrada->siguiente = NULL;
    return entrada;
}

/* Libera una entrada de lista de archivo
	Entrada: la entrada
	Salida: la entrada queda liberada */
void liberar_entrada_lista_archivo(struct entrada_lista_archivo* entrada) {
    liberar_string(entrada->nombre);
    free(entrada);
}

/* Busca una entrada de lista de archivo por indice
	Entrada: la lista y el indice
	Salida: la entrada si existe, NULL si no */
struct entrada_lista_archivo* buscar_entrada_lista_archivo(struct lista_archivo* lista, int indice) {
    struct entrada_lista_archivo* actual = lista->inicio;
    while (actual != NULL) {
        if (actual->indice == indice) {
            return actual;
        }
        actual = actual->siguiente;
    }
    return NULL;
}

/* Agrega una entrada de lista de archivo a la lista
	Entrada: la lista y la entrada
	Salida: la entrada queda agregada a la lista */
void agregar_entrada_lista_archivo(struct lista_archivo* lista, struct entrada_lista_archivo* entrada) {
    if (lista->inicio == NULL) {
        lista->inicio = entrada;
        lista->ultimo = entrada;
    } else {
        lista->ultimo->siguiente = entrada;
        lista->ultimo = entrada;
    }
}

/* Elimina una entrada de lista de archivo por nombre
	Entrada: la lista y el nombre
	Salida: la entrada queda eliminada de la lista */
void eliminar_entrada_lista_archivo(struct lista_archivo* lista, struct string* nombre) {
    struct entrada_lista_archivo* anterior = NULL;
    struct entrada_lista_archivo* actual = lista->inicio;
    while (actual != NULL) {
        if (mismos_nombres(actual->nombre, nombre)) {
            if (anterior == NULL) {
                lista->inicio = actual->siguiente;
            } else {
                anterior->siguiente = actual->siguiente;
            }
            if (lista->ultimo == actual) {
                lista->ultimo = anterior;
            }
            liberar_entrada_lista_archivo(actual);
            return;
        }
        anterior = actual;
        actual = actual->siguiente;
    }
}

/* Baraja la lista de archivo
	Entrada: la lista
	Salida: la lista barajada */
void barajar_lista_archivo(struct lista_archivo* lista) {
    int cantidad = 0;
    struct entrada_lista_archivo* actual = lista->inicio;
    while (actual != NULL) {
        cantidad++;
        actual = actual->siguiente;
    }
    if (cantidad < 2) {
        return;
    }

    struct entrada_lista_archivo** entradas = malloc(cantidad * sizeof(struct entrada_lista_archivo*));
    actual = lista->inicio;
    for (int i = 0; i < cantidad; i++) {
        entradas[i] = actual;
        actual = actual->siguiente;
    }
    for (int i = cantidad - 1; i > 0; i--) {
        int indice = aleatorio(0, i);
        struct entrada_lista_archivo* tmp = entradas[i];
        entradas[i] = entradas[indice];
        entradas[indice] = tmp;
    }
    for (int i = 0; i < cantidad - 1; i++) {
        entradas[i]->siguiente = entradas[i + 1];
    }
    entradas[cantidad - 1]->siguiente = NULL;
    lista->inicio = entradas[0];
    lista->ultimo = entradas[cantidad - 1];
    free(entradas);
}

/* Crea un char_array con el caracter
	Entrada: el caracter
	Salida: el char_array creado */
struct char_array* crear_char_array(char caracter) {
    struct char_array* char_array =
    calloc(1, sizeof(struct char_array));
    char_array->caracter = caracter;
    return char_array;
}

/* Libera un nodo de la cadena
	Entrada: el char_array
	Salida: la memoria de ese nodo queda libre */
void liberar_char_array(struct char_array* char_array) {
    free(char_array);
}

/* Crea un string con la cadena
	Entrada: la cadena
	Salida: el string creado */
struct string* crear_string(char* cadena) {
    struct string* string = calloc(1, sizeof(struct string));
    if (cadena == NULL || cadena[0] == '\0') {
        return string;
    }
    string->inicio = crear_char_array(cadena[0]);
    struct char_array* actual = string->inicio;
    for (int i = 1; cadena[i] != '\0'; i++) {
        actual->siguiente = crear_char_array(cadena[i]);
        actual = actual->siguiente;
    }
    return string;
}

/* Libera un string y su cadena
	Entrada: el string
	Salida: la memoria del string queda libre. La cadena tiene que haber salido de malloc */
void liberar_string(struct string* string) {
    struct char_array* actual = string->inicio;
    while (actual != NULL) {
        struct char_array* siguiente = actual->siguiente;
        liberar_char_array(actual);
        actual = siguiente;
    }
    free(string);
}



/* Crea un recurso con nombre, cantidad, maximo y relevancia
	Entrada: nombre, necesidad, maximo, relevancia, cantidad
	Salida: el nodo del recurso creado */
struct nodo_recurso* crear_nodo_recurso(struct string* nombre, bool necesidad,  int maximo, int relevancia, int cantidad) {
    struct nodo_recurso* recurso =
    calloc(1, sizeof(struct nodo_recurso));
    recurso->nombre = nombre;
    recurso->necesidad = necesidad;
    recurso->maximo = maximo;
    recurso->relevancia = relevancia;
    recurso->cantidad = cantidad;
    return recurso;
}



/* Arma el nodo de la lista doble a partir de un recurso
	Entrada: el recurso que va dentro del nodo
	Salida: el nodo de la lista, con anterior y siguiente vacios */
struct nodo_lista_recursos* crear_nodo_lista_recursos(struct nodo_recurso* recurso) {
    struct nodo_lista_recursos* nodo_lista_recursos =
    calloc(1, sizeof(struct nodo_lista_recursos));
    nodo_lista_recursos->recurso = recurso;
    return nodo_lista_recursos;
}

/* Crea la lista doble de recursos
	Entrada: ninguna
	Salida: la lista, con inicio en NULL */
struct lista_recursos* crear_lista_recursos() {
    struct lista_recursos* lista_recursos =
    calloc(1, sizeof(struct lista_recursos));
    lista_recursos->inicio = NULL;
    return lista_recursos;
}

/* Arma el nodo del circulo a partir de una comuna
	Entrada: la comuna que va dentro del nodo
	Salida: el nodo, sin enlazar todavia */
struct nodo_lista_comunas* crear_nodo_lista_comunas(struct comuna* comuna) {
    struct nodo_lista_comunas* nodo_lista_comunas =
    calloc(1, sizeof(struct nodo_lista_comunas));
    nodo_lista_comunas->comuna = comuna;
    return nodo_lista_comunas;
}

/* Crea la lista circular de comunas
	Entrada: ninguna
	Salida: la lista, con inicio en NULL */
struct lista_comunas* crear_lista_comunas() {
    struct lista_comunas* lista_comunas =
    calloc(1, sizeof(struct lista_comunas));
    lista_comunas->inicio = NULL;
    return lista_comunas;
}

void imprimit_lista_comunas(struct lista_comunas* lista_comunas) {
    struct nodo_lista_comunas* nodo = lista_comunas->inicio;
    if (nodo == NULL) {
        return;
    }
    do {
        struct char_array* actual = nodo->comuna->nombre->inicio;
        while (actual != NULL) {
            putchar(actual->caracter);
            actual = actual->siguiente;
        }
        putchar('\n');
        nodo = nodo->siguiente;
    } while (nodo != lista_comunas->inicio);
}

/* Crea una comuna con sus dos listas de inventario vacias
	Entrada: el nombre de la comuna
	Salida: la comuna, con bienes y servicios listos para usar */
struct comuna* crear_comuna(struct string* nombre) {
    struct comuna* comuna =
    calloc(1, sizeof(struct comuna));
    comuna->nombre = nombre;
    comuna->bienes = crear_lista_recursos();
    comuna->servicios = crear_lista_recursos();
    comuna->satisfaccion = 0;
    comuna->necesidad = 0;
    return comuna;
}

/* Recorre la lista doble hasta encontrar un recurso por nombre
	Entrada: la lista y el nombre a buscar
	Salida: el recurso si esta, NULL si no */
struct nodo_recurso* buscar_recurso(struct lista_recursos* lista_recursos, struct string* nombre) {
    struct nodo_lista_recursos* nodo_lista_recursos = lista_recursos->inicio;
    while (nodo_lista_recursos != NULL) {
        if (mismos_nombres(nodo_lista_recursos->recurso->nombre, nombre)) {
            return nodo_lista_recursos->recurso;
        }
        nodo_lista_recursos = nodo_lista_recursos->siguiente;
    }
    return NULL;
}

/* Mete un recurso al frente de la lista doble
	Entrada: la lista y el recurso
	Salida: la lista queda con ese recurso de primero */
void agregar_recurso(struct lista_recursos* lista_recursos, struct nodo_recurso* recurso) {
    struct nodo_lista_recursos* nodo_lista_recursos = crear_nodo_lista_recursos(recurso);
    if (lista_recursos->inicio == NULL) {
        lista_recursos->inicio = nodo_lista_recursos;
    } else {
        nodo_lista_recursos->siguiente = lista_recursos->inicio;
        lista_recursos->inicio->anterior = nodo_lista_recursos;
        lista_recursos->inicio = nodo_lista_recursos;
    }
}

/* Mete una comuna en el circulo
	Entrada: la lista circular y la comuna
	Salida: la comuna queda enlazada y pasa a ser el inicio. Si era la primera, se apunta a si misma */
void agregar_comuna(struct lista_comunas* lista_comunas, struct comuna* comuna) {
    struct nodo_lista_comunas* nodo = crear_nodo_lista_comunas(comuna);
    if (lista_comunas->inicio == NULL) {
        nodo->siguiente = nodo;
        nodo->anterior = nodo;
        lista_comunas->inicio = nodo;
    } else {
        struct nodo_lista_comunas* ultimo = lista_comunas->inicio->anterior;
        nodo->siguiente = lista_comunas->inicio;
        nodo->anterior = ultimo;
        ultimo->siguiente = nodo;
        lista_comunas->inicio->anterior = nodo;
        lista_comunas->inicio = nodo;
    }
}

/* Da una vuelta al circulo buscando una comuna por nombre
	Entrada: la lista circular y el nombre
	Salida: el nodo de la comuna si esta, NULL si la lista esta vacia o no aparece */
struct nodo_lista_comunas* buscar_comuna(struct lista_comunas* lista_comunas, struct string* nombre) {
    struct nodo_lista_comunas* nodo = lista_comunas->inicio;
    if (nodo == NULL) {
        return NULL;
    }

    if (mismos_nombres(nodo->comuna->nombre, nombre)) {
        return nodo;
    }
    nodo = nodo->siguiente;

    while (nodo != lista_comunas->inicio) {
        if (mismos_nombres(nodo->comuna->nombre, nombre)) {
            return nodo;
        }
        nodo = nodo->siguiente;
    }
    return NULL;
}

/* Saca un recurso de la lista doble y libera su memoria
	Entrada: la lista y el nombre del recurso
	Salida: el nodo desaparece y los vecinos quedan enlazados. Si no esta, la lista no cambia */
void eliminar_recurso(struct lista_recursos* lista_recursos, struct string* nombre) {
    struct nodo_lista_recursos* nodo = lista_recursos->inicio;
    while (nodo != NULL) {
        if (mismos_nombres(nodo->recurso->nombre, nombre)) {
            if (nodo->anterior != NULL) {
                nodo->anterior->siguiente = nodo->siguiente;
            } else {
                lista_recursos->inicio = nodo->siguiente;
            }
            if (nodo->siguiente != NULL) {
                nodo->siguiente->anterior = nodo->anterior;
            }
            liberar_recurso(nodo->recurso);
            free(nodo);
            return;
        }
        nodo = nodo->siguiente;
    }
}

/* Saca una comuna del circulo y libera la comuna con sus listas
	Entrada: la lista circular y el nombre
	Salida: el circulo se cierra sin esa comuna. Si era la unica, inicio queda en NULL */
void eliminar_comuna(struct lista_comunas* lista_comunas, struct string* nombre) {
    struct nodo_lista_comunas* nodo = lista_comunas->inicio;
    if (nodo == NULL) {
        return;
    }

    if (mismos_nombres(nodo->comuna->nombre, nombre)) {
        if (nodo->siguiente == nodo) {
            lista_comunas->inicio = NULL;
        } else {
            lista_comunas->inicio = nodo->siguiente;
            lista_comunas->inicio->anterior = nodo->anterior;
            lista_comunas->inicio->anterior->siguiente = lista_comunas->inicio;
        }
        liberar_comuna(nodo->comuna);
        free(nodo);
        return;
    }
    nodo = nodo->siguiente;

    while (nodo != lista_comunas->inicio) {
        if (mismos_nombres(nodo->comuna->nombre, nombre)) {
            nodo->anterior->siguiente = nodo->siguiente;
            nodo->siguiente->anterior = nodo->anterior;
            liberar_comuna(nodo->comuna);
            free(nodo);
            return;
        }
        nodo = nodo->siguiente;
    }
}

/* Libera un recurso y su nombre
	Entrada: el recurso
	Salida: la memoria del recurso y su nombre quedan libres */
void liberar_recurso(struct nodo_recurso* recurso) {
    liberar_string(recurso->nombre);
    free(recurso);
}

/* Recorre la lista doble y libera cada recurso, cada nodo y la lista
	Entrada: la lista de recursos
	Salida: toda esa lista queda liberada */
void liberar_lista_recursos(struct lista_recursos* lista_recursos) {
    struct nodo_lista_recursos* nodo = lista_recursos->inicio;
    while (nodo != NULL) {
        struct nodo_lista_recursos* siguiente = nodo->siguiente;
        liberar_recurso(nodo->recurso);
        free(nodo);
        nodo = siguiente;
    }
    free(lista_recursos);
}

/* Libera una comuna, su nombre y las listas de bienes y servicios
	Entrada: la comuna
	Salida: la comuna y su inventario quedan liberados */
void liberar_comuna(struct comuna* comuna) {
    liberar_lista_recursos(comuna->bienes);
    liberar_lista_recursos(comuna->servicios);
    liberar_string(comuna->nombre);
    free(comuna);
}   

/* Abre el circulo y libera cada comuna, cada nodo y la lista
	Entrada: la lista circular
	Salida: todas las comunas quedan liberadas */
void liberar_lista_comunas(struct lista_comunas* lista_comunas) {
    struct nodo_lista_comunas* nodo = lista_comunas->inicio;
    if (nodo != NULL) {
        nodo->anterior->siguiente = NULL;
    }
    while (nodo != NULL) {
        struct nodo_lista_comunas* siguiente = nodo->siguiente;
        liberar_comuna(nodo->comuna);
        free(nodo);
        nodo = siguiente;
    }
    free(lista_comunas);
}

/* Libera una lista de recursos suelta y la lista de comunas
	Entrada: la lista de recursos y la lista de comunas
	Salida: ambas quedan liberadas. No pases una lista que ya viva dentro de una comuna */
void liberar_estructura(struct lista_recursos* lista_recursos, struct lista_comunas* lista_comunas) {
    liberar_lista_recursos(lista_recursos);
    liberar_lista_comunas(lista_comunas);
}

/* Da una vuelta al circulo y devuelve el largo
	Entrada: la lista circular
	Salida: el largo de la lista */
int largo_comuna(struct lista_comunas* lista) {
    struct nodo_lista_comunas* nodo = lista->inicio;
    if (nodo == NULL) {
        return 0;
    }

    int largo = 1;
    nodo = nodo->siguiente;
    while (nodo != lista->inicio) {
        largo++;
        nodo = nodo->siguiente;
    }
    return largo;
}

/* Da una vuelta a la lista doble y devuelve el largo
	Entrada: la lista doble
	Salida: el largo de la lista */
int largo_recurso(struct lista_recursos* lista) {
    struct nodo_lista_recursos* nodo = lista->inicio;
    if (nodo == NULL) {
        return 0;
    }

    int largo = 1;
    nodo = nodo->siguiente;
    while (nodo != NULL) {
        largo++;
        nodo = nodo->siguiente;
    }
    return largo;
}