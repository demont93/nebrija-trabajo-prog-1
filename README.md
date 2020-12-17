# nebrija-trabajo-prog-1

Trabajo final en grupo para Programacion I.

> **Nota: Para propósito del curso, todo estará implementado y definido dentro del main.cpp.**

## Integrantes

- Derek Montesdeoca
- Carlos Niño

## Elección de Programa

- Trabajo IV

## Roadmap

- [x] Crear representación de la data.
- [ ] Diseño de funcion acierto().
- [ ] Diseño de funcion color_de_ranura().
- [ ] Mutar cartera de jugador con base en el resultado de acierto().
- [ ] Display de bienvenida.
- [ ] Display de turnos.
  - [ ] Etapa 1.
  - [ ] Etapa 2.
  - [ ] Etapa 3.
  - [ ] Etapa 4.
- [ ] Display final de juego.
- [x] Diseñar condiciones de salida.
- [ ] Entradas de user input con errores.
  - [ ] Entrada de retirarse o seguir.
  - [ ] Entrada de elegir ranura.
- [ ] Tests de user input.
- [ ] Flair?

# Ruleta

## Reglas

La ruleta es un juego de azar jugado con una rueda que tiene 37 ranuras
numeradas de 0 a 36. Las ranuras alrededor de la carrera son alternativamente
rojas y negras con excepción de la rueda número 0, que es de color verde. El
diseño permite a los jugadores de realizar sus apuestas sobre el número o color
deseado. Se debe desarrollar un juego de ruleta con 4 jugadores. Inicialmente
cada jugador dispone de 10 Euros. Puede apostar números enteros de Euros. En
cada turno sucederá lo siguiente:

1. Cada jugador decide cuánto dinero quiere apostar en ese turno (o puede
   retirarse).
2. Cada jugador realiza una (y solo una) de las siguientes apuestas:
    - Rojo o Negro
    - Numero de 1 a 36
3. Después de que los 4 jugadores hayan hecho sus apuestas se lanza la ruleta.
4. Si sale el 0, todo el dinero va a la banca.
5. Si acierta el color, se duplica el dinero apostado (los pares son rojos, los
   impares negros).
6. Si acierta el número, se multiplica por 35 el dinero apostado.

## Finalización del Juego

El juego finalizará cuando todos los jugadores se hayan retirado o no les quede
dinero. Al finalizar el juego se debe mostrar cuánto dinero ha ganado cada
jugador y cuánto dinero ha ganado la banca.
> **Nota: El programa debe comprobar en todo momento que los datos introducidos por el usuario y las apuestas a realizar son coherentes, por ejemplo, no se puede apostar por el número 65.**

- Compilado ejecutado y probado en GCC 10.2.1.
- CXX_STANDARD=20
