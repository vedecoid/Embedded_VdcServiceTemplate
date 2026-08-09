/**
 * @file VdcXxxServiceConfig.h
 * @brief Compile-time tunables and the host-dependency hooks.
 * @version 3.0.0
 *
 * @copyright Copyright (c) 2026 Vedeco
 *
 * This is where the middleware-independence rule is discharged.
 *
 * A service MAY depend on other services — linking VdcLoggingService or
 * VdcMemAlloc is normal and, for P-3 and P-4, required. What it may not name is
 * a PRODUCT decision: a product sensor enumeration such as eSensorId, a message
 * bus, an alarm handler. Those arrive as function pointers in the hooks struct
 * below, which is what lets the same service serve a different product without
 * being edited.
 *
 * The test: would this service still make sense in a product that does not have
 * that peer? If yes, link it. If no, it is a hook.
 *
 * Every tunable is #ifndef-guarded so an application can override it from its
 * build without patching the library.
 */

#pragma once

#include "VdcXxxServiceApi.h"

#if defined(USE_FREERTOS) && (USE_FREERTOS == 1)
#include "FreeRTOS.h"
#include "task.h"
#endif

/*---------------------------------------------------------------------------*/
/* Tunables                                                                   */
/*---------------------------------------------------------------------------*/

/** Stack for the service task, in bytes, when the service owns one. */
#ifndef VdcXxxService_TASK_STACKBYTESIZE
#define VdcXxxService_TASK_STACKBYTESIZE 512
#endif

/** Task priority. Expressed relative to idle so it survives a config change. */
#ifndef VdcXxxService_TASK_PRIO
#if defined(USE_FREERTOS) && (USE_FREERTOS == 1)
#define VdcXxxService_TASK_PRIO (tskIDLE_PRIORITY + 3UL)
#else
#define VdcXxxService_TASK_PRIO 3UL
#endif
#endif

/** Nominal period between Process() calls, in milliseconds. */
#ifndef VdcXxxService_PROCESS_PERIOD_MS
#define VdcXxxService_PROCESS_PERIOD_MS 100U
#endif

/*---------------------------------------------------------------------------*/
/* Host dependencies                                                          */
/*---------------------------------------------------------------------------*/

/** @brief Return a monotonic millisecond timestamp. Mandatory. */
typedef uint32_t (*VdcXxxGetTimestampFn)(void);

/**
 * @brief Read one input value. Mandatory.
 * @param channel Host-defined identifier. Deliberately a plain uint32_t: the
 *                service must not know the product's sensor enumeration.
 * @param value   Out. Only written when the function returns true.
 * @return true when @p value holds a fresh, valid reading.
 */
typedef bool (*VdcXxxReadInputFn)(uint32_t channel, int32_t *value);

/**
 * @brief Publish a result. Optional — leave NULL to discard results.
 *
 * Optional by design: the service must work without a message bus, so that
 * host tests and other products are not forced to provide one.
 */
typedef void (*VdcXxxPublishFn)(uint32_t channel, int32_t value);

/** Everything the service needs from its host. */
typedef struct
{
	VdcXxxGetTimestampFn getTimestamp; /**< Mandatory. */
	VdcXxxReadInputFn    readInput;    /**< Mandatory. */
	VdcXxxPublishFn      publish;      /**< Optional — may be NULL. */
} VdcXxxServiceHooks_t;

/**
 * @brief Supply the host dependencies. Call after Init(), before Start().
 *
 * The struct is copied, so the caller may pass a stack temporary.
 *
 * @retval VDC_XXX_ERR_BADPARAMETER @p hooks is NULL, or a mandatory member is.
 * @retval VDC_XXX_ERR_ALREADYRUNNING Hooks cannot change while running.
 */
VdcXxxResult_e VdcXxxService_RegisterHooks(const VdcXxxServiceHooks_t *hooks);
