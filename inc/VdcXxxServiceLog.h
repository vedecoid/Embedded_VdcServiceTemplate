/**
 * @file VdcXxxServiceLog.h
 * @brief Resolves Board_Log for P-3, with a fallback for standalone builds.
 *
 * Standing rule P-3: diagnostic output goes through Board_Log() and nowhere
 * else. No printf, no putchar, no direct peripheral writes — the sink is a
 * deployment decision, not a code decision.
 *
 * When this repository is built on its own there is no VdcLoggingService to
 * link against, which must not become an excuse to reach for printf. So the
 * declaration is taken from the real service when it is on the include path,
 * and otherwise from the minimal stand-in below. Call sites are identical
 * either way, so nothing has to change when the service is integrated.
 *
 * The *definition* for a standalone build is supplied by the consumer —
 * example/ and test/ each provide one.
 */

#pragma once

#if defined(__has_include)
#if __has_include("VdcLoggingServiceApi.h")
#define VDC_XXX_HAS_LOGGING_SERVICE 1
#endif
#endif

#if defined(VDC_XXX_HAS_LOGGING_SERVICE)

#include "VdcLoggingServiceApi.h"

#else

/**
 * Stand-in only. Values match board_api_types.h so that call sites written
 * against this fallback keep their meaning once the real service is linked.
 * Levels are OR-able categories, not a severity ladder: pick the subject
 * first, then OR in LOG_ERROR on a failure path.
 */
typedef enum
{
	LOG_STARTUP = 0x001,
	LOG_BOARD   = 0x002,
	LOG_CONFIG  = 0x008,
	LOG_DEBUG   = 0x020,
	LOG_ERROR   = 0x040,
	LOG_EVENT   = 0x080,
	LOG_APP     = 0x400
} eLogLevel;

void Board_Log(eLogLevel level, const char *format, ...);

#endif /* VDC_XXX_HAS_LOGGING_SERVICE */
