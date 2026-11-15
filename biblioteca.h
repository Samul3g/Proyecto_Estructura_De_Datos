 #ifndef BIBLIOTECA_H
 #define BIBLIOTECA_H

 struct biblioteca{
	char* ruta_corpus;
	struct arbol_avl* libros;
	struct arbol_chaflaneado* busquedas;
	int cantidad_libros;
};

/* Mete al avl un nodo por cada .txt
 * Entradas: la biblioteca
 * Salidas: 1 si leyo el directorio, 0 si no */
int guardar_libros_avl(struct biblioteca* biblioteca)


/* Crea la biblioteca e indexa los libros en el avl
 * Entradas: ruta del directorio con los .txt
 * Salidas: la bilbioteca, NULL si la ruta esta vacia o no se pudo abrir el directorio */
struct biblioteca* crear_bilbioteca(char* ruta_corpus);

/* Libera la biblioteca y todod sus componentes
 * Entradas: la biblioteca
 * Salidas: nada */
void liberar_biblioteca(struct biblioteca* biblioteca);

/* Recorre la lista de comunas y les asigna la biblioteca a cada una
 * Entradas: lista comunas, bilbioteca
 * Salidas: 1 si hubo exito, cero si no */
int asignar_biblioteca_sociedad(struct lista_comunas* sociedad, struct biblioteca* biblioteca);

#endif
 
