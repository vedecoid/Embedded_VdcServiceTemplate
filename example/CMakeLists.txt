# Example runner. Links the library through its public target only, so if the
# example compiles, a consumer can compile.
add_executable(VdcXxxService_Example main.c)

target_link_libraries(VdcXxxService_Example PRIVATE VdcXxxService)

# Deliberately no linker script or --specs here: this repository builds a
# library, not a firmware image. Cross-compiling the *library* is covered by
# cmake/arm-gcc-toolchain.cmake; producing an image is the product's job.
