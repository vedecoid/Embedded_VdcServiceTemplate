#include "VdcxxxServiceApi.h"
#include <stdio.h>

int main(void)
{
    printf("Starting VdcxxxService Example...\n");

    VdcLib_xxxService_Init();

    for (int i = 0; i < 10; i++)
    {
        VdcLib_xxxService_Process();
    }

    printf("Example Finished.\n");
    return 0;
}
