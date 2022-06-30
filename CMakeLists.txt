cmake_minimum_required(VERSION 3.21)
project(Timber)

set(SFML_STATIC_LIBRARIES TRUE)
set(CMAKE_CXX_STANDARD 14)
find_package(SFML 2.5 COMPONENTS system window graphics)

add_executable(Timber Timber.cpp)
target_link_libraries(Timber sfml-system sfml-window sfml-graphics -static)