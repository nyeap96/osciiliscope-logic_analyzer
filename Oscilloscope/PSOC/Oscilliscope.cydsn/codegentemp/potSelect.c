/*******************************************************************************
* File Name: potSelect.c
* Version 1.80
*
*  Description:
*    This file contains all functions required for the analog multiplexer
*    AMux User Module.
*
*   Note:
*
*******************************************************************************
* Copyright 2008-2010, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions, 
* disclaimers, and limitations in the end user license agreement accompanying 
* the software package with which this file was provided.
********************************************************************************/

#include "potSelect.h"

static uint8 potSelect_lastChannel = potSelect_NULL_CHANNEL;


/*******************************************************************************
* Function Name: potSelect_Start
********************************************************************************
* Summary:
*  Disconnect all channels.
*
* Parameters:
*  void
*
* Return:
*  void
*
*******************************************************************************/
void potSelect_Start(void) 
{
    uint8 chan;

    for(chan = 0u; chan < potSelect_CHANNELS ; chan++)
    {
#if (potSelect_MUXTYPE == potSelect_MUX_SINGLE)
        potSelect_Unset(chan);
#else
        potSelect_CYAMUXSIDE_A_Unset(chan);
        potSelect_CYAMUXSIDE_B_Unset(chan);
#endif
    }

    potSelect_lastChannel = potSelect_NULL_CHANNEL;
}


#if (!potSelect_ATMOSTONE)
/*******************************************************************************
* Function Name: potSelect_Select
********************************************************************************
* Summary:
*  This functions first disconnects all channels then connects the given
*  channel.
*
* Parameters:
*  channel:  The channel to connect to the common terminal.
*
* Return:
*  void
*
*******************************************************************************/
void potSelect_Select(uint8 channel) 
{
    potSelect_DisconnectAll();        /* Disconnect all previous connections */
    potSelect_Connect(channel);       /* Make the given selection */
    potSelect_lastChannel = channel;  /* Update last channel */
}
#endif


/*******************************************************************************
* Function Name: potSelect_FastSelect
********************************************************************************
* Summary:
*  This function first disconnects the last connection made with FastSelect or
*  Select, then connects the given channel. The FastSelect function is similar
*  to the Select function, except it is faster since it only disconnects the
*  last channel selected rather than all channels.
*
* Parameters:
*  channel:  The channel to connect to the common terminal.
*
* Return:
*  void
*
*******************************************************************************/
void potSelect_FastSelect(uint8 channel) 
{
    /* Disconnect the last valid channel */
    if( potSelect_lastChannel != potSelect_NULL_CHANNEL)
    {
        potSelect_Disconnect(potSelect_lastChannel);
    }

    /* Make the new channel connection */
#if (potSelect_MUXTYPE == potSelect_MUX_SINGLE)
    potSelect_Set(channel);
#else
    potSelect_CYAMUXSIDE_A_Set(channel);
    potSelect_CYAMUXSIDE_B_Set(channel);
#endif


    potSelect_lastChannel = channel;   /* Update last channel */
}


#if (potSelect_MUXTYPE == potSelect_MUX_DIFF)
#if (!potSelect_ATMOSTONE)
/*******************************************************************************
* Function Name: potSelect_Connect
********************************************************************************
* Summary:
*  This function connects the given channel without affecting other connections.
*
* Parameters:
*  channel:  The channel to connect to the common terminal.
*
* Return:
*  void
*
*******************************************************************************/
void potSelect_Connect(uint8 channel) 
{
    potSelect_CYAMUXSIDE_A_Set(channel);
    potSelect_CYAMUXSIDE_B_Set(channel);
}
#endif

/*******************************************************************************
* Function Name: potSelect_Disconnect
********************************************************************************
* Summary:
*  This function disconnects the given channel from the common or output
*  terminal without affecting other connections.
*
* Parameters:
*  channel:  The channel to disconnect from the common terminal.
*
* Return:
*  void
*
*******************************************************************************/
void potSelect_Disconnect(uint8 channel) 
{
    potSelect_CYAMUXSIDE_A_Unset(channel);
    potSelect_CYAMUXSIDE_B_Unset(channel);
}
#endif

#if (potSelect_ATMOSTONE)
/*******************************************************************************
* Function Name: potSelect_DisconnectAll
********************************************************************************
* Summary:
*  This function disconnects all channels.
*
* Parameters:
*  void
*
* Return:
*  void
*
*******************************************************************************/
void potSelect_DisconnectAll(void) 
{
    if(potSelect_lastChannel != potSelect_NULL_CHANNEL) 
    {
        potSelect_Disconnect(potSelect_lastChannel);
        potSelect_lastChannel = potSelect_NULL_CHANNEL;
    }
}
#endif

/* [] END OF FILE */
