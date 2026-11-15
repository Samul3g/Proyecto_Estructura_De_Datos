#ifndef DIALOGOS_H
#define DIALOGOS_H

/* Una frase del oficio. Si el nombre no esta en el archivo, sale el placeholder.
	Entrada: el nombre del servicio
	Salida: la frase. Dura hasta la proxima llamada que arme un texto */
const char* dialogo_de_servicio(const char* servicio);

/* Chiste corto, tomado de las frases de ese oficio.
	Entrada: el servicio de quien habla, o NULL si no tiene
	Salida: la frase */
const char* dialogo_meme(const char* servicio);

/* Queja de un oficio. Si el texto trae %s, entra el recurso.
	Entrada: el servicio de quien habla (o NULL) y el recurso que falta
	Salida: la frase */
const char* dialogo_queja_de(const char* servicio, const char* recurso);

/* Queja usando el placeholder.
	Entrada: el nombre del recurso
	Salida: la frase */
const char* dialogo_queja(const char* recurso);

/* Pide que alguien busque un texto para leer.
	Entrada: el servicio de quien habla
	Salida: la frase */
const char* dialogo_pedido_lectura(const char* servicio);

/* Sugiere regalar un bien que sobra, con las frases de ese oficio.
	Entrada: el servicio de quien habla y el nombre del bien, o NULL
	Salida: la frase */
const char* dialogo_sugerencia(const char* servicio, const char* bien);

/* Comentario suelto al cerrar el dia.
	Entrada: el servicio de quien habla, o NULL
	Salida: la frase */
const char* dialogo_cierre_dia(const char* servicio);

/* Lo que dice alguien al mudarse.
	Entrada: el servicio de nacimiento
	Salida: la frase */
const char* dialogo_despedida(const char* servicio);

/* Dibuja el sprite de esa profesion.
	Entrada: el nombre del servicio, o NULL
	Salida: el dibujo en pantalla */
void dialogo_imprimir_sprite(const char* servicio);

#endif
