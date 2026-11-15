#ifndef WIDGETS_H
#define WIDGETS_H

/* Titulo de una pantalla.
	Entrada: el texto
	Salida: el titulo pintado */
void widget_titulo(const char* texto);

/* Carrusel de una comuna del jugador.
	Entrada: el nombre, la satisfaccion y la necesidad
	Salida: la linea con flechas */
void widget_carrusel(const char* nombre, float satisfaccion, float necesidad);

/* Dos acciones, una marcada.
	Entrada: los rotulos y cual esta elegida (0 o 1)
	Salida: la barra de acciones */
void widget_acciones(const char* izquierda, const char* derecha, int seleccion);

/* Una fila de una lista.
	Entrada: si esta marcada, el texto y un extra a la derecha
	Salida: la fila */
void widget_fila(int marcada, const char* texto, const char* extra);

/* Anillo de comunas. Indice humano, total, nombre e indices.
	Entrada: datos de la comuna elegida y si es del jugador
	Salida: el selector circular */
void widget_anillo(int indice, int total, const char* nombre, float necesidad, float satisfaccion, int propia);

/* Barra de 20 casillas para un indice de 0 a 100.
	Entrada: la etiqueta, el valor y el color ANSI
	Salida: la barra */
void widget_barra(const char* etiqueta, float valor, const char* color);

/* Pie de pantalla.
	Entrada: el texto de ayuda
	Salida: la linea tenue */
void widget_ayuda(const char* texto);

/* Marco de un regalo autonomo.
	Entrada: 1 si el destino es del jugador
	Salida: el borde de arriba */
void widget_marco_abrir(int destacado);

/* Cierra el marco abierto por widget_marco_abrir.
	Entrada: 1 si iba destacado
	Salida: el borde de abajo */
void widget_marco_cerrar(int destacado);

#endif
