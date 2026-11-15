#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include "estructuras.h"
#include "indices.h"


/*Los índices de necesidad y satisfacción como flotantes, con una fórmula propia. 
 * La necesidad es cuánto falta para el máximo y nunca llega a 1. 
 * La satisfacción mira el nivel de vida de la propia comuna y de las comunas de alrededor.
 * */
 

// Variables globales/privadas (se utilizan solo en este archivo)
float numerador;
float denominador;


/*Funcion que calcula que tan "vacio" esta el
 * recurso en la comuna
 * Entrada: nodo lista recurso
 * Salida: float entre 1 y 0 
 * Significado del resultado: 1 si no hay nada, 0 si esta lleno
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
 * Salidas: 0 (fallo), 1 (exito)
 */
float sumatoria_recursos(struct lista_recursos* lista){
	
	if(lista == NULL){
			return 0;
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
	return 1;
	
}

 
/*Funcion que calcula el indice de necesidad de una comuna.
 *Usa la siguiente formula: necesidad = base + (95 − base) × Σ (necesidad * importancia * carencia) / Σ importancia
 *base = numero minimo al que llega el indice
 *Necesidad puede ser 1 o 0, dependiendo si es true o false, el recurso solo se toma en cuenta en el 
 *numerador si es true. En el denominador se toman en cuenta todos asi sean una necesidad o no.
 *Esto permite que los recursos con mayor importancia y que sean necesarios aporten mas peso
 *al indice que los que no.
 *Entradas: struct comuna
 *Salidas: indice de necesidad (float)
 */	
float calcular_necesidad_local(struct comuna* comuna){
	if(comuna == NULL){
		return 0;
	}
	
	numerador = 0;
	denominador = 0;
	float resultado = 0;
	float base = 5.0;
	
	sumatoria_recursos(comuna->bienes);
	
	struct lista_personas* personas = comuna->personas;
	struct nodo_lista_personas* actual = personas->inicio;
	struct nodo_lista_personas* ultimo = personas->ultimo;
	
	
	while(actual != ultimo){
		sumatoria_recursos(actual->persona->servicios);
		actual =  actual->siguiente;
	}
	
	sumatoria_recursos(ultimo->persona->servicios);
	
	
	
	if(denominador == 0){
			return 0;
	}
		
		
	float promedio = numerador / denominador;   // entre 0 y 1
	resultado = base + (95.0 - base) * promedio;


    comuna->necesidad_local = resultado;
	return resultado;
    
}

/* Funcion que calcula el indice de necesidad de una comuna basado en demas indices
 * propios y de las dos comuna siguientes y anteriores
 * Entradas: nodo_lista_comunas
 * Salidas: indice de necesidad */
float calcular_necesidad(struct nodo_lista_comunas* nodo){
	
	if(nodo == NULL){
		return 0;
	}
	
	struct nodo_lista_comunas* comunas_tomar_cuenta[5] = {
		nodo,

		nodo->anterior,
		nodo->anterior->anterior,
		
		nodo->siguiente,
		nodo->siguiente->siguiente
	};
	
	float suma_necesidad_vecinas = 0;
	float suma_insatisfaccion_vecinas = 0;
	int contadas = 0;
	
	for(int i = 1; i < 5; i++){
		
		int repetida = 0;
		
		for(int j = 0; j < i; j++){
			if(comunas_tomar_cuenta[j] == comunas_tomar_cuenta[i]){
				repetida = 1;
				break;
			}
		}

		if(!repetida){
			struct comuna* vecina = comunas_tomar_cuenta[i]->comuna;
			suma_necesidad_vecinas += vecina->necesidad_local;
			suma_insatisfaccion_vecinas += 100.0 - vecina->satisfaccion;
			contadas++;
		}
	}
	
		
	float promedio_necesidad_vecinas = suma_necesidad_vecinas / contadas;
	float promedio_insatisfaccion_vecinas = suma_insatisfaccion_vecinas / contadas;
	
	float diferencia = comuna->reciprocidad - 0.5;
	
	if(diferencia < 0){
		diferencia = -diferencia;
	}
	
	float desbalance = diferencia * 2.0f;
		
	int dados = comuna->regalos_dados;
	int recibidos = comuna->regalos_recibidos;
	float proporcion_recibidos = (float) recibidos / (dados + recibidos + 1);

	float resultado =
		  0.40 * comuna->necesidad_local
		+ 0.20 * promedio_necesidad_vecinas
		+ 0.15 * promedio_insatisfaccion_vecinas
		+ 0.15 * desbalance * 100.0
		+ 0.10 * (1.0 - proporcion_recibidos) * 100.0;


	if(resultado < 0.0){
		resultado = 0.0;
	
	}else if(resultado > 100.0){
		resultado = 100.0;
	
	}
	
	
    comuna->necesidad = resultado;
	return resultado;
    
}


/*Funcion que calcula la satisfaccion de una comuna
 *Entradas: struct nodo de la lista de comunas
 *Salidas: indice de satisfaccion (100 - promedio de necesidad_local)
 */
float calcular_satisfaccion(struct nodo_lista_comunas* nodo_lista_comunas){
	
	if(nodo_lista_comunas == NULL){
		return 0;
	}


	struct nodo_lista_comunas* comunas_tomar_cuenta[5] = {
		nodo_lista_comunas,

		nodo_lista_comunas->anterior,
		nodo_lista_comunas->anterior->anterior,
		
		nodo_lista_comunas->siguiente,
		nodo_lista_comunas->siguiente->siguiente
	};
	
	/*Ciclos for que comparan verifican que no se repitan comunas (porque es una lista circular pequena), 
	 *y si se repiten, ignora las veces repetidas*/
	float suma = 0;
	int contadas = 0;
	
	for (int i = 0; i < 5; i++) {
		int repetida = 0;
		for (int j = 0; j < i; j++) {
			if (comunas_tomar_cuenta[j] == comunas_tomar_cuenta[i]) {
				repetida = 1;
				break;
			}
		}
		
		if (!repetida) {
			suma += comunas_tomar_cuenta[i]->comuna->necesidad_local;
			contadas++;
		}
	}
	
	if(contadas == 0){
			return 0;
	}
	
	float promedio = suma / contadas;
	float resultado = 100.0 - promedio;

	nodo_lista_comunas->comuna->satisfaccion = resultado;
	return resultado;
	}
	
	
/* Funcion que  calcula el indice de reciprocidad
 * Entrada: comuna
 * Salida: 0 si comuna es NULL, o,5 o rango entre 0 o 1 */
float calcular_indice_reciprocidad(struct comuna* comuna){
		
		if(comuna == NULL){
			return 0;
		}
		
		int dados = comuna->regalos_dados;
		int recibidos = comuna->regalos_recibidos;
		
		if(dados + recibidos == 0){
				comuna->reciprocidad = 0.5;
				return 0.5;
		}
		
		float balance = (float) recibidos / (dados+recibidos);
		comuna->reciprocidad = balance;
		return balance;
		

}


/*  Funcion que calcula el indice de la sociedad
	Entrada: sociedad
	Salida: 1 (true/exito), 0 si la lista es NULL
*/
int calcular_indice_sociedad(struct lista_comunas* sociedad){
	
	if(sociedad == NULL){
			return 0;
	}
	
	
    struct nodo_lista_comunas* actual = sociedad->inicio;
    struct nodo_lista_comunas* inicio = sociedad->inicio;
	
	//Calcular necesidades locales
    calcular_necesidad_local(actual->comuna);
    actual = actual->siguiente;
    
    while(actual != inicio){
        calcular_necesidad_local(actual->comuna);
        actual = actual->siguiente;
    }
    
    //Calcular reciprocidad en todas
    calcular_indice_reciprocidad(actual->comuna);
    actual = actual->siguiente;
    
    while(actual != inicio){
        calcular_indice_reciprocidad(actual->comuna);
        actual = actual->siguiente;
    }
    
    //Calcular satisfaccion en todas
    calcular_satisfaccion(actual);
    actual = actual->siguiente;
    
    while(actual != inicio){
        calcular_satisfaccion(actual);
        actual = actual->siguiente;
    }
    
    //Calcular necesidad en todas
    calcular_necesidad(actual);
	actual = actual->siguiente;
	
	while(actual != inicio){
		calcular_necesidad(actual);
		actual = actual->siguiente;
	}
    
    return 1;
}
