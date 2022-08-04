cmake_minimum_required(VERSION 3.10)

# set the project name
project(CPPSerial)

#Generate compile_commands.json file
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_GENERATOR 'MinGW Makefiles')
  
# add the executable
add_executable(karagias main.cpp)
