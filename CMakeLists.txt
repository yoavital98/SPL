cmake_minimum_required(VERSION 3.20)
project(SPL)

set(CMAKE_CXX_STANDARD 14)

include_directories(include)

add_executable(SPL src/main.cpp
        include/Action.h
        include/Customer.h
        include/Studio.h
        include/Trainer.h
        include/Workout.h
        src/Action.cpp
        src/Customer.cpp
        src/Studio.cpp
        src/Trainer.cpp
        src/Workout.cpp)
