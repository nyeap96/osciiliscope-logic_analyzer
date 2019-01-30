/*******************************************************************************
* File Name: ADCInt1.h
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
#if !defined(CY_ISR_ADCInt1_H)
#define CY_ISR_ADCInt1_H


#include <cytypes.h>
#include <cyfitter.h>

/* Interrupt Controller API. */
void ADCInt1_Start(void);
void ADCInt1_StartEx(cyisraddress address);
void ADCInt1_Stop(void);

CY_ISR_PROTO(ADCInt1_Interrupt);

void ADCInt1_SetVector(cyisraddress address);
cyisraddress ADCInt1_GetVector(void);

void ADCInt1_SetPriority(uint8 priority);
uint8 ADCInt1_GetPriority(void);

void ADCInt1_Enable(void);
uint8 ADCInt1_GetState(void);
void ADCInt1_Disable(void);

void ADCInt1_SetPending(void);
void ADCInt1_ClearPending(void);


/* Interrupt Controller Constants */

/* Address of the INTC.VECT[x] register that contains the Address of the ADCInt1 ISR. */
#define ADCInt1_INTC_VECTOR            ((reg32 *) ADCInt1__INTC_VECT)

/* Address of the ADCInt1 ISR priority. */
#define ADCInt1_INTC_PRIOR             ((reg8 *) ADCInt1__INTC_PRIOR_REG)

/* Priority of the ADCInt1 interrupt. */
#define ADCInt1_INTC_PRIOR_NUMBER      ADCInt1__INTC_PRIOR_NUM

/* Address of the INTC.SET_EN[x] byte to bit enable ADCInt1 interrupt. */
#define ADCInt1_INTC_SET_EN            ((reg32 *) ADCInt1__INTC_SET_EN_REG)

/* Address of the INTC.CLR_EN[x] register to bit clear the ADCInt1 interrupt. */
#define ADCInt1_INTC_CLR_EN            ((reg32 *) ADCInt1__INTC_CLR_EN_REG)

/* Address of the INTC.SET_PD[x] register to set the ADCInt1 interrupt state to pending. */
#define ADCInt1_INTC_SET_PD            ((reg32 *) ADCInt1__INTC_SET_PD_REG)

/* Address of the INTC.CLR_PD[x] register to clear the ADCInt1 interrupt. */
#define ADCInt1_INTC_CLR_PD            ((reg32 *) ADCInt1__INTC_CLR_PD_REG)


#endif /* CY_ISR_ADCInt1_H */


/* [] END OF FILE */
