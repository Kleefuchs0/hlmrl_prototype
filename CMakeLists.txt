cmake_minimum_required(VERSION 3.22...3.25)

project(
    hlmrl
    VERSION
    0.1
    DESCRIPTION
    "Vocab learning cli"
    LANGUAGES
    CXX
)
add_definitions("-Wall" "-g")

include(FetchContent)

set(FETCHCONTENT_UPDATES_DISCONNECTED ON)
set(FETCHCONTENT_QUIET OFF)
set(BUILD_EXAMPLES OFF CACHE BOOL "" FORCE) # don't build the supplied examples
set(BUILD_GAMES    OFF CACHE BOOL "" FORCE) # don't build the supplied example games
set(GLFW_BUILD_WAYLAND ON CACHE BOOL "" FORCE) # enable wayland on linux
set(GLFW_BUILD_X11 OFF CACHE BOOL "" FORCE) # disable x11 on linux (switch these settings around as you please)

include_directories(src)
include_directories(src/headers)
include_directories(src/headers/app)

file(GLOB SRC_FILES src/*.cpp)

add_executable(${CMAKE_PROJECT_NAME} ${SRC_FILES})
set_property(TARGET ${CMAKE_PROJECT_NAME} PROPERTY CXX_STANDARD 23)
target_compile_definitions(${CMAKE_PROJECT_NAME} PUBLIC CMAKE_PROJECT_NAME="${CMAKE_PROJECT_NAME}")
target_compile_definitions(${CMAKE_PROJECT_NAME} PUBLIC CMAKE_PROJECT_VERSION=${CMAKE_PROJECT_VERSION})

FetchContent_Declare(raylib
  GIT_REPOSITORY https://github.com/raysan5/raylib.git
  GIT_TAG        master
  GIT_PROGRESS   TRUE
)
FetchContent_MakeAvailable(raylib)

FetchContent_Declare(json URL https://github.com/nlohmann/json/releases/download/v3.12.0/json.tar.xz)
FetchContent_MakeAvailable(json)


FetchContent_Declare(EnTT
  GIT_REPOSITORY https://github.com/skypjack/entt
  GIT_TAG        main
  GIT_PROGRESS   TRUE
)
FetchContent_MakeAvailable(EnTT)

FetchContent_Declare(
  fmt
  GIT_REPOSITORY https://github.com/fmtlib/fmt
  GIT_TAG        e69e5f977d458f2650bb346dadf2ad30c5320281) # 10.2.1
FetchContent_MakeAvailable(fmt)

target_link_libraries(${CMAKE_PROJECT_NAME}
  PRIVATE raylib
  PRIVATE nlohmann_json::nlohmann_json
  PRIVATE EnTT::EnTT
  PRIVATE fmt::fmt
)

