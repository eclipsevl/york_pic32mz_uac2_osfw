/**
  ******************************************************************************
  * @file    usbd_audio_core.h
  * @author  MCD Application Team
  * @version V1.0.0
  * @date    22-July-2011
  * @brief   header file for the usbd_audio_core.c file.
  ******************************************************************************
  * @attention
  *
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
  *
  * <h2><center>&copy; COPYRIGHT 2011 STMicroelectronics</center></h2>
  ******************************************************************************
  */ 

/* Includes ------------------------------------------------------------------*/

#ifndef __USB_AUDIO_CORE_H_
#define __USB_AUDIO_CORE_H_



/* AudioFreq * DataSize (2 bytes) * NumChannels (Stereo: 2) */
#define AUDIO_OUT_PACKET                              (uint32_t)(((USBD_AUDIO_FREQ * 2 * 2) /1000)) 

/* Number of sub-packets in the audio transfer buffer. You can modify this value but always make sure
  that it is an even number and higher than 3 */
#define OUT_PACKET_NUM                                   8
/* Total size of the audio transfer buffer */
#define TOTAL_OUT_BUF_SIZE                           ((uint32_t)(AUDIO_OUT_PACKET * OUT_PACKET_NUM))


#define CDC_OUT_DATA_EP								  0x02
#define CDC_IN_DATA_EP								  0x82
#define CDC_STATE_IN_EP								  0x83

#define CDC_REQ_SET_LINE_CODING						  0x20
#define CDC_REQ_GET_LINE_CODING						  0x21
#define CDC_REQ_SET_CONTROL_LINE_STATE				  0x22

#define CDC_INTERFACE_DESCRIPTOR_TYPE                 0x24
#define CDC_ENDPOINT_DESCRIPTOR_TYPE                  0x25
#define VIRTUAL_COM_PORT_INT_SIZE 					  0x0A
#define VIRTUAL_COM_PORT_DATA_SIZE					  0x40
#define APP_RX_DATA_SIZE							  2048

#define USB_INTERFACE_ASSOC_DESCRIPTOR_TYPE			  0x0B
#define INTERFACE_ASSOC_DESC_SIZE					  0x08

#define AUDIO_INTERFACE_DESC_SIZE                     0x09
#define USB_AUDIO_IAD_DESC_SIZE                       0x08
#define USB_AUDIO_DESC_SIZ                            0x09
#define AUDIO_STANDARD_ENDPOINT_DESC_SIZE             0x09
#define AUDIO_STREAMING_ENDPOINT_DESC_SIZE            0x07

#define AUDIO_DESCRIPTOR_TYPE                         0x21
#define USB_DEVICE_CLASS_AUDIO                        0x01
#define AUDIO_SUBCLASS_AUDIOCONTROL                   0x01
#define AUDIO_SUBCLASS_AUDIOSTREAMING                 0x02
#define AUDIO_PROTOCOL_UNDEFINED                      0x00
#define AUDIO_STREAMING_GENERAL                       0x01
#define AUDIO_STREAMING_FORMAT_TYPE                   0x02

/* Audio Descriptor Types */
#define AUDIO_INTERFACE_DESCRIPTOR_TYPE               0x24
#define AUDIO_ENDPOINT_DESCRIPTOR_TYPE                0x25

/* Audio Control Interface Descriptor Subtypes */
#define AUDIO_CONTROL_HEADER                          0x01
#define AUDIO_CONTROL_INPUT_TERMINAL                  0x02
#define AUDIO_CONTROL_OUTPUT_TERMINAL                 0x03
#define AUDIO_CONTROL_FEATURE_UNIT                    0x06

#define AUDIO_INPUT_TERMINAL_DESC_SIZE                0x0C
#define AUDIO_OUTPUT_TERMINAL_DESC_SIZE               0x09
#define AUDIO_STREAMING_INTERFACE_DESC_SIZE           0x07

#define AUDIO_CONTROL_MUTE                            0x0001

#define AUDIO_FORMAT_TYPE_I                           0x01
#define AUDIO_FORMAT_TYPE_III                         0x03

#define USB_ENDPOINT_TYPE_ASYNC_FEEDBACK  			  0x15
#define USB_ENDPOINT_TYPE_ISOCHRONOUS                 0x01
#define USB_ENDPOINT_TYPE_BULK		                  0x02
#define USB_ENDPOINT_TYPE_INTERRUPT	                  0x03
#define AUDIO_ENDPOINT_GENERAL                        0x01

