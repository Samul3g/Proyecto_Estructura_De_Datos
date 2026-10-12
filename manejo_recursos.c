#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "estructuras.h"
#include "utils.h"
#include "manejo_recursos.h"

/* Funcion que reduce los servicios que posee una comuna
	Entrada: servicio, cantidad, comuna
	Salida: 0 (false/error), 1 (true/exito) */
int reduccion_servicio(struct string* servicio, int cantidad, struct comuna* comuna){
		
		
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


/* Funcion que reduce los bienes que posee una comuna
	Entrada: bien, cantidad, comuna
	Salida: 0 (false/error), 1 (true/exito) */
int reduccion_bien(struct string* bien, int cantidad, struct comuna* comuna){
		
		
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


/* Funcion que aumenta los servicios que posee una comuna
	Entrada: servicio, cantidad, comuna
	Salida: 0 (false/error), 1 (true/exito) */
int aumento_servicio(struct string* servicio, int cantidad, struct comuna* comuna){
		
		
		// validaciones
		
		// valida que la cantidad sea positivo y diferente de 0
		if(cantidad <= 0){
		return 0;	
		}
		
		struct lista_recursos* lista_recursos = comuna->servicios;
		struct nodo_recurso* recurso = buscar_recurso(lista_recursos, servicio);

		if (recurso == NULL) {
			FILE* archivo = leer_archivo("servicios");
			struct lista_archivo* lista_archivo = generar_lista_archivo(archivo);
			fclose(archivo);
		
			int personas = largo_personas(comuna->personas);
			struct entrada_lista_archivo* actual = lista_archivo->inicio;
			while (actual != NULL) {
				struct nodo_recurso* candidato = crear_recurso_desde_linea(actual->nombre, personas);
				if (mismos_nombres(candidato->nombre, servicio)) {
					recurso = candidato;
					agregar_recurso(lista_recursos, recurso);
					break;
				}
				liberar_recurso(candidato);
				actual = actual->siguiente;
			}
			liberar_lista_archivo(lista_archivo);
		
			if (recurso == NULL) {
				return 0;
			}
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



/* Funcion que aumenta los bienes que posee una comuna
	Entrada: bien, cantidad, comuna
	Salida: 0 (false/error), 1 (true/exito) */
int aumento_bien(struct string* bien, int cantidad, struct comuna* comuna){
		
		
		// validaciones
		
		// valida que la cantidad sea positivo y diferente de 0
		if(cantidad <= 0){
		return 0;	
		}
		
		
		struct lista_recursos* lista_recursos = comuna->bienes;
		
		struct nodo_recurso* recurso = buscar_recurso(lista_recursos, bien);

		if (recurso == NULL) {
			FILE* archivo = leer_archivo("bienes");
			struct lista_archivo* lista_archivo = generar_lista_archivo(archivo);
			fclose(archivo);
		
			int personas = largo_personas(comuna->personas);
			struct entrada_lista_archivo* actual = lista_archivo->inicio;
			while (actual != NULL) {
				struct nodo_recurso* candidato = crear_recurso_desde_linea(actual->nombre, personas);
				if (mismos_nombres(candidato->nombre, bien)) {
					recurso = candidato;
					agregar_recurso(lista_recursos, recurso);
					break;
				}
				liberar_recurso(candidato);
				actual = actual->siguiente;
			}
			liberar_lista_archivo(lista_archivo);
		
			if (recurso == NULL) {
				return 0;
			}
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
