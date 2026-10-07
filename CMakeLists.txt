cmake_minimum_required(VERSION 3.15)

project(Oraculo C)

set(CMAKE_C_STANDARD 11)
set(RAYLIB_VERSION 6.0)

find_package(raylib ${RAYLIB_VERSION} QUIET) # QUIET or REQUIRED
if (NOT raylib_FOUND) # If there's none, fetch and build raylib
  include(FetchContent)
  FetchContent_Declare(
    raylib
    DOWNLOAD_EXTRACT_TIMESTAMP OFF
    URL https://github.com/raysan5/raylib/archive/refs/tags/${RAYLIB_VERSION}.tar.gz
  )
  FetchContent_GetProperties(raylib)
  if (NOT raylib_POPULATED) # Have we downloaded raylib yet?
    set(FETCHCONTENT_QUIET NO)
    FetchContent_MakeAvailable(raylib)
  endif()
endif()


# Encontrar arquivos .c
file(GLOB_RECURSE SOURCES
    "${CMAKE_SOURCE_DIR}/src/*.c"
    "${CMAKE_SOURCE_DIR}/src/**/*.c"
)
# Setup do Executavel
add_executable(Oraculo ${SOURCES})

# Diretórios de headers
target_include_directories(Oraculo PUBLIC
    "${CMAKE_SOURCE_DIR}/include"
)

# Bibliotecas
target_link_libraries(Oraculo raylib)

if (APPLE)
    target_link_libraries(Oraculo "-framework IOKit")
    target_link_libraries(Oraculo "-framework Cocoa")
    target_link_libraries(Oraculo "-framework OpenGL")
    target_link_libraries(Oraculo "-framework QuartzCore")
endif()