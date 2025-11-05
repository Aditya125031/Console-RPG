cmake_minimum_required(VERSION 3.10)
project(Console-RPG CXX C)

# ==============================================================================
# 1. DEPENDENCY SETUP
# ==============================================================================

# --- PDCurses Setup (Static) ---
set(PDCURSES_ROOT_DIR "${CMAKE_CURRENT_SOURCE_DIR}/extern/pdcurses")

file(GLOB PDCURSES_COMMON_SRCS "${PDCURSES_ROOT_DIR}/pdcurses/*.[cC]")
file(GLOB PDCURSES_WINCON_SRCS "${PDCURSES_ROOT_DIR}/wincon/*.[cC]")

add_library(pdcurses_lib STATIC
    ${PDCURSES_COMMON_SRCS}
    ${PDCURSES_WINCON_SRCS}
)

target_include_directories(pdcurses_lib PUBLIC ${PDCURSES_ROOT_DIR})

# PDCurses definitions for Windows/Unicode
target_compile_definitions(pdcurses_lib PUBLIC
    PDC_WIDE
    PDC_FORCE_UTF8
    _WIN32_WINNT=0x0601
    WINVER=0x0601
)

# --- SDL3 Setup (Dynamic) ---
set(SDL3_ROOT "${CMAKE_CURRENT_SOURCE_DIR}/extern/SDL3")
set(SDL2_MIXER_ROOT "${CMAKE_CURRENT_SOURCE_DIR}/extern/SDL2_mixer")

# Define paths based on the folder structure you created
set(SDL3_INCLUDE_DIR "${SDL3_ROOT}/include")
set(SDL3_LIB_DIR     "${SDL3_ROOT}/lib")
set(SDL3_BIN_DIR     "${SDL3_ROOT}/bin")

set(SDL2_MIXER_INCLUDE_DIR "${SDL2_MIXER_ROOT}/include")
set(SDL2_MIXER_LIB_DIR     "${SDL2_MIXER_ROOT}/lib")
set(SDL2_MIXER_BIN_DIR     "${SDL2_MIXER_ROOT}/bin")

# Find the main SDL3 library file to link against
find_library(SDL3_LIB NAMES SDL3 PATHS ${SDL3_LIB_DIR} NO_DEFAULT_PATH)
find_library(SDL2_MIXER_LIB NAMES SDL2_mixer PATHS ${SDL2_MIXER_LIB_DIR} NO_DEFAULT_PATH)

# Find all DLLs in the bin folder to copy later (SDL3.dll)
file(GLOB SDL3_DLLS "${SDL3_BIN_DIR}/*.dll" "${SDL2_MIXER_BIN_DIR}/*.dll")

# ==============================================================================
# 2. GAME EXECUTABLE SETUP
# ==============================================================================

# Gather all your source files
file(GLOB GAME_SRCS
    src/main.cpp
    src/characters/*.cpp
    src/maps/*.cpp
    src/items/*.cpp
    src/items/Equipables/*.cpp
    src/items/Usables/*.cpp
    src/game.cpp
)

# Create the game executable
add_executable(Console-RPG ${GAME_SRCS})

# --- Include Paths ---
# Allows #include "game.h" and #include <SDL3/SDL.h>
target_include_directories(Console-RPG PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${SDL3_INCLUDE_DIR}
    ${SDL2_MIXER_INCLUDE_DIR}
)

# --- Linking Libraries ---
target_link_libraries(Console-RPG
    pdcurses_lib
    ${SDL3_LIB}
    ${SDL2_MIXER_LIB}
)

# Additional system libraries often needed by PDCurses/SDL on Windows
if (WIN32)
    target_link_libraries(Console-RPG
        mingw32 # Important for MinGW entry points
        user32
        gdi32
        winmm
        imm32
        ole32
        oleaut32
        version
        uuid
        advapi32
        setupapi
        shell32
    )
endif()

# ==============================================================================
# 3. POST-BUILD STEPS
# ==============================================================================

# Automatically copy SDL3.dll next to Console-RPG.exe after every build
add_custom_command(TARGET Console-RPG POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
    ${SDL3_DLLS}
    $<TARGET_FILE_DIR:Console-RPG>
    COMMENT "Copying runtime DLLs to output directory"
)