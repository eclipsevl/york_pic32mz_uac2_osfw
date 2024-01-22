/* York PIC32MZ UAC2.0 Firmware                */
/* Author: Dortonyan (Aliaksei Fisko)     */
/* Author: eclipsevl (Vladislav Knyazkov) */

#include "../../app/system.h"
#include "usb_descriptors.h"
#include "usb_PL.h"
#include <stdio.h>



/*****************************************************************************
 Data Types
 ****************************************************************************/
typedef struct{ // feedback freq values in 16.16 format
    uint32_t nom;   // nominal frequency value
    uint32_t lo;    // low limit frequency
    uint32_t hi;    // high limit frequency
}tFbSettFreq;

typedef struct{ // Data size to Input Stream transmitt
    uint16_t nom;   // nominal size value
    uint16_t lo;    // low size value
    uint16_t hi;    // high size value
}tBlockSize;

int sample;
int first_feedback;
/*****************************************************************************
 Constatnt data
 ****************************************************************************/
/* 
 * tFeedBackFreq - Frequences array to feedback with +/-400ppm limits 
 */
static const tFbSettFreq tFeedBackFreq[USB_FREQ_NUM] = { 
    {
     _FB_FREQ_VAL_GET(PULL_FREQ_NOM_44k, EP1RX_PULL_TIME_mS),
     _FB_FREQ_VAL_GET(PULL_FREQ_LOW_44k, EP1RX_PULL_TIME_mS),
     _FB_FREQ_VAL_GET(PULL_FREQ_HI_44k, EP1RX_PULL_TIME_mS)
 }, {
     _FB_FREQ_VAL_GET(PULL_FREQ_NOM_48k, EP1RX_PULL_TIME_mS),
     _FB_FREQ_VAL_GET(PULL_FREQ_LOW_48k, EP1RX_PULL_TIME_mS),
     _FB_FREQ_VAL_GET(PULL_FREQ_HI_48k, EP1RX_PULL_TIME_mS)
 }, {
     _FB_FREQ_VAL_GET(PULL_FREQ_NOM_44k * 2, EP1RX_PULL_TIME_mS),
     _FB_FREQ_VAL_GET(PULL_FREQ_LOW_44k * 2, EP1RX_PULL_TIME_mS),
     _FB_FREQ_VAL_GET(PULL_FREQ_HI_44k * 2, EP1RX_PULL_TIME_mS)
 }, {
     _FB_FREQ_VAL_GET(PULL_FREQ_NOM_48k * 2, EP1RX_PULL_TIME_mS),
     _FB_FREQ_VAL_GET(PULL_FREQ_LOW_48k * 2, EP1RX_PULL_TIME_mS),
     _FB_FREQ_VAL_GET(PULL_FREQ_HI_48k * 2, EP1RX_PULL_TIME_mS)
 }, {
     _FB_FREQ_VAL_GET(PULL_FREQ_NOM_44k * 4, EP1RX_PULL_TIME_mS),
     _FB_FREQ_VAL_GET(PULL_FREQ_LOW_44k * 4, EP1RX_PULL_TIME_mS),
     _FB_FREQ_VAL_GET(PULL_FREQ_HI_44k * 4, EP1RX_PULL_TIME_mS)
 }, {
     _FB_FREQ_VAL_GET(PULL_FREQ_NOM_48k * 4, EP1RX_PULL_TIME_mS),
     _FB_FREQ_VAL_GET(PULL_FREQ_LOW_48k * 4, EP1RX_PULL_TIME_mS),
     _FB_FREQ_VAL_GET(PULL_FREQ_HI_48k * 4, EP1RX_PULL_TIME_mS)
 }, {
     _FB_FREQ_VAL_GET(PULL_FREQ_NOM_44k * 8, EP1RX_PULL_TIME_mS),
     _FB_FREQ_VAL_GET(PULL_FREQ_LOW_44k * 8, EP1RX_PULL_TIME_mS),
     _FB_FREQ_VAL_GET(PULL_FREQ_HI_44k * 8, EP1RX_PULL_TIME_mS)
 }, {
     _FB_FREQ_VAL_GET(PULL_FREQ_NOM_48k * 8, EP1RX_PULL_TIME_mS),
     _FB_FREQ_VAL_GET(PULL_FREQ_LOW_48k * 8, EP1RX_PULL_TIME_mS),
     _FB_FREQ_VAL_GET(PULL_FREQ_HI_48k * 8, EP1RX_PULL_TIME_mS)
 }, {
     _FB_FREQ_VAL_GET(PULL_FREQ_NOM_44k * 16, EP1RX_PULL_TIME_mS),
     _FB_FREQ_VAL_GET(PULL_FREQ_LOW_44k * 16, EP1RX_PULL_TIME_mS),
     _FB_FREQ_VAL_GET(PULL_FREQ_HI_44k * 16, EP1RX_PULL_TIME_mS)
 }, {
     _FB_FREQ_VAL_GET(PULL_FREQ_NOM_48k * 16, EP1RX_PULL_TIME_mS),
     _FB_FREQ_VAL_GET(PULL_FREQ_LOW_48k * 16, EP1RX_PULL_TIME_mS),
     _FB_FREQ_VAL_GET(PULL_FREQ_HI_48k * 16, EP1RX_PULL_TIME_mS)
 } };
