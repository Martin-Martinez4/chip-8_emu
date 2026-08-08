# =============================================
# chip8-emu - CMake Configuration
# Cross-platform: Windows + Linux
# =============================================
cmake_minimum_required(VERSION 3.10)
project(chip-8_emu LANGUAGES C)

# =============================================
# Source Files
# =============================================
set(sources
    ${CMAKE_SOURCE_DIR}/src/opcodes.c
    ${CMAKE_SOURCE_DIR}/src/cpu.c
)

# May not be needed for tests so it is separate
set(app_sources
    ${CMAKE_SOURCE_DIR}/src/main.c
)

# =============================================
# Include Dirs / header files
# =============================================
set(source_dirs
    ${CMAKE_SOURCE_DIR}/src
)

# =============================================
# Executable Target
# =============================================
if(WIN32)
    add_executable(${CMAKE_PROJECT_NAME} ${sources} ${app_sources})
else()
    add_executable(${CMAKE_PROJECT_NAME} ${sources} ${app_sources})
endif()

# =============================================
# Link Libs
# =============================================
# Will have more later
target_link_libraries(${CMAKE_PROJECT_NAME} PUBLIC)


