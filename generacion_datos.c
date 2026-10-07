#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "estructuras.h"
#include "generacion_datos.h"
#include "utils.h"

/* Obtiene el siguiente caracter del archivo 
    Entrada: el archivo
    Salida: el siguiente caracter del archivo */
int siguiente_caracter(FILE* archivo) {
    return fgetc(archivo);
}

/* Obtiene la linea del archivo 
    Entrada: el archivo
    Salida: la linea del archivo */
struct string* linea_archivo(FILE* archivo) {

    if (archivo == NULL) {
        return NULL;
    }

    struct string* linea = crear_string("");
    struct char_array* actual = NULL;
    int c = siguiente_caracter(archivo);
    while (c != '\n' && c != EOF) {
        if (actual == NULL) {
            actual = crear_char_array(c);
            linea->inicio = actual;
        } else {
            actual->siguiente = crear_char_array(c);
            actual = actual->siguiente;
        }
        c = siguiente_caracter(archivo);
    }
    if (c == EOF) {
        return NULL;
    } else {
        return linea;
    }
}

/* Genera la lista de archivos a partir de un archivo de lectura
    Entrada: el archivo
   Salida: la lista de archivos */
struct lista_archivo* generar_lista_archivo(FILE* archivo) {

    if (archivo == NULL) {
        return NULL;
    }

    struct lista_archivo* lista = crear_lista_archivo();
    struct string* linea = linea_archivo(archivo);
    int indice = 0;
    while (linea != NULL) {
        agregar_entrada_lista_archivo(lista, crear_entrada_lista_archivo(linea, indice));
        indice++;
        linea = linea_archivo(archivo);
    }
    return lista;
}

/* Lee el archivo y lo devuelve como arhivo de lectura
    Entrada: el nombre del archivo
    Salida: el archivo de lectura */ 
FILE* leer_archivo(char* nombre) {
    FILE* archivo = fopen(nombre, "r");
    if (archivo == NULL) {
        printf("Error al abrir el archivo %s\n", nombre);
        exit(1);
    }
    return archivo;
}
/* Genera la lista de comunas a partir de una lista de archivos con los nombres de las comunas
    Entrada: la lista de archivos
    Salida: la lista de comunas */
struct lista_comunas* generar_lista_comunas(struct lista_archivo* lista_archivo, int cantidad_comunas) {

    if (lista_archivo == NULL) {
        return NULL;
    }

    struct lista_comunas* lista = crear_lista_comunas();
    int indice = 0;
    struct entrada_lista_archivo* actual = lista_archivo->inicio;

    while (actual != NULL && indice < cantidad_comunas) {
        agregar_comuna(lista, crear_comuna(actual->nombre));
        indice++;
        actual = actual->siguiente;
    }
    return lista;
}

/* Genera la lista de recursos a partir de una lista de archivos con los nombres  y datos de los recursos
    Entrada: la lista de archivos con los nombres y datos de los recursos
    Salida: la lista de recursos */
struct lista_recursos* generar_lista_recursos(struct lista_archivo* lista_archivo, int cantidad) {
    if (lista_archivo == NULL) {
        return NULL;
    }

    struct lista_recursos* lista = crear_lista_recursos();
    int indice = 0;
    struct entrada_lista_archivo* actual = lista_archivo->inicio;
    while (actual != NULL && indice < cantidad) {


        bool es_necesidad = 1;
        int maximo_int = 0;
        int relevancia_int = 0;
        int cantidad = 0;

        struct char_array* c = actual->nombre->inicio;
        struct string* nombre = crear_string("");

        nombre->inicio = crear_char_array(c->caracter);

        struct char_array* nombre_recorrer = nombre->inicio;
        c = c->siguiente;
        while (c->caracter != ',') {
            nombre_recorrer->siguiente = crear_char_array(c->caracter);
            nombre_recorrer = nombre_recorrer->siguiente;
            c = c->siguiente;
        }

        c = c->siguiente;

        char necesidad[10];
        int i = 0;

        while (c->caracter != ',') {
            necesidad[i] = c->caracter;
            i++;
            c = c->siguiente;
        }

        necesidad[i] = '\0';

        if (strcmp(necesidad, "true") == 0) {
             es_necesidad = 1;
        } else {
             es_necesidad = 0;
        }
        i = 0;

        c = c->siguiente;
        char maximo[8];
        while (c->caracter != ',') {
            maximo[i] = c->caracter;
            i++;
            c = c->siguiente;
        }
        maximo[i] = '\0';
        
        maximo_int = atoi(maximo);
        i = 0;

        c = c->siguiente;
        char relevancia[6];
        while (c != NULL) {
            relevancia[i] = c->caracter;
            i++;
            c = c->siguiente;
        }
        
        relevancia[i] = '\0';

        relevancia_int = atoi(relevancia);

        cantidad = aleatorio(0, maximo_int);

        agregar_recurso(lista, crear_nodo_recurso(nombre, es_necesidad, maximo_int, relevancia_int, cantidad));
        indice++;
        actual = actual->siguiente;
    }
    return lista;
}

/* Agrega los recursos a las comunas
    Entrada: la lista de comunas, las listas de bienes y servicios, y cuantos de cada uno lleva cada comuna
    Salida: las comunas con sus recursos */
void agregar_recursos_comunas(struct lista_comunas* lista_comunas, struct lista_archivo* lista_bienes, struct lista_archivo* lista_servicios, int cantidad_bienes, int cantidad_servicios) {
    
    struct nodo_lista_comunas* inicio = lista_comunas->inicio;
    struct nodo_lista_comunas* actual = inicio;

    barajar_lista_archivo(lista_bienes);
    barajar_lista_archivo(lista_servicios);
    actual -> comuna -> bienes = generar_lista_recursos(lista_bienes, cantidad_bienes);
    actual -> comuna -> servicios = generar_lista_recursos(lista_servicios, cantidad_servicios);
    barajar_lista_archivo(lista_bienes);
    barajar_lista_archivo(lista_servicios);
    actual = actual->siguiente;

    while (actual != inicio) {
        actual -> comuna -> bienes = generar_lista_recursos(lista_bienes, cantidad_bienes);
        actual -> comuna -> servicios = generar_lista_recursos(lista_servicios, cantidad_servicios);
        barajar_lista_archivo(lista_bienes);
        barajar_lista_archivo(lista_servicios);
        actual = actual->siguiente;
    }
}

/* Genera los datos de las comunas y los recursos
    Entrada: la cantidad de comunas, de servicios y de bienes por comuna
    Salida: la lista de comunas con sus recursos */

struct lista_comunas* generar_datos(int cantidad_comunas, int cantidad_servicios, int cantidad_bienes) {

    FILE* archivo = leer_archivo("comunas");
    struct lista_archivo* lista_archivo_comunas = generar_lista_archivo(archivo);
    fclose(archivo);


    archivo = leer_archivo("bienes");
    struct lista_archivo* lista_archivo_bienes = generar_lista_archivo(archivo);
    fclose(archivo);


    archivo = leer_archivo("servicios");
    struct lista_archivo* lista_archivo_servicios = generar_lista_archivo(archivo);
    fclose(archivo);


    barajar_lista_archivo(lista_archivo_comunas);
    barajar_lista_archivo(lista_archivo_bienes);
    barajar_lista_archivo(lista_archivo_servicios);

    struct lista_comunas* lista_comunas = generar_lista_comunas(lista_archivo_comunas, cantidad_comunas);

    agregar_recursos_comunas(lista_comunas, lista_archivo_bienes, lista_archivo_servicios, cantidad_bienes, cantidad_servicios);

    return lista_comunas;
}