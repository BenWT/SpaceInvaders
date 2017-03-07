set(PROJECT_NAME "SpaceInvaders")

project(${PROJECT_NAME})
cmake_minimum_required(VERSION 3.0)

include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup()

add_executable(${PROJECT_NAME}
	src/main.cpp
	src/headers/Globals.h
	src/headers/Shaders.h
	src/headers/GameObject.h
	src/headers/Alien.h
	src/headers/Plane.h
)

target_link_libraries(${PROJECT_NAME} ${CONAN_LIBS})
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
set_property(DIRECTORY PROPERTY VS_STARTUP_PROJECT "${PROJECT_NAME}")

file(COPY src/assets DESTINATION bin)
