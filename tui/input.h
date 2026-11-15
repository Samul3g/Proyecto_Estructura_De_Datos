#ifndef INPUT_H
#define INPUT_H

enum tui_codigo {
	TECLA_NINGUNA = 0,
	TECLA_ARRIBA,
	TECLA_ABAJO,
	TECLA_IZQUIERDA,
	TECLA_DERECHA,
	TECLA_ENTER,
	TECLA_ESC,
	TECLA_TAB,
	TECLA_Q
};

struct tui_tecla {
	int codigo;
	char caracter;
};

/* Lee una tecla. Flechas, hjkl y wasd mueven. Esc suelta sale al rato.
	Entrada: ninguna
	Salida: el codigo y, si era un caracter suelto, ese caracter */
struct tui_tecla tui_leer_tecla(void);

#endif
