set(CMAKE_SYSTEM_NAME Generic)
set(CMAKE_SYSTEM_PROCESSOR arm)

if(NOT CMAKE_C_COMPILER)
    find_program(ARM_CC arm-none-eabi-gcc)
    set(CMAKE_C_COMPILER ${ARM_CC})
endif()

if(NOT CMAKE_CXX_COMPILER)
    find_program(ARM_CXX arm-none-eabi-g++)
    set(CMAKE_CXX_COMPILER ${ARM_CXX})
endif()

set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_PACKAGE ONLY)

# -mfloat-abi MUST match PcFirmWareV3_00/cmake/VdcToolchain.cmake and
# Embedded_VdcBoardLibs/PcLpc40xx/TestFirmware/Lpc4078TestToolchain.cmake
# (both -mfloat-abi=softfp). This is not just a style/warning-level mismatch:
# several static libraries in this workspace are linked in as prebuilt
# binaries (not recompiled from source per consumer), and hard/softfp/soft
# are three different, mutually incompatible calling conventions for how
# floats are passed to and returned from a function. Building this template
# (or a service derived from it) with a different -mfloat-abi than the
# consumer it links into does not necessarily fail at link time - it can
# link cleanly and then crash the moment a call crosses the ABI boundary
# with a float/double argument or return value, because caller and callee
# disagree on which register (or stack slot) the value lives in.
#
# -ffunction-sections/-fdata-sections is what lets the linker's --gc-sections
# actually discard unused functions per-function rather than per-object-file
# (see /BUILD-AND-SEARCH-POLICY.md - do not delete uncalled code, rely on this).
set(COMMON_FLAGS "-mcpu=cortex-m4 -mthumb -mfloat-abi=softfp -mfpu=fpv4-sp-d16 -ffunction-sections -fdata-sections")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${COMMON_FLAGS} -g -Wall")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${COMMON_FLAGS} -g -Wall")
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${COMMON_FLAGS} -Wl,--gc-sections")
