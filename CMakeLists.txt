cmake_minimum_required(VERSION 3.16)
project(GameClient LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(LOGMANAGER_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/../LogManager")
if(NOT EXISTS "${LOGMANAGER_SOURCE_DIR}/CMakeLists.txt")
	message(FATAL_ERROR "LogManager project not found at ${LOGMANAGER_SOURCE_DIR}")
endif()
add_subdirectory("${LOGMANAGER_SOURCE_DIR}" "${CMAKE_BINARY_DIR}/LogManager")

file(GLOB_RECURSE GAMECLIENT_SOURCES CONFIGURE_DEPENDS
	"${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp"
)
list(APPEND GAMECLIENT_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/main.cpp")

add_executable(GameClient ${GAMECLIENT_SOURCES})

target_link_libraries(GameClient PRIVATE LogM)

target_include_directories(GameClient
	PRIVATE
		"${CMAKE_CURRENT_SOURCE_DIR}/src"
		"${CMAKE_CURRENT_SOURCE_DIR}/src/Battle"
		"${CMAKE_CURRENT_SOURCE_DIR}/src/Battle/header"
		"${CMAKE_CURRENT_SOURCE_DIR}/src/Character"
		"${CMAKE_CURRENT_SOURCE_DIR}/src/Character/header"
		"${CMAKE_CURRENT_SOURCE_DIR}/src/DataConfig"
		"${CMAKE_CURRENT_SOURCE_DIR}/src/Item"
		"${CMAKE_CURRENT_SOURCE_DIR}/src/CommonTools"
		"${CMAKE_CURRENT_SOURCE_DIR}/src/Network"
		"${CMAKE_CURRENT_SOURCE_DIR}/lib"
)

if(MSVC)
	target_compile_options(GameClient PRIVATE /W4 /permissive- /Zc:__cplusplus /MP)
	target_compile_definitions(GameClient PRIVATE _CRT_SECURE_NO_WARNINGS NOMINMAX)
else()
	target_compile_options(GameClient PRIVATE -Wall -Wextra -Wpedantic)
endif()

if(WIN32)
	target_link_libraries(GameClient PRIVATE ws2_32)
elseif(UNIX)
	find_package(Threads REQUIRED)
	target_link_libraries(GameClient PRIVATE Threads::Threads)
endif()
