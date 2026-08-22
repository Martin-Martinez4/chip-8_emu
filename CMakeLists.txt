# =============================================
# chip8-emu - CMake Configuration
# Cross-platform: Windows + Linux
# =============================================
cmake_minimum_required(VERSION 3.10)
project(chip8_emu LANGUAGES C)


# =============================================
# Source Files
# =============================================
set(sources
${CMAKE_SOURCE_DIR}/src/opcodes.c
${CMAKE_SOURCE_DIR}/src/cpu.c
${CMAKE_SOURCE_DIR}/src/graphics.c
${CMAKE_SOURCE_DIR}/src/error_helpers.c
)

# May not be needed for tests so it is separate
set(app_sources
${CMAKE_SOURCE_DIR}/src/main.c
)


add_subdirectory(vendored/SDL3 EXCLUDE_FROM_ALL)

# =============================================
# Executable Target
# =============================================
if(WIN32)
add_executable(${CMAKE_PROJECT_NAME} ${sources} ${app_sources})
else()
add_executable(${CMAKE_PROJECT_NAME} ${sources} ${app_sources})
endif()

# =============================================
# Include Dirs / header files
# =============================================
set(source_dirs
    ${CMAKE_SOURCE_DIR}/src
)


target_include_directories(
    ${CMAKE_PROJECT_NAME} PUBLIC
    ${source_dirs}
)




# =============================================
# Link Libs
# =============================================
# Will have more later
target_link_libraries(${CMAKE_PROJECT_NAME} PUBLIC SDL3::SDL3 )


# --------------------------------------------
# Test executable
# --------------------------------------------

set(test_sources
    ${CMAKE_SOURCE_DIR}/test/main.c
    ${CMAKE_SOURCE_DIR}/test/test_opcodes_0_to_7.c
    ${CMAKE_SOURCE_DIR}/test/test_opcodes_8.c
    ${CMAKE_SOURCE_DIR}/test/test_cpu_helpers.c
)

set(test_source_dirs
    ${CMAKE_SOURCE_DIR}/test
)


add_executable(${CMAKE_PROJECT_NAME}_test 
    ${sources} 
    ${test_sources}
)

target_include_directories(${CMAKE_PROJECT_NAME}_test PUBLIC  
    ${source_dirs} 
    ${test_source_dirs}
)

target_link_libraries(${CMAKE_PROJECT_NAME}_test PUBLIC SDL3::SDL3 )



