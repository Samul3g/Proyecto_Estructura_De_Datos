#include "estructuras.h"
#include "manejo_recursos.h"
#include "utils.h"



int emergencia(struct comuna* comuna){
    int tipo = aleatorio(1, 2);
    int i = 0;
    if(tipo == 1){
        struct nodo_lista_recursos* actual = comuna->bienes->inicio;
        int largo = largo_recurso(comuna->bienes);
        if(largo <= 1){
            i = largo;
        }else{
        i = aleatorio(0, largo - 1);}
        while(i > 0){
            actual = actual->siguiente;
            i--;
        }
        if(actual == NULL){
            return 0;
        }

        if (actual -> recurso -> cantidad == 0){
            return 0;
        }
        actual -> recurso -> cantidad = actual -> recurso -> cantidad - aleatorio(1, actual -> recurso -> cantidad);
        if (actual -> recurso -> cantidad < 0){
            actual -> recurso -> cantidad = 0;
        }
        return 1;
    }else{
        struct nodo_lista_recursos* actual = comuna->servicios->inicio;
        int largo = largo_recurso(comuna->servicios);
        if(largo <= 1){
            i = largo;
        }else{
        i = aleatorio(0, largo - 1);}
        while(i > 0){
            actual = actual->siguiente;
            i--;
        }
        if(actual == NULL){
            return 0;
        }

        if (actual -> recurso -> cantidad == 0){
            return 0;
        }
        actual -> recurso -> cantidad = actual -> recurso -> cantidad - aleatorio(1, actual -> recurso -> cantidad);
        if (actual -> recurso -> cantidad < 0){
            actual -> recurso -> cantidad = 0;
        }
        return 1;
    } return 0;
}

int prosperidad(struct comuna* comuna){
    int tipo = aleatorio(1, 2);
    int i = 0;
    if(tipo == 1){
        struct nodo_lista_recursos* actual = comuna->bienes->inicio;
        int largo = largo_recurso(comuna->bienes);
        if(largo <= 1){
            i = largo;
        }else{
        i = aleatorio(0, largo - 1);}
        while(i > 0){
            actual = actual->siguiente;
            i--;
        }
        if(actual == NULL){
            return 0;
        }

        if (actual -> recurso -> cantidad == 0){
            return 0;
        }
        actual -> recurso -> cantidad = actual -> recurso -> cantidad + aleatorio(1, actual -> recurso -> cantidad);
        if (actual -> recurso -> cantidad > actual -> recurso -> maximo){
            actual -> recurso -> cantidad = actual -> recurso -> maximo;
        }
        return 1;
    }else{
        struct nodo_lista_recursos* actual = comuna->servicios->inicio;
        int largo = largo_recurso(comuna->servicios);
        if(largo <= 1){
            i = largo;
        }else{
        i = aleatorio(0, largo - 1);}

        while(i > 0){
            actual = actual->siguiente;
            i--;
        }
        if(actual == NULL){
            return 0;
        }

        if (actual -> recurso -> cantidad == 0){
            return 0;
        }
        actual -> recurso -> cantidad = actual -> recurso -> cantidad + aleatorio(1, actual -> recurso -> cantidad);
        if (actual -> recurso -> cantidad > actual -> recurso -> maximo){
            actual -> recurso -> cantidad = actual -> recurso -> maximo;
        }

        return 1;
    } return 0;
}
