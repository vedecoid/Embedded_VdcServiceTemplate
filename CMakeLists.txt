cmake_minimum_required(VERSION 3.15)
project(VdcxxxService C)

add_library(VdcxxxService STATIC
    src/VdcxxxService.c
)

target_include_directories(VdcxxxService
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/api
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/src
)

add_executable(VdcxxxService_Example
    example/main.c
)

target_link_libraries(VdcxxxService_Example PRIVATE VdcxxxService)

if(CMAKE_SYSTEM_NAME STREQUAL "Generic")
    target_link_options(VdcxxxService_Example PRIVATE "-T${CMAKE_CURRENT_SOURCE_DIR}/linker.ld" "--specs=nosys.specs")
endif()

# API documentation (Doxygen). Regenerate when api/*.h changes:
#   cmake --build build --target XxxServiceApiDocs
#   .\api\doc\run-doxygen.ps1
option(VDC_XXX_ENABLE_DOCS "Generate API documentation with Doxygen" ON)
if(VDC_XXX_ENABLE_DOCS)
    find_package(Doxygen)
    if(DOXYGEN_FOUND)
        set(_vdc_docs_dir ${CMAKE_CURRENT_SOURCE_DIR}/api/doc)
        set(VDC_DOCS_PROJECT_NAME "VDC Service Template API")
        set(VDC_DOCS_PROJECT_VERSION "3.0.0")
        configure_file(
            ${_vdc_docs_dir}/Doxyfile.in
            ${_vdc_docs_dir}/Doxyfile
            @ONLY
        )
        add_custom_target(XxxServiceApiDocs
            COMMAND ${DOXYGEN_EXECUTABLE} Doxyfile
            WORKING_DIRECTORY ${_vdc_docs_dir}
            COMMENT "Generating VDC Service Template API documentation"
            VERBATIM
        )
    else()
        message(STATUS "Doxygen not found - XxxServiceApiDocs target not available")
    endif()
endif()
