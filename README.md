# Vdc Service Template

Lightweight template project for implementing a Vdc-style embedded service.

This repository provides a minimal, well-structured starting point for new Vdc services with:

- A small static library target (`VdcxxxService`) in `src/` + `include/`
- A runnable example (`example/main.c`) to exercise the service in a host or cross-compile build
- A small `cmake` toolchain and `linker.ld` example for ARM Cortex-M cross-compiles
- VS Code `launch.json` configured for Cortex-Debug (JLink/OpenOCD) and a GDB option

## Structure

- `src/` — service implementation (`VdcxxxService.c`)
- `include/` — public API (`VdcxxxServiceApi.h`)
- `example/` — small example runner (`main.c`)
- `cmake/` — illustrative `arm-gcc-toolchain.cmake`
- `linker.ld` — example linker script for quick testing
- `.vscode/` — launch configurations

## Build (host or cross)

Build with CMake. For a native host build (to run the example locally):

```powershell
cmake -S . -B build -DCMAKE_BUILD_TYPE=Debug
cmake --build build --config Debug
```

To cross-compile for ARM Cortex-M using the included example toolchain file:

```powershell
cmake -S . -B build-arm \
  -DCMAKE_TOOLCHAIN_FILE=cmake/arm-gcc-toolchain.cmake \
  -DVDC_MCU_CPU=cortex-m4 \
  -DVDC_MCU_FPU=fpv4-sp-d16 \
  -DVDC_MCU_FLOAT_ABI=softfp
cmake --build build-arm --config Release
```

Notes:
- The `cmake/arm-gcc-toolchain.cmake` is illustrative — adapt paths and compiler locations to your environment.
- Provide a suitable `linker.ld` for your MCU before flashing.

## Debugging

Open this folder in VS Code (or the workspace) and use the provided launch configurations in `.vscode/launch.json`.
The cortex-debug configurations reference `../svd/LPC408x_7x.svd` for register views; update or remove if you don't use an SVD.

## Using as a dependency

You can consume this template as a CMake subproject or with CPM. Example using CPM in a consumer project:

```cmake
include(cmake/CPM.cmake)

CPMAddPackage(
  NAME VdcServiceTemplate
  GIT_REPOSITORY https://github.com/vedecoid/Embedded_VdcServiceTemplate.git
  GIT_TAG main
)

target_link_libraries(your_app PRIVATE VdcxxxService)
```

## Contributing

This template is intended to be copied or forked as a starting point for new Vdc services. Keep implementation specifics out of the template — add device/board-specific code in the consumer project or a dedicated board layer.

## License

Follow the same license used by the rest of the `Embedded_VdcPropertyService` organization (no license file included here by default). Add one as appropriate for your project.
