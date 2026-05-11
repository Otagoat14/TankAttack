cmake_minimum_required(VERSION 4.2)
project(TankAttack)

set(CMAKE_CXX_STANDARD 17)

include(FetchContent)
FetchContent_Declare(
        SFML
        GIT_REPOSITORY https://github.com/SFML/SFML.git
        GIT_TAG 2.6.1
)
FetchContent_MakeAvailable(SFML)

add_executable(TankAttack
        main.cpp
        str/Grafo.cpp
        str/Map.cpp
        Personajes/Tanques/Tank.cpp
        Personajes/Tanques/TankBFS.cpp
        Personajes/Tanques/TankDijkstra.cpp
        Personajes/Balas/Bala.cpp
        Interfaz/MenuRenderer.cpp
        Interfaz/UI/UIManager.h
        Interfaz/UI/Boton.h
        Interfaz/UI/Boton.cpp
        Interfaz/UI/Texto.h
        Interfaz/UI/Texto.cpp
)

target_link_libraries(TankAttack
        sfml-graphics
        sfml-window
        sfml-system
)

add_custom_command(TARGET TankAttack POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "${CMAKE_BINARY_DIR}/_deps/sfml-build/lib/sfml-graphics-d-2.dll"
        "${CMAKE_BINARY_DIR}"
)
add_custom_command(TARGET TankAttack POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "${CMAKE_BINARY_DIR}/_deps/sfml-build/lib/sfml-window-d-2.dll"
        "${CMAKE_BINARY_DIR}"
)
add_custom_command(TARGET TankAttack POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "${CMAKE_BINARY_DIR}/_deps/sfml-build/lib/sfml-system-d-2.dll"
        "${CMAKE_BINARY_DIR}"
)