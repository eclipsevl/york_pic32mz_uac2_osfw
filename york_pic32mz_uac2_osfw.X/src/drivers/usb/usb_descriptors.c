/* York PIC32MZ UAC2.0 Firmware                */
/* Author: eclipsevl (Vladislav Knyazkov) */

#include "../../app/system.h"
#include "usb_descriptors.h"

/**************************************
 *  String descriptors.
 *************************************/
char* s_string1 = "VK Audio";
char* s_string2 = "York - Open Source Composite Device";
char* s_string3 = "0100";
char* s_string4 = "Configuration";
char* s_string5 = "York Open Source USB Audio 2.0";
char* s_string6 = "York Open Source USB Audio 2.0";
char* s_string7 = "Audio Interface Description";


static const uint8_t deviceDescriptor[] __attribute__ ((aligned (4))) =
{
    0x12,                       /*bLength */
    USB_DESC_TYPE_DEVICE,       /*bDescriptorType*/
    0x00,0x02,                  /*bcdUSB */    
    0xEF,                       /*bDeviceClass*/
    0x02,                       /*bDeviceSubClass*/
    0x01,                       /*bDeviceProtocol*/
    USB_MAX_EP0_SIZE,           /*bMaxPacketSize*/
    LOBYTE(USB_VID),            /*idVendor*/
    HIBYTE(USB_VID),            /*idVendor*/
    LOBYTE(USB_PID),            /*idProduct*/
    HIBYTE(USB_PID),            /*idProduct*/
    0x00,0x02,                  /*bcdDevice rel. 2.00*/    
    USBD_IDX_MFC_STR,           /*Index of manufacturer  string*/
    USBD_IDX_PRODUCT_STR,       /*Index of product string*/
    USBD_IDX_SERIAL_STR,        /*Index of serial number string*/
    USBD_MAX_NUM_CONFIGURATION  /*bNumConfigurations*/
};

/*******************************************
 *  USB Device Qualifier Descriptor for this
 *  demo.
 *******************************************/
static const uint8_t deviceQualifierDescriptor[] __attribute__ ((aligned (4))) =
{
    0x0A,                               // Device Qualifier Descriptor Size
    USB_DESCRIPTOR_DEVICE_QUALIFIER,    // Device Qualifier Type
    0x00,0x02,                            
    0xEF,                               // Class Code
    0x02,                               // Subclass code
    0x01,                               // Protocol code
    USB_MAX_EP0_SIZE,                   // Maximum packet size for endpoint 0
    USBD_MAX_NUM_CONFIGURATION,         // Number of possible configurations
    0x00                                // Reserved for future use.
}; 

static uint8_t USBD_GetLen(uint8_t *buf)
{
    uint8_t  len = 0;

    while (*buf != 0) 
    {
        len++;
        buf++;
    }

    return len;
}

void USBD_GetString(uint8_t *desc, uint8_t *unicode, uint16_t *len)
{
  uint8_t idx = 0;
  
  if (desc != 0) 
  {
    *len =  USBD_GetLen(desc) * 2 + 2;    
    unicode[idx++] = *len;
    unicode[idx++] =  USB_DESC_TYPE_STRING;
    
    while (*desc != 0) 
    {
      unicode[idx++] = *desc++;
      unicode[idx++] =  0x00;
    }
  } 
}

uint16_t GetLangIDStrDescriptor(uint8_t *desc)
{
    uint16_t len = 4;
    desc[0] = 0x04; /* 	Descriptor Length                       */
    desc[1] = 0x03; /*  DescriptorType: STRING  				*/
    desc[2] = 0x09; /* 	Language ID: English					*/
    desc[3] = 0x04;

    return len;
}  

uint16_t GetManufacturerStrDescriptor(uint8_t *desc)
{
    uint16_t len = 0;
    USBD_GetString(s_string1, desc, &len);
    return len;
}  

uint16_t GetProductStrDescriptor(uint8_t *desc)
{
    uint16_t len = 0;
    USBD_GetString(s_string2, desc, &len);
    return len;
}  

uint16_t GetSerialStrDescriptor(uint8_t *desc)
{
    uint16_t len = 0;
    USBD_GetString(s_string3, desc, &len);
    return len;
}  