/* 
 * Data size array to EP5 transmitt
 */
static const tBlockSize tEp5DataSize[USB_FREQ_NUM] = {
    {48, 40, 56},
    {48, 40, 56},
    {88, 80, 96},
    {96, 88, 104},
    {176, 168, 184},
    {192, 184, 200},
    {352, 344, 360},
    {384, 376, 392},
    {704, 696, 712},
    {768, 760, 776}
};
/* 
 * I2S clock dividers array to I2S sclk output frequency control
 */
#ifdef I2S_MCLK_512Fs
static const uint8_t i2sClkDiv[USB_FREQ_NUM] = {2, 2, 1, 1, 0, 0, 0, 0, 0, 0};
#endif

#ifdef I2S_MCLK_1024Fs
static const uint8_t i2sClkDiv[USB_FREQ_NUM] = {4, 4, 2, 2, 1, 1, 0, 0, 0, 0};
#endif


/*****************************************************************************
 Local variables
 ****************************************************************************/
static uint8_t dbuf[512] __attribute__((aligned (4)));
static tUSB_CON_STAT tUsbControlStatus = {0};

// EP0 buffer and control variables
static uint8_t ep0buf[64] __attribute__((aligned (4)));
static uint8_t statusAnswer[] = {0x00, 0x00}; // Double Zero

// EP1_OUT buffer and control variables
static uint8_t ep1buf[OUTPUT_TRANSFER_FIFO_SIZE] __attribute__((coherent)) __attribute__((aligned (16)));
static tFbSettFreq tFreqLims;
static tFIFO_CONTROL tEp1Fifo;

// EP5_IN buffer and control variables
static uint8_t ep5buf[INPUT_TRANSFER_FIFO_SIZE] __attribute__((coherent)) __attribute__((aligned (16)));
static uint16_t ep5txsize;  // size of data to USB transmitt
static tBlockSize tEp5BlockSize; // size of data to transimt to host in one packet
static tFIFO_CONTROL tEp5Fifo;

/*****************************************************************************
 Global variables
 ****************************************************************************/

/*
    usb_PL_init()
 */
void usb_PL_init (void)
{  
    sample = 0;
    first_feedback = 1;
    // EP-OUT data buffer initialisation
    tEp1Fifo.adr = KVA_TO_PA(ep1buf);
    // EP-FeedBack variables initialisation
    tFreqLims = tFeedBackFreq[USB_SFREQ_48k];
    // EP-IN data buffer initialisation
    tEp5Fifo.adr = KVA_TO_PA(ep5buf);
    // EP-FeedBack variables initialisation
    tEp5BlockSize = tEp5DataSize[USB_SFREQ_48k];
    
    tUsbControlStatus.streamSamplingFreq[eSTREAM_OUTPUT] = USB_SFREQ_48k;
    tUsbControlStatus.streamSamplingFreq[eSTREAM_INPUT] = USB_SFREQ_48k;
    
    _CLK_I2S_Div_Set( i2sClkDiv[USB_SFREQ_48k] );
    
    _DMA_OutputSourceAddress_Set(tEp1Fifo.adr);
    _DMA_InputDestinationAddress_Set(tEp5Fifo.adr);
    
    usb_LL_Init();
}


/*
    usb_PL_InputSamplingFreq_Set()
 */
static inline void usb_PL_InputSamplingFreq_Set ( void )
{
    switch (tUsbControlStatus.streamSamplingFreq[eSTREAM_INPUT]) {
        default: // 44/48k
            FS0_SET_LOW();
            FS1_SET_LOW();
            break;

        case USB_SFREQ_88k:
        case USB_SFREQ_96k:
            FS0_SET_HIGH();
            FS1_SET_LOW();
            break;

        case USB_SFREQ_176k:
        case USB_SFREQ_192k:
        case USB_SFREQ_352k:
        case USB_SFREQ_384k:
            FS0_SET_LOW();
            FS1_SET_HIGH();
            break;
    }
}

/*
    usb_PL_OutputSamplingFreq_Set()
 */