#define AUDIO_REQ_GET_CUR                             0x81
#define AUDIO_REQ_SET_CUR                             0x01

#define AUDIO_OUT_STREAMING_CTRL                      0x02

#define USB_CONFIGUARTION_DESC_SIZE					  0x09
#define AUDIO_OUTPUT_TERMINAL_DESC_SIZE 			  0x09
#define AUDIO_FEATURE_UNIT_DESC_SZ					  0x0A
#define AUDIO_FORMAT_TYPE_III_DESC_SZ				  0x0B
#define AUDIO_FORMAT_TYPE_I_DESC_SZ				      0x0B


enum {
    AUDIO_CONTROL_INTERFACE = 0,
    AUDIO_STREAMING_OUTPUT_INTERFACE,
    AUDIO_INTERFACES_COUNT
};
#define AUDIO_FIRST_INTERFACE_NUM                     0x00
#define AUDIO_CONTROL_INTERFACE_NUM                   (AUDIO_FIRST_INTERFACE_NUM + AUDIO_CONTROL_INTERFACE)
#define AUDIO_STREAMING_OUTPUT_INTERFACE_NUM          (AUDIO_FIRST_INTERFACE_NUM + AUDIO_STREAMING_OUTPUT_INTERFACE)

enum {
    AUDIO_IN_TERMINAL_OUTPUT = 1,
    AUDIO_IN_TERMINAL_INPUT,
    AUDIO_OUT_TERMINAL_OUTPUT,
    AUDIO_OUT_TERMINAL_INPUT,
    AUDIO_SCLOCK_TERMINAL_OUTPUT,
    AUDIO_SCLOCK_TERMINAL_INPUT
};

#define USB_AUDIO_OUTPUT_ALTSET_OFF                   0x0
#define USB_AUDIO_OUTPUT_ALTSET_2CH_ON                0x1
#define USB_AUDIO_OUTPUT_ALTSET_8CH_ON                0x2

#define USB_AUDIO_INPUT_ALTSET_OFF                    0x0
#define USB_AUDIO_INPUT_ALTSET_2CH_ON                 0x1



#ifndef USBD_AUDIO_FREQ
/* AUDIO Class Config */
#define USBD_AUDIO_FREQ                               48000U
#endif /* USBD_AUDIO_FREQ */

#ifndef USBD_MAX_NUM_INTERFACES
#define USBD_MAX_NUM_INTERFACES                       1U
#endif /* USBD_AUDIO_FREQ */

#ifndef AUDIO_HS_BINTERVAL
#define AUDIO_HS_BINTERVAL                            0x01U
#endif /* AUDIO_HS_BINTERVAL */

#ifndef AUDIO_FS_BINTERVAL
#define AUDIO_FS_BINTERVAL                            0x01U
#endif /* AUDIO_FS_BINTERVAL */

#define AUDIO_OUT_EP                                  0x01U
#define AUDIO_FB_EP                                   0x81U
#define AUDIO_IN_EP                                   0x85U

#ifdef CDC
#define USB_AUDIO_CONFIG_DESC_SIZ                     338U // 279(Audio) + 59(CDC) = 338
#else
#define USB_AUDIO_CONFIG_DESC_SIZ                     279U//338U // 279(Audio) + 59(CDC) = 338        
#endif

#define AUDIO_OUT_TC                                  0x01U
#define AUDIO_IN_TC                                   0x02U


#define AUDIO_DEFAULT_VOLUME                          70U

/* Number of sub-packets in the audio transfer buffer. You can modify this value but always make sure
  that it is an even number and higher than 3 */
#define AUDIO_OUT_PACKET_NUM                          80U
/* Total size of the audio transfer buffer */
#define AUDIO_TOTAL_BUF_SIZE                          ((uint16_t)(AUDIO_OUT_PACKET * AUDIO_OUT_PACKET_NUM))
#define AUDIO_TOTAL_BUF_WSIZE                         (AUDIO_TOTAL_BUF_SIZE/4)


#define SAMPLE_FREQ(frq)               (uint8_t)(frq), (uint8_t)((frq >> 8)), (uint8_t)((frq >> 16))




#endif  // __USB_AUDIO_CORE_H_
/**
  * @}
  */ 

/**
  * @}
  */ 
  
/******************* (C) COPYRIGHT 2011 STMicroelectronics *****END OF FILE****/
