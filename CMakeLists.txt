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

target_compile_definitions(pdcurses_lib PUBLIC
    PDC_WIDE
    PDC_FORCE_UTF8
    _WIN32_WINNT=0x0601
    WINVER=0x0601
)

# --- SDL2 Setup (Dynamic) ---
set(SDL2_ROOT "${CMAKE_CURRENT_SOURCE_DIR}/extern/SDL2")
set(SDL2_MIXER_ROOT "${CMAKE_CURRENT_SOURCE_DIR}/extern/SDL2_mixer")

# Define paths
set(SDL2_INCLUDE_DIR       "${SDL2_ROOT}/include")
set(SDL2_LIB_DIR           "${SDL2_ROOT}/lib") # Adjust 'x64' if your folder is different
set(SDL2_BIN_DIR           "${SDL2_ROOT}/bin")

set(SDL2_MIXER_INCLUDE_DIR "${SDL2_MIXER_ROOT}/include")
set(SDL2_MIXER_LIB_DIR     "${SDL2_MIXER_ROOT}/lib")
set(SDL2_MIXER_BIN_DIR     "${SDL2_MIXER_ROOT}/bin")

message(STATUS "DEBUG: SDL2 root is: ${SDL2_ROOT}")
message(STATUS "DEBUG: Looking for SDL2 libs in: ${SDL2_LIB_DIR}")
# Find libraries
find_library(SDL2MAIN_LIB NAMES SDL2main PATHS ${SDL2_LIB_DIR} NO_DEFAULT_PATH)
find_library(SDL2_LIB NAMES SDL2 PATHS ${SDL2_LIB_DIR} NO_DEFAULT_PATH)
find_library(SDL2_MIXER_LIB NAMES SDL2_mixer PATHS ${SDL2_MIXER_LIB_DIR} NO_DEFAULT_PATH)

# Collect DLLs
file(GLOB SDL2_DLLS "${SDL2_BIN_DIR}/*.dll" "${SDL2_MIXER_BIN_DIR}/*.dll")

# ==============================================================================
# 2. GAME EXECUTABLE SETUP
# ==============================================================================

file(GLOB GAME_SRCS
    src/main.cpp
    src/characters/*.cpp
    src/maps/*.cpp
    src/items/*.cpp
    src/items/Equipables/*.cpp
    src/items/Usables/*.cpp
    src/game.cpp
    src/audiomanager.cpp
    src/combat/*.cpp
)

add_executable(Console-RPG ${GAME_SRCS})

# --- Include Paths ---
target_include_directories(Console-RPG PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    "${SDL2_ROOT}/include"
    "${SDL2_ROOT}/include/SDL2"        # <--- ADD THIS LINE
    "${SDL2_MIXER_ROOT}/include"
    "${SDL2_MIXER_ROOT}/include/SDL2"  # <--- ADD THIS LINE (for safety)
)

# --- Linking Libraries ---
# NOTE: Link order matters for SDL2! SDL2main must often come before SDL2.
target_link_libraries(Console-RPG
    pdcurses_lib
    mingw32           # Must be linked BEFORE SDL2main for MinGW
    ${SDL2MAIN_LIB}
    ${SDL2_LIB}
    ${SDL2_MIXER_LIB}
)

if (WIN32)
    target_link_libraries(Console-RPG
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

add_custom_command(TARGET Console-RPG POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
    ${SDL2_DLLS}
    $<TARGET_FILE_DIR:Console-RPG>
    COMMENT "Copying SDL2 runtime DLLs to output directory"
)