static inline void usb_PL_OutputSamplingFreq_Set ( void )
{
    static eSYSCLK_SPEED sysSpeed_prev = eSYSCLK_SPEED_LOW;
    static eUSB_SAMPLING_FREQ sfreq_prev = USB_SFREQ_48k;
    static bool dsd_detected_prev = false;
    
    uint8_t freq_chng = sfreq_prev ^ tUsbControlStatus.streamSamplingFreq[eSTREAM_OUTPUT];
    uint8_t format_chng = dsd_detected_prev ^ tUsbControlStatus.dsd_detected;    
    
    if (freq_chng || format_chng)
    {
        eSYSCLK_SPEED sys_speed = eSYSCLK_SPEED_LOW;
        
        if (tUsbControlStatus.dsd_detected) {
            if (tUsbControlStatus.streamSamplingFreq[eSTREAM_OUTPUT] > USB_SFREQ_384k)
                sys_speed = eSYSCLK_SPEED_HIGH;
            else if (tUsbControlStatus.streamSamplingFreq[eSTREAM_OUTPUT] > USB_SFREQ_192k)
                sys_speed = eSYSCLK_SPEED_MID;
        }
        else if (tUsbControlStatus.streamInterfaceAltsetting[eSTREAM_OUTPUT] == USB_AUDIO_OUTPUT_ALTSET_8CH_ON)
        {
            if (tUsbControlStatus.streamSamplingFreq[eSTREAM_OUTPUT] > USB_SFREQ_96k)
                sys_speed = eSYSCLK_SPEED_HIGH;
            else if (tUsbControlStatus.streamSamplingFreq[eSTREAM_OUTPUT] > USB_SFREQ_48k)
                sys_speed = eSYSCLK_SPEED_MID;
        }
        
        uint8_t sysSpeed_chng = 0;//sysSpeed_prev ^ sys_speed;
        
        tFreqLims = tFeedBackFreq[tUsbControlStatus.streamSamplingFreq[eSTREAM_OUTPUT]];
        _I2S_Output_Transfer_Stop();
        MUTE_SET_HIGH();
        
        if (tUsbControlStatus.streamInterfaceAltsetting[eSTREAM_INPUT] == USB_AUDIO_INPUT_ALTSET_2CH_ON)
        if (sysSpeed_chng) {
            _I2S_Input_Transfer_Stop();
            _DMA_Input_Transfer_Stop();
        }
        
        if ((freq_chng & 0xE) || format_chng) {   // if frequency oversampling or dsd/pcm mode is chainged
            // Set clock divider for I2S output modules
            eUSB_SAMPLING_FREQ freq_div = tUsbControlStatus.streamSamplingFreq[eSTREAM_OUTPUT];            
            if (tUsbControlStatus.dsd_detected) // I2S clock frequency for DSD is x4 lower than for PCM
                freq_div -= 4;
            
            _CLK_I2S_Div_Set( i2sClkDiv[freq_div] );
        }
        
        if (format_chng) { // If DSD/PCM mode is chainged
            if (tUsbControlStatus.dsd_detected) {
                _I2S_Output_Transfer_Config_DSD();
            }
            else {
                _I2S_Output_Transfer_Config_PCM();
                dma_output_stop(); // Stop transfer because DSD in mute
            }
        }
        
        if (sysSpeed_chng)
            sys_performance_set( sys_speed );
        
        _I2S_Output_Transfer_Start();
        
        if (tUsbControlStatus.streamInterfaceAltsetting[eSTREAM_INPUT] == USB_AUDIO_INPUT_ALTSET_2CH_ON)
        if (sysSpeed_chng) {
            _PORT_Input_Transfer_Sync();    // Start Input Transfer on edge LRCK input
        }
        
        sfreq_prev = tUsbControlStatus.streamSamplingFreq[eSTREAM_OUTPUT];
        dsd_detected_prev = tUsbControlStatus.dsd_detected;
        sysSpeed_prev = sys_speed;
    }
}

/*
    usb_PL_InterfaceState_Update()
 */
static inline void usb_PL_InterfaceState_Update ( USB_SETUP_PACKET *p )
{
    eSTREAM_TYPE stream;
    
    if (p->bIntfID == AUDIO_STREAMING_OUTPUT_INTERFACE_NUM)
        stream = eSTREAM_OUTPUT;
    else
        return;
    
    tUsbControlStatus.streamInterfaceAltsetting[stream] = p->bAltID;
    
    switch (stream) {
        case eSTREAM_OUTPUT:
            if (p->bAltID == USB_AUDIO_OUTPUT_ALTSET_2CH_ON) {
                // Calculate buffer length in dependence of sampling frequency
                tEp1Fifo.len = sizeof(ep1buf) >> ( (USB_SFREQ_768k - tUsbControlStatus.streamSamplingFreq[stream]) >> 1 );
                tEp1Fifo.min = tEp1Fifo.len >> 4; // 4
                tEp1Fifo.max = tEp1Fifo.len >> 3; //3
                tEp1Fifo.ptr = tEp1Fifo.len >> 1; //1
                tEp1Fifo.rem = 0;
                // Start stream from scheck format of received packets
                tUsbControlStatus.format_check = true;
                _EP1_FeedBack_Data_Send(tFreqLims.nom);
            }
            if (p->bAltID == USB_AUDIO_OUTPUT_ALTSET_OFF) {
                if (tUsbControlStatus.dsd_detected == true)
                    dma_output_dsd_mute();
                else
                    dma_output_stop();
                
                MUTE_SET_HIGH();
            }
            break;
    }
}

