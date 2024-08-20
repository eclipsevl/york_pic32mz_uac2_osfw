#ifndef _PORT_H_    /* Guard against multiple inclusion */
#define _PORT_H_


/* Provide C++ Compatibility */
#ifdef __cplusplus
extern "C" {
#endif

    
#define _BIT(bit_num)                   ((uint32_t)(0x01 << (bit_num)))
    
#define OSCSEL_SET_HIGH()               LATDSET = _BIT(5)          // set oscsel to high
#define OSCSEL_SET_LOW()                LATDCLR = _BIT(5)          // set oscsel to low
    
#define OSCEN_FS48_SET_LOW()                LATDCLR = _BIT(11) 
#define OSCEN_FS44_SET_LOW()                LATDCLR = _BIT(9) 
#define OSCEN_FS48_SET_HIGH()               LATDSET = _BIT(11)
#define OSCEN_FS44_SET_HIGH()               LATDSET = _BIT(9)
    
#define FS0_SET_HIGH()                  LATBSET = _BIT(5)
#define FS0_SET_LOW()                   LATBCLR = _BIT(5)

#define FS1_SET_HIGH()                  LATBSET = _BIT(8)
#define FS1_SET_LOW()                   LATBCLR = _BIT(8)
    
// Amanero IF
#define DSDON_SET_HIGH()                  LATESET = _BIT(0)
#define DSDON_SET_LOW()                   LATECLR = _BIT(0)
    
#define MUTE_SET_HIGH()                  LATESET = _BIT(2)
#define MUTE_SET_LOW()                   LATECLR = _BIT(2)
    
#define PLUG_SET_HIGH()                  LATESET = _BIT(3)
#define PLUG_SET_LOW()                   LATECLR = _BIT(3)

// Turn On exteral interrupt on LRCK input pin
#define _PORT_Input_Transfer_Sync()     IEC0SET = _IEC0_INT1IE_MASK
    
void port_init (void);


/* Provide C++ Compatibility */
#ifdef __cplusplus
}
#endif

#endif /* _PORT_H_ */


