#include <stdio.h>
#include <stdlib.h>
#include "main.h"
#include "estructuras.h"
#include "manejo_recursos.h"
#include "generacion_datos.h"

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


/*Funcion que imprime una lista de recursos
 * Entradas: lista recursos
 * Salidas: impresion en la consola y 0 en caso de fallo, 1 en caso de exito
 */
 int imprimir_recurso(struct lista_recursos* lista){
	if(lista == NULL){
			return 0;
	}
	
	struct nodo_lista_recursos* nodo = lista->inicio;
	int numero = 1;
	 
    while (nodo != NULL) {
        printf("%d. ", numero);
        struct char_array* actual = nodo->recurso->nombre->inicio;
        while (actual != NULL) {
            putchar(actual->caracter);
            actual = actual->siguiente;
        }
        printf(" - Cantidad disponible: %d\n", nodo->recurso->cantidad);
        nodo = nodo->siguiente;
        numero++;
    }
    return 1;
}
	 
	 
/*Funcion que pide al usuario la comuna que regala y recibe y el servicio o bien a regalar y luego da el regalo
 * Entradas: Lista comunas
 * Salidas: 1 para exito, 0 para fracaso*/
int usuario_intercambio(struct lista_comunas* lista_comunas){
	if(lista_comunas == NULL){
			return 0;
	}
	
	//Pedir numero de las comunas
	int comunaRegalo = 0;
	int comunaRecibidora = 0;
	
	imprimir_lista_comunas(lista_comunas);
	
	int largoLista = largo_comuna(lista_comunas);
	int correcto = 0; //ayuda para vaidaciones
	
	printf("\nElija el numero de la comuna que va a dar el regalo: ");
	scanf("%d", &comunaRegalo);
	
	while(correcto == 0){
		if(comunaRegalo <= 0 || comunaRegalo > largoLista){
			printf("\nNumero invalido, fuera del rango");
			printf("\nElija el numero de la comuna que va a dar el regalo: ");
			scanf("%d", &comunaRegalo);
		}else{
				correcto = 1;
		}
	}
	
	correcto = 0;
	
	printf("\nElija el numero de la comuna que va a recibir el regalo: ");
	scanf("%d", &comunaRecibidora);
	
	while(correcto == 0){
		if(comunaRecibidora <= 0 || comunaRecibidora > largoLista || comunaRecibidora == comunaRegalo){
			printf("\nNumero invalido, fuera del rango o igual a la comuna que da el regalo");
			printf("\nElija el numero de la comuna que va a recibir el regalo: ");
			scanf("%d", &comunaRecibidora);
		}else{
				correcto = 1;
		}
	}
	correcto = 0;
	
	//Asignar comuna y numero (mas o menos)
	struct comuna* comunaDa = NULL;
	struct comuna* comunaRecibe = NULL;
	
	int i = 1;
	struct nodo_lista_comunas* inicio = lista_comunas->inicio;
	struct nodo_lista_comunas* actual = lista_comunas->inicio;
	if(i == comunaRegalo){
			comunaDa = actual->comuna;
	}else if(i == comunaRecibidora){
			comunaRecibe = actual->comuna;
	}
	
	actual = actual->siguiente;
	i++;
	
	while(actual != inicio){
		if(i == comunaRegalo){
			comunaDa = actual->comuna;
		}else if(i == comunaRecibidora){
			comunaRecibe = actual->comuna;
		}
		
		actual = actual->siguiente;
		i++;	
	}
	
	//Pedir si dar bien o servicio
	int regalo = 0;
	printf("\n\nElija el numero de lo que quiere regalar: ");
	printf("\n1. Bien \n2. Servicio\n");
	scanf("%d", &regalo);
	
	while(correcto == 0){
		if(regalo != 1 && regalo !=2){
			printf("\nNumero invalido, fuera del rango");
			printf("\nElija el numero de lo que quiere regalar: ");
			printf("\n1. Bien \n2. Servicio");
			scanf("%d", &regalo);
		}else{
				correcto = 1;
		}
	}
	correcto = 0;
	
	
	//conseguir listas de bienes o servicios
		struct lista_recursos* lista_dador;
		if(regalo == 1){
			lista_dador = comunaDa->bienes;
		}else{
			lista_dador = comunaDa->servicios;
		}
			
	
	//preguntar que servicio regalar
	int n_recurso = 0;
	int largo_recu = largo_recurso(lista_dador);
	
	imprimir_recurso(lista_dador);
	
	printf("ingrese el numero del recurso que desea regalar: ");
	scanf("%d", &n_recurso);
	
	while(correcto == 0){
		if(n_recurso <= 0 || n_recurso > largo_recu){
			printf("\nNumero invalido, fuera del rango");
			printf("ingrese el numero del recurso que desea regalar: ");
			scanf("%d", &n_recurso);
		}else{
				correcto = 1;
		}	
	}
	correcto = 0;
	
	//Asignar nodo recurso
	i = 1;
	struct nodo_lista_recursos* rec_actual = lista_dador->inicio;
	struct nodo_recurso* recurso_dar = NULL;
	
	while(rec_actual != NULL){
			if(i == n_recurso){
					recurso_dar = rec_actual->recurso;
					break;
			}else{
				rec_actual = rec_actual->siguiente;
				i++;
			}
	}
	
	//Preguntar cantidad a regalar del recurso
	int cant_regalar = 0;
	printf("ingrese la cantidad del recurso que desea regalar: ");
	scanf("%d", &cant_regalar);
	
	while(correcto == 0){
		if(cant_regalar <= 0 || cant_regalar > recurso_dar->cantidad){
			printf("\nNumero invalido, fuera del rango");
			printf("ingrese la cantidad del recurso que desea regalar: ");
			scanf("%d", &cant_regalar);
		}else{
				correcto = 1;
		}	
	}
	correcto = 0;
	
	//Efectuar regalo
	int exito = 0;
	
	if (regalo == 1) {
		exito = aumento_bien(recurso_dar->nombre, cant_regalar, comunaRecibe);
		if(exito){
			exito = reduccion_bien(recurso_dar->nombre, cant_regalar, comunaDa);
		}
	} else {
		exito = aumento_servicio(recurso_dar->nombre, cant_regalar, comunaRecibe);
		if(exito){
			exito = reduccion_servicio(recurso_dar->nombre, cant_regalar, comunaDa);
		}
	}

	return exito;
}

/*Funcion principal de toda la simulacion*/
int main(){
	configuracion_inicial();
	struct lista_comunas* generar_datos = generar_datos(cantidad_comunas, servicios, bienes, personas);
	
	
	return 0;
	
	}
	
