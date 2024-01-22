
#include "../../app/system.h"
#include "clk.h"


/*
 * sysclk_value_get()
 * inputs:
 * pfreq - primary crystal frequency (set 0 if not use), sfreq - secondary crystal frequency (set 0 if not use)
 * output:
 * SYSCLK frequency
 */
int sysclk_value_get (int pfreq, int sfreq)
{
    int val;
    
    switch (OSCCONbits.NOSC)
    {
        case 0: // Internal Fast RC (FRC) Oscillator divided by FRCDIV<2:0> bits (FRCDIV)
        case 7: // Internal Fast RC (FRC) Oscillator divided by FRCDIV<2:0> bits (FRCDIV)
            val = 8000000;
            //  Internal Fast RC (FRC) Oscillator Clock Divider
            val >>= OSCCONbits.FRCDIV;            
            if (OSCCONbits.FRCDIV == 7)
                val >>= 1;
            break;

        case 1: // System PLL (SPLL)
            if (SPLLCONbits.PLLICLK == 1) // if FRC is selected as the input to the System PLL
                val = 8000000;
            else    // if POSC is selected as the input to the System PLL
                val = pfreq;

            // System PLL Multiplier 
            val *= (SPLLCONbits.PLLMULT + 1);
            // System PLL Input Clock Divider
            val /= (SPLLCONbits.PLLIDIV + 1);
            // System PLL Output Clock Divider
            val >>= SPLLCONbits.PLLODIV;
            break;

        case 2: // Primary Oscillator (POSC) (HS or EC)
            val = pfreq;
            break;

        case 4: // Secondary Oscillator (SOSC)
            val = sfreq;
            break;

        case 5: // Internal Low-Power RC (LPRC) Oscillator
            val = 32768;
            break;

        default:
            return -1;
    }
    
    return val;
}




/* *****************************************************************************
 End of File
 */
