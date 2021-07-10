cmake_minimum_required(VERSION 3.8) ##setting minimum CMAKE version

project(CitiesExe CXX)   #setting project name

#SET(CMAKE_C_STANDARD 17)    #setting C version
SET(CMAKE_CXX_STANDARD 11)  #setting CPP version

enable_testing() #telling we gonna use unit tests

#add_subdirectory(googletest)

add_executable(CitiesExe main.cpp)
TARGET_LINK_LIBRARIES(CitiesExe Cities)

#C:\Users\Filip\source\repos\Cities\build\Release

