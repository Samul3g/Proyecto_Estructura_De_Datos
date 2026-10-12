#ifndef GENERACION_DATOS_H
#define GENERACION_DATOS_H

#include "estructuras.h"

/* Genera la lista de comunas a partir de una lista de archivos con los nombres de las comunas
    Entrada: la lista de archivos
    Salida: la lista de comunas */
struct lista_comunas* generar_lista_comunas(struct lista_archivo* lista_archivo, int cantidad);

/* Genera la lista de recursos a partir de una lista de archivos con los nombres y datos de los recursos
    Entrada: la lista de archivos con los nombres y datos de los recursos
    Salida: la lista de recursos */
struct lista_recursos* generar_lista_recursos(struct lista_archivo* lista_archivo, int cantidad_recursos, int cantidad_personas);

/* Genera la lista de personas a partir de una lista de archivos con los nombres
    Entrada: la lista de archivos con los nombres y cuantas personas lleva cada comuna
    Salida: la lista de personas */
struct lista_personas* generar_lista_personas(struct lista_archivo* lista_archivo, int cantidad);

/* Agrega los recursos a las comunas
    Entrada: la lista de comunas, las listas de bienes, servicios y personas, y cuantos de cada uno lleva cada comuna
    Salida: las comunas con sus recursos */
void agregar_recursos_comunas(struct lista_comunas* lista_comunas, struct lista_archivo* lista_bienes, struct lista_archivo* lista_servicios, struct lista_archivo* lista_personas, int cantidad_bienes, int cantidad_servicios, int cantidad_personas);

/* Genera los datos de las comunas y los recursos
    Entrada: la cantidad de comunas, de servicios, de bienes y de personas por comuna
    Salida: la lista de comunas con sus recursos */
struct lista_comunas* generar_datos(int cantidad_comunas, int cantidad_servicios, int cantidad_bienes, int cantidad_personas);

#endif
