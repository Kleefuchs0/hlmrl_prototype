cmake_minimum_required(VERSION 3.22...3.25)

project(
    hlmrl_lib
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
include_directories(src/app)
include_directories(src/headers)
include_directories(src/lib/headers)

option (HLMRL_ENABLE_TESTS "Build tests for quartix?" ON)


FetchContent_Declare(raylib
  GIT_REPOSITORY https://github.com/raysan5/raylib.git
  GIT_TAG        master
  GIT_PROGRESS   TRUE
)
FetchContent_MakeAvailable(raylib)

FetchContent_Declare(raygui
  GIT_REPOSITORY https://github.com/raysan5/raygui
  GIT_TAG        master
  GIT_PROGRESS   TRUE
)
FetchContent_GetProperties(raygui)
if(NOT raygui_POPULATED)
    FetchContent_Populate(raygui)
endif()


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

file(GLOB LIB_SOURCE_FILES src/lib/*.cpp)

add_library(hlmrl_lib ${LIB_SOURCE_FILES})
set_property(TARGET hlmrl_lib PROPERTY CXX_STANDARD 23)
target_compile_definitions(hlmrl_lib PUBLIC PROJECT_NAME="hlmrl_lib")
target_compile_definitions(hlmrl_lib PUBLIC PROJECT_VERSION=${CMAKE_PROJECT_VERSION})


target_compile_options(hlmrl_lib PRIVATE
    $<$<AND:$<CONFIG:Debug>,$<CXX_COMPILER_ID:GNU,Clang,AppleClang>>:-g;-Wall>
    $<$<AND:$<CONFIG:Release>,$<CXX_COMPILER_ID:GNU,Clang,AppleClang>>:-Wall>

    $<$<AND:$<CONFIG:Debug>,$<CXX_COMPILER_ID:MSVC>>:/Od;/Zi;/Wall>
    $<$<AND:$<CONFIG:Release>,$<CXX_COMPILER_ID:MSVC>>:/O2>
)

target_link_libraries(hlmrl_lib
  PRIVATE raylib
  PRIVATE nlohmann_json::nlohmann_json
  PRIVATE EnTT::EnTT
  PRIVATE fmt::fmt
)

target_include_directories(hlmrl_lib PRIVATE
    ${raygui_SOURCE_DIR}/src
)


if (HLMRL_ENABLE_TESTS)
    include(FetchContent)

    FetchContent_Declare(
        Catch2
        GIT_REPOSITORY https://github.com/catchorg/Catch2.git
        GIT_TAG v3.15.2 # or a later release
    )

    FetchContent_MakeAvailable(Catch2)

    file(GLOB_RECURSE TEST_SOURCE_FILES tests/*.cpp)
    enable_testing ()
    add_executable(hlmrl_test ${TEST_SOURCE_FILES})
    target_link_libraries(hlmrl_test
        PRIVATE hlmrl_lib
        PRIVATE raylib
        PRIVATE nlohmann_json::nlohmann_json
        PRIVATE EnTT::EnTT
        PRIVATE fmt::fmt
        PRIVATE Catch2::Catch2WithMain
    )
    add_test (NAME hlmrl_test COMMAND quartix_test)
    set_property(TARGET hlmrl_test PROPERTY CXX_STANDARD 23)
endif()

file(GLOB APP_SOURCE_FILES src/app/*.cpp)

add_executable(hlmrl ${APP_SOURCE_FILES})
set_property(TARGET hlmrl PROPERTY CXX_STANDARD 23)
target_compile_definitions(hlmrl PUBLIC PROJECT_NAME="hlmrl")
target_compile_definitions(hlmrl PUBLIC PROJECT_VERSION=${CMAKE_PROJECT_VERSION})


target_compile_options(hlmrl PRIVATE
    $<$<AND:$<CONFIG:Debug>,$<CXX_COMPILER_ID:GNU,Clang,AppleClang>>:-g;-Wall>
    $<$<AND:$<CONFIG:Release>,$<CXX_COMPILER_ID:GNU,Clang,AppleClang>>:-Wall>

    $<$<AND:$<CONFIG:Debug>,$<CXX_COMPILER_ID:MSVC>>:/Od;/Zi;/Wall>
    $<$<AND:$<CONFIG:Release>,$<CXX_COMPILER_ID:MSVC>>:/O2>
)

target_link_libraries(hlmrl
    PRIVATE hlmrl_lib
    PRIVATE raylib
    PRIVATE nlohmann_json::nlohmann_json
    PRIVATE EnTT::EnTT
    PRIVATE fmt::fmt
)

target_include_directories(hlmrl_lib PRIVATE
    ${raygui_SOURCE_DIR}/src
)
