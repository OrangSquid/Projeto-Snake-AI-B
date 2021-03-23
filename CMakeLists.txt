CMAKE_MINIMUM_REQUIRED(VERSION 3.19)

PROJECT(Snake VERSION 0.1)

SET(CMAKE_C_STANDARD 11)
SET(CMAKE_C_STANDARD_REQUIRED True)

ADD_EXECUTABLE(Main 
               src/main.c
               src/snake_classico.c
               src/labirinto.c
               src/core.c)

TARGET_INCLUDE_DIRECTORIES(Main PUBLIC
                           "include/conio21")

TARGET_LINK_LIBRARIES(Main conio_unicode_64)

ADD_EXECUTABLE(getch_test src/getch_test.c)

TARGET_INCLUDE_DIRECTORIES(getch_test PUBLIC
                           "include/conio21")

TARGET_LINK_LIBRARIES(getch_test conio_unicode_64)

#ADD_COMPILE_DEFINITIONS(UNICODE _UNICODE)