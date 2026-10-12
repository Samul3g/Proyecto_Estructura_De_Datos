# Pendientes

El orden dentro de cada sección es el orden para implementarlos. Si un punto espera a otro, está dicho en la línea de abajo.

## Samuel

### generacion_datos.c

- [x] Calcular el máximo de cada bien y servicio a partir de cuántas personas viven en la comuna.

### manejo_recursos.h y manejo_recursos.c

Van primero. Sin esto el programa no compila junto y no se puede reducir ni aumentar nada.

- [X] `manejo_recursos.h` declara la comuna como `char*`. `manejo_recursos.c` la usa como `struct comuna*`.
- [X] `buscar_recurso` recibe un `struct string*`. Las funciones de reducir y aumentar le pasan un `char*`.
- [X] `aumento_bien` y `aumento_servicio` devuelven 0 si la comuna no tiene ese recurso. Al recibir un regalo de un bien o servicio que no tiene, hay que agregarlo, con la cantidad regalada. El máximo, la necesidad y la relevancia salen del recurso de la comuna que lo da.
  Antes del intercambio del día.


### generacion_datos.c

Antes de que `main` arme la sociedad.

- [X] En `generar_lista_recursos`, una variable local llamada `cantidad` tapa el parámetro del mismo nombre.
- [X] El nombre de la comuna es el mismo `struct string*` de la lista leída del archivo, sin copia. Si esa lista se libera, la comuna queda apuntando a memoria libre.
<Baja Peoridad, ignorar por el momento>
- [ ] Si una línea de `bienes` o `servicios` no trae las comas esperadas, el lector usa un puntero nulo y el programa se cae.










## Paula

### estructuras.h

- [x] Necesidad y satisfacción están como `float`. El enunciado pide números flotantes.

### main.c

- [ ] `main` acepta de 1 a 500 comunas. El archivo `comunas` tiene 50, así que el tope es 50.
  Hacerlo antes de que `main` llame a `generar_datos`.

### generacion_datos.c

- [ ] `agregar_recursos_comunas` sustituye las listas vacías que creó `crear_comuna` y esa memoria no se libera.
  Hacerlo antes de crear la sociedad, junto con los otros arreglos de este archivo.

### simulacion.c

- [ ] `regalo_bien` y `regalo_servicio` siempre devuelven éxito, aunque la reducción o el aumento hayan fallado.
  Después de unir las firmas de `manejo_recursos.h` y `manejo_recursos.c`. Antes del intercambio del día.











## Sin asignar



### main.c

- [ ] Crear la sociedad con los números que pidió la persona usuaria. `main` guarda comunas, bienes, servicios y personas, y no llama a `generar_datos`.
  Después del tope de 50 comunas y de los arreglos de `generacion_datos.c`.

### indices.c

En este orden: primero que la fórmula no llegue a 100, después las vecinas cuando hay pocas comunas, y al final guardar los dos números. Al guardarlos se arregla que la satisfacción lea una necesidad que sigue en 0.

- [ ] `calcular_necesidad` puede devolver 100. Si todos los recursos activos son necesidad y están vacíos, el promedio vale 1 y la fórmula da `5 + 95 * 1`. El enunciado dice que la necesidad nunca llega al máximo.
- [ ] Con menos de 5 comunas el círculo se pisa: la misma vecina entra dos veces en el promedio.
- [ ] Guardar los índices. `calcular_necesidad` y `calcular_satisfaccion` devuelven el número y no lo escriben en la comuna.
- [ ] La satisfacción lee `comuna->necesidad`. Si nadie guardó ese campo antes, el promedio es 0 y la satisfacción sale 100.

### simulacion.c

Después de la sociedad, de los índices guardados y de `manejo_recursos`.

- [ ] El día: cada turno se consumen bienes, con una cantidad distinta en cada comuna.
- [ ] La emergencia: cada ciertos turnos, en un rango aleatorio, baja bienes y servicios al azar y baja la satisfacción de las comunas cercanas. Cercanas son más de dos.
  Después del consumo del día y de los índices guardados.

### main.c

- [ ] El intercambio durante el día: la persona usuaria elige qué pasa de una comuna a otra, y eso cambia los índices.
  Después del regalo que avisa si falló, de que el aumento agregue un recurso que la comuna no tenía, y de los índices guardados.

### simulacion.c

- [ ] Una subida de bienes y servicios del mismo estilo que la emergencia. En la guía se llamó prosperidad. El enunciado pide que los valores suban con los intercambios.
  Después de la emergencia.
