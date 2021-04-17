cmake_minimum_required(VERSION 3.19)

project(Snake VERSION 1.0)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED True)

add_executable(Projeto_Cobra 
               src/main.c
               src/snake_classico.c
               src/labirinto.c
               src/core.c)

target_link_libraries(Projeto_Cobra conio_64)

set_target_properties(Projeto_Cobra PROPERTIES COMPILE_FLAGS "-save-temps")