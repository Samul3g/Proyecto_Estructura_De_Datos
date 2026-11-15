#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include "estructuras.h"
#include "arbol_chaflaneado.h"
#include "biblioteca.h"
#include "busqueda.h"
#include "utils.h"
 
 
/* Cuenta los caracteres de un string
 * Entrada: el string
 * Salidas: el largo */
static int largo_string(struct string* s){
	
	int largo = 0;
	struct char_array* actual = s->inicio;
	
	while(actual != NULL){
		largo++;
		actual = actual->siguiente;
	}
	
	return largo;
}
 
/* Compara la ventana con el texto buscado
 * Entradas: la ventana, su largo, donde empieza y el texto
 * Salidas: 1 si son iguales, o si no */
static int ventana_coincide(char* ventana, int largo, int inicio, struct string* texto){
	
	struct char_array* actual = texto->inicio;
	int posicion = inicio;
	
	for(int i = 0; i < largo; i++){
		
		if(ventana[posicion] != actual->caracter){
			return 0;
		}
		
		actual = actual->siguiente;
		posicion++;
		
		if(posicion == largo){
			posicion = 0;
		}
	}
	
	return 1;
}
 
/* Lee un archivo y dice si contiene el texto
 * Entradas: ruta del archivo, texto, su largo, una ventana de se largo
 * Salidas: 1 si lo contiene, 0 si no o no lo pudo abrir */
static int archivo_contiene(char* ruta, struct string* texto, int largo, char* ventana){
	
	FILE* archivo = fopen(ruta, "r");
	
	if(archivo == NULL){
		return 0;
	}
	
	
	int leidos = 0;
	int inicio = 0;
	int c = fgetc(archivo);
 
	while (c != EOF) {
 
		if (leidos < largo) {
 
			ventana[leidos] = c;
			leidos++;
 
		} else {
 
			ventana[inicio] = c;
			inicio++;
 
			if (inicio == largo) {
				inicio = 0;
			}
		
		}
 
		if (leidos == largo && ventana_coincide(ventana, largo, inicio, texto)) {
			
			fclose(archivo);
			return 1;
		
		}
 
		c = fgetc(archivo);
	
	}
 
	fclose(archivo);
	return 0;
}
 
 
 
/* Lee cada .txt del directorio y guarda los que contienen el texto
 * Entrada: la ruta del directorio y el texto
 * Salida: la lista de nombres (vacia si ninguno lo contiene), NULL si el texto esta vacio o no se pudo leer el directorio */
struct lista_archivo* buscar_texto_en_archivos(const char* ruta_corpus, struct string* texto) {
 
	if (ruta_corpus == NULL || texto == NULL || texto->inicio == NULL) {
		return NULL;
	}
 
	DIR* directorio = opendir(ruta_corpus);
 
	if (directorio == NULL) {
		return NULL;
	}
 
	int largo = largo_string(texto);
	char* ventana = calloc(1, largo);
 
	if (ventana == NULL) {
		closedir(directorio);
		return NULL;
	}
 
	struct lista_archivo* lista = crear_lista_archivo();
	struct dirent* entrada = readdir(directorio);
	char ruta[4096];
	int indice = 0;
 
	while (entrada != NULL) {
 
		if (es_txt(entrada->d_name)) {
 
			snprintf(ruta, sizeof(ruta), "%s/%s", ruta_corpus, entrada->d_name);
 
			if (archivo_contiene(ruta, texto, largo, ventana)) {
			
				agregar_entrada_lista_archivo(lista, crear_entrada_lista_archivo(crear_string(entrada->d_name), indice));
				
				indice++;
			
			}
		}
 
		entrada = readdir(directorio);
	}
 
	closedir(directorio);
	free(ventana);
 
	return lista;
}
 
 
 
 
 
 
/* Busca un texto, primero en el arbol chaflaneado y si no esta, en los archivos
 * Entrada: la biblioteca y el texto
 * Salida: el resultado,NULL si la biblioteca o el texto son NULL, el texto esta vacio, o no se pudieron leer los archivos */
struct resultado_busqueda* buscar_texto(struct biblioteca* biblioteca, struct string* texto) {
 
	if (biblioteca == NULL || texto == NULL || texto->inicio == NULL) {
		return NULL;
	}
 
	struct resultado_busqueda* resultado = calloc(1, sizeof(struct resultado_busqueda));
 
	if (resultado == NULL) {
		return NULL;
	}
 
	mostrar_pasos_chaflaneado(biblioteca->busquedas, 1);

	printf("  Raiz de la cache antes:   ");
	imprimir_raiz_chaflaneado(biblioteca->busquedas);
 
	int encontrado = 0;
	struct lista_archivo* libros = buscar_chaflaneado(biblioteca->busquedas, texto, &encontrado);
 
	if (encontrado){
 
		resultado->libros = libros;
		resultado->en_cache = 1;
 
	}else{
 
		libros = buscar_texto_en_archivos(biblioteca->ruta_corpus, texto);
 
		if (libros == NULL) {
			free(resultado);
			return NULL;
		}
 
 
		struct string* clave = copiar_string(texto);
 
 
		if (insertar_chaflaneado(biblioteca->busquedas, clave, libros) == 0) {
			
			liberar_string(clave);
			liberar_lista_archivo(libros);
			free(resultado);
			
			return NULL;
		
		}
 
		resultado->libros = libros;
		resultado->en_cache = 0;
	}
 
	printf("  Raiz de la cache despues: ");
	imprimir_raiz_chaflaneado(biblioteca->busquedas);
 
	if (resultado->en_cache) {
		printf("  (salio de la cache, no se abrio ningun archivo)\n");
	
	} else {
		printf("  (no estaba en la cache: se leyeron los archivos)\n");
	
	}
 
	return resultado;
}
 
 
 
/* Libera el struct del resultado
 * Entrada: el resultado
 * Salida: nada */
void liberar_resultado_busqueda(struct resultado_busqueda* resultado) {
 
	if (resultado == NULL) {
		return;
	}
 
	free(resultado);
}
 
 
 
 



























