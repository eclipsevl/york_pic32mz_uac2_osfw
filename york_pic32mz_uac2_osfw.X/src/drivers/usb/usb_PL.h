#ifndef _USB_PL_H_    /* Guard against multiple inclusion */
#define _USB_PL_H_

#include "usb_chapter_9.h"


/* Provide C++ Compatibility */
#ifdef __cplusplus
extern "C" {
#endif


// Define transfers size in bytes. Value must be muliple of 256 bytes (8x16x2 = 256 - 8 bytes frame, x16 - max oversampling, x2 - start divider !)
// (65536 bytes max - DMA incorrect works with max size!!!)
#define OUTPUT_TRANSFER_FIFO_SIZE       32768u      // Max vlue = 65536 - 256 = 65280u // 32768u
#define INPUT_TRANSFER_FIFO_SIZE        32768u      // 

// Output transfer microframes pooling period (mS)
#define EP1RX_PULL_TIME_mS              (0.125)

#define PULL_FREQ_NOM_44k               44.1
#define PULL_FREQ_LOW_44k               44.08236    // -400ppm
#define PULL_FREQ_HI_44k                44.11764    // +400ppm

#define PULL_FREQ_NOM_48k               48
#define PULL_FREQ_LOW_48k               47.9808     // -400ppm
#define PULL_FREQ_HI_48k                48.0192     // +400ppm

#define _ERROR      (-1)    
#define BMBREQ(bmReqType,bReq)              ((uint16_t)( ((uint16_t)(bmReqType) << 8) | ((uint16_t)(bReq) & 0x00FF) ))

    
/*****************************************************************/
/************ Standart USB Requests ******************************/
/*****************************************************************/
#define STDREQ_CLEARE_FEATURE_DEVICE        BMBREQ(USB_REQUEST_CLEAR_FEATURE, USB_SETUP_RECIPIENT_DEVICE)
#define STDREQ_CLEARE_FEATURE_INTERFACE     BMBREQ(USB_REQUEST_CLEAR_FEATURE, USB_SETUP_RECIPIENT_INTERFACE)
#define STDREQ_CLEARE_FEATURE_ENDPOINT      BMBREQ(USB_REQUEST_CLEAR_FEATURE, USB_SETUP_RECIPIENT_ENDPOINT)
#define STDREQ_GET_CONFIGURATION            BMBREQ(USB_REQUEST_GET_CONFIGURATION, (USB_SETUP_DIRN_DEVICE_TO_HOST | USB_SETUP_RECIPIENT_DEVICE))
#define STDREQ_GET_DESCRIPTOR               BMBREQ(USB_REQUEST_GET_DESCRIPTOR, (USB_SETUP_DIRN_DEVICE_TO_HOST | USB_SETUP_RECIPIENT_DEVICE))
#define STDREQ_GET_INTERFACE                BMBREQ(USB_REQUEST_GET_INTERFACE, (USB_SETUP_DIRN_DEVICE_TO_HOST | USB_SETUP_RECIPIENT_INTERFACE))
#define STDREQ_GET_STATUS_DEVICE            BMBREQ(USB_REQUEST_GET_STATUS, (USB_SETUP_DIRN_DEVICE_TO_HOST | USB_SETUP_RECIPIENT_DEVICE))
#define STDREQ_GET_STATUS_INTERFACE         BMBREQ(USB_REQUEST_GET_STATUS, (USB_SETUP_DIRN_DEVICE_TO_HOST | USB_SETUP_RECIPIENT_INTERFACE))
#define STDREQ_GET_STATUS_ENDPOINT          BMBREQ(USB_REQUEST_GET_STATUS, (USB_SETUP_DIRN_DEVICE_TO_HOST | USB_SETUP_RECIPIENT_ENDPOINT))
#define STDREQ_SET_ADDRESS                  BMBREQ(USB_REQUEST_SET_ADDRESS, USB_SETUP_RECIPIENT_DEVICE)
#define STDREQ_SET_CONFIGURATION            BMBREQ(USB_REQUEST_SET_CONFIGURATION, USB_SETUP_RECIPIENT_DEVICE)
#define STDREQ_SET_DESCRIPTOR               BMBREQ(USB_REQUEST_SET_DESCRIPTOR, USB_SETUP_RECIPIENT_DEVICE)
#define STDREQ_SET_FEATURE_DEVICE           BMBREQ(USB_REQUEST_SET_FEATURE, USB_SETUP_RECIPIENT_DEVICE)
#define STDREQ_SET_FEATURE_INTERFACE        BMBREQ(USB_REQUEST_SET_FEATURE, USB_SETUP_RECIPIENT_INTERFACE)
#define STDREQ_SET_FEATURE_ENDPOINT         BMBREQ(USB_REQUEST_SET_FEATURE, USB_SETUP_RECIPIENT_ENDPOINT)
#define STDREQ_SET_INTERFACE                BMBREQ(USB_REQUEST_SET_INTERFACE, USB_SETUP_RECIPIENT_INTERFACE)
#define STDREQ_SYNCH_FRAME                  BMBREQ(USB_REQUEST_SYNCH_FRAME, (USB_SETUP_DIRN_DEVICE_TO_HOST | USB_SETUP_RECIPIENT_ENDPOINT))


/*****************************************************************/
/************ Audio Class Specific Requests **********************/
/*****************************************************************/
#define CSREQ_SET_CURR_INTERFACE            0x0121
#define CSREQ_GET_CURR_INTERFACE            0x01A1
#define CSREQ_GET_CURR_ENDPOINT             0x81A2
#define CSREQ_SET_MIN_INTERFACE             0x0221
#define CSREQ_SET_MIN_ENDPOINT              0x0222
#define CSREQ_GET_MIN_INTERFACE             0x82A1
#define CSREQ_GET_MIN_ENDPOINT              0x82A2
#define CSREQ_SET_MAX_INTERFACE             0x0321
#define CSREQ_SET_MAX_ENDPOINT              0x0322
#define CSREQ_GET_MAX_INTERFACE             0x83A1
#define CSREQ_GET_MAX_ENDPOINT              0x83A2
#define CSREQ_SET_RESOLUTION_INTERFACE      0x0421
#define CSREQ_SET_RESOLUTION_ENDPOINT       0x0422
#define CSREQ_GET_RESOLUTION_INTERFACE      0x84A1
#define CSREQ_GET_RESOLUTION_ENDPOINT       0x84A2
#define CSREQ_SET_MEMORY_INTERFACE          0x0521
#define CSREQ_SET_MEMORY_ENDPOINT           0x0522
#define CSREQ_GET_MEMORY_INTERFACE          0x85A1
#define CSREQ_GET_MEMORY_ENDPOINT           0x85A2
#define CSREQ_GET_STATUS_INTERFACE          0xFFA1
#define CSREQ_GET_STATUS_ENDPOINT           0xFFA2    
#define CSREQ_GET_RANGE_INTERFACE           0x02A1
    
/*****************************************************************/
/************ CDC Class Specific Requests ************************/
/*****************************************************************/
#define CSREQ_SET_LINE_CODING               0x2021
#define CSREQ_GET_LINE_CODING               0x21A1
#define CSREQ_SET_CONTROL_LINE_STATE        0x2221
    

/* Macros to calculate frequency limits to isochronous feedback data
 * sfreq - float point value frequency in KHz
 * ptime - float point value pooling time in uS */
#define _FB_INT_GET(sfreq, ptime)           ( (uint16_t)( (double)sfreq * (double)ptime ) )
#define _FB_FRACT_GET(sfreq, ptime)         ( (uint16_t)( ( ( (double)sfreq * (double)ptime ) - _FB_INT_GET(sfreq, ptime) ) * 65536 ) )
#define _FB_FREQ_VAL_GET(sfreq, ptime)      ( (uint32_t)( (_FB_INT_GET(sfreq, ptime) << 16) | _FB_FRACT_GET(sfreq, ptime) ) )
    
typedef enum{
    eSTREAM_OUTPUT = 0,
    eSTREAM_INPUT,
    eSTREAM_NUM
}eSTREAM_TYPE;
    
typedef enum{
    USB_SFREQ_44k = 0,
    USB_SFREQ_48k,
    USB_SFREQ_88k,
    USB_SFREQ_96k,
    USB_SFREQ_176k,
    USB_SFREQ_192k,
    USB_SFREQ_352k,
    USB_SFREQ_384k,
    USB_SFREQ_705k,
    USB_SFREQ_768k,
    USB_FREQ_NUM
}eUSB_SAMPLING_FREQ;

typedef enum{
    USB_IF_STATE_IDLE = 0,
    USB_IF_STATE_SAMPLE_FREQ_SET,
    USB_IF_STATE_RUN
}eUSB_INTERFACE_STATE;

typedef enum{
    eSTREAM_FORMAT_UNREC = 0,
    eSTREAM_FORMAT_PCM,
    eSTREAM_FORMAT_DOP
}eSTREAM_FORMAT;

typedef struct __attribute__ ((packed)) {
    uint32_t min;
    uint32_t max;
    uint32_t stp;
}tRANGE_BLOCK;

typedef struct __attribute__ ((packed)) {
    uint16_t num;
    tRANGE_BLOCK range[USB_SFREQ_768k+1];
}tUSB_AUDIO_REQ_L3_PARAM_BLOCK;

typedef struct {
    uint32_t adr;   // data buffer pointer
    int len;   // data buffer length
    int ptr;   // current data pointer
    int rem;   // remained data if no more free space if buffer
    int min;   // minimal FIFO buffer fill
    int max;   // maximum FIFO buffer fill
}tFIFO_CONTROL;

typedef struct {
    bool format_check;
    bool dsd_detected;
    bool adr_pending;
    uint8_t adr;
    //eUSB_INTERFACE_STATE interfaceState[eSTREAM_NUM];
    eUSB_SAMPLING_FREQ streamSamplingFreq[eSTREAM_NUM];
    uint8_t streamInterfaceAltsetting[eSTREAM_NUM];
    uint8_t configuration;
    uint8_t setFeature[USB_SETUP_REQUEST_RECIPIENT_OTHER];
    uint16_t frameNumber;
}tUSB_CON_STAT;

typedef struct __attribute__ ((packed)) {
    uint32_t dwDTERate;     // Data terminal rate, in bits per second
    uint8_t bCharFormat;    // 0 - 1 Stop bit, 1 - 1.5 Stop bits, 2 - 2 Stop bits
    uint8_t bParityType;    // 0 - None, 1 - Odd, 2 - Even, 3 - Mark, 4 - Space
    uint8_t bDataBits;      // Data bits (5, 6, 7, 8 or 16)
}tUSB_CDC_LINE_CODING;

/*
Carrier control for half duplex modems. This signal corresponds to V.24 signal
105 and RS-232 signal RTS.
0 - Deactivate carrier
1 - Activate carrier
The device ignores the value of this bit when operating in full duplex mode.
 
Indicates to DCE if DTE is present or not. This signal corresponds to V.24
signal 108/2 and RS-232 signal DTR.
0 - Not Present
1 - Present*/
typedef union __attribute__ ((packed)) {
    uint16_t dw;
    struct {
        uint16_t D0         :1;
        uint16_t D1         :1;
        uint16_t reserved   :14;
    }bits;
}tUSB_CDC_LINE_STATE;


/*****************************************************************************
 Global variables
 ****************************************************************************/


/*****************************************************************************
 Global functions declarations
 ****************************************************************************/
void usb_PL_init (void);
void usb_PL_tasks (void);



    /* Provide C++ Compatibility */
#ifdef __cplusplus
}
#endif

#endif /* _USB_PL_H_ */

/* *****************************************************************************
 End of File
 */
