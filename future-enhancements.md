# Future enhancements

No product-specific work belongs in this repository. It is a cloneable
P-5 skeleton, not a Pool Commander subsystem.

Keep the template in **lockstep with P-3 / P-4 / P-5** (and P-7/P-8 as
they appear in the README):

- **P-3** — diagnostics only through `Board_Log`; no `printf` in `src/`;
  respect the ISR logging ceiling.
- **P-4** — short-lived buffers from `VdcMemAlloc` block pools, not
  `malloc` / `pvPortMalloc`.
- **P-5** — `api/` PUBLIC, `inc/` PRIVATE, Doxygen on `api/` only,
  `example/` singular, host tests that run the service without a board.

When those standing rules gain a new obligation (for example a logging
argument-lifetime rule), update the template in the same spirit as the
README so the next copied service inherits it. Do not add a sample
product table or sensor enum “to make it more realistic”.
