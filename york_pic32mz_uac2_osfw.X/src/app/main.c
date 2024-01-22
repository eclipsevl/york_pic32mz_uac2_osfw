/* York PIC32MZ UAC2.0 Firmware                */
/* Author: Dortonyan (Aliaksei Fisko)     */
/* Author: eclipsevl (Vladislav Knyazkov) */


#include "system.h"
#include <stdio.h>

void main (void) 
{      
    sysFrequHz = sysclk_value_get(PRIMARY_CRYSTAL_FREQUENCY_Hz, SECONDARY_CRYSTAL_FREQUENCY_Hz);
    
    __builtin_enable_interrupts();
    
    sys_init(sysFrequHz);

    /*  When enabling the USB PLL, add a three second
        delay before turning on the USB module  */

    tmr_Task_Delayed_mS_set(SYS_TASK_USB_CONNECT, 3000);
    __builtin_enable_interrupts();

    port_init();    
    usb_PL_init();
    dma_init();
    i2s_init();
    
    // Set System Performance and Enable Global Interrupts
    sys_performance_set(eSYSCLK_SPEED_HIGH);
        
    // Main Loop
    while (true)
    {
        if (_System_Task_Get() == 0)   // If no more system tasks
        {
            GO_TO_IDLE();              // go to idle mode with clocks enabled
        }
        else
        {
            usb_PL_tasks();
        }
    };
}

