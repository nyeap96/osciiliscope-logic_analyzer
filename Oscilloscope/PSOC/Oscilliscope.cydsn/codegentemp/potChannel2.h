/*******************************************************************************
* File Name: potChannel2.h  
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

#if !defined(CY_PINS_potChannel2_H) /* Pins potChannel2_H */
#define CY_PINS_potChannel2_H

#include "cytypes.h"
#include "cyfitter.h"
#include "cypins.h"
#include "potChannel2_aliases.h"

/* APIs are not generated for P15[7:6] */
#if !(CY_PSOC5A &&\
	 potChannel2__PORT == 15 && ((potChannel2__MASK & 0xC0) != 0))


/***************************************
*        Function Prototypes             
***************************************/    

/**
* \addtogroup group_general
* @{
*/
void    potChannel2_Write(uint8 value);
void    potChannel2_SetDriveMode(uint8 mode);
uint8   potChannel2_ReadDataReg(void);
uint8   potChannel2_Read(void);
void    potChannel2_SetInterruptMode(uint16 position, uint16 mode);
uint8   potChannel2_ClearInterrupt(void);
/** @} general */

/***************************************
*           API Constants        
***************************************/
/**
* \addtogroup group_constants
* @{
*/
    /** \addtogroup driveMode Drive mode constants
     * \brief Constants to be passed as "mode" parameter in the potChannel2_SetDriveMode() function.
     *  @{
     */
        #define potChannel2_DM_ALG_HIZ         PIN_DM_ALG_HIZ
        #define potChannel2_DM_DIG_HIZ         PIN_DM_DIG_HIZ
        #define potChannel2_DM_RES_UP          PIN_DM_RES_UP
        #define potChannel2_DM_RES_DWN         PIN_DM_RES_DWN
        #define potChannel2_DM_OD_LO           PIN_DM_OD_LO
        #define potChannel2_DM_OD_HI           PIN_DM_OD_HI
        #define potChannel2_DM_STRONG          PIN_DM_STRONG
        #define potChannel2_DM_RES_UPDWN       PIN_DM_RES_UPDWN
    /** @} driveMode */
/** @} group_constants */
    
/* Digital Port Constants */
#define potChannel2_MASK               potChannel2__MASK
#define potChannel2_SHIFT              potChannel2__SHIFT
#define potChannel2_WIDTH              1u

/* Interrupt constants */
#if defined(potChannel2__INTSTAT)
/**
* \addtogroup group_constants
* @{
*/
    /** \addtogroup intrMode Interrupt constants
     * \brief Constants to be passed as "mode" parameter in potChannel2_SetInterruptMode() function.
     *  @{
     */
        #define potChannel2_INTR_NONE      (uint16)(0x0000u)
        #define potChannel2_INTR_RISING    (uint16)(0x0001u)
        #define potChannel2_INTR_FALLING   (uint16)(0x0002u)
        #define potChannel2_INTR_BOTH      (uint16)(0x0003u) 
    /** @} intrMode */
/** @} group_constants */

    #define potChannel2_INTR_MASK      (0x01u) 
#endif /* (potChannel2__INTSTAT) */


/***************************************
*             Registers        
***************************************/

/* Main Port Registers */
/* Pin State */
#define potChannel2_PS                     (* (reg8 *) potChannel2__PS)
/* Data Register */
#define potChannel2_DR                     (* (reg8 *) potChannel2__DR)
/* Port Number */
#define potChannel2_PRT_NUM                (* (reg8 *) potChannel2__PRT) 
/* Connect to Analog Globals */                                                  
#define potChannel2_AG                     (* (reg8 *) potChannel2__AG)                       
/* Analog MUX bux enable */
#define potChannel2_AMUX                   (* (reg8 *) potChannel2__AMUX) 
/* Bidirectional Enable */                                                        
#define potChannel2_BIE                    (* (reg8 *) potChannel2__BIE)
/* Bit-mask for Aliased Register Access */
#define potChannel2_BIT_MASK               (* (reg8 *) potChannel2__BIT_MASK)
/* Bypass Enable */
#define potChannel2_BYP                    (* (reg8 *) potChannel2__BYP)
/* Port wide control signals */                                                   
#define potChannel2_CTL                    (* (reg8 *) potChannel2__CTL)
/* Drive Modes */
#define potChannel2_DM0                    (* (reg8 *) potChannel2__DM0) 
#define potChannel2_DM1                    (* (reg8 *) potChannel2__DM1)
#define potChannel2_DM2                    (* (reg8 *) potChannel2__DM2) 
/* Input Buffer Disable Override */
#define potChannel2_INP_DIS                (* (reg8 *) potChannel2__INP_DIS)
/* LCD Common or Segment Drive */
#define potChannel2_LCD_COM_SEG            (* (reg8 *) potChannel2__LCD_COM_SEG)
/* Enable Segment LCD */
#define potChannel2_LCD_EN                 (* (reg8 *) potChannel2__LCD_EN)
/* Slew Rate Control */
#define potChannel2_SLW                    (* (reg8 *) potChannel2__SLW)

/* DSI Port Registers */
/* Global DSI Select Register */
#define potChannel2_PRTDSI__CAPS_SEL       (* (reg8 *) potChannel2__PRTDSI__CAPS_SEL) 
/* Double Sync Enable */
#define potChannel2_PRTDSI__DBL_SYNC_IN    (* (reg8 *) potChannel2__PRTDSI__DBL_SYNC_IN) 
/* Output Enable Select Drive Strength */
#define potChannel2_PRTDSI__OE_SEL0        (* (reg8 *) potChannel2__PRTDSI__OE_SEL0) 
#define potChannel2_PRTDSI__OE_SEL1        (* (reg8 *) potChannel2__PRTDSI__OE_SEL1) 
/* Port Pin Output Select Registers */
#define potChannel2_PRTDSI__OUT_SEL0       (* (reg8 *) potChannel2__PRTDSI__OUT_SEL0) 
#define potChannel2_PRTDSI__OUT_SEL1       (* (reg8 *) potChannel2__PRTDSI__OUT_SEL1) 
/* Sync Output Enable Registers */
#define potChannel2_PRTDSI__SYNC_OUT       (* (reg8 *) potChannel2__PRTDSI__SYNC_OUT) 

/* SIO registers */
#if defined(potChannel2__SIO_CFG)
    #define potChannel2_SIO_HYST_EN        (* (reg8 *) potChannel2__SIO_HYST_EN)
    #define potChannel2_SIO_REG_HIFREQ     (* (reg8 *) potChannel2__SIO_REG_HIFREQ)
    #define potChannel2_SIO_CFG            (* (reg8 *) potChannel2__SIO_CFG)
    #define potChannel2_SIO_DIFF           (* (reg8 *) potChannel2__SIO_DIFF)
#endif /* (potChannel2__SIO_CFG) */

/* Interrupt Registers */
#if defined(potChannel2__INTSTAT)
    #define potChannel2_INTSTAT            (* (reg8 *) potChannel2__INTSTAT)
    #define potChannel2_SNAP               (* (reg8 *) potChannel2__SNAP)
    
	#define potChannel2_0_INTTYPE_REG 		(* (reg8 *) potChannel2__0__INTTYPE)
#endif /* (potChannel2__INTSTAT) */

#endif /* CY_PSOC5A... */

#endif /*  CY_PINS_potChannel2_H */


/* [] END OF FILE */
