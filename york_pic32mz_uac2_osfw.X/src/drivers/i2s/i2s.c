/* York PIC32MZ UAC2.0 Firmware                */
/* Author: Dortonyan (Aliaksei Fisko)     */
/* Author: eclipsevl (Vladislav Knyazkov) */


#include "../../app/system.h"
#include "i2s.h"



/*
 SPI based I2S modules initialisation  
 */
void i2s_init (void)
{ 
    /**************************************************************************/
    /* I2S1 output module initialisation                                      */
    /**************************************************************************/
    SPI1CON2 = 0
            | _SPI1CON2_AUDEN_MASK      // Audio protocol enabled
            | (0x0 << _SPI1CON2_AUDMOD_POSITION) // I2S mode mode
                                                 //   11 = PCM/DSP mode
                                                 //   10 = Right Justified mode
                                                 //   01 = Left Justified mode
                                                 //   00 = I2S mode
            | _SPI1CON2_IGNTUR_MASK     // Ignore Transmit Underrun
            ;    
    SPI1CON = 0
            | _SPI1CON_FRMPOL_MASK      // Frame Sync Polarity
            | _SPI1CON_MCLKSEL_MASK     // MCLK is used by the Baud Rate Generator
            | _SPI1CON_ENHBUF_MASK      // Enhanced Buffer mode is enabled
            | _SPI1CON_MODE32_MASK      // 32-bit Data, 32-bit FIFO, 32-bit Channel/64-bit Frame
            | _SPI1CON_CKP_MASK         // Idle state for clock is a high level; active state is a low leve
            | _SPI1CON_MSTEN_MASK       // Master mode
            | _SPI1CON_DISSDI_MASK      // SDIx pin is not used by the SPI module (pin is controlled by PORT function)
            | (0x2 << _SPI1CON_STXISEL_POSITION) // see lower            
                // STXISEL<1:0>: SPI Transmit Buffer Empty Interrupt Mode bits
                // 11 = Interrupt is generated when the buffer is not full (has one or more empty elements)
                // 10 = Interrupt is generated when the buffer is empty by one-half or more
                // 01 = Interrupt is generated when the buffer is completely empty
                // 00 = Interrupt is generated when the last transfer is shifted out of SPISR and transmit operations are complete
            | _SPI1CON_ON_MASK
            ;
    // Set interupt priority on I2S output
    IPC27SET = 0
            | (0x6 << _IPC27_SPI1TXIP_POSITION)        // SPI1 TX Interrupt Priority
            | (0x3 << _IPC27_SPI1TXIS_POSITION)        // SPI1 TX Interrupt Sub-Priority
            ;
    
    _CLK_I2S_ON();
}










