#include "estructuras.h"
#include "manejo_recursos.h"
#include "indices.h"
#include "utils.h"
#include "simulacion.h"


/* Funcion que devuelve una comuna aleatoria de la lista de comunas
	Entrada: sociedad
	Salida: comuna
*/
struct comuna* comuna_random(struct lista_comunas* sociedad){
    int largo = largo_comuna(sociedad);
    int comuna = aleatorio(1, largo);
    struct nodo_lista_comunas* actual = sociedad->inicio;
    while(comuna > 1){
        actual = actual->siguiente;
        comuna--;
    }
    return actual->comuna;
}

/* Funcion que realiza un paso de dia en la simulacion
	Entrada: sociedad
	Salida: 1 (true/exito)
*/
int paso_dia(struct lista_comunas* sociedad){
    int valido = 0;
    valido = usuario_intercambio(sociedad);
    if(valido == 0){
        printf("\033[31m  No se pudo realizar el intercambio.\033[0m\n");
    }
    valido = reduccion_bienes_servicios(sociedad);
    if(valido == 0){
        printf("\033[31m  No se pudo reducir los bienes y servicios.\033[0m\n");
    }
    int evento = aleatorio(1, 10);
    if(evento < 3){
        struct comuna* afectada = comuna_random(sociedad);
        printf("\n\033[31m  Evento: Emergencia en \033[1m");
        imprimirString(afectada->nombre);
        printf("\033[0m\n");
        valido = emergencia(afectada);
        if(valido == 0){
            printf("\033[31m  No se pudo realizar el evento.\033[0m\n");
        }
    }else if(evento < 8){
        struct comuna* afectada = comuna_random(sociedad);
        printf("\n\033[32m  Evento: Prosperidad en \033[1m");
        imprimirString(afectada->nombre);
        printf("\033[0m\n");
        valido = prosperidad(afectada);
        if(valido == 0){
            printf("\033[31m  No se pudo realizar el evento.\033[0m\n");
        }
    }else{
        printf("\n\033[2m  Evento: Ninguno\033[0m\n");
    }

    valido = calcular_indice_sociedad(sociedad);
    if(valido == 0){
        printf("\033[31m  No se pudo calcular el indice de la sociedad.\033[0m\n");
    }

    return 1;
}

/* Funcion que calcula el indice de la sociedad
	Entrada: sociedad
	Salida: 1 (true/exito)
*/
int calcular_indice_sociedad(struct lista_comunas* sociedad){
    struct nodo_lista_comunas* actual = sociedad->inicio;
    struct nodo_lista_comunas* inicio = sociedad->inicio;

    calcular_necesidad(actual->comuna);
    calcular_satisfaccion(actual);
    actual = actual->siguiente;

    while(actual != inicio){
        calcular_necesidad(actual->comuna);
        calcular_satisfaccion(actual);
        actual = actual->siguiente;
    }
    return 1;
}

/* Funcion que reduce la cantidad de bienes y servicios en una comuna
	Entrada: sociedad
	Salida: 1 (true/exito)
*/
int reduccion_bienes_servicios(struct lista_comunas* sociedad){
    struct nodo_lista_comunas* inicio = sociedad->inicio;
    struct nodo_lista_comunas* actual = inicio;

    struct nodo_lista_recursos* bienes = actual->comuna->bienes->inicio;
    struct nodo_lista_recursos* servicios = actual->comuna->servicios->inicio;

    int largo_bienes = largo_recurso(actual->comuna->bienes);
    int largo_servicios = largo_recurso(actual->comuna->servicios);
    int i = 0;
    while(i < largo_bienes){
        bienes->recurso->cantidad = bienes->recurso->cantidad  * 0.9;
        bienes = bienes->siguiente;
        i++;
    }

    i = 0;
    while(i < largo_servicios){
        servicios->recurso->cantidad = servicios->recurso->cantidad  * 0.9;
        servicios = servicios->siguiente;
        i++;
    }
    actual = actual->siguiente;

    while(actual != inicio){
        bienes = actual->comuna->bienes->inicio;
        servicios = actual->comuna->servicios->inicio;
        largo_bienes = largo_recurso(actual->comuna->bienes);
        largo_servicios = largo_recurso(actual->comuna->servicios);
        i = 0;
        while(i < largo_bienes){
            bienes->recurso->cantidad = bienes->recurso->cantidad  * 0.9;
            bienes = bienes->siguiente;
            i++;
        }
        i = 0;
        while(i < largo_servicios){
            servicios->recurso->cantidad = servicios->recurso->cantidad  * 0.9;
            servicios = servicios->siguiente;
            i++;
        }
        actual = actual->siguiente;
    }
    return 1;
}

/* Elige un bien o un servicio al azar de la comuna
	Entrada: comuna
	Salida: el nodo elegido, o NULL si esa lista esta vacia
*/
struct nodo_lista_recursos* recurso_aleatorio(struct comuna* comuna){
	int tipo = aleatorio(1, 2);
	struct lista_recursos* lista;
	if(tipo == 1){
		lista = comuna->bienes;
	}else{
		lista = comuna->servicios;
	}

	int largo = largo_recurso(lista);
	if(largo == 0){
		return NULL;
	}

	int i = aleatorio(0, largo - 1);
	struct nodo_lista_recursos* actual = lista->inicio;
	while(i > 0){
		actual = actual->siguiente;
		i--;
	}
	return actual;
}

