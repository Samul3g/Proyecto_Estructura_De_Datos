# Pendientes

El orden dentro de cada sección es el orden para implementarlos. Si un punto espera a otro, está dicho en la línea de abajo.

## Samuel

### generacion_datos.c

- [x] Calcular el máximo de cada bien y servicio a partir de cuántas personas viven en la comuna.

### manejo_recursos.h y manejo_recursos.c

Van primero. Sin esto el programa no compila junto y no se puede reducir ni aumentar nada.

- [X] `manejo_recursos.h` declara la comuna como `char*`. `manejo_recursos.c` la usa como `struct comuna*`.
- [X] `buscar_recurso` recibe un `struct string*`. Las funciones de reducir y aumentar le pasan un `char*`.
- [X] `aumento_bien` y `aumento_servicio` devuelven 0 si la comuna no tiene ese recurso. Al recibir un regalo de un bien o servicio que no tiene, hay que agregarlo, con la cantidad regalada. El máximo, la necesidad y la relevancia salen del bien y de la comuna que recibe. De quien da solo importa cuánto da.
  Antes del intercambio del día.


### generacion_datos.c

Antes de que `main` arme la sociedad.

- [X] En `generar_lista_recursos`, una variable local llamada `cantidad` tapa el parámetro del mismo nombre.
- [X] El nombre de la comuna es el mismo `struct string*` de la lista leída del archivo, sin copia. Si esa lista se libera, la comuna queda apuntando a memoria libre.
<Baja Peoridad, ignorar por el momento>
- [ ] Si una línea de `bienes` o `servicios` no trae las comas esperadas, el lector usa un puntero nulo y el programa se cae.


### generacion_datos.c

- [X] `agregar_recursos_comunas` sustituye las listas vacías que creó `crear_comuna` y esa memoria no se libera.
  Hacerlo antes de crear la sociedad, junto con los otros arreglos de este archivo.

### simulacion.c

- [X] Una subida de bienes y servicios del mismo estilo que la emergencia. En la guía se llamó prosperidad. El enunciado pide que los valores suban con los intercambios.
  Después de la emergencia.


### simulacion.c

- [X] `regalo_bien` y `regalo_servicio` siguen recibiendo `char*`. `manejo_recursos` pide `struct string*`, y a `return 1` de `regalo_bien` le falta el punto y coma. `usuario_intercambio` no las usa: aumenta y después reduce por su cuenta.
- [X] `emergencia` y `prosperidad` eligen un nodo si hay más de uno. Con un solo recurso, `i` queda en el largo, el recorrido se sale y no cambia la cantidad. Si la cantidad del nodo elegido ya es 0, `aleatorio(1, cantidad)` divide por cero y el programa se cae.
  Arreglar el archivo antes del día y de la emergencia de Samuel. La prosperidad está marcada hecha arriba, pero esta función es otro borrador.

### manejo_recursos.c

- [x] Si la comuna no tiene el bien o el servicio, `aumento_bien` y `aumento_servicio` lo arman desde el archivo con las personas de quien recibe. Así es: el máximo, la necesidad y la relevancia dependen del bien y de la comuna que recibe. De quien da solo importa cuánto da.


### main.h y main.c

- [x] `main.h` no incluye `estructuras.h`. `imprimir_recurso` y `usuario_intercambio` quedan declaradas con un `struct` distinto al de `main.c`, y el archivo no compila.
- [x] En `main`, la variable se llama `generar_datos`, igual que la función. La llamada no compila.
  Después del encabezado.

### generacion_datos.c

- [x] `generar_datos` no libera las cuatro listas leídas de `comunas`, `bienes`, `servicios` y `personas`. Los nombres de las comunas ya van copiados, así que esas listas se pueden liberar al terminar.
  Después de crear la sociedad.











## Paula

### estructuras.h

- [x] Necesidad y satisfacción están como `float`. El enunciado pide números flotantes.

### main.c

- [x] `main` acepta de 1 a 500 comunas. El archivo `comunas` tiene 50, así que el tope es 50.
  Hacerlo antes de que `main` llame a `generar_datos`.

