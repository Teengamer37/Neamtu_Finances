cmake_minimum_required(VERSION 3.28)
project(Neamtu_Finances)

set(CMAKE_CXX_STANDARD 14)

add_subdirectory(test)

set(SOURCE_FILES
        Transaction.cpp
        Transaction.h
        Account.cpp
        Account.h
        AdminMenu.h
)
add_executable(${CMAKE_PROJECT_NAME} main.cpp)
add_library(${CMAKE_PROJECT_NAME}_lib ${SOURCE_FILES})
target_link_libraries(${CMAKE_PROJECT_NAME} ${CMAKE_PROJECT_NAME}_lib)
