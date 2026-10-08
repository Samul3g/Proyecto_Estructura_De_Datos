#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include "estructuras.h"


/*Los índices de necesidad y satisfacción como flotantes, con una fórmula propia. 
 * La necesidad es cuánto falta para el máximo y nunca llega a 1. 
 * La satisfacción mira el nivel de vida de la propia comuna y de las comunas de alrededor. indices.c está vacío.
 * */
 
 
/*Funcion que calcula el indice de necesidad de una comuna.
 *Usa la siguiente formula: necesidad = base + (100 − base) × Σ (necesidad * importancia * carencia) / Σ importancia
 *base = numero minimo al que llega el indice
 *Necesidad puede ser 1 o 0, dependiendo si es true o false, el recurso solo se toma en cuenta en el 
 *numerador si es true. En el denominador se toman en cuenta todos asi sean una necesidad o no.
 *Esto permite que los recursos con mayor importancia y que sean necesarios aporten mas peso
 *al indice que los que no.
 *Entradas: struct comuna
 *Salidas: indice de necesidad (float)
 */	
 
// Variables globales/privadas (se utilizan solo en este archivo)
float numerador;
float denominador;


/*Funcion que calcula que tan "vacio" esta el
 * recurso en la comuna
 * Entrada: nodo lista recurso
 * Salida: float entre 1 y 0 
 * Significado: 0 si no hay nada, 1 si esta lleno
 */
float carencia(struct nodo_lista_recursos* nodo){
	struct nodo_recurso* recurso = nodo->recurso;
	int maximo = recurso->maximo;
	int existencia_actual = recurso->cantidad;
	
	if(maximo == 0){
			return 0;
	}
	
	float indice_carencia = (float)(maximo - existencia_actual) / maximo;
	
	if(indice_carencia < 0.0){
		indice_carencia = 0.0;
	}else if(indice_carencia > 1.0){
			indice_carencia = 1.0;
	}
	return indice_carencia;
	
}


/*Funcion que va calculando las sumatorias del numerador y denomunador
 * de la formula que calcula el indice.
 * Entradas: lista, numerador y denominador
 * Salidas: ninguna
 */
void sumatoria_recursos(struct lista_recursos* lista){
	
	if(lista == NULL){
			return -1;
	}
	
	struct nodo_lista_recursos* actual = lista->inicio;
	while(actual != NULL){
		struct nodo_recurso* recurso = actual->recurso;
		
		denominador += recurso->relevancia;
		
		if(recurso->necesidad == true){
				numerador += recurso->relevancia * carencia(actual);
		}
		
		actual = actual->siguiente;
		
	}
	
}


float calcular_necesidad(struct comuna* comuna){
	
	numerador = 0;
	denominador = 0;
	float base = 5.0;
	
	sumatoria_recursos(comuna->bienes);
	sumatoria_recursos(comuna->servicios);
		
	 if (denominador == 0) {
        return base;         // sin recursos o todos con importancia 0
    }


    float promedio = numerador / denominador;   // entre 0 y 1
    return base + (100 - base) * promedio;
    
}



	

	
	
	
	
	
