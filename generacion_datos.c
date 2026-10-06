#include <stdio.h>
#include <stdlib.h>
#include "estructuras.h"
FILE *comunas_file;

int siguiente_caracter(FILE *archivo) {
    return fgetc(archivo);
}


struct lista_comunas* generar_comunas(int numero_comunas){
    struct lista_comunas* lista_comunas = crear_lista_comunas();
    comunas_file = fopen("comunas", "r");
    if (comunas_file == NULL) {
        printf("Error al abrir el archivo de comunas\n");
        return NULL;
    }
    struct string* nombre_comuna = crear_string("");
    int i = 0;
    int a = 0;

    while (i < numero_comunas) {
        int c = fgetc(comunas_file);
        if (c == EOF) {
            printf("Error al leer el archivo de comunas\n");
            return NULL;
        }

        if(c != '\n'){
            nombre_comuna[a] = c;
            a++;
        }else{
            a = 0;
            struct comuna* comuna = crear_comuna(nombre_comuna);
            agregar_comuna(lista_comunas, comuna);
            nombre_comuna = "";
            i++;
        }
    }
    fclose(comunas_file);
    return lista_comunas;
}

