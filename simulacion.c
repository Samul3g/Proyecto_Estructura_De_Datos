#include "estructuras.h"
#include "manejo_recursos.h"
#include "utils.h"



int emergencia(struct comuna* comuna){
    int tipo = aleatorio(1, 2);
    int i = 0;
    if(tipo == 1){
        struct nodo_lista_recursos* actual = comuna->bienes->inicio;
        int largo = largo_recurso(comuna->bienes);
        if(largo <= 1){
            i = largo;
        }else{
        i = aleatorio(0, largo - 1);}
        while(i > 0){
            actual = actual->siguiente;
            i--;
        }
        if(actual == NULL){
            return 0;
        }

        if (actual -> recurso -> cantidad == 0){
            return 0;
        }
        actual -> recurso -> cantidad = actual -> recurso -> cantidad - aleatorio(1, actual -> recurso -> cantidad);
        if (actual -> recurso -> cantidad < 0){
            actual -> recurso -> cantidad = 0;
        }
        return 1;
    }else{
        struct nodo_lista_recursos* actual = comuna->servicios->inicio;
        int largo = largo_recurso(comuna->servicios);
        if(largo <= 1){
            i = largo;
        }else{
        i = aleatorio(0, largo - 1);}
        while(i > 0){
            actual = actual->siguiente;
            i--;
        }
        if(actual == NULL){
            return 0;
        }

        if (actual -> recurso -> cantidad == 0){
            return 0;
        }
        actual -> recurso -> cantidad = actual -> recurso -> cantidad - aleatorio(1, actual -> recurso -> cantidad);
        if (actual -> recurso -> cantidad < 0){
            actual -> recurso -> cantidad = 0;
        }
        return 1;
    } return 0;
}

int prosperidad(struct comuna* comuna){
    int tipo = aleatorio(1, 2);
    int i = 0;
    if(tipo == 1){
        struct nodo_lista_recursos* actual = comuna->bienes->inicio;
        int largo = largo_recurso(comuna->bienes);
        if(largo <= 1){
            i = largo;
        }else{
        i = aleatorio(0, largo - 1);}
        while(i > 0){
            actual = actual->siguiente;
            i--;
        }
        if(actual == NULL){
            return 0;
        }

        if (actual -> recurso -> cantidad == 0){
            return 0;
        }
        actual -> recurso -> cantidad = actual -> recurso -> cantidad + aleatorio(1, actual -> recurso -> cantidad);
        if (actual -> recurso -> cantidad > actual -> recurso -> maximo){
            actual -> recurso -> cantidad = actual -> recurso -> maximo;
        }
        return 1;
    }else{
        struct nodo_lista_recursos* actual = comuna->servicios->inicio;
        int largo = largo_recurso(comuna->servicios);
        if(largo <= 1){
            i = largo;
        }else{
        i = aleatorio(0, largo - 1);}

        while(i > 0){
            actual = actual->siguiente;
            i--;
        }
        if(actual == NULL){
            return 0;
        }

        if (actual -> recurso -> cantidad == 0){
            return 0;
        }
        actual -> recurso -> cantidad = actual -> recurso -> cantidad + aleatorio(1, actual -> recurso -> cantidad);
        if (actual -> recurso -> cantidad > actual -> recurso -> maximo){
            actual -> recurso -> cantidad = actual -> recurso -> maximo;
        }

        return 1;
    } return 0;
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
