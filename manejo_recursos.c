#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "manejo_recursos.h"

/*Funcion que reduce los servicios que posee una comuna
	* Entradas: servicio, cantidad, comuna
	* salidas: 0 (false/error), 1 (true/exito) 
*/
int reduccion_servicio(char* servicio, int cantidad, struct comuna* comuna){
		
		
		// validaciones
		
		// valida que la cantidad sea positivo y diferente de 0
		if(cantidad <= 0){
		return 0;	
		}
		
		
		struct lista_recursos* lista_recursos = comuna->servicios;
		struct nodo_recurso* recurso = buscar_recurso(lista_recursos, servicio);
		if(recurso == NULL){ // valida que exista el servicio
		return 0;		
		}
		
		
		// caso 1: si cantidad es mayor o igual a la cantidad actual
		if(cantidad >= recurso->cantidad){
				recurso->cantidad = 0;
				
		}else{  // caso 2: si cantidad es menor a la cantidad actual
				recurso->cantidad -= cantidad;
		}
		
		return 1;
}


/*Funcion que reduce los bienes que posee una comuna
	* Entradas: bien, cantidad, comuna
	* salidas: 0 (false/error), 1 (true/exito) 
*/
int reduccion_bien(char* bien, int cantidad, struct comuna* comuna){
		
		
		// validaciones
		
		// valida que la cantidad sea positivo y diferente de 0
		if(cantidad <= 0){
		return 0;	
		}
		
			
		struct lista_recursos* lista_recursos = comuna->bienes;
		struct nodo_recurso* recurso = buscar_recurso(lista_recursos, bien);
		if(recurso == NULL){ // valida que exista el bien
		return 0;		
		}
		
		
		// caso 1: si cantidad es mayor o igual a la cantidad actual
		if(cantidad >= recurso->cantidad){
				recurso->cantidad = 0;
				
		}else{  // caso 2: si cantidad es menor a la cantidad actual
				recurso->cantidad -= cantidad;
		}
		
		return 1;
}


/*Funcion que aumenta los servicios que posee una comuna
	* Entradas: servicio, cantidad, comuna
	* salidas: 0 (false/error), 1 (true/exito) 
*/
int aumento_servicio(char* servicio, int cantidad, struct comuna* comuna){
		
		
		// validaciones
		
		// valida que la cantidad sea positivo y diferente de 0
		if(cantidad <= 0){
		return 0;	
		}
		
		struct lista_recursos* lista_recursos = comuna->servicios;
		struct nodo_recurso* recurso = buscar_recurso(lista_recursos, servicio);
		if(recurso == NULL){ // valida que exista el servicio
		return 0;		
		}
		
		
		int aumentado = recurso->cantidad + cantidad;
		
		// caso 1: si aumentado exede o es igual al maximo del bien
		if(aumentado >= recurso->maximo ){
				recurso->cantidad = recurso->maximo;
				
		}else{  // caso 2: si aumentado es menor al maximo del bien
				recurso->cantidad = aumentado;
		}
		
		return 1;
}



/*Funcion que aumenta los bienes que posee una comuna
	* Entradas: bien, cantidad, comuna
	* salidas: 0 (false/error), 1 (true/exito) 
*/
int aumento_bien(char* bien, int cantidad, struct comuna* comuna){
		
		
		// validaciones
		
		// valida que la cantidad sea positivo y diferente de 0
		if(cantidad <= 0){
		return 0;	
		}
		
		
		struct lista_recursos* lista_recursos = comuna->bienes;
		
		struct nodo_recurso* recurso = buscar_recurso(lista_recursos, bien);
		if(recurso == NULL){ // valida que exista el bien
		return 0;		
		}
		
		int aumentado = recurso->cantidad + cantidad;
		
		// caso 1: si aumentado exede o es igual al maximo del bien
		if(aumentado >= recurso->maximo ){
				recurso->cantidad = recurso->maximo;
				
		}else{  // caso 2: si aumentado es menor al maximo del bien
				recurso->cantidad = aumentado;
		}
		
		return 1;
}
