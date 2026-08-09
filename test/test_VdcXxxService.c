/**
 * @file test_VdcXxxService.c
 * @brief Host tests for the service lifecycle. No hardware, no RTOS.
 *
 * The point of the hooks pattern is visible here: because every dependency is
 * a function pointer, the whole service is testable on the build machine with
 * no board, no FreeRTOS and no mocking framework.
 *
 * Deliberately dependency-free so the template builds anywhere. Add Unity or
 * CMocka in a real service if the suite grows.
 */

#include "VdcXxxServiceApi.h"
#include "VdcXxxServiceConfig.h"

#include <stdarg.h>
#include <stdio.h>

static int gFailures = 0;

#define CHECK(cond)                                                     \
	do {                                                                \
		if (!(cond)) {                                                  \
			(void)printf("FAIL %s:%d  %s\n", __FILE__, __LINE__, #cond); \
			gFailures++;                                                \
		}                                                               \
	} while (0)

/* Logging sink for the test build — see example/main.c for why this is here. */
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
	(void)level;
	(void)format;
}

/*---------------------------------------------------------------------------*/
/* Test doubles                                                               */
/*---------------------------------------------------------------------------*/

static uint32_t gClockMs      = 0U;
static bool     gReadSucceeds = true;
static uint32_t gPublishCount = 0U;

static uint32_t FakeTimestamp(void)      { gClockMs += 10U; return gClockMs; }
static void     FakePublish(uint32_t c, int32_t v) { (void)c; (void)v; gPublishCount++; }

static bool FakeReadInput(uint32_t channel, int32_t *value)
{
	(void)channel;
	if (!gReadSucceeds || (value == NULL))
	{
		return false;
	}
	*value = 42;
	return true;
}

static VdcXxxServiceHooks_t ValidHooks(void)
{
	VdcXxxServiceHooks_t hooks;
	hooks.getTimestamp = FakeTimestamp;
	hooks.readInput    = FakeReadInput;
	hooks.publish      = FakePublish;
	return hooks;
}

static void ResetFixture(void)
{
	gClockMs      = 0U;
	gReadSucceeds = true;
	gPublishCount = 0U;
	(void)VdcXxxService_Init();
}

/*---------------------------------------------------------------------------*/
/* Tests                                                                      */
/*---------------------------------------------------------------------------*/

static void test_StartRequiresHooks(void)
{
	ResetFixture();
	CHECK(VdcXxxService_Start() == VDC_XXX_ERR_NOHOOKS);
	CHECK(VdcXxxService_IsRunning() == false);
}

static void test_MandatoryHooksAreValidated(void)
{
	VdcXxxServiceHooks_t hooks = ValidHooks();

	ResetFixture();
	CHECK(VdcXxxService_RegisterHooks(NULL) == VDC_XXX_ERR_BADPARAMETER);

	hooks.readInput = NULL;
	CHECK(VdcXxxService_RegisterHooks(&hooks) == VDC_XXX_ERR_BADPARAMETER);
}

static void test_PublishIsOptional(void)
{
	VdcXxxServiceHooks_t hooks = ValidHooks();
	hooks.publish = NULL;

	ResetFixture();
	CHECK(VdcXxxService_RegisterHooks(&hooks) == VDC_XXX_OK);
	CHECK(VdcXxxService_Start() == VDC_XXX_OK);
	CHECK(VdcXxxService_Process() == VDC_XXX_OK);
	CHECK(VdcXxxService_GetProcessedCount() == 1U);
	CHECK(gPublishCount == 0U);
}

static void test_LifecycleOrdering(void)
{
	VdcXxxServiceHooks_t hooks = ValidHooks();

	ResetFixture();
	CHECK(VdcXxxService_Process() == VDC_XXX_ERR_NOTRUNNING);
	CHECK(VdcXxxService_Stop() == VDC_XXX_ERR_NOTRUNNING);

	CHECK(VdcXxxService_RegisterHooks(&hooks) == VDC_XXX_OK);
	CHECK(VdcXxxService_Start() == VDC_XXX_OK);
	CHECK(VdcXxxService_Start() == VDC_XXX_ERR_ALREADYRUNNING);
	CHECK(VdcXxxService_RegisterHooks(&hooks) == VDC_XXX_ERR_ALREADYRUNNING);
	CHECK(VdcXxxService_Stop() == VDC_XXX_OK);
}

static void test_FailedReadDoesNotCount(void)
{
	VdcXxxServiceHooks_t hooks = ValidHooks();

	ResetFixture();
	CHECK(VdcXxxService_RegisterHooks(&hooks) == VDC_XXX_OK);
	CHECK(VdcXxxService_Start() == VDC_XXX_OK);

	gReadSucceeds = false;
	CHECK(VdcXxxService_Process() == VDC_XXX_OK);
	CHECK(VdcXxxService_GetProcessedCount() == 0U);
	CHECK(gPublishCount == 0U);

	gReadSucceeds = true;
	CHECK(VdcXxxService_Process() == VDC_XXX_OK);
	CHECK(VdcXxxService_GetProcessedCount() == 1U);
	CHECK(gPublishCount == 1U);
}

int main(void)
{
	test_StartRequiresHooks();
	test_MandatoryHooksAreValidated();
	test_PublishIsOptional();
	test_LifecycleOrdering();
	test_FailedReadDoesNotCount();

	if (gFailures == 0)
	{
		(void)printf("All tests passed.\n");
		return 0;
	}
	(void)printf("%d check(s) failed.\n", gFailures);
	return 1;
}
