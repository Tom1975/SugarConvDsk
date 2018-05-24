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


set(SRCS
	test.cpp
	${CMAKE_CURRENT_LIST_DIR}/../CPCCoreEmu/CAPSFile.cpp
	${CMAKE_CURRENT_LIST_DIR}/../CPCCoreEmu/CRC.cpp
	${CMAKE_CURRENT_LIST_DIR}/../CPCCoreEmu/DiskBuilder.cpp
	${CMAKE_CURRENT_LIST_DIR}/../CPCCoreEmu/DiskContainer.cpp
	${CMAKE_CURRENT_LIST_DIR}/../CPCCoreEmu/DiskGen.cpp
	${CMAKE_CURRENT_LIST_DIR}/../CPCCoreEmu/FileAccess.cpp
	${CMAKE_CURRENT_LIST_DIR}/../CPCCoreEmu/FormatTypeCTRAW.cpp
	${CMAKE_CURRENT_LIST_DIR}/../CPCCoreEmu/FormatTypeEDSK.cpp
	${CMAKE_CURRENT_LIST_DIR}/../CPCCoreEmu/FormatTypeDSK.cpp
	${CMAKE_CURRENT_LIST_DIR}/../CPCCoreEmu/FormatTypeHFE.cpp
	${CMAKE_CURRENT_LIST_DIR}/../CPCCoreEmu/FormatTypeIPF.cpp
	${CMAKE_CURRENT_LIST_DIR}/../CPCCoreEmu/FormatTypeRAW.cpp
	${CMAKE_CURRENT_LIST_DIR}/../CPCCoreEmu/FormatTypeSCP.cpp
	${CMAKE_CURRENT_LIST_DIR}/../CPCCoreEmu/IDisk.cpp
	${CMAKE_CURRENT_LIST_DIR}/../CPCCoreEmu/MediaManager.cpp
	)

set(HEADERS
	${CMAKE_CURRENT_LIST_DIR}/../CPCCoreEmu/CAPSFile.h
	${CMAKE_CURRENT_LIST_DIR}/../CPCCoreEmu/CRC.h
	${CMAKE_CURRENT_LIST_DIR}/../CPCCoreEmu/DiskBuilder.h
	${CMAKE_CURRENT_LIST_DIR}/../CPCCoreEmu/DiskContainer.h
	${CMAKE_CURRENT_LIST_DIR}/../CPCCoreEmu/DiskGen.h
	${CMAKE_CURRENT_LIST_DIR}/../CPCCoreEmu/DskTypeManager.h
	${CMAKE_CURRENT_LIST_DIR}/../CPCCoreEmu/FDC765.h
	${CMAKE_CURRENT_LIST_DIR}/../CPCCoreEmu/FileAccess.h
	${CMAKE_CURRENT_LIST_DIR}/../CPCCoreEmu/FormatTypeCTRAW.h
	${CMAKE_CURRENT_LIST_DIR}/../CPCCoreEmu/FormatTypeEDSK.h
	${CMAKE_CURRENT_LIST_DIR}/../CPCCoreEmu/FormatTypeDSK.h
	${CMAKE_CURRENT_LIST_DIR}/../CPCCoreEmu/FormatTypeHFE.h
	${CMAKE_CURRENT_LIST_DIR}/../CPCCoreEmu/FormatTypeIPF.h
	${CMAKE_CURRENT_LIST_DIR}/../CPCCoreEmu/FormatTypeRAW.h
	${CMAKE_CURRENT_LIST_DIR}/../CPCCoreEmu/FormatTypeSCP.h
	${CMAKE_CURRENT_LIST_DIR}/../CPCCoreEmu/IDisk.h
	${CMAKE_CURRENT_LIST_DIR}/../CPCCoreEmu/ILoadingProgress.h
	${CMAKE_CURRENT_LIST_DIR}/../CPCCoreEmu/ILog.h
	${CMAKE_CURRENT_LIST_DIR}/../CPCCoreEmu/includes.h
	${CMAKE_CURRENT_LIST_DIR}/../CPCCoreEmu/macro.h
	${CMAKE_CURRENT_LIST_DIR}/../CPCCoreEmu/MediaManager.h
	${CMAKE_CURRENT_LIST_DIR}/../CPCCoreEmu/stdafx.h
)

add_executable( Tests   ${SRCS} ${HEADERS} )
include_directories(${CMAKE_CURRENT_LIST_DIR}/../CPCCoreEmu ${CMAKE_CURRENT_BINARY_DIR}/../zlib)

# Link test executable against gtest & gtest_main
target_include_directories( Tests PUBLIC ${CMAKE_CURRENT_LIST_DIR}/../CPCCoreEmu ${CMAKE_CURRENT_LIST_DIR}/../zlib)
target_link_libraries(Tests Shlwapi.lib Comctl32.lib XmlLite.lib gtest gtest_main
                                             Msimg32.lib 
                                             Winmm.lib)

add_test( NAME Tests COMMAND Tests)

if(CMAKE_COMPILER_IS_GNUCC)
	set ( CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS} --std=c++1z )
endif()

set ( CMAKE_CXX_FLAGS_DEBUG ${CMAKE_CXX_FLAGS_DEBUG} )

if(MSVC)
	target_link_libraries ( Tests zlibstatic)
else ()
	target_link_libraries ( Tests  m stdc++fs libz.a)
endif()

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
