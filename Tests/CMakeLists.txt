cmake_minimum_required(VERSION 3.0)

project(Tests)
enable_testing()

include_directories( ${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR} ${SugarConvDsk})

set(SRCS
	test.cpp
	)

set(HEADERS
)

add_executable( Tests   ${SRCS} ${HEADERS})
include_directories(${CMAKE_CURRENT_LIST_DIR}/../CPCCore/CPCCoreEmu ${CMAKE_CURRENT_BINARY_DIR}/../zlib)
target_include_directories( Tests PUBLIC ${CMAKE_CURRENT_LIST_DIR}/../CPCCore ${CMAKE_CURRENT_LIST_DIR}/../zlib)

set(CompilerFlags
        CMAKE_CXX_FLAGS
        CMAKE_CXX_FLAGS_DEBUG
        CMAKE_CXX_FLAGS_RELEASE
        CMAKE_CXX_FLAGS_RELWITHDEBINFO
        CMAKE_C_FLAGS
        CMAKE_C_FLAGS_DEBUG
        CMAKE_C_FLAGS_RELEASE
		_SILENCE_TR1_NAMESPACE_DEPRECATION_WARNING 
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

# Link test executable against gtest & gtest_main

if(MSVC)
	target_link_libraries ( Tests zlibstatic CPCCoreEmu gtest gtest_main )
else ()
	target_link_libraries ( Tests  m stdc++fs libz.a CPCCoreEmu gtest gtest_main )
endif()

add_test( NAME Tests COMMAND Tests)

if(CMAKE_COMPILER_IS_GNUCC)
	set ( CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS} --std=c++1z )
endif()

set ( CMAKE_CXX_FLAGS_DEBUG ${CMAKE_CXX_FLAGS_DEBUG} )

if( MSVC)
	install ( FILES ${SugarConvDsk_BINARY_DIR}/Debug/SugarConvDsk.exe CONFIGURATIONS Debug  DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/Debug )
	install ( FILES ${SugarConvDsk_BINARY_DIR}/Release/SugarConvDsk.exe CONFIGURATIONS Release  DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/Release )
	install ( FILES ${SugarConvDsk_BINARY_DIR}/RelWithDebInfo/SugarConvDsk.exe CONFIGURATIONS RelWithDebInfo  DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/RelWithDebInfo )

	# Output temporary directory
	file(MAKE_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/Debug/out)
	file(MAKE_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/Release/out)
	file(MAKE_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/RelWithDebInfo/out)

	# Copy file to compare
	install ( DIRECTORY "./Expected" CONFIGURATIONS Debug DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/Debug/ )
	install ( DIRECTORY "./Expected" CONFIGURATIONS Release DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/Release/ )
	install ( DIRECTORY "./Expected" CONFIGURATIONS RelWithDebInfo DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/RelWithDebInfo/ )

	install ( DIRECTORY "./Ressources" CONFIGURATIONS Debug DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/Debug/ )
	install ( DIRECTORY "./Ressources" CONFIGURATIONS Release DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/Release/ )
	install ( DIRECTORY "./Ressources" CONFIGURATIONS RelWithDebInfo DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/RelWithDebInfo/ )
else()
	install ( FILES ${SugarConvDsk_BINARY_DIR}/SugarConvDsk DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

	# Output temporary directory
	file(MAKE_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/out)

	# Copy file to compare
	install ( DIRECTORY "./Expected" DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
	install ( DIRECTORY "./Ressources" DESTINATION ${CMAKE_CURRENT_BINARY_DIR} )
endif()

