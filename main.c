#include <stdio.h>
#include <stdlib.h>
#include "main.h"
#include "estructuras.h"
#include "manejo_recursos.h"
#include "generacion_datos.h"
#include "indices.h"
#include "simulacion.h"

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
	printf("\n\n\nBienvenid@ a la simulacion: Una sociedad solarpunk de comunas anarquistas");
	printf("\n\n      Hecho por: Paula Flores Solano y Samuel Ureña Gonzalez");
	printf("\n\n                Estructuras de Datos, IIS 2026");
}

/* Funcion de impresion del logo y despedida
 * Entradas: nada
 * Salidas: nada (impresion en consola)
 */
void despedida(){
	printf("\033[33m                      \\ | /");
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
	printf("\n\nYa se acabo la simulacion, adios!!!")
}


/* Funcion que pide los valores de las comunas, bienes y servicios que el usuario pida y los coloca en variables globales
 * Entradas: nada
 * Salidas: ninguna*/
void configuracion_inicial(){
	saludo_inicio();
	
	int correcto = 0; // ayuda para hacer validaciones
	
	//variables locales
	int cant_bienes = 0;
	int cant_comunas = 0;
	int cant_servicios = 0;
	int cant_personas = 0;
	
	//pedir cantidad comunas
	
	printf("\n\nInserte la cantidad de comunas que quiere (entre 1 y 50): ");
	scanf("%d", &cant_comunas);
	
	while(correcto == 0){
		if(cant_comunas < 1 || cant_comunas > 50){
			printf("\033[31m\nValor invalido.");
			printf("\033[0m Inserte la cantidad de comunas que quiere (entre 1 y 50): ");
			scanf("%d", &cant_comunas);
		}else{
			cantidad_comunas = cant_comunas;
			correcto = 1;
		}
	}
	
	correcto = 0;
	
	//pedir cantidad bienes
	printf("\n\nInserte la cantidad de bienes que quiere (entre 1 y 20): ");
	scanf("%d", &cant_bienes);
	while(correcto == 0){
		if(cant_bienes < 1 || cant_bienes > 20){
			printf("\033[31m\nValor invalido.");
			printf("\033[0m Inserte la cantidad de bienes que quiere (entre 1 y 20): ");
			scanf("%d", &cant_bienes);
		}else{
			bienes = cant_bienes;
			correcto = 1;
		}
	}
		
	correcto = 0;
		
	//pedir cantidad servicios
	printf("\n\nInserte la cantidad de servicios que quiere (entre 1 y 20): ");
	scanf("%d", &cant_servicios);
	while(correcto == 0){
		if(cant_servicios < 1 || cant_servicios > 20){
			printf("\033[31m\nValor invalido.");
			printf("\033[0m Inserte la cantidad de servicios que quiere (entre 1 y 20): ");
			scanf("%d", &cant_servicios);
		}else{
			servicios = cant_servicios;
			correcto = 1;
		}
	}

	correcto = 0;

	//pedir cantidad personas
	printf("\n\nInserte la cantidad de personas que quiere (entre 1 y 500): ");
	scanf("%d", &cant_personas);
	while(correcto == 0){
		if(cant_personas < 1 || cant_personas > 500){
			printf("\033[31m\nValor invalido.");
			printf("\033[0m Inserte la cantidad de personas que quiere (entre 1 y 500): ");
			scanf("%d", &cant_personas);
		}else{
			personas = cant_personas;
			correcto = 1;
		}
	}
}



	 
	 


/*Funcion principal de toda la simulacion*/
int main(){
	configuracion_inicial();
	struct lista_comunas* sociedad = generar_datos(cantidad_comunas, servicios, bienes, personas);

	calcular_indice_sociedad(sociedad);
	
	int turno = 1;
	while(turno <= 10){
		printf("Turno %d\n", turno);
		paso_dia(sociedad);
		turno++;
	}
	despedida();
}
	
