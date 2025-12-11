cmake_minimum_required(VERSION 4.0)
project(
  raylib_prairie_king
  VERSION 1.0
  LANGUAGES CXX
)
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_BUILD_TYPE Debug)

# Adding Raylib
include(FetchContent)
set(FETCHCONTENT_QUIET FALSE)
set(BUILD_EXAMPLES OFF CACHE BOOL "" FORCE) # don't build the supplied examples
set(BUILD_GAMES    OFF CACHE BOOL "" FORCE) # don't build the supplied example games

FetchContent_Declare(
    raylib
    GIT_REPOSITORY "https://github.com/raysan5/raylib.git"
    GIT_TAG "master"
    GIT_PROGRESS TRUE
)

FetchContent_MakeAvailable(raylib)

# Adding our source files
file(GLOB_RECURSE PROJECT_SOURCES CONFIGURE_DEPENDS "${CMAKE_CURRENT_LIST_DIR}/sources/*.c") # Define PROJECT_SOURCES as a list of all source files
set(PROJECT_INCLUDE "${CMAKE_CURRENT_LIST_DIR}/src/") # Define PROJECT_INCLUDE to be the path to the include directory of the project

# Declaring our executable
add_executable(
  ${PROJECT_NAME}
  src/main.cpp
  src/game.cpp
  src/game.h
  src/utils.cpp
  src/utils.h
  src/entities/entity.cpp
  src/entities/entity.h
  src/assets/asset_manager.cpp
  src/assets/asset_manager.h
  src/assets/animation.cpp
  src/assets/animation.h
  src/map.cpp
  src/map.h
  src/globals.h
  src/assets/assets.h
  src/entities/player.cpp
  src/entities/player.h
  src/entities/zombie.cpp
  src/entities/zombie.h
  src/entities/bullet.cpp
  src/entities/bullet.h
  src/entities/entity_manager.cpp
  src/entities/entity_manager.h
  src/entities/entity_display.cpp
  src/entities/entity_display.h
)
target_sources(${PROJECT_NAME} PRIVATE ${PROJECT_SOURCES})
target_include_directories(${PROJECT_NAME} PRIVATE ${PROJECT_INCLUDE})
target_link_libraries(${PROJECT_NAME} PRIVATE raylib)

# Setting ASSETS_PATH
target_compile_definitions(${PROJECT_NAME} PUBLIC ASSETS_PATH="${CMAKE_CURRENT_SOURCE_DIR}/assets/") # Set the asset path macro to the absolute path on the dev machine
#target_compile_definitions(${PROJECT_NAME} PUBLIC ASSETS_PATH="./assets") # Set the asset path macro in release mode to a relative path that assumes the assets folder is in the same directory as the game executable
