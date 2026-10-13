#include <stdio.h>
#include <stdlib.h>
#include "main.h"
#include "estructuras.h"
#include "manejo_recursos.h"
#include "generacion_datos.h"
#include "indices.h"
#include "simulacion.h"
#include "utils.h"

	//Variables globales
	int cantidad_comunas = 0;
	int bienes = 0;
	int servicios = 0;
	int personas = 0;


/* Funcion de impresion del logo y saludo inicial
 * Entradas: nada
 * Salidas: nada
 */
void saludo_inicio(){
	printf("\033[33m                      \\ | /");
	printf("\033[33m\n                    --- ☀ ---");
	printf("\033[33m\n                      / | \\");
	printf("\033[32m\n\n             /\\        /\\        /\\");
	printf("\033[32m\n            /  \\  /\\  /  \\  /\\  /  \\");
	printf("\033[32m\n           /    \\/  \\/    \\/  \\/    \\");
	printf("\033[32m\n          /______\\__ \\____/____\\_____\\");
	printf("\033[0m\n            ╔═════════════════════╗");
	printf("\n            ║ Comunas Anarquistas ║");
	printf("\n            ╚═════════╤═╤═════════╝");
	printf("\n                     ╱ │ ╲");
	printf("\n                    ╱  │  ╲");
	printf("\n                 ──┴───┴───┴──");
	printf("\n\n\n\033[1;33mBienvenid@ a la simulacion:\033[0m Una sociedad solarpunk de comunas anarquistas");
	printf("\n\n      \033[32mHecho por:\033[0m Paula Flores Solano y Samuel Ureña Gonzalez");
	printf("\n\n                \033[2mEstructuras de Datos, IIS 2026\033[0m");
}

/* Funcion de impresion del logo y despedida
 * Entradas: nada
 * Salidas: nada (impresion en consola)
 */
void despedida(){
	printf("\n\033[33m                      \\ | /");
	printf("\033[33m\n                    --- ☀ ---");
	printf("\033[33m\n                      / | \\");
	printf("\033[32m\n\n             /\\        /\\        /\\");
	printf("\033[32m\n            /  \\  /\\  /  \\  /\\  /  \\");
	printf("\033[32m\n           /    \\/  \\/    \\/  \\/    \\");
	printf("\033[32m\n          /______\\__ \\____/____\\_____\\");
	printf("\033[0m\n            ╔═════════════════════╗");
	printf("\n            ║ Por Samuel y Paula  ║");
	printf("\n            ╚═════════╤═╤═════════╝");
	printf("\n                     ╱ │ ╲");
	printf("\n                    ╱  │  ╲");
	printf("\n                 ──┴───┴───┴──");
	printf("\n\n\033[1;33mYa se acabo la simulacion, adios!!!\033[0m\n\n");
}


/* Pide un entero dentro del rango. Si escriben texto, lo descarta y vuelve a preguntar.
 * Entradas: la pregunta, el minimo y el maximo
 * Salidas: el entero valido */
int pedir_cantidad(const char* pregunta, int minimo, int maximo){
	int valor = 0;
	printf("%s", pregunta);
	int estado = leer_entero(&valor);
	while(estado <= 0 || valor < minimo || valor > maximo){
		if(estado < 0){
			exit(1);
		}
		printf("\033[31m  Valor invalido.\033[0m Tiene que estar en el rango.\n");
		printf("%s", pregunta);
		estado = leer_entero(&valor);
	}
	return valor;
}

/* Funcion que pide los valores de las comunas, bienes y servicios que el usuario pida y los coloca en variables globales
 * Entradas: nada
 * Salidas: ninguna*/
void configuracion_inicial(){
	saludo_inicio();

	printf("\n\n\033[33m  ────────────────────────────────────────\033[0m\n");
	printf("\033[1;33m   Configuracion\033[0m\n");
	printf("\033[33m  ────────────────────────────────────────\033[0m\n\n");
	cantidad_comunas = pedir_cantidad("\033[36m  ›\033[0m Inserte la cantidad de comunas que quiere \033[2m(entre 1 y 50)\033[0m: ", 1, 50);
	printf("\n");
	bienes = pedir_cantidad("\033[36m  ›\033[0m Inserte la cantidad de bienes que quiere \033[2m(entre 1 y 20)\033[0m: ", 1, 20);
	printf("\n");
	servicios = pedir_cantidad("\033[36m  ›\033[0m Inserte la cantidad de servicios que quiere \033[2m(entre 1 y 20)\033[0m: ", 1, 20);
	printf("\n");
	personas = pedir_cantidad("\033[36m  ›\033[0m Inserte la cantidad de personas que quiere \033[2m(entre 1 y 50)\033[0m: ", 1, 50);
}



	 
	 


/*Funcion principal de toda la simulacion*/
int main(){
	configuracion_inicial();
	struct lista_comunas* sociedad = generar_datos(cantidad_comunas, servicios, bienes, personas);

	calcular_indice_sociedad(sociedad);

	printf("\n\033[32m  La sociedad ya esta lista.\033[0m\n");

	int turno = 1;
	while(turno <= 10){
		printf("\n\033[33m  ────────────────────────────────────────\033[0m\n");
		printf("\033[1;33m   ☀  Turno %d\033[0m\n", turno);
		printf("\033[33m  ────────────────────────────────────────\033[0m\n\n");
		paso_dia(sociedad);
		turno++;
	}
	despedida();
}
	