uint16_t GetConfigurationStrDescriptor(uint8_t *desc)
{
    uint16_t len = 0;
    USBD_GetString(s_string4, desc, &len);
    return len;
}  

uint16_t GetInterfaceStrDescriptor(uint8_t *desc)
{
    uint16_t len = 0;
    USBD_GetString(s_string5, desc, &len);
    return len;
}  

uint16_t GetUsrStrDescriptor(uint8_t *desc, uint8_t idx)
{
    uint16_t len = 0;

    switch(idx)
    {
        case 6:
            USBD_GetString(s_string6, desc, &len);
            break;
            
        default:  
            USBD_GetString("Default", desc, &len);           
    }

    return len;
}  

uint16_t GetDeviceDescriptor(uint8_t *desc)
{
    uint16_t len = sizeof(deviceDescriptor);
    memcpy(desc,deviceDescriptor,len);
    return len;
} 

uint16_t GetDeviceQualifierDescriptor(uint8_t *desc)
{
    uint16_t len = sizeof(deviceQualifierDescriptor);
    memcpy(desc,deviceQualifierDescriptor,len);
    return len;
} 

/*******************************************
 *  Configuration Descriptor
 *******************************************/
static const uint8_t ConfigurationDescriptor[] __attribute__ ((aligned (4))) = 
{
    /* Configuration descriptor 
     * The interface descriptor is immediately after the descriptor. The interface 
     * descriptor indicates the type of interface and the number of corresponding endpoints */
    0x09,                                           /* bLength */
    USB_DESC_TYPE_CONFIGURATION,                    /* bDescriptorType */
    0x86,              /* wTotalLength  226 bytes + 59 cdc*/
    0x00,
    0x02,  /* bNumInterfaces */
    0x01,                                           /* bConfigurationValue */
    USBD_IDX_CONFIG_STR,                            /* iConfiguration */
    0x80,                                           /* bmAttributes: Bus Powered according to user configuration */
    USBD_MAX_POWER,                                 /* bMaxPower = 100 mA */
    /* 09 byte */
    
    /* Interface Association descriptor 
     * USB composite devices are generally implemented with Interface Association Descriptor (IAD), 
     * which is to add IAD descriptor before the interface to be merged */
    USB_AUDIO_IAD_DESC_SIZE,                /* bLength */
    USB_DESC_TYPE_IAD,                      /* bDescriptorType */
    AUDIO_FIRST_INTERFACE_NUM,              /* bFirstInterface */
    AUDIO_INTERFACES_COUNT,                 /* bInterfaceCount */
    USB_DEVICE_CLASS_AUDIO,                 /* bFunctionClass */
    USB_AUDIO_V2_FUNCTION_SUBCLASS_UNDEFINED, /* bFunctionSubClass */
    USB_AUDIO_V2_AF_VERSION_02_00,          /* bFunctionProtocol */
    0x05,                                   /* iFunction - USB Audio 2.0 */
    /* 08 byte */

    /* Interface descriptor */
    AUDIO_INTERFACE_DESC_SIZE,              /* bLength */
    USB_DESC_TYPE_INTERFACE,                /* bDescriptorType */
    AUDIO_CONTROL_INTERFACE_NUM,            /* bInterfaceNumber */
    0x00,                                   /* bAlternateSetting */
    0x00,                                   /* bNumEndpoints */
    USB_AUDIO_V2_CLASS_CODE,                /* bInterfaceClass */
    USB_AUDIO_V2_AUDIOCONTROL,              /* bInterfaceSubClass */
    USB_AUDIO_V2_IP_VERSION_02_00,          /* bInterfaceProtocol */
    0x06,                                   /* iInterface - Topology Control */
    /* 09 byte */

    /* AudioControl Interface Descriptor
     * The audio device interface header should contain a class-specific AC interface header descriptor,
     * which is used to define other functional ports of the interface.
     * Class-specific AC interface header descriptor (AC interface header)
     *******************************************************************/
    AUDIO_INTERFACE_DESC_SIZE,              /* bLength */
    USB_AUDIO_V2_CS_INTERFACE,              /* bDescriptorType */
    USB_AUDIO_V2_HEADER,                    /* bDescriptorSubtype */
    0x00,0x02,          /* 2.00 */          /* bcdADC */
    USB_AUDIO_V2_DESKTOP_SPEAKER,                    /* bCategory */
    0x2E,0x00,                              /* wTotalLength  */
    0x00,                                   /* bmControls */
    /* 09 byte */

    /* AudioControl Interface Descriptor
     * Audio device frequency description Clock source for OUT traffic */
    0x08,                                   /* bLength */
    USB_AUDIO_V2_CS_INTERFACE,              /* bDescriptorType */
    USB_AUDIO_V2_CLOCK_SOURCE,              /* bDescriptorSubtype */
    AUDIO_SCLOCK_TERMINAL_OUTPUT,           /* bClockID */
    0x00,                                   /* bmAttributes - 0 - External clock cource, 1 - Internal fixed clock, 3 - Internal programmable clock */
    0x03,                                   /* bmControls - Clock Frequency Control (read/write) */
    0x00,                                   /* bAssocTerminal - Constant corresponding output port ID */
    0x00,                                   /* iClockSource -  */
    /* 08 byte */

    /* AudioControl Interface Descriptor
     * USB Input Terminal for OUTPUT Descriptor */
    0x11,                                   /* bLength */
    USB_AUDIO_V2_CS_INTERFACE,              /* bDescriptorType */
    USB_AUDIO_V2_INPUT_TERMINAL,            /* bDescriptorSubtype */
    AUDIO_IN_TERMINAL_OUTPUT,               /* bTerminalID */
    0x01,0x01,                              /* wTerminalType AUDIO_TERMINAL_USB_STREAMING  0x0101 */
    0x00,                                   /* bAssocTerminal */
    AUDIO_SCLOCK_TERMINAL_OUTPUT,           /* bCSourceID */
    0x02,                                   /* bNrChannels */
    0x00,0x00,0x00,0x00,                    /* bmChannelConfig 0x00000000  all channels control */
    0x00,0x00,                              /* bmControls - 0x0003 Copy Protect Control (read/write) */
    0x00,                                   /* iChannelNames */
    0x00,                                   /* iTerminal - USBH Out */
    /* 17 byte */

    
    /* AudioControl Interface Descriptor
     * USB Output Terminal for OUTPUT Descriptor */
    0x0C,                                   /* bLength */
    USB_AUDIO_V2_CS_INTERFACE,              /* bDescriptorType */
    USB_AUDIO_V2_OUTPUT_TERMINAL,           /* bDescriptorSubtype */
    AUDIO_OUT_TERMINAL_OUTPUT,              /* bTerminalID */
    0x02,0x06,                              /* wTerminalType - A generic digital audio interface */
    0x00,                                   /* bAssocTerminal */
    AUDIO_IN_TERMINAL_OUTPUT,               /* bSourceID */
    AUDIO_SCLOCK_TERMINAL_OUTPUT,           /* bCSourceID */
    0x00,0x00,                              /* bmControls - 0x0003 Copy Protect Control (read/write) */
    0x00,                                   /* iTerminal - USBD In */
    /* 12 byte */

    /* Interface Descriptor - Audio Streaming OUT Interface - Alt0 */
    AUDIO_INTERFACE_DESC_SIZE,              /* bLength */
    USB_DESC_TYPE_INTERFACE,                /* bDescriptorType */
    AUDIO_STREAMING_OUTPUT_INTERFACE_NUM,   /* bInterfaceNumber */
    USB_AUDIO_OUTPUT_ALTSET_OFF,            /* bAlternateSetting */
    0x00,                                   /* bNumEndpoints */
    USB_AUDIO_V2_CLASS_CODE,                /* bInterfaceClass */
    USB_AUDIO_V2_AUDIOSTREAMING,            /* bInterfaceSubClass */
    USB_AUDIO_V2_IP_VERSION_02_00,          /* bInterfaceProtocol */
    0x00,                                   /* iInterface - Playback Inactive */
    /* 09 byte */
    
    /* Interface Descriptor - Audio Streaming OUT Interface - Alt1 */
    AUDIO_INTERFACE_DESC_SIZE,              /* bLength */
    USB_DESC_TYPE_INTERFACE,                /* bDescriptorType */
    AUDIO_STREAMING_OUTPUT_INTERFACE_NUM,   /* bInterfaceNumber */
    USB_AUDIO_OUTPUT_ALTSET_2CH_ON,         /* bAlternateSetting */
    0x02,                                   /* bNumEndpoints */
    USB_AUDIO_V2_CLASS_CODE,                /* bInterfaceClass */
    USB_AUDIO_V2_AUDIOSTREAMING,            /* bInterfaceSubClass */
    USB_AUDIO_V2_IP_VERSION_02_00,          /* bInterfaceProtocol */
    0x00,                                   /* iInterface - Playback Active */
    /* 09 byte */

    /* AudioStreaming Interface Descriptor - Audio Stream OUT Interface Desc */
    /* Interface 1, Alternate Setting 1                                           */
    0x10,                                   /* bLength */
    USB_AUDIO_V2_CS_INTERFACE,              /* bDescriptorType */
    USB_AUDIO_V2_AS_GENERAL,                /* bDescriptorSubtype */
    AUDIO_IN_TERMINAL_OUTPUT,               /* bTerminalLink */
    0x00,                                   /* bmControls */
    USB_AUDIO_V2_FORMAT_TYPE_I,             /* bFormatType */
    0x01,0x00,0x00,0x00,                    /* bmFormats D0 - PCM */
    0x02,                                   /* bNrChannels */
    0x00,0x00,0x00,0x00,                    /* bmChannelConfig 0x00000000 driver choose channels */
    0x00,                                   /* iChannelNames */
    /* 16 byte */

    /* AudioStreaming Interface Descriptor - Audio USB_OUT Format */
    0x6,                                    /* bLength */
    USB_AUDIO_V2_CS_INTERFACE,              /* bDescriptorType */
    AUDIO_STREAMING_FORMAT_TYPE,            /* bDescriptorSubtype */
    USB_AUDIO_V2_FORMAT_TYPE_I,             /* bFormatType */
    0x04,                                   /* bSubslotSize :  4 Bytes per frame (32bits) */
    32,                                     /* bBitResolution (32-bits per sample) */
    /* 06 byte */ 

    /* Endpoint Descriptor - STD AS ISO OUT Endpoint */
    0x07,                                   /* bLength */
    USB_DESC_TYPE_ENDPOINT,                 /* bDescriptorType */
    AUDIO_OUT_EP,                           /* bEndpointAddress 1 out endpoint */
    0x05,                                   /* bmAttributes Asynchronous Isochronous mode */
    0x00,0x04,                              /* wMaxPacketSize in Bytes 1024 per micro-frame */
    0x01,                                   /* bInterval (Frames/Subframes) 2^(1-1)=1 */
    /* 07 byte */
    
    /* AudioControl Endpoint Descriptor - CS AS ISO OUT Endpoint */
    0x08,                                   /* bLength */
    USB_AUDIO_V2_CS_ENDPOINT,               /* bDescriptorType */
    USB_AUDIO_V2_EP_GENERAL,                /* bDescriptorSubtype */
    0x00,                                   /* bmAttributes */
    0x00,                                   /* bmControls */
    0x00,                                   /* bLockDelayUnits - Undefined */
    0x00,0x00,                              /* wLockDelay */
    /* 08 byte */
    
    /* Endpoint Descriptor - STD AS ISO Feedback Endpoint */
    0x07,                                   /* bLength */
    USB_DESC_TYPE_ENDPOINT,                 /* bDescriptorType */
    AUDIO_FB_EP,                            /* bEndpointAddress 1 int endpoint */
    0x11,                                   /* bmAttributes FeedBack Isochronous mode */
    0x04,0x00,                              /* wMaxPacketSize in Bytes 4 per micro-frame */
    0x03,                                   /* bInterval (Frames/Subframes) 2^(3-1)=4 */
    /* 07 byte */
};

uint16_t GetDeviceConfigurationDescriptor(uint8_t *desc)
{
    uint16_t len = 0;

    memcpy(desc+len, &ConfigurationDescriptor, sizeof(ConfigurationDescriptor));
    len = sizeof(ConfigurationDescriptor);  
    
    return len;
} 