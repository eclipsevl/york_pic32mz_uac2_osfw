#ifndef _TMR_H_    /* Guard against multiple inclusion */
#define _TMR_H_


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

void tmr_Task_Delayed_mS_set (const uint32_t task, const uint16_t time_mS);

    /* Provide C++ Compatibility */
#ifdef __cplusplus
}
#endif

#endif /* _TMR_H_ */


