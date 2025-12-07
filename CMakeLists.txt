cmake_minimum_required(VERSION 3.15)
project(VdcxxxService C)

# Library
add_library(VdcxxxService STATIC
    src/VdcxxxService.c
)

target_include_directories(VdcxxxService
    PUBLIC
        include
    PRIVATE
        src
)

# Example Executable
add_executable(VdcxxxService_Example
    example/main.c
)

target_link_libraries(VdcxxxService_Example PRIVATE VdcxxxService)

if(CMAKE_SYSTEM_NAME STREQUAL "Generic")
    target_link_options(VdcxxxService_Example PRIVATE "-T${CMAKE_CURRENT_SOURCE_DIR}/linker.ld" "--specs=nosys.specs")
endif()

