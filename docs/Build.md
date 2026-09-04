# Build

Host (library + example + tests), from the README:

```powershell
cmake -S . -B build
cmake --build build
ctest --test-dir build --output-on-failure
```

Example and tests build only when this tree is the CMake top-level project.
A product consuming via CPM gets the static library only.

```powershell
cmake --build build --target XxxServiceApiDocs
```

ARM library (no firmware image — no linker script here):

```powershell
cmake -S . -B build-arm -DCMAKE_TOOLCHAIN_FILE=cmake/arm-gcc-toolchain.cmake `
  -DVDC_MCU_CPU=cortex-m4 -DVDC_MCU_FPU=fpv4-sp-d16 -DVDC_MCU_FLOAT_ABI=softfp
cmake --build build-arm
```

`FLOAT_ABI` must stay **softfp** (B-4). Options:
`VDC_XXX_BUILD_EXAMPLE`, `VDC_XXX_BUILD_TESTS`, `VDC_XXX_ENABLE_DOCS`.
