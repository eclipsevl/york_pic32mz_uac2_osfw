
#ifndef _SYSTEM_H_    /* Guard against multiple inclusion */
#define _SYSTEM_H_


/* Provide C++ Compatibility */
#ifdef __cplusplus
extern "C" {
#endif
    
/*******************************************
 * System libriares
 *******************************************/
#include <stdbool.h>
#include <string.h>
#include <sys/attribs.h>
#include <sys/kmem.h>
#include <xc.h>

/*******************************************
 * Project libriares
 *******************************************/
#include "../drivers/port/port.h"
#include "../drivers/clk/clk.h"
#include "../drivers/tmr/tmr.h"
#include "../drivers/dma/dma.h"
#include "../drivers/i2s/i2s.h"
#include "../drivers/usb/usb_LL.h"
#include "../drivers/usb/usb_PL.h"
    
    
    
typedef enum {
    eSYSCLK_SPEED_LOW,
    eSYSCLK_SPEED_MID,
    eSYSCLK_SPEED_HIGH
}eSYSCLK_SPEED;
    
typedef union {
    uint32_t dw;
    struct {
        uint16_t w0;
        uint16_t w1;
    }word;
    struct {
        uint8_t b0;
        uint8_t b1;
        uint8_t b2;
        uint8_t b3;
    }byte;
}tDWORD;
    
    
// Choose I2S masterclock
#define I2S_MCLK_512Fs
//#define I2S_MCLK_1024Fs


#define GO_TO_IDLE()                    asm volatile ( "wait" )
    
// Define primary clock frequency
#define PRIMARY_CRYSTAL_FREQUENCY_Hz    12000000u
// Define secondary clock frequency
#define SECONDARY_CRYSTAL_FREQUENCY_Hz  0u
    
    
// System task bits list
#define SYS_TASK_USB_CONNECT            (0x01 << 0)
#define SYS_TASK_USB_EP0_ISR            (0x01 << 1)
#define SYS_TASK_USB_STREAM_OUT_INIT    (0x01 << 2)
#define SYS_TASK_USB_START_DSD          (0x01 << 3)


// System tasks control macroses
#define _System_Task_Set(bit)           systemTasks |= (uint32_t)bit
#define _System_Task_Clr(bit)           systemTasks &= ~((uint32_t)bit)
#define _System_Task_Check(bit)         (systemTasks & ((uint32_t)bit))
#define _System_Task_Get()              systemTasks


extern uint32_t systemTasks;
extern int sysFrequHz;

    
/* System Func */
void sys_init (int sysfreq);
void sys_performance_set (const eSYSCLK_SPEED speed);



    /* Provide C++ Compatibility */
#ifdef __cplusplus
}
#endif

#endif /* _SYSTEM_H_ */

/* *****************************************************************************
 End of File
 */
