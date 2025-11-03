cmake_minimum_required(VERSION 3.10)
project(Console-RPG CXX C)

# --- VENDORIZED PDCURSES LIBRARY SETUP ---

set(PDCURSES_ROOT_DIR "${CMAKE_CURRENT_SOURCE_DIR}/extern/pdcurses")

# Define file groups for PDCurses source files (Use case-insensitive globbing)
file(GLOB PDCURSES_COMMON_SRCS "${PDCURSES_ROOT_DIR}/pdcurses/*.[cC]")
file(GLOB PDCURSES_WINCON_SRCS "${PDCURSES_ROOT_DIR}/wincon/*.[cC]")

# DEBUGGING MESSAGES: SHOWS WHAT FILES WERE FOUND
# message(STATUS "COMMON PATH USED: ${PDCURSES_ROOT_DIR}/pdcurses/*.[cC]")
# message(STATUS "WINCON PATH USED: ${PDCURSES_ROOT_DIR}/wincon/*.[cC]")
# message(STATUS "COMMON SOURCES FOUND: ${PDCURSES_COMMON_SRCS}")
# message(STATUS "WINCON SOURCES FOUND: ${PDCURSES_WINCON_SRCS}")
# END DEBUGGING MESSAGES

# Create the static library target
add_library(pdcurses_lib STATIC
    ${PDCURSES_COMMON_SRCS}
    ${PDCURSES_WINCON_SRCS}
)

# Set include path for PDCurses headers (curses.h, panel.h)
target_include_directories(pdcurses_lib PUBLIC
    ${PDCURSES_ROOT_DIR}
)

# Add compiler definitions for WIDE and UTF-8 support
target_compile_definitions(pdcurses_lib PUBLIC
    PDC_WIDE
    PDC_FORCE_UTF8
    # We can keep the Windows version set, as it is standard practice
    _WIN32_WINNT=0x0601
    WINVER=0x0601
)

# --- GAME EXECUTABLE SETUP ---

# Define a file group for all of your game's source files
file(GLOB GAME_SRCS
    src/main.cpp
    src/characters/*.cpp
    src/maps/*.cpp
    src/game.cpp
)

# Create the executable target
add_executable(Console-RPG ${GAME_SRCS})

# Include directory for your custom headers (tile.h, map.h, etc.)
target_include_directories(Console-RPG PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

# Link your executable to the custom-built PDCurses library
target_link_libraries(Console-RPG
    pdcurses_lib
)

# Add system libraries required by PDCurses on Windows
if (WIN32)
    target_link_libraries(Console-RPG
        user32
        gdi32
    )
endif()