### simulacion.c

- [x] `regalo_bien` y `regalo_servicio` siempre devuelven éxito, aunque la reducción o el aumento hayan fallado.
  Después de unir las firmas de `manejo_recursos.h` y `manejo_recursos.c`. Antes del intercambio del día.


### main.c

- [x] Crear la sociedad con los números que pidió la persona usuaria. `main` guarda comunas, bienes, servicios y personas, y no llama a `generar_datos`.
  Después del tope de 50 comunas y de los arreglos de `generacion_datos.c`.

### indices.c

En este orden: primero que la fórmula no llegue a 100, después las vecinas cuando hay pocas comunas, y al final guardar los dos números. Al guardarlos se arregla que la satisfacción lea una necesidad que sigue en 0.

- [x] `calcular_necesidad` puede devolver 100. Si todos los recursos activos son necesidad y están vacíos, el promedio vale 1 y la fórmula da `5 + 95 * 1`. El enunciado dice que la necesidad nunca llega al máximo.
- [x] Con menos de 5 comunas el círculo se pisa: la misma vecina entra dos veces en el promedio.
- [x] Guardar los índices. `calcular_necesidad` y `calcular_satisfaccion` devuelven el número y no lo escriben en la comuna.
- [x] La satisfacción lee `comuna->necesidad`. Si nadie guardó ese campo antes, el promedio es 0 y la satisfacción sale 100.



### main.c

- [x] El intercambio durante el día: la persona usuaria elige qué pasa de una comuna a otra, y eso cambia los índices.
  Después del regalo que avisa si falló, de que el aumento agregue un recurso que la comuna no tenía, y de los índices guardados.









### indices.c

- [x] No hay `indices.h`. Aunque el archivo compile, `main` no tiene dónde declarar estas funciones.

- [ ] `calcular_necesidad` usa `resultado` sin declararlo. La fórmula está dentro de `if (denominador == 0)`: cuando el denominador vale 0 divide por cero, y cuando no vale 0 no calcula nada ni escribe la necesidad.
- [ ] `calcular_satisfaccion` no compila. El arreglo guarda nodos, no punteros, y usa `candidatos`, `suma`, `contadas` y `comuna`, que no existen. Dentro del promedio, si una necesidad vale 0 la función retorna 0 y no guarda la satisfacción.
  Después de que la necesidad sí se calcule.

### main.c

El programa no compila junto. Este orden: primero los tres archivos que no compilan, después el día en `main`, y al final lo que el intercambio deja a medias.


- [ ] `main` crea la sociedad y retorna. No llama a `usuario_intercambio`, no recorre turnos y no libera la lista.
  Después de que `main` compile. Antes del día y de la emergencia.

### main.c

- [ ] Nadie llama a `calcular_necesidad` ni a `calcular_satisfaccion`. Al crear la sociedad esos campos quedan en 0. El consumo, la emergencia y `usuario_intercambio` cambian cantidades y no vuelven a escribir los índices. El enunciado pide calcularlos al armar la sociedad, modificarlos cada día y alterarlos con el intercambio.
  Después de que esas funciones sí calculen y guarden. Junto con el recorrido de turnos, el día y la emergencia.

- [ ] `imprimir_lista_comunas` solo imprime el nombre. La persona usuaria no ve necesidad, satisfacción ni inventario al pasar el día ni al elegir el regalo.
  Después de guardar los índices. Junto con el recorrido de turnos.

- [ ] Cuando `main` recorra turnos, no hay tope ni salida: no se pide cuántos días ni hay forma de terminar la simulación.
  Junto con el recorrido de turnos.

### simulacion.c

Después de la sociedad, de los índices guardados y de `manejo_recursos`.

- [ ] El día: cada turno se consumen bienes, con una cantidad distinta en cada comuna.
- [ ] La emergencia: cada ciertos turnos, en un rango aleatorio, baja bienes y servicios al azar y baja la satisfacción de las comunas cercanas. Cercanas son más de dos.
  Después del consumo del día y de los índices guardados.









## Sin asignar




