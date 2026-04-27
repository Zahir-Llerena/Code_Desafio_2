cmake_minimum_required(VERSION 3.16)

project(Code_D2 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(Code_D2 main.cpp)

include(GNUInstallDirs)
install(TARGETS Code_D2
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
