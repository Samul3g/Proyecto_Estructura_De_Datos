#include <stdio.h>
#include <stdlib.h>
#include "estructuras.h"
#include "generacion_datos.h"
#include "utils.h"
FILE *comunas_file;

int siguiente_caracter(FILE *archivo) {
    return fgetc(archivo);
}

/* Genera las comunas
	Entrada: el numero de comunas
	Salida: la lista de comunas */
struct lista_comunas* generar_comunas(int numero_comunas){
    struct lista_comunas* lista_comunas = crear_lista_comunas();
    struct char_array* inicio = NULL;
    struct char_array* actual = NULL;
    int i = 0;

    comunas_file = fopen("comunas", "r");
    if (comunas_file == NULL) {
        printf("Error al abrir el archivo de comunas\n");
        free(lista_comunas);
        return NULL;
    }

    while (i < numero_comunas) {
        int c = fgetc(comunas_file);
        if (c == EOF) {
            struct string* parcial = calloc(1, sizeof(struct string));
            printf("Error al leer el archivo de comunas\n");
            parcial->inicio = inicio;
            liberar_string(parcial);
            fclose(comunas_file);
            liberar_lista_comunas(lista_comunas);
            return NULL;
        }

        if (c != '\n') {
            struct char_array* nodo = crear_char_array((char) c);
            if (inicio == NULL) {
                inicio = nodo;
            } else {
                actual->siguiente = nodo;
            }
            actual = nodo;
        } else {
            struct string* nombre = calloc(1, sizeof(struct string));
            nombre->inicio = inicio;
            
            agregar_comuna(lista_comunas, crear_comuna(nombre));
            i++;
            inicio = NULL;
            actual = NULL;
            
        }
    }
    fclose(comunas_file);
    return lista_comunas;
}

