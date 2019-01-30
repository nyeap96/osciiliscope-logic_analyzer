/*******************************************************************************
* File Name: probe1.h  
* Version 2.20
*
* Description:
*  This file contains Pin function prototypes and register defines
*
* Note:
*
********************************************************************************
* Copyright 2008-2015, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions, 
* disclaimers, and limitations in the end user license agreement accompanying 
* the software package with which this file was provided.
*******************************************************************************/

#if !defined(CY_PINS_probe1_H) /* Pins probe1_H */
#define CY_PINS_probe1_H

#include "cytypes.h"
#include "cyfitter.h"
#include "cypins.h"
#include "probe1_aliases.h"

/* APIs are not generated for P15[7:6] */
#if !(CY_PSOC5A &&\
	 probe1__PORT == 15 && ((probe1__MASK & 0xC0) != 0))


/***************************************
*        Function Prototypes             
***************************************/    

/**
* \addtogroup group_general
* @{
*/
void    probe1_Write(uint8 value);
void    probe1_SetDriveMode(uint8 mode);
uint8   probe1_ReadDataReg(void);
uint8   probe1_Read(void);
void    probe1_SetInterruptMode(uint16 position, uint16 mode);
uint8   probe1_ClearInterrupt(void);
/** @} general */

/***************************************
*           API Constants        
***************************************/
/**
* \addtogroup group_constants
* @{
*/
    /** \addtogroup driveMode Drive mode constants
     * \brief Constants to be passed as "mode" parameter in the probe1_SetDriveMode() function.
     *  @{
     */
        #define probe1_DM_ALG_HIZ         PIN_DM_ALG_HIZ
        #define probe1_DM_DIG_HIZ         PIN_DM_DIG_HIZ
        #define probe1_DM_RES_UP          PIN_DM_RES_UP
        #define probe1_DM_RES_DWN         PIN_DM_RES_DWN
        #define probe1_DM_OD_LO           PIN_DM_OD_LO
        #define probe1_DM_OD_HI           PIN_DM_OD_HI
        #define probe1_DM_STRONG          PIN_DM_STRONG
        #define probe1_DM_RES_UPDWN       PIN_DM_RES_UPDWN
    /** @} driveMode */
/** @} group_constants */
    
/* Digital Port Constants */
#define probe1_MASK               probe1__MASK
#define probe1_SHIFT              probe1__SHIFT
#define probe1_WIDTH              1u

/* Interrupt constants */
#if defined(probe1__INTSTAT)
/**
* \addtogroup group_constants
* @{
*/
    /** \addtogroup intrMode Interrupt constants
     * \brief Constants to be passed as "mode" parameter in probe1_SetInterruptMode() function.
     *  @{
     */
        #define probe1_INTR_NONE      (uint16)(0x0000u)
        #define probe1_INTR_RISING    (uint16)(0x0001u)
        #define probe1_INTR_FALLING   (uint16)(0x0002u)
        #define probe1_INTR_BOTH      (uint16)(0x0003u) 
    /** @} intrMode */
/** @} group_constants */

    #define probe1_INTR_MASK      (0x01u) 
#endif /* (probe1__INTSTAT) */


/***************************************
*             Registers        
***************************************/

/* Main Port Registers */
/* Pin State */
#define probe1_PS                     (* (reg8 *) probe1__PS)
/* Data Register */
#define probe1_DR                     (* (reg8 *) probe1__DR)
/* Port Number */
#define probe1_PRT_NUM                (* (reg8 *) probe1__PRT) 
/* Connect to Analog Globals */                                                  
#define probe1_AG                     (* (reg8 *) probe1__AG)                       
/* Analog MUX bux enable */
#define probe1_AMUX                   (* (reg8 *) probe1__AMUX) 
/* Bidirectional Enable */                                                        
#define probe1_BIE                    (* (reg8 *) probe1__BIE)
/* Bit-mask for Aliased Register Access */
#define probe1_BIT_MASK               (* (reg8 *) probe1__BIT_MASK)
/* Bypass Enable */
#define probe1_BYP                    (* (reg8 *) probe1__BYP)
/* Port wide control signals */                                                   
#define probe1_CTL                    (* (reg8 *) probe1__CTL)
/* Drive Modes */
#define probe1_DM0                    (* (reg8 *) probe1__DM0) 
#define probe1_DM1                    (* (reg8 *) probe1__DM1)
#define probe1_DM2                    (* (reg8 *) probe1__DM2) 
/* Input Buffer Disable Override */
#define probe1_INP_DIS                (* (reg8 *) probe1__INP_DIS)
/* LCD Common or Segment Drive */
#define probe1_LCD_COM_SEG            (* (reg8 *) probe1__LCD_COM_SEG)
/* Enable Segment LCD */
#define probe1_LCD_EN                 (* (reg8 *) probe1__LCD_EN)
/* Slew Rate Control */
#define probe1_SLW                    (* (reg8 *) probe1__SLW)

/* DSI Port Registers */
/* Global DSI Select Register */
#define probe1_PRTDSI__CAPS_SEL       (* (reg8 *) probe1__PRTDSI__CAPS_SEL) 
/* Double Sync Enable */
#define probe1_PRTDSI__DBL_SYNC_IN    (* (reg8 *) probe1__PRTDSI__DBL_SYNC_IN) 
/* Output Enable Select Drive Strength */
#define probe1_PRTDSI__OE_SEL0        (* (reg8 *) probe1__PRTDSI__OE_SEL0) 
#define probe1_PRTDSI__OE_SEL1        (* (reg8 *) probe1__PRTDSI__OE_SEL1) 
/* Port Pin Output Select Registers */
#define probe1_PRTDSI__OUT_SEL0       (* (reg8 *) probe1__PRTDSI__OUT_SEL0) 
#define probe1_PRTDSI__OUT_SEL1       (* (reg8 *) probe1__PRTDSI__OUT_SEL1) 
/* Sync Output Enable Registers */
#define probe1_PRTDSI__SYNC_OUT       (* (reg8 *) probe1__PRTDSI__SYNC_OUT) 

/* SIO registers */
#if defined(probe1__SIO_CFG)
    #define probe1_SIO_HYST_EN        (* (reg8 *) probe1__SIO_HYST_EN)
    #define probe1_SIO_REG_HIFREQ     (* (reg8 *) probe1__SIO_REG_HIFREQ)
    #define probe1_SIO_CFG            (* (reg8 *) probe1__SIO_CFG)
    #define probe1_SIO_DIFF           (* (reg8 *) probe1__SIO_DIFF)
#endif /* (probe1__SIO_CFG) */

/* Interrupt Registers */
#if defined(probe1__INTSTAT)
    #define probe1_INTSTAT            (* (reg8 *) probe1__INTSTAT)
    #define probe1_SNAP               (* (reg8 *) probe1__SNAP)
    
	#define probe1_0_INTTYPE_REG 		(* (reg8 *) probe1__0__INTTYPE)
#endif /* (probe1__INTSTAT) */

#endif /* CY_PSOC5A... */

#endif /*  CY_PINS_probe1_H */


/* [] END OF FILE */
