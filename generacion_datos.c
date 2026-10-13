#include <stdio.h>
#include "estructuras.h"
#include "generacion_datos.h"
#include "utils.h"

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
        struct char_array* c = actual->nombre->inicio;
        struct string* nombre = crear_string("");
        nombre->inicio = crear_char_array(c->caracter);

        struct char_array* copiar = nombre->inicio;
        c = c->siguiente;
        while (c != NULL) {
            copiar->siguiente = crear_char_array(c->caracter);
            copiar = copiar->siguiente;
            c = c->siguiente;
        }

        agregar_comuna(lista, crear_comuna(nombre));

        indice++;
        actual = actual->siguiente;
    }
    return lista;
}

/* Genera la lista de recursos a partir de una lista de archivos con los nombres  y datos de los recursos
    Entrada: la lista de archivos con los nombres y datos de los recursos
    Salida: la lista de recursos */
struct lista_recursos* generar_lista_recursos(struct lista_archivo* lista_archivo, int cantidad_recursos, int cantidad_personas) {
    if (lista_archivo == NULL) {
        return NULL;
    }

    struct lista_recursos* lista = crear_lista_recursos();
    int indice = 0;
    struct entrada_lista_archivo* actual = lista_archivo->inicio;
    while (actual != NULL && indice < cantidad_recursos) {
        struct nodo_recurso* recurso = crear_recurso_desde_linea(actual->nombre, cantidad_personas);
        recurso->cantidad = aleatorio(0, recurso->maximo);
        agregar_recurso(lista, recurso);
        indice++;
        actual = actual->siguiente;
    }
    return lista;
}

/* Genera la lista de personas a partir de una lista de archivos con los nombres
    Entrada: la lista de archivos con los nombres y cuantas personas lleva cada comuna
    Salida: la lista de personas */
struct lista_personas* generar_lista_personas(struct lista_archivo* lista_archivo, int cantidad) {
    if (lista_archivo == NULL) {
        return NULL;
    }

    struct lista_personas* lista = crear_lista_personas();
    int indice = 0;
    struct entrada_lista_archivo* actual = lista_archivo->inicio;
    while (actual != NULL && indice < cantidad) {
        struct char_array* c = actual->nombre->inicio;
        struct string* nombre = crear_string("");

        nombre->inicio = crear_char_array(c->caracter);

        struct char_array* nombre_recorrer = nombre->inicio;
        c = c->siguiente;
        while (c != NULL) {
            nombre_recorrer->siguiente = crear_char_array(c->caracter);
            nombre_recorrer = nombre_recorrer->siguiente;
            c = c->siguiente;
        }

        agregar_persona(lista, crear_persona(nombre));
        indice++;
        actual = actual->siguiente;
    }
    return lista;
}

/* Agrega los recursos a las comunas
    Entrada: la lista de comunas, las listas de bienes, servicios y personas, y cuantos de cada uno lleva cada comuna
    Salida: las comunas con sus recursos */
void agregar_recursos_comunas(struct lista_comunas* lista_comunas, struct lista_archivo* lista_bienes, struct lista_archivo* lista_servicios, struct lista_archivo* lista_personas, int cantidad_bienes, int cantidad_servicios, int cantidad_personas) {
    
    struct nodo_lista_comunas* inicio = lista_comunas->inicio;
    struct nodo_lista_comunas* actual = inicio;

    barajar_lista_archivo(lista_bienes);
    barajar_lista_archivo(lista_servicios);
    barajar_lista_archivo(lista_personas);
    liberar_lista_recursos(actual -> comuna -> bienes);
    liberar_lista_recursos(actual -> comuna -> servicios);
    liberar_lista_personas(actual -> comuna -> personas);
    actual -> comuna -> bienes = generar_lista_recursos(lista_bienes, cantidad_bienes, cantidad_personas);
    actual -> comuna -> servicios = generar_lista_recursos(lista_servicios, cantidad_servicios, cantidad_personas);
    actual -> comuna -> personas = generar_lista_personas(lista_personas, cantidad_personas);
    barajar_lista_archivo(lista_bienes);
    barajar_lista_archivo(lista_servicios);
    barajar_lista_archivo(lista_personas);
    actual = actual->siguiente;

    while (actual != inicio) {
        liberar_lista_recursos(actual -> comuna -> bienes);
        liberar_lista_recursos(actual -> comuna -> servicios);
        liberar_lista_personas(actual -> comuna -> personas);
        actual -> comuna -> bienes = generar_lista_recursos(lista_bienes, cantidad_bienes, cantidad_personas);
        actual -> comuna -> servicios = generar_lista_recursos(lista_servicios, cantidad_servicios, cantidad_personas);
        actual -> comuna -> personas = generar_lista_personas(lista_personas, cantidad_personas);
        barajar_lista_archivo(lista_bienes);
        barajar_lista_archivo(lista_servicios);
        barajar_lista_archivo(lista_personas);
        actual = actual->siguiente;
    }
}

/* Genera los datos de las comunas y los recursos
    Entrada: la cantidad de comunas, de servicios, de bienes y de personas por comuna
    Salida: la lista de comunas con sus recursos */

struct lista_comunas* generar_datos(int cantidad_comunas, int cantidad_servicios, int cantidad_bienes, int cantidad_personas) {

    FILE* archivo = leer_archivo("comunas");
    struct lista_archivo* lista_archivo_comunas = generar_lista_archivo(archivo);
    fclose(archivo);


    archivo = leer_archivo("bienes");
    struct lista_archivo* lista_archivo_bienes = generar_lista_archivo(archivo);
    fclose(archivo);


    archivo = leer_archivo("servicios");
    struct lista_archivo* lista_archivo_servicios = generar_lista_archivo(archivo);
    fclose(archivo);

    archivo = leer_archivo("personas");
    struct lista_archivo* lista_archivo_personas = generar_lista_archivo(archivo);
    fclose(archivo);


    barajar_lista_archivo(lista_archivo_comunas);
    barajar_lista_archivo(lista_archivo_bienes);
    barajar_lista_archivo(lista_archivo_servicios);
    barajar_lista_archivo(lista_archivo_personas);

    struct lista_comunas* lista_comunas = generar_lista_comunas(lista_archivo_comunas, cantidad_comunas);

    agregar_recursos_comunas(lista_comunas, lista_archivo_bienes, lista_archivo_servicios, lista_archivo_personas, cantidad_bienes, cantidad_servicios, cantidad_personas);

    return lista_comunas;
}
