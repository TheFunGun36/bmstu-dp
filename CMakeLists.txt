cmake_minimum_required(VERSION 3.16)
set(CMAKE_CXX_STANDARD 20)
set(Boost_USE_STATIC_LIBS ON)
set(Boost_USE_MULTITHREADED ON)
set(Boost_USE_STATIC_RUNTIME OFF)


project(bmstu-data-protection
	VERSION 0.1
	DESCRIPTION "BMSTU, data protection lab collection"
	LANGUAGES CXX)

# Enigma machine (lab 1)
add_subdirectory(enigma)
