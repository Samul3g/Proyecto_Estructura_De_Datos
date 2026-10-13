#include "estructuras.h"
#include "manejo_recursos.h"

/* Funcion que realiza el regalo de un bien entre dos comunas
    Entrada: comuna_dar, comuna_recibir, bien, cantidad
    Salida: 1 (true/exito)
*/
int regalo_bien(struct comuna* comuna_dar, struct comuna* comuna_recibir, char* bien, int cantidad){
    int reducir = reduccion_bien(bien, cantidad, comuna_dar);
    int aumentar = aumento_bien(bien, cantidad, comuna_recibir);
    
    if(reducir == 1 && aumentar == 1){
			return 1
	}
    return 0;
}

/* Funcion que realiza el regalo de un servicio entre dos comunas
    Entrada: comuna_dar, comuna_recibir, servicio, cantidad
    Salida: 1 (true/exito)
*/
int regalo_servicio(struct comuna* comuna_dar, struct comuna* comuna_recibir, char* servicio, int cantidad){
    int reduccion = reduccion_servicio(servicio, cantidad, comuna_dar);
    int aumento = aumento_servicio(servicio, cantidad, comuna_recibir);
    
    if(reduccion == 1 && aumento == 1){
		return 1;
	}
	return 0;
}

int emergencia(struct comuna* comuna){
    int tipo = aleatorio(1, 2);
    if(tipo == 1){
        actual = comuna->bienes->inicio;
        i = random(1, largo_bienes(comuna->bienes));
        while(i > 0){
            actual = actual->siguiente;
            i--;
        }
        if(actual != NULL){
            return emergencia_bien(comuna);
        }

        actual -> cantidad = actual -> cantidad - aleatorio(1, actual -> cantidad);
        if (actual -> cantidad < 0){
            actual -> cantidad = 0;
        }
        return 1;
    }
    else{
        actual = comuna->servicios->inicio;
        i = random(1, largo_servicios(comuna->servicios));
        while(i > 0){
            actual = actual->siguiente;
            i--;
        }
        if(actual != NULL){
            return emergencia_servicio(comuna);
        }
    }

int prosperidad(struct comuna* comuna){
    int i = random(1, largo_bienes(comuna->bienes));
    while(i > 0){
        actual = actual->siguiente;
        i--;
    }
    if(actual != NULL){
        return prosperidad_bien(comuna);
    }
    actual -> cantidad = actual -> cantidad + aleatorio(1, actual -> cantidad);
    if (actual -> cantidad > actual -> maximo){
        actual -> cantidad = actual -> maximo;
    }
    return 1;
    else{
        actual = comuna->servicios->inicio;
        i = random(1, largo_servicios(comuna->servicios));
        while(i > 0){
            actual = actual->siguiente;
            i--;
        }
    }
    if(actual != NULL){
        return prosperidad_servicio(comuna);
    }
    actual -> cantidad = actual -> cantidad + aleatorio(1, actual -> cantidad);
    if (actual -> cantidad > actual -> maximo){
        actual -> cantidad = actual -> maximo;
    }
    return 1;
}
