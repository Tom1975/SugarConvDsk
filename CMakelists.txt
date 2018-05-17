cmake_minimum_required(VERSION 3.0)

# Nom du projet
project(SugarConvDsk)

set (gtest_force_shared_crt ON)

add_subdirectory(zlib)
add_subdirectory(SugarConvDsk)
add_subdirectory(googletest/googletest)
add_subdirectory(Tests)

