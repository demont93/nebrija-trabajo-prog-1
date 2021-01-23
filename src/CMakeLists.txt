add_executable(ruleta
               main.cpp
               Animacion.h
               Screen.h
               Animacion.cpp Jugador.h Jugadores.cpp Jugadores.h IO.cpp IO.h Acierto.cpp Acierto.h TablaResultados.h Juego.h TerminalScreen.cpp TerminalScreen.h)

add_custom_command(TARGET ruleta
                   POST_BUILD
                   COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_SOURCE_DIR}/roulette.txt ${CMAKE_BINARY_DIR})

if (CMAKE_CXX_COMPILER_ID MATCHES "Clang")
    set(CMAKE_CXX_FLAGS
        "-Wall -Wshadow -Wnon-virtual-dtor -Wold-style-cast -Wunused -Woverloaded-virtual \
        -Wpedantic -Wconversion -Wsign-conversion -Wmisleading-indentation \
        -Wnull-dereference -Wdouble-promotion \
        -Wformat=2")
elseif (CMAKE_CXX_COMPILER_ID MATCHES "GNU")
    set(CMAKE_CXX_FLAGS
        "-Wall -Wshadow -Wnon-virtual-dtor -Wold-style-cast -Wunused -Woverloaded-virtual \
        -Wpedantic -Wconversion -Wsign-conversion -Wmisleading-indentation -Wduplicated-cond \
        -Wduplicated-branches -Wlogical-op -Wnull-dereference -Wuseless-cast -Wdouble-promotion \
        -Wformat=2")
else ()
    message(WARNING "Este compilador no ha sido probado con este programa, uso bajo su propio riesgo.")
endif ()
