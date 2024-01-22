#include "../../app/system.h"
#include "usbd_def.h"
#include "usb_LL.h"


#define USB_TRX_TIMEOUT     1000

/*
 *  FIFO sizes for endpoints
 */
#define USB_EP7RX_FSIZE     0x6     // FIFO size for CDC data receive 2^(3+6) = 512 bytes
#define USB_EP7TX_FSIZE     0x6     // FIFO size for CDC data transmitt 2^(3+6) = 512 bytes

#define USB_EP5RX_FSIZE     0x0     // not used 2^(3+0) = 8 bytes
#define USB_EP5TX_FSIZE     0x7     // FIFO size for Input Stream data transmitt 2^(3+7) = 1024 bytes

#define USB_EP1RX_FSIZE     0x7     // FIFO size for Output Stream data receive 2^(3+7) = 1024 bytes
#define USB_EP1TX_FSIZE     0x0     // FIFO size for Stream FeedBack data transmitt 2^(3+0) = 8 bytes

/*
 *  All endpoints addresses (USBFIFOA) must be set without free spaces between addresses!!!
 *  Address size is multiple by 8 bytes values (0x100 - mean 2048 bytes)
 */
#define USB_EP7RX_ADR       0x000   // Start FIFO address for CDC data receive
#define USB_EP7TX_ADR       0x080   // Start FIFO address for CDC data transmitt

#define USB_EP5RX_ADR       0x100   // same adr as tx, because not used
#define USB_EP5TX_ADR       0x100   // Start FIFO address for Input Stream data transmitt

#define USB_EP1RX_ADR       0x200   // Start FIFO address for Output Stream data receive
#define USB_EP1TX_ADR       0x300   // Start FIFO address for Output Stream FeedBack data transmitt


/* 
    usb_LL_initReset() usb high speed phy initialisation 
 */
