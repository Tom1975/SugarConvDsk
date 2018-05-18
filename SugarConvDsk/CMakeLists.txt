cmake_minimum_required(VERSION 3.0)
project(SugarConvDsk)

set(SRCS
	main.cpp
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

#Fichier ressource

add_definitions( -D_CONSOLE)
add_executable(SugarConvDsk ${SRCS} ${HEADERS})
include_directories(${CMAKE_CURRENT_LIST_DIR}/../CPCCoreEmu ${CMAKE_CURRENT_BINARY_DIR}/../zlib)

target_include_directories( SugarConvDsk PUBLIC ${CMAKE_CURRENT_LIST_DIR}/../CPCCoreEmu ${CMAKE_CURRENT_LIST_DIR}/../zlib)

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

if(MSVC)
	target_link_libraries ( SugarConvDsk zlibstatic)
else ()
	target_link_libraries ( SugarConvDsk  m stdc++fs zlib.a)
endif()

