/**
 * @file main.c
 * @brief Minimal integration of VdcXxxService, buildable with no dependencies.
 *
 * Shows the three things every integration does: supply the host dependencies
 * as hooks, drive the lifecycle in order, and provide a logging sink.
 *
 * Note what is *not* here: no include of anything from inc/. An example that
 * needs an internal header is evidence the public API is incomplete.
 */

#include "VdcXxxServiceApi.h"
#include "VdcXxxServiceConfig.h"

#include <stdarg.h>
#include <stdio.h>

/*---------------------------------------------------------------------------*/
/* Logging sink                                                               */
/*---------------------------------------------------------------------------*/
/*
 * In a product this comes from VdcLoggingService and this block disappears.
 * Standalone, the consumer supplies the definition — the same arrangement
 * BoardLibs uses in board_app_glue_weak.c when VDC_BOARD_BUILD_LOGGING=OFF.
 *
 * printf appears *here*, in a host-only shim that stands in for the logging
 * backend. That is the one place P-3 permits it: the rule governs how library
 * and application code emits diagnostics, not how a backend finally writes
 * them out. Service code calls Board_Log and nothing else.
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

void Board_Log(eLogLevel level, const char *format, ...)
{
	va_list args;
	va_start(args, format);
	(void)printf("[%03x] ", (unsigned)level);
	(void)vprintf(format, args);
	(void)printf("\n");
	va_end(args);
}

/*---------------------------------------------------------------------------*/
/* Host dependencies                                                          */
/*---------------------------------------------------------------------------*/

static uint32_t gFakeClockMs = 0U;

static uint32_t ExampleGetTimestamp(void)
{
	gFakeClockMs += VdcXxxService_PROCESS_PERIOD_MS;
	return gFakeClockMs;
}

static bool ExampleReadInput(uint32_t channel, int32_t *value)
{
	if (value == NULL)
	{
		return false;
	}
	*value = (int32_t)(channel + (gFakeClockMs / 100U));
	return true;
}

static void ExamplePublish(uint32_t channel, int32_t value)
{
	Board_Log(LOG_APP, "publish ch=%u value=%d", (unsigned)channel, (int)value);
}

int main(void)
{
	const VdcXxxServiceHooks_t hooks = {
		.getTimestamp = ExampleGetTimestamp,
		.readInput    = ExampleReadInput,
		.publish      = ExamplePublish,
	};

	if (VdcXxxService_Init() != VDC_XXX_OK)
	{
		return 1;
	}
	if (VdcXxxService_RegisterHooks(&hooks) != VDC_XXX_OK)
	{
		return 1;
	}
	if (VdcXxxService_Start() != VDC_XXX_OK)
	{
		return 1;
	}

	for (int i = 0; i < 5; i++)
	{
		if (VdcXxxService_Process() != VDC_XXX_OK)
		{
			return 1;
		}
	}

	(void)VdcXxxService_Stop();
	return 0;
}