/*
    usb_PL_SamplingFreq_Set()
 */
static inline void usb_PL_SamplingFreq_Set ( eSTREAM_TYPE stream )
{
    uint32_t freq;
    usb_LL_Ep0_BulkRead( &freq, sizeof(freq) );

    eUSB_SAMPLING_FREQ sfreq;

    switch (freq) {
        case 44100: sfreq = USB_SFREQ_44k; break;
        case 48000: sfreq = USB_SFREQ_48k; break;
        case 88200: sfreq = USB_SFREQ_88k; break;
        case 96000: sfreq = USB_SFREQ_96k; break;
        case 176400: sfreq = USB_SFREQ_176k; break;
        case 192000: sfreq = USB_SFREQ_192k; break;
        case 352800: sfreq = USB_SFREQ_352k; break;
        case 384000: sfreq = USB_SFREQ_384k; break;
        case 705600: sfreq = USB_SFREQ_705k; break;
        case 768000: sfreq = USB_SFREQ_768k; break;
        default: return;
    }

    if (stream == eSTREAM_OUTPUT) {
        uint8_t freq_chng = sfreq ^ tUsbControlStatus.streamSamplingFreq[eSTREAM_OUTPUT];

        if (freq_chng & 0x1) {   // if frequency domain is chainged                
            if (tUsbControlStatus.streamInterfaceAltsetting[eSTREAM_INPUT] == USB_AUDIO_INPUT_ALTSET_2CH_ON) {
                _I2S_Input_Transfer_Stop();
                _DMA_Input_Transfer_Stop();
            }

            if (sfreq & 0x1) 
            {
                OSCEN_FS44_SET_LOW();  
                OSCEN_FS48_SET_HIGH();  
                OSCSEL_SET_LOW();  // Set 48kHz clock domain
            }else{
                OSCEN_FS48_SET_LOW();  
                OSCEN_FS44_SET_HIGH();  
                OSCSEL_SET_HIGH(); // Set 44.1kHz clock domain
            }

            if (tUsbControlStatus.streamInterfaceAltsetting[eSTREAM_INPUT] == USB_AUDIO_INPUT_ALTSET_2CH_ON) {
                _PORT_Input_Transfer_Sync();    // Start Input Transfer on edge LRCK input
            }
        }
    }

    tUsbControlStatus.streamSamplingFreq[stream] = sfreq;
}


/*
    usb_PL_GetDescriptor()
 */
static inline int usb_PL_GetDescriptor ( USB_SETUP_PACKET *p, const void **dp )
{
    int len = 0;
    
    switch (p->bDescriptorType)
    {
        case USB_DESCRIPTOR_DEVICE:
            len = GetDeviceDescriptor(dbuf);
            break;
            
        case USB_DESCRIPTOR_CONFIGURATION:
            len = GetDeviceConfigurationDescriptor(dbuf);
            break;
            
        case USB_DESCRIPTOR_STRING:
            switch (p->bDscIndex)
            {
                case USBD_IDX_LANGID_STR:
                    len = GetLangIDStrDescriptor(dbuf);        
                    break;
                
                case USBD_IDX_MFC_STR:
                    len = GetManufacturerStrDescriptor(dbuf);
                    break;
                
                case USBD_IDX_PRODUCT_STR:
                    len = GetProductStrDescriptor(dbuf);
                    break;
                
                case USBD_IDX_SERIAL_STR:
                    len = GetSerialStrDescriptor(dbuf);
                    break;
                
                case USBD_IDX_CONFIG_STR:
                    len = GetConfigurationStrDescriptor(dbuf);
                    break;
                
                case USBD_IDX_INTERFACE_STR:
                    len = GetInterfaceStrDescriptor(dbuf);
                    break;
                
                default:
                    len = GetUsrStrDescriptor(dbuf, p->bDscIndex);
                    break;    
            }
            break;
            
        case USB_DESCRIPTOR_DEVICE_QUALIFIER:
            len = GetDeviceQualifierDescriptor(dbuf);
            
            break;
    }

    *dp=&dbuf;
    return len;
}

/*
    usb_PL_ReqProcess()
 */
