# Embedded_VdcServiceTemplate

The starting point for a new `Embedded_Vdc<Name>Service` repository. Copy it, rename `Xxx`, replace the body
of `Process()`, and delete what you don't need.

It is a **working service**, not a skeleton: it builds, runs and passes its tests as-is, so if something
breaks after you copy it, the breakage is yours. What it demonstrates is the part that is easy to get wrong —
the lifecycle, the public/internal boundary, and how a service reaches its host without depending on it.

## Layout — standing rule `P-5`

| Folder | Holds | Visible to consumers |
|---|---|---|
| `api/` | **The public surface, and nothing else.** `VdcXxxServiceApi.h` + `VdcXxxServiceConfig.h` | **Yes** (CMake `PUBLIC`) |
| `inc/` | Internal headers | No (CMake `PRIVATE`) |
| `src/` | Implementation | No |
| `example/` | Runnable integration — singular, not `examples/` | — |
| `test/` | Host tests; on-target tests under `test/firmware/` | — |
| `api/doc/` | Doxygen scaffolding; `INPUT` is `api/` only | — |
| `cmake/` | Illustrative ARM cross-compile toolchain | — |

The boundary is enforced by the build, not by convention: `api/` is a `PUBLIC` include directory and `inc/` is
`PRIVATE`, so **a consumer physically cannot include an internal header.** If you find yourself wanting to,
the public API is missing something — extend `api/` rather than widening the include path.

Doxygen scans `api/` only, so the generated documentation *is* the contract. Anything undocumented there is
provably not part of it.

## Three rules this template already obeys

- **`P-3` — all diagnostics go through `Board_Log()`.** No `printf` in `src/`. Levels are OR-able categories,
  not a severity ladder, so a failed write is `LOG_CONFIG | LOG_ERROR`. `inc/VdcXxxServiceLog.h` picks up the
  real `VdcLoggingServiceApi.h` when it is on the include path and falls back to a local declaration when the
  repository is built alone — call sites never change. The *definition* for a standalone build comes from the
  consumer, which is why `example/` and `test/` each supply one; that mirrors `board_app_glue_weak.c` in
  BoardLibs. `printf` inside those shims is fine: they stand in for the logging backend.
- **`P-4` — short-lived allocations come from `VdcMemAlloc` block pools.** This template allocates nothing, so
  there is nothing to show. If your service needs a per-operation buffer, use `BLOCKMALLOC_LARGE` /
  `BLOCKFREE_LARGE`, check for `NULL`, and free on every path including errors.
- **Independence.** A service may not name a product type — no `eSensorId`, no `TriggerHandle_e`, no
  `VdcPubSub`, no product time service. Note that `readInput` takes a plain `uint32_t channel` for exactly
  this reason. Everything the host provides arrives as a function pointer in `VdcXxxServiceHooks_t`.

## Build and test

```powershell
cmake -S . -B build
cmake --build build
ctest --test-dir build --output-on-failure
```

The example and tests build only when this is the top-level project, so a product consuming it via CPM gets
just the library.

```powershell
cmake --build build --target XxxServiceApiDocs   # writes api/doc/html (gitignored)
```

Cross-compiling the library:

```powershell
cmake -S . -B build-arm -DCMAKE_TOOLCHAIN_FILE=cmake/arm-gcc-toolchain.cmake `
  -DVDC_MCU_CPU=cortex-m4 -DVDC_MCU_FPU=fpv4-sp-d16 -DVDC_MCU_FLOAT_ABI=softfp
```

There is no linker script here on purpose: this repository produces a **library**. Building a firmware image
is the product's job.

## Starting a new service

1. Copy the folder to `Embedded_Vdc<Name>Service` and re-init git on branch `3.0.0`.
2. Rename `Xxx` to `<Name>` in file names, symbols and CMake targets — including the `XxxServiceApiDocs`
   target and the `VDC_XXX_*` options. Nothing else in the tree contains `Xxx`.
3. Replace `VdcXxxServiceHooks_t` with what your service actually needs from its host, and validate the
   mandatory members in `RegisterHooks` so a missing dependency fails at configuration time rather than as a
   null call inside `Process()`.
4. Put the real work in `Process()`. Keep it free of logging — it runs every cycle, and `P-3` forbids
   unthrottled logging on a periodic path. Log state changes instead.
5. Decide **now** whether the service is single- or multi-instance. This template uses one static context; a
   multi-instance service takes a context pointer as the first argument of every public function. Changing
   your mind later is an API break.
6. Keep the tests. They are the evidence that the hooks pattern works — the whole service runs on the build
   machine with no board and no RTOS.

## Services that own a FreeRTOS task

This template exports `Process()` and lets the host drive it, which is the simpler contract and the one that
stays host-testable. When the service owns its task instead, keep `Init` / `Start` / `Stop` / `IsRunning`,
drop `Process()` from `api/`, and call it from the task loop using
`VdcXxxService_TASK_STACKBYTESIZE`, `VdcXxxService_TASK_PRIO` and `VdcXxxService_PROCESS_PERIOD_MS` from
`VdcXxxServiceConfig.h`. Those are `#ifndef`-guarded so a product can size the task without patching the
library. Keep the core logic in functions that take no RTOS types, so the tests still run on the host.

## Consuming it

```cmake
CPMAddPackage(
  NAME Embedded_VdcXxxService
  GIT_REPOSITORY https://github.com/vedecoid/Embedded_VdcXxxService.git
  GIT_TAG 3.0.0
)

target_link_libraries(your_app PRIVATE VdcXxxService)
```

Products consume services with **CPM**. The submodules in the workspace superproject exist only to record
which commit belongs in which directory for reconstruction — they are not the build mechanism.
