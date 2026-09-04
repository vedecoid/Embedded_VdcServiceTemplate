# Troubleshooting

## Something broke after I copied the template

The template is a working service. Bisect your rename and `Process()`
body. Host `ctest` should still pass if hooks are registered and
`Process` stays free of board/RTOS types.

## Consumer can `#include` an `inc/` header

That is a CMake bug: `inc/` must stay `PRIVATE`. Do not add it to the
public include path. Extend `api/` instead (README / P-5).

## `VDC_XXX_ERR_NOHOOKS` at Start

`RegisterHooks` after `Init`, before `Start`, with every mandatory
function pointer non-NULL. Optional `publish` may be NULL.

## `Board_Log` undefined when building the library alone

Call sites use the shim in `inc/VdcXxxServiceLog.h`. The **definition**
comes from `example/` or `test/` (or the real logging service in a
product). Do not add `printf` to `src/`.

## Cross-compile links then crashes on first float

`-mfloat-abi` mismatch against a prebuilt consumer. Keep `softfp`.

## Tempted to delete uncalled helpers

Don’t. `-ffunction-sections`/`--gc-sections` already drop true dead
code; uncalled symbols may be copy-paste surface for the next service
(B-1).