static inline void usb_PL_ReqProcess ( void *pbuf )
{
    USB_SETUP_PACKET *p = pbuf;
    if (p->DataDir == USB_SETUP_REQUEST_DIRECTION_HOST_TO_DEVICE)
    {
        switch (p->bmbRequest)
        {
            /***************************************************/
            /************ Standart requests ********************/
            /***************************************************/
            case STDREQ_CLEARE_FEATURE_DEVICE:            
            case STDREQ_CLEARE_FEATURE_INTERFACE:            
            case STDREQ_CLEARE_FEATURE_ENDPOINT:
                USBE0CSR0bits.STALL = 1;
                break;

            case STDREQ_SET_CONFIGURATION:
                tUsbControlStatus.configuration = p->bConfigurationValue;
                break;

            case STDREQ_SET_FEATURE_DEVICE:
                tUsbControlStatus.setFeature[USB_SETUP_REQUEST_RECIPIENT_DEVICE] = p->bFeature;
                break;

            case STDREQ_SET_FEATURE_INTERFACE:
                tUsbControlStatus.setFeature[USB_SETUP_REQUEST_RECIPIENT_INTERFACE] = p->bFeature;
                break;

            case STDREQ_SET_FEATURE_ENDPOINT:
                tUsbControlStatus.setFeature[USB_SETUP_REQUEST_RECIPIENT_ENDPOINT] = p->bFeature;
                break;

            case STDREQ_SET_INTERFACE:
                usb_PL_InterfaceState_Update( p );
                break;

            /***************************************************/
            /********** Audio Device - specific requests *******/
            /***************************************************/
            case CSREQ_SET_CURR_INTERFACE:
                switch (p->W_Value.byte.HB)
                {
                    case USB_AUDIO_V2_CS_SAM_FREQ_CONTROL:
                    {
                        switch (p->bIntfID_H) {
                            case AUDIO_SCLOCK_TERMINAL_OUTPUT: usb_PL_SamplingFreq_Set( eSTREAM_OUTPUT ); break;
                        }
                        break;
                    }
                    break;
                }
                break;
                
            default:
                USBE0CSR0bits.STALL = 1;
                break;
        }
    }
    else // p->DataDir == USB_SETUP_REQUEST_DIRECTION_DEVICE_TO_HOST
    {
        switch (p->bmbRequest)
        {
            /***************************************************/
            /************ Standart requests ********************/
            /***************************************************/
            case STDREQ_GET_CONFIGURATION:
                usb_LL_Ep0_BulkWrite( &tUsbControlStatus.configuration, sizeof(tUsbControlStatus.configuration) );
                break;

            case STDREQ_GET_DESCRIPTOR:
                {
                    const void *dp = NULL;
                    int len = usb_PL_GetDescriptor( p, &dp );
                    // If the descriptor is longer than the wLength field,
                    // only the initial bytes of the descriptor are returned
                    if (len > p->wLength)
                        len = p->wLength;
                   
                    if (len) usb_LL_Ep0_BulkWrite( dp, len );
                    else USBE0CSR0bits.STALL = 1;
                }
                break;

            case STDREQ_GET_INTERFACE:
                switch (p->bIntfID) {
                    case AUDIO_STREAMING_OUTPUT_INTERFACE_NUM: 
                        usb_LL_Ep0_BulkWrite( &tUsbControlStatus.streamInterfaceAltsetting[eSTREAM_OUTPUT], 
                                            sizeof(tUsbControlStatus.streamInterfaceAltsetting[eSTREAM_OUTPUT]) );
                        break;                    
                }
                break;

            case STDREQ_GET_STATUS_DEVICE:
            case STDREQ_GET_STATUS_INTERFACE:
            case STDREQ_GET_STATUS_ENDPOINT:
                usb_LL_Ep0_BulkWrite( statusAnswer, sizeof(statusAnswer) );
                break;

            /***************************************************/
            /********** Audio Device - specific requests *******/
            /***************************************************/
            case CSREQ_GET_RANGE_INTERFACE: 
                switch (p->W_Value.byte.HB)
                {
                    case USB_AUDIO_SAMPLING_FREQ_CONTROL:
                    {                        
                        const tUSB_AUDIO_REQ_L3_PARAM_BLOCK tRangeOutput __attribute__ ((aligned (4))) = 
                            {   
                                (USB_SFREQ_192k+1),
                                {
                                    {44100, 44100, 0},
                                    {48000, 48000, 0},
                                    {88200, 88200, 0},
                                    {96000, 96000, 0},
                                    {176400, 176400, 0},                                            
                                    {192000, 192000, 0},
                                }
                            };
                                        
                        const tUSB_AUDIO_REQ_L3_PARAM_BLOCK *ptRange;
                        switch (p->bIntfID_H) {
                            case AUDIO_SCLOCK_TERMINAL_OUTPUT: ptRange = &tRangeOutput; break; // Output stream
                        }

                        int len = sizeof(tUSB_AUDIO_REQ_L3_PARAM_BLOCK);
                        
                        if (p->wLength < len)
                            len = p->wLength;

                        usb_LL_Ep0_BulkWrite( ptRange, len );
                    }
                    break;
                }
                break;
                
            case CSREQ_GET_CURR_INTERFACE: 
                switch (p->W_Value.byte.HB)
                {
                    case USB_AUDIO_SAMPLING_FREQ_CONTROL:
                    {
                        const uint32_t afreq[] = {44100, 48000, 88200, 96000, 176400, 192000};//, 352800, 384000, 705600,768000};
                        usb_LL_Ep0_BulkWrite( &afreq[tUsbControlStatus.streamSamplingFreq[eSTREAM_INPUT]], sizeof(uint32_t) );
                    }
                    break;
                }
                break;

            default:
                USBE0CSR0bits.STALL = 1;
                break;
        }
    }
}

