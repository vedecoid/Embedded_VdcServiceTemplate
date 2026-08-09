# Host tests. Run with:  ctest --test-dir build --output-on-failure
add_executable(VdcXxxService_Tests test_VdcXxxService.c)

target_link_libraries(VdcXxxService_Tests PRIVATE VdcXxxService)

add_test(NAME VdcXxxService_Tests COMMAND VdcXxxService_Tests)

# On-target tests live in test/firmware/ and are built by the product, not
# here — they need a board, a linker script and a flashing step.
