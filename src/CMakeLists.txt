add_executable(ruleta
        main.cpp state/State.h Utility.h Game.h Game.cpp
        MessageQueue.cpp MessageQueue.h StateMachine.cpp
        StateMachine.h JugadorState.cpp
        JugadorState.h state/Impl.h state/Impl.cpp RondasState.cpp
        RondasState.h
        Utility.cpp)

target_link_libraries(ruleta ncurses)

add_custom_command(
        TARGET ruleta
        POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_SOURCE_DIR}/roulette.txt ${CMAKE_BINARY_DIR}
)
