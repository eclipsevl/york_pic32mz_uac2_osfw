
#include "../../app/system.h"
#include "tmr.h"


static uint32_t tmr_Task = 0;

/*
 * tmr_1_mS_set() - 
 */
void tmr_1_mS_set (const uint16_t time_mS)
{      
    int freq = sysFrequHz / (PB3DIVbits.PBDIV + 1);
    
    // time value calculate
    uint32_t val = time_mS;
    val *= (freq / 1000);
    val >>= 8;
    val--;

    T1CON = 0x0; // Stop 16-bit Timer1 and clear control register
    T1CONSET = (0x3 << _T1CON_TCKPS_POSITION); // prescaler at 256 prescale,
    // internal clock source
    
    TMR1 = 0x0; // Clear contents of the TMR1
    PR1 = val; // Load PR1 register with 16-bit value   
    IPC1SET = ((uint32_t)0x1 << _IPC1_T1IP_POSITION) | ((uint32_t)0x0 << _IPC1_T1IS_POSITION); // Set priority level = 1, sub-priority level = 0
    
    IEC0SET = _IEC0_T1IE_MASK; // Enable Timer1 interrupts
    IFS0CLR = _IFS0_T1IF_MASK; // Clear the Timer1 interrupt status flag
    T1CONSET = _T1CON_ON_MASK; // Start the timer
}


/*
 * tmr_23_mS_set()
 */
static inline void tmr_23_mS_set (const uint16_t time_mS)
{    
    int freq = sysFrequHz / (PB3DIVbits.PBDIV + 1);
    
    // time value calculate
    uint32_t val = time_mS;
    val *= (freq / 1000);
    val >>= 8;      // val/256
    val--;
    
    T2CON = 0x0;    // Stop 16/32-bit Timer2 and clear control register
    T3CON = 0x0;    // Stop 16/32-bit Timer3 and clear co ntrol register
    T2CONSET = 0
            | (0x7 << _T2CON_TCKPS_POSITION) // prescaler 1:256,
            | _T2CON_T32_MASK                 // Enable 32-bit mode,
            ;
    
    TMR2 = 0x0;     // Clear contents of the TMR2
    PR2 = (uint32_t)val;      // Load PR2 register with 32-bit value

    IPC3SET = ((uint32_t)0x1 << _IPC3_T3IP_POSITION) | ((uint32_t)0x0 << _IPC3_T3IS_POSITION); // Set priority level = 1, sub-priority level = 0
    IFS0CLR = _IFS0_T3IF_MASK; // Clear the Timer3 interrupt status flag
    IEC0SET = _IEC0_T3IE_MASK; // Enable Timer3 interrupts    
    T2CONSET = _T2CON_ON_MASK; // Start the timer
}

/*
 * tmr_Task_Delayed_mS_set()
*/
void tmr_Task_Delayed_mS_set (const uint32_t task, const uint16_t time_mS)
{ 
    tmr_23_mS_set( time_mS );
    tmr_Task = task;
}


/*
 * Timer1_Interrupt()
*/
void __ISR(_TIMER_1_VECTOR, ipl1AUTO) Timer1_Interrupt(void)
{   
    T1CON = 0x0; // Stop 16-bit Timer1 and clear control register
    IFS0bits.T1IF = 0;          // Clear the Timer1 interrupt status flag
}

/*
 * Timer23_Interrupt()
*/
void __ISR(_TIMER_3_VECTOR, ipl1AUTO) Timer23_Interrupt(void)
{
    T2CONbits.ON = 0;           // Switch off timer
    
    _System_Task_Set(tmr_Task);
    tmr_Task = 0;
    
    IFS0bits.T3IF = 0;          // Clear the Timer1 interrupt status flag
}





