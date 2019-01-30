/*******************************************************************************
* File Name: ADCInt2.h
* Version 1.70
*
*  Description:
*   Provides the function definitions for the Interrupt Controller.
*
*
********************************************************************************
* Copyright 2008-2015, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions, 
* disclaimers, and limitations in the end user license agreement accompanying 
* the software package with which this file was provided.
*******************************************************************************/
#if !defined(CY_ISR_ADCInt2_H)
#define CY_ISR_ADCInt2_H


#include <cytypes.h>
#include <cyfitter.h>

/* Interrupt Controller API. */
void ADCInt2_Start(void);
void ADCInt2_StartEx(cyisraddress address);
void ADCInt2_Stop(void);

CY_ISR_PROTO(ADCInt2_Interrupt);

void ADCInt2_SetVector(cyisraddress address);
cyisraddress ADCInt2_GetVector(void);

void ADCInt2_SetPriority(uint8 priority);
uint8 ADCInt2_GetPriority(void);

void ADCInt2_Enable(void);
uint8 ADCInt2_GetState(void);
void ADCInt2_Disable(void);

void ADCInt2_SetPending(void);
void ADCInt2_ClearPending(void);


/* Interrupt Controller Constants */

/* Address of the INTC.VECT[x] register that contains the Address of the ADCInt2 ISR. */
#define ADCInt2_INTC_VECTOR            ((reg32 *) ADCInt2__INTC_VECT)

/* Address of the ADCInt2 ISR priority. */
#define ADCInt2_INTC_PRIOR             ((reg8 *) ADCInt2__INTC_PRIOR_REG)

/* Priority of the ADCInt2 interrupt. */
#define ADCInt2_INTC_PRIOR_NUMBER      ADCInt2__INTC_PRIOR_NUM

/* Address of the INTC.SET_EN[x] byte to bit enable ADCInt2 interrupt. */
#define ADCInt2_INTC_SET_EN            ((reg32 *) ADCInt2__INTC_SET_EN_REG)

/* Address of the INTC.CLR_EN[x] register to bit clear the ADCInt2 interrupt. */
#define ADCInt2_INTC_CLR_EN            ((reg32 *) ADCInt2__INTC_CLR_EN_REG)

/* Address of the INTC.SET_PD[x] register to set the ADCInt2 interrupt state to pending. */
#define ADCInt2_INTC_SET_PD            ((reg32 *) ADCInt2__INTC_SET_PD_REG)

/* Address of the INTC.CLR_PD[x] register to clear the ADCInt2 interrupt. */
#define ADCInt2_INTC_CLR_PD            ((reg32 *) ADCInt2__INTC_CLR_PD_REG)


#endif /* CY_ISR_ADCInt2_H */


/* [] END OF FILE */