/* Funcion que realiza la emergencia de un bien o un servicio en una comuna
	Entrada: comuna
	Salida: 1 (true/exito)
*/
int emergencia(struct comuna* comuna){
	struct nodo_lista_recursos* actual = recurso_aleatorio(comuna);
	if(actual == NULL || actual->recurso->cantidad == 0){
		return 0;
	}

	int baja = aleatorio(1, actual->recurso->cantidad);
	actual->recurso->cantidad = actual->recurso->cantidad - baja;
	return 1;
}

/* Funcion que realiza la prosperidad de un bien o un servicio en una comuna
	Entrada: comuna
	Salida: 1 (true/exito)
*/
int prosperidad(struct comuna* comuna){
	struct nodo_lista_recursos* actual = recurso_aleatorio(comuna);
	if(actual == NULL){
		return 0;
	}

    if(actual->recurso->cantidad == 0){
        actual->recurso->cantidad = 1;
        return 1;
    }
	int sube = aleatorio(1, actual->recurso->cantidad);
	actual->recurso->cantidad = actual->recurso->cantidad + sube;
	if(actual->recurso->cantidad > actual->recurso->maximo){
		actual->recurso->cantidad = actual->recurso->maximo;
	}
	return 1;
}


/* Pide un entero dentro del rango. Si escriben texto, lo descarta y vuelve a preguntar.
	Entrada: la pregunta, el minimo y el maximo
	Salida: el entero, o -1 si se acabo la entrada */
int pedir_numero(const char* pregunta, int minimo, int maximo){
	int valor = 0;
	printf("%s", pregunta);
	int estado = leer_entero(&valor);
	while(estado <= 0 || valor < minimo || valor > maximo){
		if(estado < 0){
			return -1;
		}
		printf("\033[31m  Numero invalido, fuera del rango.\033[0m\n");
		printf("%s", pregunta);
		estado = leer_entero(&valor);
	}
	return valor;
}

/* Recorre la lista y dice si algun recurso tiene cantidad mayor que 0
	Entrada: la lista de recursos
	Salida: 1 si hay algo para regalar, 0 si no */
int hay_cantidad_disponible(struct lista_recursos* lista){
	if(lista == NULL){
		return 0;
	}
	struct nodo_lista_recursos* actual = lista->inicio;
	while(actual != NULL){
		if(actual->recurso->cantidad > 0){
			return 1;
		}
		actual = actual->siguiente;
	}
	return 0;
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
	if(largoLista < 2){
		printf("\033[33m  Hay una sola comuna, no hay otra a quien regalar.\033[0m\n");
		return 1;
	}

	printf("\n\033[33m  Regalo\033[0m\n");
	comunaRegalo = pedir_numero("\033[36m  ›\033[0m Elija el numero de la comuna que va a dar el regalo: ", 1, largoLista);
	if(comunaRegalo < 0){
		return 0;
	}
	
	printf("\n\033[36m  ›\033[0m Elija el numero de la comuna que va a recibir el regalo: ");
	int estado = leer_entero(&comunaRecibidora);
	
	while(estado <= 0 || comunaRecibidora <= 0 || comunaRecibidora > largoLista || comunaRecibidora == comunaRegalo){
		if(estado < 0){
			return 0;
		}
		printf("\033[31m  Numero invalido, fuera del rango o igual a la comuna que da el regalo.\033[0m\n");
		printf("\033[36m  ›\033[0m Elija el numero de la comuna que va a recibir el regalo: ");
		estado = leer_entero(&comunaRecibidora);
	}
	
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
	int regalo = pedir_numero("\n\033[36m  ›\033[0m Elija el numero de lo que quiere regalar:\n    \033[1;33m1\033[0m  Bien\n    \033[1;33m2\033[0m  Servicio\n\033[36m  ›\033[0m ", 1, 2);
	if(regalo < 0){
		return 0;
	}
	
	
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

	if(hay_cantidad_disponible(lista_dador) == 0){
		printf("\033[33m  No hay cantidad disponible para regalar.\033[0m\n");
		return 1;
	}
	
	struct nodo_recurso* recurso_dar = NULL;
	while(recurso_dar == NULL){
		n_recurso = pedir_numero("\033[36m  ›\033[0m Ingrese el numero del recurso que desea regalar: ", 1, largo_recu);
		if(n_recurso < 0){
			return 0;
		}

		i = 1;
		struct nodo_lista_recursos* rec_actual = lista_dador->inicio;
		while(rec_actual != NULL){
			if(i == n_recurso){
				recurso_dar = rec_actual->recurso;
				break;
			}
			rec_actual = rec_actual->siguiente;
			i++;
		}

		if(recurso_dar != NULL && recurso_dar->cantidad <= 0){
			printf("\033[31m  Ese recurso esta en 0. Elija otro.\033[0m\n");
			recurso_dar = NULL;
		}
	}
	
	//Preguntar cantidad a regalar del recurso
	int cant_regalar = pedir_numero("\033[36m  ›\033[0m Ingrese la cantidad del recurso que desea regalar: ", 1, recurso_dar->cantidad);
	if(cant_regalar < 0){
		return 0;
	}
	
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

	if(exito){
		printf("\n\033[32m  El regalo quedo hecho.\033[0m\n");
	}
	return exito;
}
