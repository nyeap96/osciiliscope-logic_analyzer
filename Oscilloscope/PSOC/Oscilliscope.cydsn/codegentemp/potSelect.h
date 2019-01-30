/*******************************************************************************
* File Name: potSelect.h
* Version 1.80
*
*  Description:
*    This file contains the constants and function prototypes for the Analog
*    Multiplexer User Module AMux.
*
*   Note:
*
********************************************************************************
* Copyright 2008-2010, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions, 
* disclaimers, and limitations in the end user license agreement accompanying 
* the software package with which this file was provided.
********************************************************************************/

#if !defined(CY_AMUX_potSelect_H)
#define CY_AMUX_potSelect_H

#include "cytypes.h"
#include "cyfitter.h"
#include "cyfitter_cfg.h"


/***************************************
*        Function Prototypes
***************************************/

void potSelect_Start(void) ;
#define potSelect_Init() potSelect_Start()
void potSelect_FastSelect(uint8 channel) ;
/* The Stop, Select, Connect, Disconnect and DisconnectAll functions are declared elsewhere */
/* void potSelect_Stop(void); */
/* void potSelect_Select(uint8 channel); */
/* void potSelect_Connect(uint8 channel); */
/* void potSelect_Disconnect(uint8 channel); */
/* void potSelect_DisconnectAll(void) */


/***************************************
*         Parameter Constants
***************************************/

#define potSelect_CHANNELS  2u
#define potSelect_MUXTYPE   1
#define potSelect_ATMOSTONE 0

/***************************************
*             API Constants
***************************************/

#define potSelect_NULL_CHANNEL 0xFFu
#define potSelect_MUX_SINGLE   1
#define potSelect_MUX_DIFF     2


/***************************************
*        Conditional Functions
***************************************/

#if potSelect_MUXTYPE == potSelect_MUX_SINGLE
# if !potSelect_ATMOSTONE
#  define potSelect_Connect(channel) potSelect_Set(channel)
# endif
# define potSelect_Disconnect(channel) potSelect_Unset(channel)
#else
# if !potSelect_ATMOSTONE
void potSelect_Connect(uint8 channel) ;
# endif
void potSelect_Disconnect(uint8 channel) ;
#endif

#if potSelect_ATMOSTONE
# define potSelect_Stop() potSelect_DisconnectAll()
# define potSelect_Select(channel) potSelect_FastSelect(channel)
void potSelect_DisconnectAll(void) ;
#else
# define potSelect_Stop() potSelect_Start()
void potSelect_Select(uint8 channel) ;
# define potSelect_DisconnectAll() potSelect_Start()
#endif

#endif /* CY_AMUX_potSelect_H */


/* [] END OF FILE */
