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
- [x] Diseño de funcion acierto().
- [x] Diseño de funcion color_de_ranura().
- [x] Mutar cartera de jugador con base en el resultado de acierto().
- [x] Administrar condicion especial ranura 0.
- [x] Calcular ganancia/perdida banca.
- [x] Display de bienvenida.
- [x] Display de ronda.
- [x] Display de turnos.
  - [x] Etapa 1.
  - [x] Etapa 2.
- [x] Display final de juego.
- [x] Diseñar condiciones de salida.
- [x] Entradas de user input con errores.
  - [x] Entrada de retirarse o seguir.
  - [x] Entrada de elegir ranura.
  - [x] Entrada de elegir apuesta.
- [ ] Tests de user input.
- [x] Flair?

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

# Requerimientos

### Compiladores Compatibles
- GCC 10 .. 10.2.1.

### Standard de C++
- 20

### Build Systems Compatibles
- CMake

### Tests (Necesario para un debug build.)
- CTest (Incluído en CMake)
- [GoogleTest](https://github.com/google/googletest)

# Instrucciones de build con CMake desde Linux
```bash
cd nebrija-trabajo-prog-1
mkdir cmake-build-release
cd cmake-build-release
cmake -DCMAKE_BUILD_TYPE=Release ../
cmake --build . -- -j6
./ruleta
```

# Tests
Crea un debug build del proyecto con
```bash
cd nebrija-trabajo-prog-1
mkdir cmake-build-debug
cd cmake-build-debug
cmake -DCMAKE_BUILD_TYPE=debug ../
cmake --build . -- -j6
cd tests
```

Para mostrar los tests disponibles:
```bash
ctest -N
```

Para ejecutar todos los tests:
```bash
ctest
```

Para ejecutar un test especifico:
```bash
ctest -L NombreDelTest
```
