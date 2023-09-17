add_subdirectory(enigma-lib)

add_executable(enigma-console "enigma-console.cpp")
target_link_libraries(enigma-console enigma-lib)
