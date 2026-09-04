# Developer guide

Follow the numbered “Starting a new service” list in
[`README.md`](../README.md). Summary:

1. Copy to `Embedded_Vdc<Name>Service`, branch `3.0.0`.
2. Rename every `Xxx` / `VDC_XXX_*` / `XxxServiceApiDocs`.
3. Replace hooks; validate mandatory members in `RegisterHooks`.
4. Put work in `Process()`; do not log every cycle (P-3 latency).
5. Decide single- vs multi-instance **now** (static context vs pointer).
6. Keep the tests — they prove the hooks pattern on the host.

If the service owns a FreeRTOS task: drop `Process()` from `api/`, keep
tunables in `VdcXxxServiceConfig.h`, keep core logic RTOS-type-free so
host tests still run. Details in the README section “Services that own a
FreeRTOS task”.

Do not add a product sensor enum to `api/`. If the service would not make
sense without that peer, it is a hook.
