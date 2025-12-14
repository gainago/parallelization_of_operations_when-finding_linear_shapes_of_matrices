cmake_minimum_required(VERSION 3.5)

project(make_abstract_syntax_tree LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Wpedantic")

add_executable(make_abstract_syntax_tree main.cpp
    raw_AST_nodes.h
    Tokens.h
    Tokens.cpp
    Tests.h
    Tests.cpp
    parser.h parser.cpp
    raw_AST_nodes.cpp
    typed_AST_nodes.h
    typed_AST_nodes.cpp
    printer_to_dot.h printer_to_dot.cpp
    evaluatenode.h evaluatenode.cpp)

include(GNUInstallDirs)
install(TARGETS make_abstract_syntax_tree
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
#some comments for test Qt git tool
