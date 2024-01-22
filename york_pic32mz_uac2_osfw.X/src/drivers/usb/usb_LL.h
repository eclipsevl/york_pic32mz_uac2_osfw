#ifndef _USB_LL_H_    /* Guard against multiple inclusion */
#define _USB_LL_H_

/* Provide C++ Compatibility */
#ifdef __cplusplus
extern "C" {
#endif
   
// 1 = The USB D+/D- lines are enabled and active, 0 = The USB D+/D- lines are disabled and are tri-stated.
#define _USB_SOFT_CONNECT()                         {   USBCSR0bits.SOFTCONN = 1; }
    
// Read 8 byte data from EP0 fifo to pointer "ptr"
#define _EP0_Control_Cmd_Read(adr)                  {   uint32_t *p = (uint32_t*)(adr);\
                                                        *p++ = USBFIFO0;\
                                                        *p = USBFIFO0; \
                                                        USBE0CSR0bits.RXRDYC = 1; }

#define _EP1_FeedBack_Data_Send(data)               {   USBFIFO1 = data;\
                                                        USBE1CSR0bits.TXPKTRDY = 1; } // Send New feed back data
    
#define USB_DEV_SPEED   0x1   // 1 - high speed, 2 - Full speed

#define _USBDMA_EP1RX_Transfer_Start(adr, size)     {   USBDMA1A = adr;\
                                                        USBDMA1N = size;\
                                                        USBDMA1Cbits.DMAEN = 1; }  // Start USB DMA1 transaction
    
#define _USBDMA_EP5TX_Transfer_Start(adr, size)     {   USBDMA5A = adr;\
                                                        USBDMA5N = size;\
                                                        USBDMA5Cbits.DMAEN = 1; }  // Start USB DMA5 transaction
    
    
#define _USBDMA_EP7RX_ADR_SET(adr)                  {   USBDMA7A = KVA_TO_PA( adr ); }
#define _USBDMA_EP7RX_Transfer_Start(size)          {   USBDMA7N = size;\
                                                        USBDMA7Cbits.DMAEN = 1; }  // Start USB DMA7 transaction

void usb_LL_Init (void);
void usb_LL_initReset (void);
int usb_LL_Ep0_BulkRead (void *p, int len);
int usb_LL_Ep0_BulkWrite (const void *p, int len);

/* Provide C++ Compatibility */
#ifdef __cplusplus
}
#endif

#endif /* _USB_LL_H_ */

/* *****************************************************************************
 End of File
 */
