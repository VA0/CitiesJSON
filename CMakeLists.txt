cmake_minimum_required(VERSION 3.8) ##setting minimum CMAKE version

project(Cities CXX)   #setting project name

#SET(CMAKE_C_STANDARD 17)    #setting C version
SET(CMAKE_CXX_STANDARD 11)  #setting CPP version

enable_testing() #telling we gonna use unit tests

#add_definitions(-DGTEST_HAS_PTHREAD=0)
add_subdirectory(googletest)
#remove_definitions(-DGTEST_HAS_PTHREAD=0)

#set_property(DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/googletest" APPEND  //ato?
#  PROPERTY COMPILE_DEFINITIONS "-DGTEST_HAS_PTHREAD=0")  //ito?


# target_include_directories(Cities PRIVATE rapidjson)
# target_include_directories(Cities PRIVATE rapidjson/error)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/rapidjson)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/rapidjson/error)

#add_library(Cities STATIC Cities.cpp Cities.h rapidjson/)

#making static Cities.lib library so we can test it with gtest (we cant test exe from gtest exe)
add_library(Cities "Cities.cpp" "Cities.h" "rapidjson/document.h" "rapidjson/stringbuffer.h" "rapidjson/writer.h" "rapidjson/error/en.h" "cities.json" "filters.json")

#cmake_policy(SET CMP0079 NEW) 

add_subdirectory(test)  #Unit Tests from gtest

add_subdirectory(CitiesExe) #Launcher for Cities.lib

