#ifndef _I2S_H_    /* Guard against multiple inclusion */
#define _I2S_H_


/* ************************************************************************** */
/* ************************************************************************** */
/* Section: Included Files                                                    */
/* ************************************************************************** */
/* ************************************************************************** */

/* This section lists the other files that are included in this file.
 */

/* TODO:  Include other files here if needed. */


/* Provide C++ Compatibility */
#ifdef __cplusplus
extern "C" {
#endif


#define     _I2S_Output_Int_Enable()                IEC3SET = _IEC3_SPI1TXIE_MASK
#define     _I2S_Output_Int_Disable()           {   IEC3CLR = _IEC3_SPI1TXIE_MASK;\
                                                    IFS3CLR = _IFS3_SPI1TXIF_MASK; }
    
#define     _I2S_Output_Transfer_Start()        {   SPI1CONSET = _SPI1CON_ON_MASK;\
                                                    SPI3CONSET = _SPI3CON_ON_MASK;\
                                                    _CLK_I2S_ON(); }
    
                                                    //SPI4CONSET = _SPI4CON_ON_MASK;\

#define     _I2S_Output_Transfer_Stop()         {   _CLK_I2S_OFF();\
                                                    SPI1CONCLR = _SPI1CON_ON_MASK;\
                                                    SPI3CONCLR = _SPI3CON_ON_MASK;}
   
                                                    //SPI4CONCLR = _SPI4CON_ON_MASK; 

#define     _I2S_Input_Transfer_Start()             SPI2CONSET = _SPI2CON_ON_MASK
#define     _I2S_Input_Transfer_Stop()              SPI2CONCLR = _SPI2CON_ON_MASK
    
#define     _I2S_Output_Transfer_Config_DSD()   {   SPI1CONbits.STXISEL = 0x3;\
                                                    SPI1CONbits.MODE32 = 0;\
                                                    SPI3CONbits.STXISEL = 0x3;\
                                                    SPI3CONbits.MODE32 = 0; }
    
#define     _I2S_Output_Transfer_Config_PCM()   {   SPI1CONbits.STXISEL = 0x2;\
                                                    SPI1CONbits.MODE32 = 1;\
                                                    SPI3CONbits.STXISEL = 0x2;\
                                                    SPI3CONbits.MODE32 = 1; }

    

    

void i2s_init (void);


    /* Provide C++ Compatibility */
#ifdef __cplusplus
}
#endif

#endif /* _I2S_H_ */


