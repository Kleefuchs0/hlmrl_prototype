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

function(enable_target_warnings TARGET)
    target_compile_options(${TARGET} PRIVATE
        $<$<AND:$<CONFIG:Debug>,$<CXX_COMPILER_ID:GNU,Clang,AppleClang>>:
        -g;-Wall;-Wextra;-Wpedantic;-Wdouble-promotion;-Wnull-dereference;-Wnrvo;-Wsign-conversion;-Wformat=2;-Wcast-align;-Wold-style-cast;-Wsuggest-override;-fno-omit-frame-pointer
        >
        $<$<AND:$<CONFIG:Release>,$<CXX_COMPILER_ID:GNU,Clang,AppleClang>>:
        -Wall;-Wextra;-Wpedantic;-Wdouble-promotion;-Wnull-dereference;-Wnrvo;-Wsign-conversion;-Wformat=2;-Wcast-align;-Wold-style-cast;-Wsuggest-override;-fno-omit-frame-pointer
        >

        $<$<AND:$<CONFIG:Debug>,$<CXX_COMPILER_ID:MSVC>>:
        /Od;/Zi;/W4
        >
        $<$<AND:$<CONFIG:Release>,$<CXX_COMPILER_ID:MSVC>>:
        /O2;/W4
        >
    )
endfunction()

add_definitions("-g")

include(FetchContent)

set(FETCHCONTENT_UPDATES_DISCONNECTED ON)
set(FETCHCONTENT_QUIET OFF)
set(BUILD_EXAMPLES OFF CACHE BOOL "" FORCE) # don't build the supplied examples
set(BUILD_GAMES    OFF CACHE BOOL "" FORCE) # don't build the supplied example games
set(GLFW_BUILD_WAYLAND ON CACHE BOOL "" FORCE) # enable wayland on linux
set(GLFW_BUILD_X11 OFF CACHE BOOL "" FORCE) # disable x11 on linux (switch these settings around as you please)

include_directories(headers)

option (HLMRL_ENABLE_TESTS "Build tests for hlmrl?" ON)

find_package(raylib CONFIG QUIET)

if(NOT raylib_FOUND)
    message(STATUS "raylib not found, fetching it...")

    FetchContent_Declare(raylib
        GIT_REPOSITORY https://github.com/raysan5/raylib.git
        GIT_TAG        master
        GIT_PROGRESS   TRUE
    )
    FetchContent_MakeAvailable(raylib)

endif()

add_library(raylib_system INTERFACE)

target_link_libraries(raylib_system INTERFACE raylib)

target_include_directories(raylib_system SYSTEM INTERFACE
    ${raylib_SOURCE_DIR}/src
)


find_package(raygui CONFIG QUIET)

if(NOT raygui_FOUND)
    message(STATUS "raygui not found, fetching it...")

    FetchContent_Declare(raygui
        GIT_REPOSITORY https://github.com/raysan5/raygui
        GIT_TAG        master
        GIT_PROGRESS   TRUE
    )

    FetchContent_GetProperties(raygui)
    if(NOT raygui_POPULATED)
        FetchContent_MakeAvailable(raygui)
    endif()

endif()

find_package(nlohmann_json CONFIG QUIET)

if (NOT nlohmann_json_FOUND)

    message(STATUS "nlohmann_json not found, fetching it...")

    FetchContent_Declare(nlohmann_json
        URL https://github.com/nlohmann/json/releases/download/v3.12.0/json.tar.xz
    )
    FetchContent_MakeAvailable(nlohmann_json)

endif()

find_package(EnTT CONFIG QUIET)

if (NOT EnTT_FOUND)

    message(STATUS "EnTT not found, fetching it...")
    FetchContent_Declare(EnTT
        GIT_REPOSITORY https://github.com/skypjack/entt
        GIT_TAG        main
        GIT_PROGRESS   TRUE
    )

    FetchContent_MakeAvailable(EnTT)
endif()

add_library(entt_system INTERFACE)
target_link_libraries(entt_system INTERFACE EnTT::EnTT)

target_include_directories(entt_system SYSTEM INTERFACE
    ${entt_SOURCE_DIR}/src
)

find_package(fmt CONFIG QUIET)

if(NOT fmt_FOUND)
    message(STATUS "fmt not found, fetching it...")

    FetchContent_Declare(
        fmt
        GIT_REPOSITORY https://github.com/fmtlib/fmt
        GIT_TAG        e69e5f977d458f2650bb346dadf2ad30c5320281) # 10.2.1
    FetchContent_MakeAvailable(fmt)

endif()

file(GLOB LIB_SOURCE_FILES src/lib/*.cpp)

add_library(hlmrl_lib ${LIB_SOURCE_FILES})
set_property(TARGET hlmrl_lib PROPERTY CXX_STANDARD 23)

enable_target_warnings(hlmrl_lib)

target_link_libraries(hlmrl_lib
  PRIVATE raylib_system
  PRIVATE nlohmann_json::nlohmann_json
  PRIVATE entt_system
  PRIVATE fmt::fmt
)

target_include_directories(hlmrl_lib SYSTEM PRIVATE
    ${raygui_SOURCE_DIR}/src
)


if (HLMRL_ENABLE_TESTS)

    find_package(Catch2 3 CONFIG QUIET)

    if (NOT Catch2_FOUND)
        FetchContent_Declare(
            Catch2
            GIT_REPOSITORY https://github.com/catchorg/Catch2.git
            GIT_TAG v3.15.2 # or a later release
        )

        FetchContent_MakeAvailable(Catch2)
    endif()

    file(GLOB_RECURSE TEST_SOURCE_FILES tests/*.cpp)
    enable_testing ()
    add_executable(hlmrl_test ${TEST_SOURCE_FILES})
    target_link_libraries(hlmrl_test
        PRIVATE hlmrl_lib
        PRIVATE raylib_system
        PRIVATE nlohmann_json::nlohmann_json
        PRIVATE entt_system
        PRIVATE fmt::fmt
        PRIVATE Catch2::Catch2WithMain
    )
    add_test (NAME hlmrl_test COMMAND hlmrl_test)
    set_property(TARGET hlmrl_test PROPERTY CXX_STANDARD 23)
    enable_target_warnings(hlmrl_test)
endif()

file(GLOB GAME_SOURCE_FILES src/game/*.cpp)

add_executable(hlmrl ${GAME_SOURCE_FILES})
set_property(TARGET hlmrl PROPERTY CXX_STANDARD 23)

enable_target_warnings(hlmrl)

target_link_libraries(hlmrl
    PRIVATE hlmrl_lib
    PRIVATE raylib_system
    PRIVATE nlohmann_json::nlohmann_json
    PRIVATE entt_system
    PRIVATE fmt::fmt
)

target_include_directories(hlmrl PRIVATE
    ${raygui_SOURCE_DIR}/src
)

file(GLOB MAP_CREATOR_SOURCE_FILES src/map_creator/*.cpp)

add_executable(map_creator ${MAP_CREATOR_SOURCE_FILES})
set_property(TARGET map_creator PROPERTY CXX_STANDARD 23)

enable_target_warnings(map_creator)

target_link_libraries(map_creator
    PRIVATE hlmrl_lib
    PRIVATE raylib_system
    PRIVATE nlohmann_json::nlohmann_json
    PRIVATE entt_system
    PRIVATE fmt::fmt
)

target_include_directories(map_creator PRIVATE
    ${raygui_SOURCE_DIR}/src
)
