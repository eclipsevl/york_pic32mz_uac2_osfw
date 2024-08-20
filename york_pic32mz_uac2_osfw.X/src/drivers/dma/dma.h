#ifndef _DMA_H_    /* Guard against multiple inclusion */
#define _DMA_H_


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

#define _DMA_SUSPEND_ALL()                          {   DMACONbits.SUSPEND = 1;\
                                                        while (DMACONbits.DMABUSY); }
#define _DMA_RESUME_ALL()                               DMACONbits.SUSPEND = 0
    
/******************* Output transfer defines ********************************/
#define _DMA_OutputSourceAddress_Set(adr)               {   DCH0SSA = adr; DCH7DSA = adr;}
#define _DMA_InputDestinationAddress_Set(adr)           DCH5DSA = adr

#define DMA_OUTPUT_TRANSFET_PTR                         DCH0SPTR
    
/******************* Input transfer defines ********************************/    
#define _DMA_Input_Transfer_Start()                     DCH5CONbits.CHEN = 1
#define _DMA_Input_Transfer_Size_Set(len)               DCH5DSIZ = len    
#define _DMA_Input_Transfer_Stop()                  {   if (DCH5CONbits.CHEN) {\
                                                        DCH5ECONbits.CABORT = 1; } }
    
#define DMA_INPUT_TRANSFET_PTR                          DCH5DPTR



// DMA initialization
void dma_init (void);

// DMA control
void dma_output_2ch_start (const uint16_t size);
void dma_output_dsd_start (const uint16_t size);
void dma_output_stop (void);
void dma_output_dsd_mute (void);



    /* Provide C++ Compatibility */
#ifdef __cplusplus
}
#endif

#endif /* _DMA_H_ */


