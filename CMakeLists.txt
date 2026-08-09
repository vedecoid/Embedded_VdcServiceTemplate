cmake_minimum_required(VERSION 3.15)
project(VdcXxxService C)

# Off by default when consumed as a subproject: a product pulling this in with
# CPM wants the library, not our example and tests.
if(CMAKE_SOURCE_DIR STREQUAL CMAKE_CURRENT_SOURCE_DIR)
    set(_vdc_xxx_standalone ON)
else()
    set(_vdc_xxx_standalone OFF)
endif()

option(VDC_XXX_BUILD_EXAMPLE "Build the example runner"        ${_vdc_xxx_standalone})
option(VDC_XXX_BUILD_TESTS   "Build the host tests"            ${_vdc_xxx_standalone})
option(VDC_XXX_ENABLE_DOCS   "Generate API docs with Doxygen"  ${_vdc_xxx_standalone})

#------------------------------------------------------------------------------
# The library
#------------------------------------------------------------------------------
add_library(VdcXxxService STATIC
    src/VdcXxxService.c
)

# api/ is PUBLIC and inc/ is PRIVATE. This is the P-5 boundary expressed in the
# build: a consumer physically cannot include an internal header, so the public
# surface cannot drift by accident.
target_include_directories(VdcXxxService
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/api
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/inc
)

target_compile_features(VdcXxxService PUBLIC c_std_11)

target_compile_options(VdcXxxService PRIVATE
    $<$<C_COMPILER_ID:GNU,Clang>:-Wall -Wextra -Wpedantic>
)

#------------------------------------------------------------------------------
# Example and tests
#------------------------------------------------------------------------------
if(VDC_XXX_BUILD_EXAMPLE)
    add_subdirectory(example)
endif()

if(VDC_XXX_BUILD_TESTS)
    enable_testing()
    add_subdirectory(test)
endif()

#------------------------------------------------------------------------------
# API documentation
#------------------------------------------------------------------------------
# Doxyfile.in sets INPUT to api/ only, so the generated documentation is the
# public contract by construction. Regenerate with:
#   cmake --build build --target XxxServiceApiDocs
if(VDC_XXX_ENABLE_DOCS)
    find_package(Doxygen)
    if(DOXYGEN_FOUND)
        set(_vdc_docs_dir ${CMAKE_CURRENT_SOURCE_DIR}/api/doc)
        set(VDC_DOCS_PROJECT_NAME "VdcXxxService API")
        set(VDC_DOCS_PROJECT_VERSION "3.0.0")
        configure_file(
            ${_vdc_docs_dir}/Doxyfile.in
            ${_vdc_docs_dir}/Doxyfile
            @ONLY
        )
        add_custom_target(XxxServiceApiDocs
            COMMAND ${DOXYGEN_EXECUTABLE} Doxyfile
            WORKING_DIRECTORY ${_vdc_docs_dir}
            COMMENT "Generating VdcXxxService API documentation"
            VERBATIM
        )
    else()
        message(STATUS "Doxygen not found - XxxServiceApiDocs target not available")
    endif()
endif()
