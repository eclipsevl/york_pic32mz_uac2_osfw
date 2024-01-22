#ifndef _USB_DESCRIPTORS_H_    /* Guard against multiple inclusion */
#define _USB_DESCRIPTORS_H_

#include "usb_audio_v1_0.h"
#include "usb_audio_v2_0.h"
#include "usb_chapter_9.h"
#include "usbd_def.h"
#include "usbd_audio_core.h"

/* Provide C++ Compatibility */
#ifdef __cplusplus
extern "C" {
#endif
    
// Microchip VID/PID
#define USB_VID 0x04D8
#define USB_PID 0x0064
    
/*** USB Device Stack Configuration ***/

/*******************************************
 * USB Device Layer Descriptors
 *******************************************/

/*******************************************
 *  USB Device Descriptor 
 *******************************************/
uint16_t GetDeviceDescriptor(uint8_t *desc);
uint16_t GetDeviceQualifierDescriptor(uint8_t *desc);
uint16_t GetDeviceConfigurationDescriptor(uint8_t *desc);

/**************************************
 *  String descriptors.
 *************************************/
uint16_t GetLangIDStrDescriptor(uint8_t *desc);
uint16_t GetManufacturerStrDescriptor(uint8_t *desc);
uint16_t GetProductStrDescriptor(uint8_t *desc);
uint16_t GetSerialStrDescriptor(uint8_t *desc);
uint16_t GetConfigurationStrDescriptor(uint8_t *desc);
uint16_t GetInterfaceStrDescriptor(uint8_t *desc);
uint16_t GetUsrStrDescriptor(uint8_t *desc, uint8_t idx);

    /* Provide C++ Compatibility */
#ifdef __cplusplus
}
#endif

#endif /* _USB_DESCRIPTORS_H_ */

/* *****************************************************************************
 End of File
 */
