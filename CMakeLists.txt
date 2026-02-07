cmake_minimum_required(VERSION 3.16)
project(GameClient LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# ---- 默认构建类型（单配置生成器时生效，比如 Linux Make/Ninja）----
if(NOT CMAKE_CONFIGURATION_TYPES AND NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Debug CACHE STRING "Build type" FORCE)
  set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS Debug Release RelWithDebInfo MinSizeRel)
endif()

# ---- 生成 compile_commands.json（给 clangd / VSCode 用）----
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# ---- 防止因为 -march=native / AVX 等导致云机器 SIGILL ----
# 如果确定机器指令集一致，可以改成 native；否则建议保持 x86-64/ generic
if(NOT MSVC)
  add_compile_options(-march=x86-64 -mtune=generic)
endif()

# ---- LogManager ----
set(LOGMANAGER_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/../LogManager")
if(NOT EXISTS "${LOGMANAGER_SOURCE_DIR}/CMakeLists.txt")
  message(FATAL_ERROR "LogManager project not found at ${LOGMANAGER_SOURCE_DIR}")
endif()
add_subdirectory("${LOGMANAGER_SOURCE_DIR}" "${CMAKE_BINARY_DIR}/LogManager")

# ---- Sources ----
file(GLOB_RECURSE GAMECLIENT_SOURCES CONFIGURE_DEPENDS
  "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp"
)
list(APPEND GAMECLIENT_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/main.cpp")

add_executable(GameClient ${GAMECLIENT_SOURCES})
target_link_libraries(GameClient PRIVATE LogM)

target_include_directories(GameClient PRIVATE
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

# ---- Warnings ----
if(MSVC)
  target_compile_options(GameClient PRIVATE /W4 /permissive- /Zc:__cplusplus /MP)
  target_compile_definitions(GameClient PRIVATE _CRT_SECURE_NO_WARNINGS NOMINMAX)
else()
  target_compile_options(GameClient PRIVATE -Wall -Wextra -Wpedantic)
endif()

# ---- 平台链接 ----
if(WIN32)
  target_link_libraries(GameClient PRIVATE ws2_32)
elseif(UNIX)
  find_package(Threads REQUIRED)
  target_link_libraries(GameClient PRIVATE Threads::Threads)
endif()

# ---- Debug / RelWithDebInfo 生成符号，方便 gdb/core ----
if(NOT MSVC)
  target_compile_options(GameClient PRIVATE
    $<$<CONFIG:Debug>:-O0 -g3 -fno-omit-frame-pointer>
    $<$<CONFIG:RelWithDebInfo>:-O2 -g2 -fno-omit-frame-pointer>
  )
endif()

# ---- 可选：一键开启 ASan/UBSan（用于抓 SIGILL 的 UB 根因）----
option(ENABLE_SANITIZERS "Enable Address/Undefined sanitizers" OFF)
if(ENABLE_SANITIZERS AND NOT MSVC)
  target_compile_options(GameClient PRIVATE -fsanitize=address,undefined -fno-omit-frame-pointer -O0 -g3)
  target_link_options(GameClient PRIVATE -fsanitize=address,undefined)
endif()
