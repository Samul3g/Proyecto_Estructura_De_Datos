#include "estructuras.h"
#include "manejo_recursos.h"

/* Funcion que realiza el regalo de un bien entre dos comunas
    Entrada: comuna_dar, comuna_recibir, bien, cantidad
    Salida: 1 (true/exito)
*/
int regalo_bien(struct comuna* comuna_dar, struct comuna* comuna_recibir, char* bien, int cantidad){
    reduccion_bien(bien, cantidad, comuna_dar);
    aumento_bien(bien, cantidad, comuna_recibir);
    return 1;
}

/* Funcion que realiza el regalo de un servicio entre dos comunas
    Entrada: comuna_dar, comuna_recibir, servicio, cantidad
    Salida: 1 (true/exito)
*/
int regalo_servicio(struct comuna* comuna_dar, struct comuna* comuna_recibir, char* servicio, int cantidad){
    reduccion_servicio(servicio, cantidad, comuna_dar);
    aumento_servicio(servicio, cantidad, comuna_recibir);
    return 1;
}
