
#ifndef _CLK_H_    /* Guard against multiple inclusion */
#define _CLK_H_


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
    
#define _CLK_I2S_ON()                   {   REFO1CONbits.ON = 1; }
#define _CLK_I2S_OFF()                  {   REFO1CONbits.ON = 0; }
    
#define _CLK_I2S_Div_Set(div)           {   while (REFO1CONbits.ACTIVE != REFO1CONbits.ON);\
                                            REFO1CON = 0\
                                                    | (div << _REFO1CON_RODIV_POSITION)\
                                                    | (0x08 << _REFO1CON_ROSEL_POSITION)\
                                                    | _REFO1CON_OE_MASK\
                                                    ; }


int sysclk_value_get (int pfreq, int sfreq);

    /* Provide C++ Compatibility */
#ifdef __cplusplus
}
#endif

#endif /* _CLK_H_ */

