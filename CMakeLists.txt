cmake_minimum_required(VERSION 4.2)
project(TankAttack)

set(CMAKE_CXX_STANDARD 14)

add_executable(TankAttack
        main.cpp
        Utils.h
        str/Grafo.cpp
        str/Grafo.h
        Personajes/Tank.h
        Personajes/TankBFS.h
        Personajes/TankDijkstra.h
        Personajes/Tank.cpp
        Personajes/TankBFS.cpp
        Personajes/TankDijkstra.cpp)
