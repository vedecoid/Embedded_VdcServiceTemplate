/**
 * @file VdcXxxService.h
 * @brief Service-internal types and state. Not part of the public contract.
 *
 * Nothing outside this repository may include this header. If a consumer needs
 * something declared here, that is evidence the public API is incomplete —
 * extend api/ rather than widening the include path.
 *
 * Larger services split this further into VdcXxxService_Internal.h for detail
 * that even the other .c files in src/ do not need.
 */

#pragma once

#include "VdcXxxServiceApi.h"
#include "VdcXxxServiceConfig.h"

/** Whole service state. Single instance; see the README on multi-instance. */
typedef struct
{
	bool                 initialised;
	bool                 running;
	uint32_t             processedCount;
	uint32_t             lastTimestampMs;
	VdcXxxServiceHooks_t hooks;
} VdcXxxServiceContext_t;

/** @brief Access to the singleton context, for other src/ files and tests. */
VdcXxxServiceContext_t *VdcXxxService_GetContext(void);
