
#include "../../app/system.h"
#include "dma.h"


// One sample 8-ch DMA buffer to channels separation
static uint32_t dmabuf[8] __attribute__((coherent));
// One DoP sample buffer to mute data generation
static uint16_t dsdMutebuf __attribute__((coherent));
// Accelerate initialisation addresses
static uint32_t dma_spi1_adr;
static uint32_t dma_buf0_adr;
static uint32_t dma_buf1_adr;
static uint32_t dma_buf4_adr;
static uint32_t dma_dsd_mute_adr;


// data source to fifo clear
static uint32_t dwclr __attribute__((coherent));

/*
 * dma_init()
 */
void dma_init (void)
{
    dsdMutebuf = 0xAAAA;                         // Output bitstream for mute operation
    // Init phisical addresses
    dma_dsd_mute_adr = KVA_TO_PA( &dsdMutebuf );
    dma_spi1_adr = KVA_TO_PA( &SPI1BUF );
    dma_buf0_adr = KVA_TO_PA( &dmabuf[0] );
    dma_buf1_adr = KVA_TO_PA( &dmabuf[1] );
    dma_buf4_adr = KVA_TO_PA( &dmabuf[4] );
    
    DMACONbits.ON = 1;                          // Enable DMA controller
    
    /**************************************************************************/
    /* Channel 0 config - to copy data from EP1 FIFO to dmabuf                */
    /**************************************************************************/
    DCH0CONbits.CHAEN = 1;                      // Channel is continuously enabled, and not automatically disabled after a block transfer is complete
    DCH0ECONbits.SIRQEN = 1;                    // Enable transfer IRQ

    /**************************************************************************/
    /* Channel 1 config - to copy data from dmabuf buffer to I2S1 module      */
    /**************************************************************************/
    DCH1DSA = KVA_TO_PA( &SPI1BUF );            // Destination address (I2S FIFO buffer)
    DCH1DSIZ = 4;                               // Destination size
    DCH1SSA = KVA_TO_PA( &dmabuf[0] );          // Source data start address
    DCH1SSIZ = 8;                               // Source size
    DCH1CSIZ = 8;                               // Cell data size: channel audio data size (4 bytes)
    DCH1CONbits.CHAEN = 1;                      // Channel is continuously enabled, and not automatically disabled after a block transfer is complete
    DCH1CONbits.CHPRI = 0x1;                    // Set channel priority higher than ch0
    DCH1ECONbits.CHSIRQ = _SPI1_TX_VECTOR;      // Channel Transfer Start IRQ
    DCH1ECONbits.SIRQEN = 1;                    // Enable transfer IRQ

    /**************************************************************************/
    /* Channel 7 config - to cleare fifo buf                                  */
    /**************************************************************************/
    dwclr = 0;
    DCH7SSA = KVA_TO_PA( &dwclr );              // Source data start address
    DCH7SSIZ = 4;                               // Source size
    DCH7CONbits.CHAEN = 1;                      // Channel is continuously enabled, and not automatically disabled after a block transfer is complete 
    DCH7ECONbits.SIRQEN = 1;                    // Enable transfer IRQ
}

/*
 * dma_output_2ch_start()
 */
void dma_output_2ch_start (const uint16_t size)
{
    // Config channel to stereo output
    DCH0SSIZ = size;                        // Source size
    DCH0DSA = dma_spi1_adr;                 // Destination address (I2S FIFO buffer)
    DCH0DSIZ = 4;                           // Destination size ( buf data size 4 bytes * 8 channels = 32 bytes )
    DCH0CSIZ = 8;                           // Cell data size: channel audio data size (4 bytes)
    DCH0ECONbits.CHSIRQ = _SPI1_TX_VECTOR;  // Channel Transfer Start IRQ    

    // Config fifo clear
    DCH7DSIZ = size;                        // Destination size ( buf data size 4 bytes * 8 channels = 32 bytes )
    DCH7CSIZ = 8;                          // Cell data size: channel audio data size (4 bytes)
    DCH7ECONbits.CHSIRQ = _SPI1_TX_VECTOR;  // Channel Transfer Start IRQ   

    // Enable DMA channel
    DCH0CONbits.CHEN = 1;
    DCH7CONbits.CHEN = 1;
}

/*
 * dma_output_dsd_start()
 */
void dma_output_dsd_start (const uint16_t size)
{
    // Config channel to copy stereo DoP sample
    DCH0SSIZ = size;                            // Source size
    DCH0DSA = dma_buf0_adr;                     // Destination address (I2S FIFO buffer)
    DCH0DSIZ = 8;                               // Destination size ( buf data size 4 bytes * 8 channels = 32 bytes )
    DCH0CSIZ = 8;                               // Cell data size: channel audio data size (4 bytes)   
    DCH0ECONbits.CHSIRQ = _SPI3_TX_VECTOR;      // Channel Transfer Start IRQ
    
    // Config channel to DSD L output
    DCH1DSIZ = 2;                               // Destination size
    DCH1SSA = dma_buf0_adr + 1;                 // Source data start address
    DCH1SSIZ = 2;                               // Source size
    DCH1CSIZ = 2;                               // Cell data size: channel audio data size (4 bytes)
    
    // Config channel to DSD R output
    DCH3DSIZ = 2;                               // Destination size
    DCH3SSA = dma_buf1_adr + 1;                 // Source data start address
    DCH3SSIZ = 2;                               // Source size
    DCH3CSIZ = 2;                               // Cell data size: channel audio data size (4 bytes)    
    
    // Enable DMA chanels
    DCH0CONbits.CHEN = 1;
    DCH1CONbits.CHEN = 1;
    DCH3CONbits.CHEN = 1;
}

/*
 * dma_output_stop()
 */
void dma_output_stop (void)
{
    if (DCH0CONbits.CHEN)
        DCH0ECONbits.CABORT = 1;

    if (DCH1CONbits.CHEN)
        DCH1ECONbits.CABORT = 1;

    if (DCH2CONbits.CHEN)
        DCH2ECONbits.CABORT = 1;

    if (DCH3CONbits.CHEN)
        DCH3ECONbits.CABORT = 1;

    if (DCH4CONbits.CHEN)
        DCH4ECONbits.CABORT = 1;

    if (DCH7CONbits.CHEN)
        DCH7ECONbits.CABORT = 1;
        
    while (DCH0CONbits.CHBUSY);
    while (DCH1CONbits.CHBUSY);
    while (DCH2CONbits.CHBUSY);
    while (DCH3CONbits.CHBUSY);
    while (DCH4CONbits.CHBUSY);
    while (DCH7CONbits.CHBUSY);
}

/*
 * dma_output_dsd_mute()
 */
void dma_output_dsd_mute (void)
{
    // Break copy samples from FIFO byffer
    DCH0ECONbits.CABORT = 1;
    DCH1ECONbits.CABORT = 1;
    DCH3ECONbits.CABORT = 1;
    while (DCH3CONbits.CHBUSY);
    
    // Switch source addresses to mute data
    DCH1SSA = dma_dsd_mute_adr;      // Source data start address 
    DCH3SSA = dma_dsd_mute_adr;      // Source data start address   
    
    DCH1CONbits.CHEN = 1;
    DCH3CONbits.CHEN = 1;
}

