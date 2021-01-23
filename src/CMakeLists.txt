add_executable(ruleta
        main.cpp
        Animacion.h
        Screen.h
        Animacion.cpp Jugador.h Jugadores.cpp Jugadores.h IO.cpp IO.h Acierto.cpp Acierto.h TablaResultados.h Juego.h TerminalScreen.cpp TerminalScreen.h)

add_custom_command(TARGET ruleta
        POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_SOURCE_DIR}/roulette.txt ${CMAKE_BINARY_DIR})