/*
    usb_PL_tasks()
 */
void usb_PL_tasks (void)
{
    if (_System_Task_Check( SYS_TASK_USB_CONNECT )) {
        _USB_SOFT_CONNECT();        // The USB D+/D- lines are enabled and active
        PLUG_SET_HIGH();
        _System_Task_Clr( SYS_TASK_USB_CONNECT );
    }
    
    if (_System_Task_Check( SYS_TASK_USB_EP0_ISR )) {
        usb_PL_ReqProcess( ep0buf ); // Process request

        if (USBE0CSR0bits.SETEND)
            USBE0CSR0bits.SETENDC = 1;

        _System_Task_Clr( SYS_TASK_USB_EP0_ISR );
    }
    
    if (_System_Task_Check( SYS_TASK_USB_STREAM_OUT_INIT )) {
        usb_PL_OutputSamplingFreq_Set();
        
        if (tUsbControlStatus.dsd_detected == true) { // If DoP stream detected
            dma_output_dsd_mute(); // mute stream
            tmr_Task_Delayed_mS_set(SYS_TASK_USB_START_DSD, 300); // set 300mS start delay to cancel start-up noise
            DSDON_SET_HIGH();
        }
        else { // If PCM stream detected
            DSDON_SET_LOW();
            dma_output_2ch_start(tEp1Fifo.len);
        }
        MUTE_SET_LOW();
        
        _System_Task_Clr( SYS_TASK_USB_STREAM_OUT_INIT );
    }
    
    if (_System_Task_Check( SYS_TASK_USB_START_DSD )) {
        dma_output_dsd_start(tEp1Fifo.len);  // turn On 2ch DSD output stream
        _System_Task_Clr( SYS_TASK_USB_START_DSD );
    }
}

/* 
 * USB_Interrupt()
 */
void __ISR(_USB_VECTOR, ipl3SRS) USB_Interrupt(void)
{
    /* 
     * Read interrupt flags registers
     * Reading from USBCSRx registers directly can reset some interrupt flags!!!
     */
    uint32_t usbcsr0 = USBCSR0;
    uint32_t usbcsr1 = USBCSR1;
    uint32_t usbcsr2 = USBCSR2;

    if (usbcsr2 & _USBCSR2_RESETIF_MASK) {
        usb_LL_initReset();
    }
    
    /* Control Endpoint 0 Requests Handler */
    if (usbcsr0 & _USBCSR0_EP0IF_MASK) {
        if (tUsbControlStatus.adr_pending == true) { // Set Address, upon first IN transaction
            USBCSR0bits.FUNC = tUsbControlStatus.adr;
            tUsbControlStatus.adr_pending = false;
        }
        
        if (_System_Task_Check(SYS_TASK_USB_EP0_ISR) == 0) // if no request processing
        if (USBE0CSR0bits.RXRDY) // if received control packet
        {   // read control cmd
            _EP0_Control_Cmd_Read( ep0buf );
            // Fast checking "set address" request (2mS processing timeout)
            if (((USB_SETUP_PACKET*)ep0buf)->bmbRequest == STDREQ_SET_ADDRESS) {
                tUsbControlStatus.adr = ((USB_SETUP_PACKET*)ep0buf)->bDevADR;
                tUsbControlStatus.adr_pending = true;
            }
            else
                _System_Task_Set( SYS_TASK_USB_EP0_ISR );   // Set delayed task
        }
        
        USBCSR0bits.EP0IF = 0;      // Clear the USB EndPoint 0 Interrupt Flag.
    }
    
    /* Endpoint 1 RX Interrupt Handler */
    if (usbcsr1 & _USBCSR1_EP1RXIF_MASK) {
        if (USBE1CSR1bits.RXPKTRDY)
        { // ISOCHRONOUS Endpoint 1 Received A Packet
            uint16_t rxcnt = USBE1CSR2bits.RXCNT;           // Load received data count
            uint32_t adr = tEp1Fifo.adr + tEp1Fifo.ptr;     // Calculate USB DMA1 memory address            
            tEp1Fifo.ptr += rxcnt;                          // Buffer pointer increment
            
            //uart_puts("Data RX: %d\n\r");
            if (tEp1Fifo.ptr >= tEp1Fifo.len) { // if not anoth space if data buffer - receive part only
                tEp1Fifo.rem = tEp1Fifo.ptr - tEp1Fifo.len; // Calculate remain value
                tEp1Fifo.ptr = tEp1Fifo.rem;                // Set pointer to remain value
                rxcnt -= tEp1Fifo.rem;                      // Set USB DMA1 data lenght
                //uart_puts("Data RX: %d\n\r");
            }
            
            _USBDMA_EP1RX_Transfer_Start(adr, rxcnt);
        }
    }
    
    /* EP1 Errors Processing */
    /*  Flush the next packet to be read from the endpoint RX FIFO. 
        The FIFO pointer is reset and the RXPKTRDY bit is cleared. 
        This should only be used when RXPKTRDY is set. If the FIFO is doublebuffered,
        FLUSH may need to be set twice to completely clear the FIFO. */
    if (USBE1CSR1bits.FIFOFULL) {
        while (USBE1CSR1bits.RXPKTRDY)
        {
            USBE1CSR1bits.FLUSH = 1;    // Flush FIFO
        }
    }    
    if (USBE1CSR1bits.ERROR) {      // An OUT packet cannot be loaded into the RX FIFO
        USBE1CSR1bits.ERROR = 0;    // Clear the OVERRUN 
    }
    if (USBE1CSR1bits.DERRNAKT) {   // The data packet has a CRC or bit-stuff error
    }
    if (USBE1CSR0bits.UNDERRUN) {   // An IN token has been received when TXPKTRDY is not set
        USBE1CSR0bits.UNDERRUN = 0; // Clear the TX UNDERRUN
    }
    
    /* EP5 Errors Processing */
    if (USBE5CSR0bits.UNDERRUN) {   // An IN token has been received when TXPKTRDY is not set
        USBE5CSR0bits.UNDERRUN = 0; // Clear the TX UNDERRUN
    }
    
    /* Interrupt Flags Processing */
    if (USBCSR2 & 0x00ff0000) {
        USBCSR2 &= ~(0x00ff0000);
    }
    
    IFS4CLR = _IFS4_USBIF_MASK;             // Clear the USB interrupt flag
}

