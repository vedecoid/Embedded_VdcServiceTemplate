#ifndef VDC_XXX_SERVICE_API_H
#define VDC_XXX_SERVICE_API_H

#include <stdint.h>
#include <stdbool.h>

// Backward compatibility macros
#define VdcxxxService_Init VdcLib_xxxService_Init
#define VdcxxxService_Process VdcLib_xxxService_Process

// Public API
void VdcLib_xxxService_Init(void);
void VdcLib_xxxService_Process(void);

#endif // VDC_XXX_SERVICE_API_H
