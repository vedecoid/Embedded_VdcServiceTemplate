# Architecture

This repository **is** the P-5 recipe. The root [`README.md`](../README.md)
is the source of truth for layout, independence, and how to copy the tree.
This folder does not replace it.

`Embedded_VdcServiceTemplate` is a working service named `Xxx`: it
builds, runs, and passes host tests. New services copy it, rename `Xxx`,
and replace `Process()`.

## Boundary the build enforces

```
Consumer  --includes-->  api/VdcXxxServiceApi.h
                         api/VdcXxxServiceConfig.h   (PUBLIC)
Implementation           inc/ + src/                 (PRIVATE)
Host tests / example     supply Board_Log shims
```

Doxygen `INPUT` is `api/` only, so generated docs **are** the contract.

## What the template already demonstrates

- **P-3** — `Board_Log` via `inc/VdcXxxServiceLog.h` (real header or
  local declaration when built alone).
- **P-4** — no short-lived heap; if you allocate, use `BLOCKMALLOC_*`.
- **P-5** — `api/` / `inc/` / `src/` / `example/` / `test/`.
- **P-7** — `_t` / `_e` suffixes on new types.
- **P-8** — product decisions arrive as `VdcXxxServiceHooks_t` function
  pointers, not `#include` of a product enum.

There is no product-specific architecture here. Keep the template aligned
with those rules — [`future-enhancements.md`](future-enhancements.md).