/*
 usb_PL_DoP_Recognition()
 * input - current FIFO data pointer
 * return - int value: 0 -> PCM, 1 -> DoP, (-1) -> Unreconized Samples detected
 */
static inline eSTREAM_FORMAT usb_PL_DoP_Recognition( void )
{
    static int pcktNum = 0;
    int wordsNum = 0;
    uint32_t *p32 = (uint32_t*)ep1buf;    
    // Set pointer to last received data block
    if (tEp1Fifo.ptr < 32) p32 += (tEp1Fifo.len >> 2);
    else p32 += (tEp1Fifo.ptr >> 2);
   
    p32--;
    // Check 8-ht last received data blocks
    if (*p32--) wordsNum++;
    if (*p32--) wordsNum++;
    if (*p32--) wordsNum++;
    if (*p32--) wordsNum++;
    if (*p32--) wordsNum++;
    if (*p32--) wordsNum++;
    if (*p32--) wordsNum++;
    if (*p32) wordsNum++;
    
    if (wordsNum < 8) { // If theare is zero words
        pcktNum++;  // count packets with zero words

        if (pcktNum >= 1000) { // Empiric number Unreconized packets
            pcktNum = 0;
            return eSTREAM_FORMAT_PCM;   // return PCM stream
        }
        else
            return eSTREAM_FORMAT_UNREC; // return Unreconized format
    }
    // If All data blocks contain non-zero data
    pcktNum = 0;
    
    // Read markers from received samples
    tDWORD tMark32;
    tMark32.dw = *p32++ & 0xff000000;
    tMark32.dw >>= 8;
    tMark32.dw |= *p32++ & 0xff000000;
    tMark32.dw >>= 8;
    tMark32.dw |= *p32++ & 0xff000000;
    tMark32.dw >>= 8;
    tMark32.dw |= *p32++ & 0xff000000;

    uint32_t mark32 = *p32++ & 0xff000000;
    mark32 >>= 8;
    mark32 |= *p32++ & 0xff000000;
    mark32 >>= 8;
    mark32 |= *p32++ & 0xff000000;
    mark32 >>= 8;
    mark32 |= *p32 & 0xff000000;
    
    if (tMark32.dw != mark32) return eSTREAM_FORMAT_PCM;   // return PCM stream
    if (tMark32.word.w0 == tMark32.word.w1) return eSTREAM_FORMAT_PCM;   // return PCM stream

    if (tMark32.byte.b0 == tMark32.byte.b1) {
        if (tMark32.byte.b2 != tMark32.byte.b3)
            return eSTREAM_FORMAT_PCM;   // return PCM stream
    }
    else if (tMark32.byte.b2 == tMark32.byte.b3) return eSTREAM_FORMAT_PCM;   // return PCM stream
    else if (tMark32.byte.b0 != tMark32.byte.b3) return eSTREAM_FORMAT_PCM;   // return PCM stream
    else if (tMark32.byte.b1 != tMark32.byte.b2) return eSTREAM_FORMAT_PCM;   // return PCM stream

    return eSTREAM_FORMAT_DOP; // return DoP stream detected
}

/*
 * usb_PL_DoP_Check() - real time validation DSD stream
 */
static inline void usb_PL_DoP_Check( void )
{
    uint32_t *p32 = (uint32_t*)ep1buf;
    // Set pointer to last received data block
    if (tEp1Fifo.ptr < 4) p32 += (tEp1Fifo.len >> 2);
    else p32 += (tEp1Fifo.ptr >> 2);
    
    p32--;    
    if ((*p32 & 0x00ffff00) == 0) // If zero data - stream is invalid
    {
        dma_output_dsd_mute(); // mute DSD stream
        tUsbControlStatus.format_check = true;
    }
}

