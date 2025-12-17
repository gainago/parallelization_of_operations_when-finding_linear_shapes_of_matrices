cmake_minimum_required(VERSION 3.5)

project(make_abstract_syntax_tree LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Wpedantic")

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

add_executable(make_abstract_syntax_tree main.cpp
    raw_AST_nodes.h
    parser.h parser.cpp
    raw_AST_nodes.cpp
    typed_AST_nodes.h
    typed_AST_nodes.cpp
    printer_to_dot.h printer_to_dot.cpp

    tests/Tests.cpp tests/Tests.h
    Tokens/Tokens.cpp Tokens/Tokens.h
    threadsafe_data_structures/thread_safe_queue.h
    threadsafe_data_structures/thread_pool.h
    threadsafe_data_structures/function_wrapper.h

    matrix.h matrix.cpp
    evaluatenode.h
    evaluatenode.cpp
    evaluation_graph_widget.h evaluation_graph_widget.cpp)

find_package(Qt6 REQUIRED COMPONENTS Widgets)
target_link_libraries( make_abstract_syntax_tree PRIVATE Qt6::Widgets)

include(GNUInstallDirs)
install(TARGETS make_abstract_syntax_tree
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
#some comments for test Qt git tool
