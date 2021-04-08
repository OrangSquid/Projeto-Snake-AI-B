cmake_minimum_required(VERSION 3.19)

project(Snake VERSION 0.1)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED True)

add_executable(Projeto_Cobra 
               src/main.c
               src/snake_classico.c
               src/labirinto.c
               src/core.c)

target_include_directories(Projeto_Cobra PUBLIC
                           "include/conio21")

target_link_libraries(Projeto_Cobra conio_64)

set_target_properties(Projeto_Cobra PROPERTIES COMPILE_FLAGS "-save-temps")

#set_target_properties(Projeto_Cobra PROPERTIES COMPILE_FLAGS "-Wa,-adhln" )

#add_executable(getch_test src/getch_test.c)

#target_include_directories(getch_test PUBLIC
                           #"include/conio21")

#target_link_libraries(getch_test conio_64)

#add_executable(Sergio src/Projetosnake.c)

#target_include_directories(Sergio PUBLIC
                          #"include/conio21")

#target_link_libraries(Sergio conio_64)

#ADD_COMPILE_DEFINITIONS(UNICODE _UNICODE)