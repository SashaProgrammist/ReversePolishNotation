cmake_minimum_required(VERSION 3.21)
project(ReversePolishNotation)

set(CMAKE_CXX_STANDARD 17)

add_executable(ReversePolishNotation main.cpp)

add_library(function
        classes/Function/Function.h classes/Function/Function.cpp SetOperators/SetOperators.h SetOperators/SetOperators.cpp)

add_library(Variable
        classes/Variable/Variable.h classes/Variable/Variable.cpp)

add_library(OperatorFunction
        classes/OperatorFunction/OperatorFunction.h classes/OperatorFunction/OperatorFunction.cpp)

target_link_libraries(ReversePolishNotation
        function
        Variable
        OperatorFunction)