void usb_LL_initReset (void)
{
    // Clear the interrupt flags
    USBCSR1 = 0;
    USBCSR2 = 0;

#if USB_DEV_SPEED == 2
    USBCSR0bits.HSEN = 0;       // 1 = Enable High Speed (480Mbps) USB mode.
#endif

    // Endpoint 0
    USBCSR0bits.EP0IF = 0;
    USBCSR0bits.EP1TXIF = 0;
    USBE0CSR2bits.SPEED = USB_DEV_SPEED;
    
    // Endpoint 1
    USBCSR3bits.ENDPOINT = 1;                // set current endpoint to EP1    
  
    USBOTG |= 0 // write FIFOSZ fields only!
            | (USB_EP1RX_FSIZE << _USBOTG_RXFIFOSZ_POSITION)    // 2^(3+7) = 1024 bytes
            | (USB_EP1TX_FSIZE << _USBOTG_TXFIFOSZ_POSITION)    // 2^(3+0) = 8 bytes
            | (0x1 << _USBOTG_RXDPB_POSITION)       // Double-packet buffer is supported. This doubles the size set in RXFIFOSZ
            | (0x1 << _USBOTG_TXDPB_POSITION)       // Double-packet buffer is supported. This doubles the size set in TXFIFOSZ
            ;
    USBFIFOA = 0 // Start address of the endpoint FIFO in units of 8 bytes
            | (USB_EP1RX_ADR << _USBFIFOA_RXFIFOAD_POSITION)  // rx fifo adress
            | (USB_EP1TX_ADR << _USBFIFOA_TXFIFOAD_POSITION)  // tx fifo adress
            ;
    USBE1CSR0 = 0
            | (0x4 << _USBE1CSR0_TXMAXP_POSITION)   // maximum tx payload 4
            | (0x1 << _USBE1CSR0_ISO_POSITION)      // isochronous tx endpoint enable bit (device mode).
            | _USBE1CSR0_CLRDT_MASK                 // Cleare data toggle
            | (0x0 << _USBE1CSR0_DMAREQMD_POSITION) // DMA Request Mode Selection
            | (0x0 << _USBE1CSR0_DMAREQEN_POSITION) // DMA Request Enable
            ;
    USBE1CSR1 = 0
            | (0x400 << _USBE1CSR1_RXMAXP_POSITION) // maximum rx payload / transaction (768 is the maximum, set to 1024)
            | (0x1 << _USBE1CSR1_ISO_POSITION)      // isochronous rx endpoint enable bit (device mode).
            | (0x0 << _USBE1CSR1_DMAREQMD_POSITION) // DMA Request Mode Selection (RXMAXP must be even when mode is 1)
            | (0x0 << _USBE1CSR1_DMAREQEN_POSITION) // Enable DMA requests for the RX endpoint
            ;
    USBE1CSR2 = 0
            | (0x1 << _USBE1CSR2_PROTOCOL_POSITION) // tx isochronous mode
            | (USB_DEV_SPEED << _USBE1CSR2_SPEED_POSITION) // set tx endpoint speed
            ;
    USBE1CSR3 = 0
            | (0x1 << _USBE1CSR3_PROTOCOL_POSITION) // rx isochronous mode
            | (USB_DEV_SPEED << _USBE1CSR3_SPEED_POSITION) // set rx endpoint speed
            ;
    USBDMA1C = 0
            | (0x1 << _USBDMA1C_DMAEP_POSITION)     // These bits hold the endpoint that the DMA channel is assigned to. Valid values are 0-7.
            | (0x0 << _USBDMA1C_DMADIR_POSITION)    // 0 - DMA Write (RX endpoint), 1 - DMA Read (TX endpoint)
            | (0x0 << _USBDMA1C_DMAMODE_POSITION)
            | (0x3 << _USBDMA1C_DMABRSTM_POSITION)
            | _USBDMA1C_DMAIE_MASK
            ;

    // Endpoint 5
    USBCSR3bits.ENDPOINT = 5;                // set current endpoint to EP5    
  
    USBOTG |= 0 // write FIFOSZ fields only!
            | (USB_EP5TX_FSIZE << _USBOTG_RXFIFOSZ_POSITION)    // 2^(3+0) = 8 bytes
            | (USB_EP5TX_FSIZE << _USBOTG_TXFIFOSZ_POSITION)    // 2^(3+7) = 1024 bytes
            | (0x0 << _USBOTG_RXDPB_POSITION)       // Double-packet buffer is supported. This doubles the size set in RXFIFOSZ
            | (0x1 << _USBOTG_TXDPB_POSITION)       // Double-packet buffer is supported. This doubles the size set in TXFIFOSZ
            ;
    USBFIFOA = 0 // Start address of the endpoint FIFO in units of 8 bytes
            | (USB_EP5RX_ADR << _USBFIFOA_RXFIFOAD_POSITION)// rx fifo adress (same address because not used)
            | (USB_EP5TX_ADR << _USBFIFOA_TXFIFOAD_POSITION)// tx fifo adress
            ;
    USBE5CSR0 = 0
            | (0x308 << _USBE5CSR0_TXMAXP_POSITION) // maximum tx payload 768+8
            | (0x1 << _USBE5CSR0_ISO_POSITION)      // isochronous tx endpoint enable bit (device mode).
            | _USBE5CSR0_CLRDT_MASK                 // Cleare data toggle
            | (0x1 << _USBE5CSR0_MODE_POSITION)     // Endpoint is 0 - RX, 1 - TX
            | (0x0 << _USBE5CSR0_DMAREQMD_POSITION) // DMA Request Mode Selection
            | (0x0 << _USBE5CSR0_DMAREQEN_POSITION) // DMA requests are enabled for this endpoint
            ;
    USBE5CSR1 = 0
            | (0x0 << _USBE5CSR1_RXMAXP_POSITION)   // maximum rx payload / transaction
            | (0x0 << _USBE5CSR1_ISO_POSITION)      // isochronous rx endpoint enable bit (device mode).
            | (0x0 << _USBE5CSR1_DMAREQMD_POSITION) // DMA Request Mode Selection
            | (0x0 << _USBE5CSR1_DMAREQEN_POSITION) // Enable DMA requests for the RX endpoint
            ;
    USBE5CSR2 = 0
            | (0x1 << _USBE5CSR2_PROTOCOL_POSITION) // tx isochronous mode
            | (USB_DEV_SPEED << _USBE5CSR2_SPEED_POSITION) // set tx endpoint speed
            ;
    USBE5CSR3 = 0
            | (0x1 << _USBE5CSR3_PROTOCOL_POSITION) // rx isochronous mode
            | (USB_DEV_SPEED << _USBE5CSR3_SPEED_POSITION) // set rx endpoint speed
            ;
    USBDMA5C = 0
            | (0x5 << _USBDMA5C_DMAEP_POSITION)     // These bits hold the endpoint that the DMA channel is assigned to. Valid values are 0-7.
            | (0x1 << _USBDMA5C_DMADIR_POSITION)    // 0 - DMA Write (RX endpoint), 1 - DMA Read (TX endpoint)
            | (0x0 << _USBDMA5C_DMAMODE_POSITION)
            | (0x3 << _USBDMA5C_DMABRSTM_POSITION)
            | _USBDMA5C_DMAIE_MASK
            ;

    // Endpoint 7
    USBCSR3bits.ENDPOINT = 7;                // set current endpoint to EP7    
  
    USBOTG |= 0 // write FIFOSZ fields only!
            | (USB_EP7TX_FSIZE << _USBOTG_RXFIFOSZ_POSITION)    // 2^(3+6) = 512 bytes
            | (USB_EP7TX_FSIZE << _USBOTG_TXFIFOSZ_POSITION)    // 2^(3+6) = 512 bytes
            | (0x0 << _USBOTG_RXDPB_POSITION)       // Double-packet buffer is supported. This doubles the size set in RXFIFOSZ
            | (0x0 << _USBOTG_TXDPB_POSITION)       // Double-packet buffer is supported. This doubles the size set in TXFIFOSZ
            ;
    USBFIFOA = 0 // Start address of the endpoint FIFO in units of 8 bytes
            | (USB_EP7RX_ADR << _USBFIFOA_RXFIFOAD_POSITION)  // rx fifo adress
            | (USB_EP7TX_ADR << _USBFIFOA_TXFIFOAD_POSITION)  // tx fifo adress
            ;
    USBE7CSR0 = 0
            | (0x200 << _USBE7CSR0_TXMAXP_POSITION) // maximum tx payload 512 bytes
            | (0x0 << _USBE7CSR0_ISO_POSITION)      // isochronous tx endpoint enable bit (device mode).
            | _USBE7CSR0_CLRDT_MASK                 // Cleare data toggle
            | (0x0 << _USBE7CSR0_MODE_POSITION)     // Endpoint is 0 - RX, 1 - TX
            | (0x0 << _USBE7CSR0_DMAREQMD_POSITION) // DMA Request Mode Selection
            | (0x1 << _USBE7CSR0_DMAREQEN_POSITION) // DMA requests are enabled for this endpoint
            ;
    USBE7CSR1 = 0
            | (0x200 << _USBE7CSR1_RXMAXP_POSITION)   // maximum rx payload / transaction 512 bytes
            | (0x0 << _USBE7CSR1_ISO_POSITION)      // isochronous rx endpoint enable bit (device mode).
            | (0x0 << _USBE7CSR1_DMAREQMD_POSITION) // DMA Request Mode Selection
            | (0x0 << _USBE7CSR1_DMAREQEN_POSITION) // Enable DMA requests for the RX endpoint
            ;
    USBE7CSR2 = 0
            | (0x2 << _USBE7CSR2_PROTOCOL_POSITION) // tx bulk mode
            | (USB_DEV_SPEED << _USBE7CSR2_SPEED_POSITION) // set tx endpoint speed
            ;
    USBE7CSR3 = 0
            | (0x2 << _USBE7CSR3_PROTOCOL_POSITION) // rx bulk mode
            | (USB_DEV_SPEED << _USBE7CSR3_SPEED_POSITION) // set rx endpoint speed
            ;
    USBDMA7C = 0
            | (0x7 << _USBDMA7C_DMAEP_POSITION)     // These bits hold the endpoint that the DMA channel is assigned to. Valid values are 0-7.
            | (0x0 << _USBDMA7C_DMADIR_POSITION)    // 0 - DMA Write (RX endpoint), 1 - DMA Read (TX endpoint)
            | (0x0 << _USBDMA7C_DMAMODE_POSITION)
            | (0x3 << _USBDMA7C_DMABRSTM_POSITION)
            | _USBDMA7C_DMAIE_MASK
            ;
    
    USBCSR3bits.ENDPOINT = 0;           // Return to EP0 settings    
    
    USBCSR1 |= 0
            | _USBCSR1_EP0IE_MASK       // Endpoint 0 interrupt enable
            | _USBCSR1_EP5TXIE_MASK     // Endpoint 5 TX interrupt enable
            ;
    USBCSR2 |= 0
            | _USBCSR2_RESETIE_MASK     // Reset interrupt enable
            | _USBCSR2_EP1RXIE_MASK     // Endpoint 1 RX interrupt enable
            | _USBCSR2_EP7RXIE_MASK     // Endpoint 7 RX interrupt enable
            //------ Other interrurps enable ----------------------------
            //| _USBCSR2_VBUSERRIE_MASK
            | _USBCSR2_SESSRQIE_MASK
            //| _USBCSR2_DISCONIE_MASK
            //| _USBCSR2_CONNIE_MASK
            //| _USBCSR2_SOFIE_MASK
            | _USBCSR2_RESUMEIE_MASK
            //| _USBCSR2_SUSPIE_MASK
            ;
}