// Debug variables to isocronous feedback check
//unsigned int fbLowCnt = 0;
//unsigned int fbHighCnt = 0;
//unsigned int fbNomCnt = 0;
/* 
 * USBDMA_Interrupt()
 */
void __ISR(_USB_DMA_VECTOR, ipl4SRS) USBDMA_Interrupt(void)
{
    uint32_t dmaInt = USBDMAINT;    // cleare flags on read
    
    if (dmaInt & _USBDMAINT_DMA1IF_MASK)
    {
        if (tEp1Fifo.rem) {
            _USBDMA_EP1RX_Transfer_Start(tEp1Fifo.adr, tEp1Fifo.rem);

            tEp1Fifo.rem = 0;
        }
        else {
            
            USBE1CSR1bits.RXPKTRDY = 0;                     // Comfirm received data packet
            char str[64];
            if(sample > 1000){
            int buff_diff = DMA_OUTPUT_TRANSFET_PTR - tEp1Fifo.ptr;  // Calculate Read and Write data difference
                if (buff_diff < 0)
                    buff_diff += tEp1Fifo.len;
            
            //sprintf(str, "%d/%d; txrdy = %d\n\r", buff_diff, tEp1Fifo.len, USBE1CSR0bits.TXPKTRDY?1:0);
            //uart_puts(str);          
            sample = 0;
            }
            sample = sample +1;
            
            if (!USBE1CSR0bits.TXPKTRDY || first_feedback) { // If previos packet has been transmitted
                int buff_diff = DMA_OUTPUT_TRANSFET_PTR - tEp1Fifo.ptr;  // Calculate Read and Write data difference
                first_feedback = 0;
            
                if (buff_diff < 0)
                    buff_diff += tEp1Fifo.len;

                // Write FeedBack data to EP1 TX FIFO
                if (buff_diff < tEp1Fifo.min) {
                    _EP1_FeedBack_Data_Send(tFreqLims.lo);
                   // uart_puts("under\n\r");
//                    fbLowCnt++;
                }
                else if (buff_diff > tEp1Fifo.max) {
                    _EP1_FeedBack_Data_Send(tFreqLims.hi);
                    //uart_puts("over\n\r");
//                    fbHighCnt++;
                }
                else {
                    _EP1_FeedBack_Data_Send(tFreqLims.nom);
                    //uart_puts("nom\n\r");
                    
//                    fbNomCnt++;
                }
            }
            
            if (tUsbControlStatus.format_check == true) {
                eSTREAM_FORMAT format = usb_PL_DoP_Recognition();
                
                if (format == eSTREAM_FORMAT_DOP) { // If DoP stream detected
                    tUsbControlStatus.format_check = false;
                    tUsbControlStatus.dsd_detected = true;
                    _System_Task_Set( SYS_TASK_USB_STREAM_OUT_INIT );
                }
                else if (format == eSTREAM_FORMAT_PCM) { // If PCM stream detected
                    tUsbControlStatus.format_check = false;
                    tUsbControlStatus.dsd_detected = false;
                    _System_Task_Set( SYS_TASK_USB_STREAM_OUT_INIT );
                }
            }
            else if (tUsbControlStatus.dsd_detected) {
                // Rial time chek DSD stream to exclude click noise
                usb_PL_DoP_Check();
            }
        }
    }
    
    if (dmaInt & _USBDMAINT_DMA5IF_MASK)
    {
        if (tEp5Fifo.rem) {
            _USBDMA_EP5TX_Transfer_Start(tEp5Fifo.adr, tEp5Fifo.rem);
            tEp5Fifo.rem = 0;
        }
        else {
            USBE5CSR0bits.TXPKTRDY = 1;                     // Confirm to send data packet
            
            static uint16_t speed_check = 0;                // checking data speed interval counter
            speed_check++;
            speed_check &= 0x3;
            if (!speed_check) { // check every 4-t transaction
                int buff_diff = DMA_INPUT_TRANSFET_PTR - tEp5Fifo.ptr; // Calculate Read and Write data difference
                if (buff_diff < 0)
                    buff_diff += tEp5Fifo.len;

                // update actual block data lenght
                if (buff_diff < tEp5Fifo.min) {         // if usb transmitting speed is too high
                    ep5txsize = tEp5BlockSize.lo;       // turn down one sample from packet lenght
                }
                else if (buff_diff > tEp5Fifo.max) {    // if usb transmitting speed is too low
                    ep5txsize = tEp5BlockSize.hi;       // add one sample to packet lenght (4byte * 2channels)
                }
                else {
                    ep5txsize = tEp5BlockSize.nom;
                }
            }
        }
    }
    
    IFS4CLR = _IFS4_USBDMAIF_MASK;              // Clear the USB DMA interrupt flag.
}


