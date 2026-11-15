#define _DEFAULT_SOURCE

#include "input.h"

#include <unistd.h>
#include <sys/select.h>

static int leer_byte(unsigned char* destino) {
	ssize_t n = read(STDIN_FILENO, destino, 1);
	while (n < 0) {
		n = read(STDIN_FILENO, destino, 1);
	}
	return n == 1;
}

static int hay_byte(void) {
	fd_set conjunto;
	struct timeval espera;
	FD_ZERO(&conjunto);
	FD_SET(STDIN_FILENO, &conjunto);
	espera.tv_sec = 0;
	espera.tv_usec = 40000;
	int listo = select(STDIN_FILENO + 1, &conjunto, NULL, NULL, &espera);
	return listo > 0;
}

static int codigo_flecha(unsigned char tecla) {
	if (tecla == 'A') {
		return TECLA_ARRIBA;
	}
	if (tecla == 'B') {
		return TECLA_ABAJO;
	}
	if (tecla == 'C') {
		return TECLA_DERECHA;
	}
	if (tecla == 'D') {
		return TECLA_IZQUIERDA;
	}
	return TECLA_NINGUNA;
}

/* Lee una tecla. Flechas, hjkl y wasd mueven. Esc suelta sale al rato.
	Entrada: ninguna
	Salida: el codigo y, si era un caracter suelto, ese caracter */
struct tui_tecla tui_leer_tecla(void) {
	struct tui_tecla tecla;
	tecla.codigo = TECLA_NINGUNA;
	tecla.caracter = 0;

	unsigned char c = 0;
	if (!leer_byte(&c)) {
		tecla.codigo = TECLA_ESC;
		return tecla;
	}

	if (c == '\033') {
		if (!hay_byte()) {
			tecla.codigo = TECLA_ESC;
			return tecla;
		}
		unsigned char marca = 0;
		if (!leer_byte(&marca)) {
			tecla.codigo = TECLA_ESC;
			return tecla;
		}
		if (marca == '[' || marca == 'O') {
			unsigned char flecha = 0;
			if (!leer_byte(&flecha)) {
				return tecla;
			}
			/* Secuencias largas, como las flechas modificadas: se descartan. */
			if (flecha >= '0' && flecha <= '9') {
				unsigned char extra = 0;
				while (leer_byte(&extra) && extra != '~' && (extra < 'A' || extra > 'D')) {
				}
				return tecla;
			}
			tecla.codigo = codigo_flecha(flecha);
			return tecla;
		}
		tecla.codigo = TECLA_ESC;
		return tecla;
	}

	tecla.caracter = (char)c;
	if (c == '\n' || c == '\r') {
		tecla.codigo = TECLA_ENTER;
	} else if (c == '\t') {
		tecla.codigo = TECLA_TAB;
	} else if (c == 'q' || c == 'Q') {
		tecla.codigo = TECLA_Q;
	} else if (c == 'k' || c == 'K' || c == 'w' || c == 'W') {
		tecla.codigo = TECLA_ARRIBA;
	} else if (c == 'j' || c == 'J' || c == 's' || c == 'S') {
		tecla.codigo = TECLA_ABAJO;
	} else if (c == 'h' || c == 'H' || c == 'a' || c == 'A') {
		tecla.codigo = TECLA_IZQUIERDA;
	} else if (c == 'l' || c == 'L' || c == 'd' || c == 'D') {
		tecla.codigo = TECLA_DERECHA;
	}
	return tecla;
}