/* 
    usb_LL_Init() usb high speed phy initialisation 
 */
void usb_LL_Init (void)
{
    usb_LL_initReset();
    
    IPC33SET = 0
            | (0x3 << _IPC33_USBIP_POSITION)        // USB Interrupt Priority.
            | (0x3 << _IPC33_USBIS_POSITION)        // USB Interrupt Sub-Priority.
            | (0x4 << _IPC33_USBDMAIP_POSITION)     // USB DMA Interrupt Priority.
            | (0x3 << _IPC33_USBDMAIS_POSITION)     // USB DMA Interrupt Sub-Priority.
            ;
    IFS4CLR = 0
            | _IFS4_USBIF_MASK                      // Clear the USB interrupt flag.
            | _IFS4_USBDMAIF_MASK                   // Clear the USB DMA interrupt flag.
            ;
    IEC4SET = 0
            | _IEC4_USBIE_MASK                      // Enable USB interrupt.
            | _IEC4_USBDMAIE_MASK                   // Enable USB DMA interrupt.
            ;
    
        #ifdef USBCRCON
    USBCRCONbits.USBIE  = 1;                        // Enable USB module interrupt
        #endif 
}

/* 
    usb_LL_Ep0_FifoRead() read EP0 FIFO data to "p" adress 
 */
