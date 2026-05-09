cmake_minimum_required(VERSION 4.2)
project(TankAttack)

set(CMAKE_CXX_STANDARD 14)

add_executable(TankAttack
        main.cpp
        Utils.h
        str/Grafo.cpp
        str/Grafo.h
        str/Map.cpp
        str/Map.h
        Personajes/Tanques/Tank.h
        Personajes/Tanques/TankBFS.h
        Personajes/Tanques/TankDijkstra.h
        Personajes/Tanques/Tank.cpp
        Personajes/Tanques/TankBFS.cpp
        Personajes/Tanques/TankDijkstra.cpp
        Personajes/Balas/Bala.h)
