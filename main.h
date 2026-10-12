#indef MAIN_H
#define MAIN_H

extern int cantidad_comunas;
extern int bienes;
extern int servicios;
extern int personas;

/* Funcion de impresion del logo y saludo inicial
 * Entradas: nada
 * Salidas: nada
 */
void saludo_inicio();

/* Funcion que pide los valores de las comunas, bienes y servicios que el usuario pida y los coloca en variables globales
 * Entradas: nada
 * Salidas: ninguna*/
void configuracion_inicial();



#endif
