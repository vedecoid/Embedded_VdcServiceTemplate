/**
 * @file VdcXxxService.c
 * @brief Reference implementation of the service lifecycle.
 *
 * Copy this repository, rename Xxx throughout, and replace the body of
 * VdcXxxService_Process() with the real work. The lifecycle, the hook
 * validation and the logging pattern are the parts worth keeping.
 */

#include "VdcXxxService.h"
#include "VdcXxxServiceLog.h"

/*
 * Single static instance. A service that must support several instances takes
 * a context pointer as the first argument of every public function instead —
 * decide that before the first consumer exists, because it is an API break.
 */
static VdcXxxServiceContext_t gContext;

VdcXxxServiceContext_t *VdcXxxService_GetContext(void)
{
	return &gContext;
}

eVdcXxxResult VdcXxxService_Init(void)
{
	gContext.initialised    = true;
	gContext.running        = false;
	gContext.processedCount = 0U;
	gContext.lastTimestampMs = 0U;
	gContext.hooks.getTimestamp = NULL;
	gContext.hooks.readInput    = NULL;
	gContext.hooks.publish      = NULL;

	Board_Log(LOG_STARTUP, "VdcXxxService: initialised");
	return VDC_XXX_OK;
}

eVdcXxxResult VdcXxxService_RegisterHooks(const VdcXxxServiceHooks_t *hooks)
{
	if (!gContext.initialised)
	{
		Board_Log(LOG_CONFIG | LOG_ERROR, "VdcXxxService: hooks before init");
		return VDC_XXX_ERR_NOTINITIALISED;
	}
	if (gContext.running)
	{
		Board_Log(LOG_CONFIG | LOG_ERROR, "VdcXxxService: hooks while running");
		return VDC_XXX_ERR_ALREADYRUNNING;
	}
	/* Mandatory hooks are checked here, not at the call site, so a missing
	   dependency fails loudly at configuration time rather than as a null
	   call somewhere in the middle of Process(). */
	if ((hooks == NULL) || (hooks->getTimestamp == NULL) || (hooks->readInput == NULL))
	{
		Board_Log(LOG_CONFIG | LOG_ERROR, "VdcXxxService: mandatory hook missing");
		return VDC_XXX_ERR_BADPARAMETER;
	}

	gContext.hooks = *hooks;
	Board_Log(LOG_CONFIG, "VdcXxxService: hooks registered");
	return VDC_XXX_OK;
}

eVdcXxxResult VdcXxxService_Start(void)
{
	if (!gContext.initialised)
	{
		return VDC_XXX_ERR_NOTINITIALISED;
	}
	if (gContext.running)
	{
		return VDC_XXX_ERR_ALREADYRUNNING;
	}
	if (gContext.hooks.getTimestamp == NULL)
	{
		Board_Log(LOG_STARTUP | LOG_ERROR, "VdcXxxService: start without hooks");
		return VDC_XXX_ERR_NOHOOKS;
	}

	gContext.running = true;
	Board_Log(LOG_STARTUP, "VdcXxxService: started");
	return VDC_XXX_OK;
}

eVdcXxxResult VdcXxxService_Stop(void)
{
	if (!gContext.initialised)
	{
		return VDC_XXX_ERR_NOTINITIALISED;
	}
	if (!gContext.running)
	{
		return VDC_XXX_ERR_NOTRUNNING;
	}

	gContext.running = false;
	Board_Log(LOG_STARTUP, "VdcXxxService: stopped after %u cycles",
	          (unsigned)gContext.processedCount);
	return VDC_XXX_OK;
}

bool VdcXxxService_IsRunning(void)
{
	return gContext.running;
}

eVdcXxxResult VdcXxxService_Process(void)
{
	int32_t value = 0;

	if (!gContext.running)
	{
		return VDC_XXX_ERR_NOTRUNNING;
	}

	gContext.lastTimestampMs = gContext.hooks.getTimestamp();

	/* Replace with the real work. Note there is no logging on this path: it
	   runs every cycle, and P-3 forbids unthrottled logging on a periodic
	   path. Log state changes, not steady state. */
	if (gContext.hooks.readInput(0U, &value))
	{
		if (gContext.hooks.publish != NULL)
		{
			gContext.hooks.publish(0U, value);
		}
		gContext.processedCount++;
	}

	return VDC_XXX_OK;
}

uint32_t VdcXxxService_GetProcessedCount(void)
{
	return gContext.processedCount;
}