static inline void usb_LL_Ep0_FifoRead (void *p, uint16_t len)
{
    if (len > USBIE0CSR2bits.RXCNT)
        len = USBIE0CSR2bits.RXCNT;    
    
    uint16_t len8 = len & 0x3;
    len >>= 2;
    
    uint32_t *p32 = p;
    volatile const uint32_t *pf32 = (const uint32_t*)&USBFIFO0;
    // read 32bit data from fifo - Not recommended in Errata !
    for (; len; len--)
        *p32++ = *pf32;
    
    uint8_t *p8 = (uint8_t*)p32;
    volatile const uint8_t *pf8 = (const uint8_t*)&USBFIFO0;
    // read 8bit data from fifo
    for (; len8; len8--)
        *p8++ = *pf8++;

    if (USBIE0CSR2bits.RXCNT) USBE0CSR0bits.FLUSH = 1;
    else USBE0CSR0bits.RXRDYC = 1;
}

/* 
 * usb_LL_Ep0_BulkRead() bulk read data from EP0 to "p" adress
 * len - buffer p size
 */
int usb_LL_Ep0_BulkRead (void *p, int len)
{
    int result = 0;
    uint16_t pckt_len;
    
    do
    {
        volatile int timeout = 0;        
        while (!USBE0CSR0bits.RXRDY && timeout < USB_TRX_TIMEOUT)
            timeout++;
        
        if (timeout >= USB_TRX_TIMEOUT) {
            result = -1;
            break;
        }
        
        pckt_len = (len < USB_MAX_EP0_SIZE) ? len : USB_MAX_EP0_SIZE;
        usb_LL_Ep0_FifoRead( p, pckt_len );
        
        if (pckt_len < USB_MAX_EP0_SIZE)
            break;

        len -= pckt_len;
        p += pckt_len;        
    }while (true);
    
    USBIE0CSR0bits.DATAEND = 1;
    return result;
}

