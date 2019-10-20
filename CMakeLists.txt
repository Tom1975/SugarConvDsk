cmake_minimum_required(VERSION 3.0)

# Nom du projet
project(SugarConvDsk)

set (USE_SHARED_CRT OFF)
set (gtest_force_shared_crt OFF)


set (CPCCORE			${PROJECT_SOURCE_DIR}/CPCCore/CPCCoreEmu 		CACHE STRING "CPCCoreEmu folder"		FORCE)
set (ZLIB				${PROJECT_SOURCE_DIR}/CPCCore/zlib 		CACHE STRING "zlib folder"		FORCE)
set (GOOGLETEST			${PROJECT_SOURCE_DIR}/CPCCore/googletest/googletest 		CACHE STRING "googletest folder"		FORCE)

add_subdirectory(${CPCCORE})
add_subdirectory(${ZLIB})
add_subdirectory(${GOOGLETEST})

add_subdirectory(SugarConvDsk)
add_subdirectory(Tests)


