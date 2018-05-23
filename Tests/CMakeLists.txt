cmake_minimum_required(VERSION 3.0)

project(Tests)
enable_testing()

include_directories( ${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR} ${SugarConvDsk})

set(CompilerFlags
        CMAKE_CXX_FLAGS
        CMAKE_CXX_FLAGS_DEBUG
        CMAKE_CXX_FLAGS_RELEASE
        CMAKE_C_FLAGS
        CMAKE_C_FLAGS_DEBUG
        CMAKE_C_FLAGS_RELEASE
        CMAKE_CXX_FLAGS_RELWITHDEBINFO
		_SILENCE_TR1_NAMESPACE_DEPRECATION_WARNING 
        )

add_executable( Tests  
					test.cpp
)

# Link test executable against gtest & gtest_main
target_link_libraries(Tests Shlwapi.lib Comctl32.lib XmlLite.lib gtest gtest_main
                                             Msimg32.lib 
                                             Winmm.lib)

add_test( NAME Tests COMMAND Tests)


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
