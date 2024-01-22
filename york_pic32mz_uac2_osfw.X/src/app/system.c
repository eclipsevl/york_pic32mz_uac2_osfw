
#include "system.h"



// ****************************************************************************
// ****************************************************************************
// Section: Configuration Bits
// ****************************************************************************
// ****************************************************************************
// <editor-fold defaultstate="collapsed" desc="Configuration Bits">

/*** DEVCFG0 ***/
#pragma config DEBUG =      OFF
#pragma config JTAGEN =     OFF
#pragma config ICESEL =     ICS_PGx1
#pragma config TRCEN =      OFF
#pragma config BOOTISA =    MIPS32
//#pragma config BOOTISA =    MICROMIPS
#pragma config FECCCON =    ON //OFF_UNLOCKED // ON
#pragma config FSLEEP =     OFF
#pragma config DBGPER =     PG_ALL
#pragma config EJTAGBEN =   NORMAL
#pragma config CP =         OFF

/*** DEVCFG1 ***/ 
#pragma config FNOSC =      SPLL
#pragma config DMTINTV =    WIN_127_128
#pragma config FSOSCEN =    OFF
#pragma config IESO =       ON // OFF
#pragma config POSCMOD =    HS
#pragma config OSCIOFNC =   OFF
#pragma config FCKSM =      CSECME
#pragma config WDTPS =      PS1048576
#pragma config WDTSPGM =    STOP
#pragma config FWDTEN =     OFF
#pragma config WINDIS =     NORMAL
#pragma config FWDTWINSZ =  WINSZ_25
#pragma config DMTCNT =     DMT31
#pragma config FDMTEN =     OFF

/*** DEVCFG2 ***/
#pragma config FPLLIDIV =   DIV_1           // FRC 8MHz/1 = 8MHz
#pragma config FPLLRNG =    RANGE_5_10_MHZ
#pragma config FPLLICLK =   PLL_FRC
#pragma config FPLLMULT =   MUL_80          // 8MHz x 80 = 640MHz (allowed range from 350 to 700 MHz)
#pragma config FPLLODIV =   DIV_4           // 640MHz/4 = 160MHz (60MHz - minimum frequecy when USB HS is ON )
#pragma config UPLLFSEL =   FREQ_12MHZ
    #ifdef __32MZ1024ECH064_H
#pragma config UPLLEN =     ON
    #endif

/*** DEVCFG3 ***/
#pragma config USERID =     0xffff
#pragma config FMIIEN =     OFF
#pragma config FETHIO =     OFF
#pragma config PGL1WAY =    ON
#pragma config PMDL1WAY =   ON
#pragma config IOL1WAY =    ON
#pragma config FUSBIDIO =   ON

/*** BF1SEQ0 ***/
#pragma config TSEQ =       0x0000
#pragma config CSEQ =       0xffff
// </editor-fold>

// Locate System Tasks bits
uint32_t systemTasks = 0;
// MCU system frequency in Hz
int sysFrequHz = 0;




/*
 * sys_perfomance_init()
 */
static inline void sys_perfomance_init (int sysfreq)
{
    uint32_t pfws, prefen;

    // Set flash frequency limits from errata
    if (DEVCFG0bits.FECCCON < 2) { // If Dynamic Flash ECC is enabled
        if (sysfreq <= 60000000) pfws = 0;
        else if (sysfreq <= 120000000) pfws = 1;
        else pfws = 2;
    }
    else {
        if (sysfreq <= 74000000) pfws = 0;
        else if (sysfreq <= 140000000) pfws = 1;
        else pfws = 2;
    }

    // Set prefetch only if flash clock divider is on
    prefen = (pfws) ? 0x3 : 0x0;

    PRECON = 0
            | (prefen << _PRECON_PREFEN_POSITION)   // Enable predictive prefetch for any address
            | (pfws << _PRECON_PFMWS_POSITION)      // Waite States PFM Access 
            ;
}


/*
 * Start MCU initialisation
 */
