add_executable(ruleta
        main.cpp
        Animacion.h
        )

add_custom_command(
        TARGET ruleta
        POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_SOURCE_DIR}/roulette.txt ${CMAKE_BINARY_DIR}
)