/* 
    usb_LL_Ep0_Fast_FifoWrite64() write 64 bytes data *p to EP0 FIFO 
 */
static inline void usb_LL_Ep0_Fast_FifoWrite64 (const void *p)
{
    const uint32_t *p32 = p;
    volatile uint32_t *pf32 = (uint32_t*)&USBFIFO0;
     
    *pf32 = *p32++; *pf32 = *p32++; *pf32 = *p32++; *pf32 = *p32++;
    *pf32 = *p32++; *pf32 = *p32++; *pf32 = *p32++; *pf32 = *p32++;
    *pf32 = *p32++; *pf32 = *p32++; *pf32 = *p32++; *pf32 = *p32++;
    *pf32 = *p32++; *pf32 = *p32++; *pf32 = *p32++; *pf32 = *p32;
    
    USBE0CSR0bits.TXRDY = 1;
}

/* 
    usb_LL_Ep0_FifoWrite() write data *p to EP0 FIFO 
 */
static inline void usb_LL_Ep0_FifoWrite (const void *p, uint16_t len)
{
    uint16_t len8 = len & 0x3;
    len >>= 2;

    const uint32_t *p32 = p;
    volatile uint32_t *pf32 = (uint32_t*)&USBFIFO0;
    // write 32bit data to fifo
    for (; len; len--)
        *pf32 = *p32++;
    
    const uint8_t *p8 = (const uint8_t*)p32;    
    volatile uint8_t *pf8 = (uint8_t*)&USBFIFO0;    
    // write 8bit data to fifo
    for (; len8; len8--)
        *pf8++ = *p8++;
    
    USBE0CSR0bits.TXRDY = 1;
}

/* 
    usb_LL_Ep0_BulkWrite() bulk write data *p to EP0 FIFO 
 */
 int usb_LL_Ep0_BulkWrite (const void *p, int len)
{    
    int result = 0;
    uint16_t pckt_len;

    do
    {
        volatile int timeout = 0;        
        while (USBE0CSR0bits.TXRDY && timeout < USB_TRX_TIMEOUT)
            timeout++;

        if (timeout >= USB_TRX_TIMEOUT) {
            result = -1;
            break;
        }
        
        if (len < USB_MAX_EP0_SIZE) {
            pckt_len = len;
            usb_LL_Ep0_FifoWrite( p, pckt_len );
        }
        else {
            pckt_len = USB_MAX_EP0_SIZE;
            usb_LL_Ep0_Fast_FifoWrite64( p );
        }
        
        if (pckt_len < USB_MAX_EP0_SIZE)
            break;
        
        len -= pckt_len;
        p += pckt_len;
    }while (true);
    
    USBIE0CSR0bits.DATAEND = 1;
    return result;
} // */