void sys_init (int sysfreq)
{  
    sys_perfomance_init( sysfreq );
    
    // Unlock peripheral bus clock control
    SYSKEY = 0x0; // Ensure OSCCON is locked
    SYSKEY = 0xAA996655; // Write Key1 to SYSKEY
    SYSKEY = 0x556699AA; // Write Key2 to SYSKEY

    if (sysfreq <= 100000000) {
        PB1DIVbits.PBDIV = 0;
        PB2DIVbits.PBDIV = 0;
        PB3DIVbits.PBDIV = 0;
        PB4DIVbits.PBDIV = 0;
        PB5DIVbits.PBDIV = 0;
    }
    PB8DIVbits.ON = 0; // Peripheral Bus 8 Output Clock Disable
    
    SYSKEY = 0x33333333; // Write any value other than Key1 or Key2
    
    /* Configure Shadow Register Set */
    PRISS = 0x76543210;
    while (PRISS != 0x76543210) {
        /* Wait for PRISS value to take effect */
    }
    INTCONbits.MVEC = 1;    // Set the CP0 registers for multi-vector interrupt
    
    // Disable unused peripheral clocks
    PMD1SET = 0            
                #ifdef _PMD1_AD1MD_MASK
            | _PMD1_AD1MD_MASK
                #endif
            | _PMD1_CVRMD_MASK
            ;
    PMD2SET = _PMD2_CMP1MD_MASK | _PMD2_CMP2MD_MASK;
    PMD3SET = _PMD3_IC1MD_MASK | _PMD3_IC2MD_MASK | _PMD3_IC3MD_MASK | _PMD3_IC4MD_MASK
            | _PMD3_IC5MD_MASK | _PMD3_IC6MD_MASK | _PMD3_IC7MD_MASK | _PMD3_IC8MD_MASK
            | _PMD3_IC9MD_MASK
            | _PMD3_OC1MD_MASK | _PMD3_OC2MD_MASK | _PMD3_OC3MD_MASK | _PMD3_OC4MD_MASK
            | _PMD3_OC5MD_MASK | _PMD3_OC6MD_MASK | _PMD3_OC7MD_MASK | _PMD3_OC8MD_MASK
            | _PMD3_OC9MD_MASK
            ;
    PMD4SET = _PMD4_T4MD_MASK | _PMD4_T5MD_MASK | _PMD4_T6MD_MASK | _PMD4_T7MD_MASK
            | _PMD4_T8MD_MASK | _PMD4_T9MD_MASK
            ;
    PMD5SET = _PMD5_U1MD_MASK | _PMD5_U2MD_MASK | _PMD5_U3MD_MASK | _PMD5_U4MD_MASK
             | _PMD5_U6MD_MASK
            | _PMD5_U5MD_MASK 
            | _PMD5_I2C1MD_MASK | _PMD5_I2C3MD_MASK
            | _PMD5_I2C4MD_MASK | _PMD5_I2C5MD_MASK
            ;
    PMD6SET = _PMD6_RTCCMD_MASK
            | _PMD6_REFO2MD_MASK | _PMD6_REFO3MD_MASK | _PMD6_REFO4MD_MASK
            | _PMD6_PMPMD_MASK
            | _PMD6_SQI1MD_MASK
            | _PMD6_ETHMD_MASK
            ;    
    PMD7SET = _PMD7_RNGMD_MASK;
}



/*
    sys_performance_set()
 */
void sys_performance_set (const eSYSCLK_SPEED speed)
{
    uint8_t pllodiv, pllmult;
    
    switch (speed)
    {
        case eSYSCLK_SPEED_LOW: // 60MHz speed
            pllodiv = 0x3;  // div by 8
            pllmult = 59;   // mult by 60
            break;
            
        case eSYSCLK_SPEED_MID: // 100MHz speed
            pllodiv = 0x2;  // div by 4
            pllmult = 49;   // mult by 50
            break;
            
        case eSYSCLK_SPEED_HIGH: // 160MHz speed
            pllodiv = 0x2;  // div by 4
            pllmult = 79;   // mult by 80
//        case eSYSCLK_SPEED_HIGH: // 200MHz speed
//            pllodiv = 0x1;  // div by 2
//            pllmult = 50;   // mult by 50
            break;
            
        default:
            return;
    }
    
    _DMA_SUSPEND_ALL();    
    __builtin_disable_interrupts();
    
    PRECON = 0
            | (0 << _PRECON_PREFEN_POSITION)    // Disable predictive prefetch
            | (2 << _PRECON_PFMWS_POSITION)     // Waite States PFM Access 
            ;
    
    // Unlock peripheral bus clock control
    SYSKEY = 0x0; // Ensure OSCCON is locked
    SYSKEY = 0xAA996655; // Write Key1 to SYSKEY
    SYSKEY = 0x556699AA; // Write Key2 to SYSKEY

    PB1DIVbits.PBDIV = 1;
    PB2DIVbits.PBDIV = 1;
    PB3DIVbits.PBDIV = 1;
    PB4DIVbits.PBDIV = 1;
    PB5DIVbits.PBDIV = 1;

    OSCCONbits.NOSC = 0x0;      // switch system clock to FRC    
    OSCCONbits.OSWEN = 1;
    while (OSCCONbits.OSWEN);
    // Config sys clk pll
    SPLLCONbits.PLLMULT = pllmult;
    SPLLCONbits.PLLODIV = pllodiv;
 
    OSCCONbits.NOSC = 0x1;      // return system clock to PLL
    OSCCONbits.OSWEN = 1;
    while (OSCCONbits.OSWEN);

    sysFrequHz = sysclk_value_get(PRIMARY_CRYSTAL_FREQUENCY_Hz, SECONDARY_CRYSTAL_FREQUENCY_Hz);
    
    if (sysFrequHz <= 100000000) {
        PB1DIVbits.PBDIV = 0;
        PB2DIVbits.PBDIV = 0;
        PB3DIVbits.PBDIV = 0;
        PB4DIVbits.PBDIV = 0;
        PB5DIVbits.PBDIV = 0;
    }

    SYSKEY = 0x33333333; // Write any value other than Key1 or Key2
    
    sys_perfomance_init( sysFrequHz );
    
    __builtin_enable_interrupts();
    _DMA_RESUME_ALL();
}
