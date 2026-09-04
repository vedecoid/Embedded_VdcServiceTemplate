# API

Public prototypes live in:

- [`api/VdcXxxServiceApi.h`](../api/VdcXxxServiceApi.h) — `Init` / `Start` /
  `Stop` / `IsRunning` / `Process` / `GetProcessedCount`, `VdcXxxResult_e`
- [`api/VdcXxxServiceConfig.h`](../api/VdcXxxServiceConfig.h) — stack/prio/
  period tunables, `VdcXxxServiceHooks_t`, `VdcXxxService_RegisterHooks`

After rename, those become `Vdc<Name>Service*`. Doxygen scans `api/` only
(`cmake --build build --target XxxServiceApiDocs` → `api/doc/html`).

There is no additional C API in this template.
