/**
 * @file VdcXxxServiceApi.h
 * @brief Public API of VdcXxxService — the only header a consumer includes.
 * @version 3.0.0
 *
 * @copyright Copyright (c) 2026 Vedeco
 *
 * This header is the service's contract. Everything a consumer is allowed to
 * use lives here or in VdcXxxServiceConfig.h; everything else is in inc/ and
 * may change without notice.
 *
 * Doxygen scans api/ only, so the generated documentation *is* this contract.
 */

#pragma once

#include <stdint.h>
#include <stdbool.h>

/** Result of every operation that can fail. Never returned as a bare bool. */
typedef enum
{
	VDC_XXX_OK = 0,             /**< Operation completed.                          */
	VDC_XXX_ERR_NOTINITIALISED, /**< Init() has not run yet.                       */
	VDC_XXX_ERR_ALREADYRUNNING, /**< Start() called on a running service.          */
	VDC_XXX_ERR_NOTRUNNING,     /**< Stop()/Process() called on a stopped service. */
	VDC_XXX_ERR_BADPARAMETER,   /**< A required argument was NULL or out of range. */
	VDC_XXX_ERR_NOHOOKS         /**< Mandatory hooks were not registered.          */
} eVdcXxxResult;

/**
 * @brief Prepare the service. Must be called once, before any other call.
 *
 * Does not start processing and does not allocate. Safe to call before the
 * scheduler is running.
 */
eVdcXxxResult VdcXxxService_Init(void);

/**
 * @brief Begin processing.
 * @retval VDC_XXX_ERR_NOHOOKS Mandatory hooks were never registered.
 */
eVdcXxxResult VdcXxxService_Start(void);

/** @brief Stop processing. Idempotent state is preserved; Start() may follow. */
eVdcXxxResult VdcXxxService_Stop(void);

/** @brief True between a successful Start() and a Stop(). */
bool VdcXxxService_IsRunning(void);

/**
 * @brief Perform one unit of work.
 *
 * Call from the host's task or timer at the cadence the product needs. A
 * service that owns its own FreeRTOS task calls this from that task instead
 * and does not export it — see the README.
 */
eVdcXxxResult VdcXxxService_Process(void);

/** @brief Number of successful Process() cycles since Init(). Diagnostics only. */
uint32_t VdcXxxService_GetProcessedCount(void);
