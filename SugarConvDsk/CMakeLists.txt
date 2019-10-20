cmake_minimum_required(VERSION 3.0)
project(SugarConvDsk)

set(SRCS
	main.cpp
	)

set(HEADERS
)

#Fichier ressource

add_definitions( -D_CONSOLE)
add_executable(SugarConvDsk ${SRCS} ${HEADERS})
include_directories(${CMAKE_CURRENT_LIST_DIR}/../CPCCore/CPCCoreEmu ${CMAKE_CURRENT_BINARY_DIR}/../zlib)

target_include_directories( SugarConvDsk PUBLIC ${CMAKE_CURRENT_LIST_DIR}/../CPCCore ${CMAKE_CURRENT_LIST_DIR}/../zlib)

set(CompilerFlags
        CMAKE_CXX_FLAGS
        CMAKE_CXX_FLAGS_DEBUG
        CMAKE_CXX_FLAGS_RELEASE
        CMAKE_CXX_FLAGS_RELWITHDEBINFO
        CMAKE_C_FLAGS
        CMAKE_C_FLAGS_DEBUG
        CMAKE_C_FLAGS_RELEASE
        )


if(CMAKE_COMPILER_IS_GNUCC)
	set ( CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS} --std=c++1z )
endif()

set ( CMAKE_CXX_FLAGS_DEBUG ${CMAKE_CXX_FLAGS_DEBUG} )

include_directories (SugarConvDsk
					 )
set(CMAKE_CXX_STANDARD 17)


if (USE_SHARED_CRT)
else()
   foreach(CompilerFlag ${CompilerFlags})
     string(REPLACE "/MD" "/MT" ${CompilerFlag} "${${CompilerFlag}}")
   endforeach()
endif()

if(CMAKE_COMPILER_IS_GNUCC)
	set ( CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS} --std=c++1z )
endif()

if(MSVC)
	target_link_libraries ( SugarConvDsk zlibstatic CPCCoreEmu)
else ()
	target_link_libraries ( SugarConvDsk  m stdc++fs libz.a CPCCoreEmu)
endif()
