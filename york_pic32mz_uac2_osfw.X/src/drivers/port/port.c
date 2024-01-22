
#include "../../app/system.h"
#include "port.h"



/*
 port_init() - mcu port power up initialisation
 */
void port_init (void)
{ 

    FS0_SET_LOW();
    FS1_SET_LOW();
    
    /*Configure ports direction. 1 - input, 0 - output*/
    TRISDCLR = _BIT(1) | _BIT(3) | _BIT(4)  // I2S output pons
            | _BIT(5)                       // oscsel port
            | _BIT(10) | _BIT(11) // oscen ports
            ;
    TRISBCLR = 0
            | _BIT(5) | _BIT(8)             // FS[1:0] outputs to control ADC sempling frequency
            | _BIT(9) | _BIT(14) | _BIT(15) // I2S3 DSD output
            ;    
    TRISECLR = 0
            //| _BIT(5)                       // UART TX
            | _BIT(0)                       // DSDON
            | _BIT(1)                       // DSD64
            | _BIT(2)                       // MUTE
            | _BIT(3)                       // PLUG
            |_BIT(6)// led
            |_BIT(7)// led
            ;
    
    ANSELECLR = _BIT(0) | _BIT(1) | _BIT(2)| _BIT(3);// | _BIT(5);    
    ANSELGCLR = _BIT(6) | _BIT(7) | _BIT(9)| _BIT(8);// Switch Off Analog Function for I2S inputs
    
    // turn on leds
    LATECLR = _BIT(6);
    LATECLR = _BIT(7);

    OSCSEL_SET_LOW();                      // set oscsel to 48k domain
    OSCEN_FS44_SET_LOW();  
    OSCEN_FS48_SET_HIGH();
    
    /*Configure ports Pull Up. 1 - pull enable*/
    CNPUFSET = _BIT(3); // USBID input Pull-Up
           
    /*Configure ports Pull Down. 1 - pull enable*/
    CNPDBSET = _BIT(2) | _BIT(3) | _BIT(4) | _BIT(6) | _BIT(7) | _BIT(10) | _BIT(11) | _BIT(12) | _BIT(13) | _BIT(15);
    CNPDCSET = _BIT(13);
    CNPDDSET = _BIT(0) | _BIT(2) | _BIT(9) | _BIT(10) | _BIT(11);
    CNPDESET = _BIT(0) | _BIT(1) | _BIT(2) | _BIT(4) | _BIT(6) | _BIT(7);
    CNPDFSET = _BIT(0) | _BIT(1);// | _BIT(4) | _BIT(5);
    //CNPDGSET = _BIT(8);
    
    /************ Remappable pins *******************/    
    REFCLKI1Rbits.REFCLKI1R = 0x07; // RPC14  REFCLKI1 input
    //RPE5R = 0xf;    // 1111 = REFCLKO1
    
    // I2S1 remappable outputs
    RPD3R = 0x5;    // 0101 = SDO1
    RPD4R = 0x5;    // 0101 = SS1out
    
    // I2S2 remappable inputs
    SDI2R = 0x1;    // 0001 = RPG7 - SDI2
    SS2R = 0x1;     // 0001 = RPG9 - SS2in
    
    // I2S3 remappable outputs
    RPB9R = 0x7;    // 0111 = SDO3
    RPB15R = 0x7;   // 0111 = SS3
    
    // I2S4 remappable outputs
    RPE3R = 0x8;    // 1000 = SS4
    
    // External Interrupt input for I2S2 input channels syncronisation
    INT1R = 0x1;    // 0001 = RPG9 - SS2in
    INTCONbits.INT1EP = 0; // External Interrupt 1 Edge Polarity, 1 = Rising edge, 0 = Falling edge    
    IPC2SET = 0
            | (0x7 << _IPC2_INT1IP_POSITION)    // Set maximum priority level to correct start I2S input
            | (0x3 << _IPC2_INT1IS_POSITION)
            ;    
    IFS0bits.INT1IF = 0;
}

/*
 * EXT1_Interrupt()
*/
void __ISR(_EXTERNAL_1_VECTOR, ipl7SRS) EXT1_Interrupt(void)
{
    _I2S_Input_Transfer_Start();    // Turn ON I2S2 on Falling edge RLCK input
    _DMA_Input_Transfer_Start();
    IEC0CLR = _IEC0_INT1IE_MASK;    // Disable EXTI1 interrupts
    IFS0CLR = _IFS0_INT1IF_MASK;    // Clear interupt flag
}
