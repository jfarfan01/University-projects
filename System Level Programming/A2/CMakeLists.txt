cmake_minimum_required(VERSION 3.5)
project(A2)

set(A2_SOURCE_FILES
        definitions.h
        resources.h
        vector.c
        vector.h
        slp_ag.c)

add_compile_options(-Wall -Werror)
add_executable(slp_ag ${A2_SOURCE_FILES})
target_link_libraries(slp_ag pthread)

