/***************************************************************************
* File Name: ADCDMA2_dma.c  
* Version 1.70
*
*  Description:
*   Provides an API for the DMAC component. The API includes functions
*   for the DMA controller, DMA channels and Transfer Descriptors.
*
*
*   Note:
*     This module requires the developer to finish or fill in the auto
*     generated funcions and setup the dma channel and TD's.
*
********************************************************************************
* Copyright 2008-2010, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions, 
* disclaimers, and limitations in the end user license agreement accompanying 
* the software package with which this file was provided.
********************************************************************************/
#include <CYLIB.H>
#include <CYDMAC.H>
#include <ADCDMA2_dma.H>



/****************************************************************************
* 
* The following defines are available in Cyfitter.h
* 
* 
* 
* ADCDMA2__DRQ_CTL_REG
* 
* 
* ADCDMA2__DRQ_NUMBER
* 
* Number of TD's used by this channel.
* ADCDMA2__NUMBEROF_TDS
* 
* Priority of this channel.
* ADCDMA2__PRIORITY
* 
* True if ADCDMA2_TERMIN_SEL is used.
* ADCDMA2__TERMIN_EN
* 
* TERMIN interrupt line to signal terminate.
* ADCDMA2__TERMIN_SEL
* 
* 
* True if ADCDMA2_TERMOUT0_SEL is used.
* ADCDMA2__TERMOUT0_EN
* 
* 
* TERMOUT0 interrupt line to signal completion.
* ADCDMA2__TERMOUT0_SEL
* 
* 
* True if ADCDMA2_TERMOUT1_SEL is used.
* ADCDMA2__TERMOUT1_EN
* 
* 
* TERMOUT1 interrupt line to signal completion.
* ADCDMA2__TERMOUT1_SEL
* 
****************************************************************************/


/* Zero based index of ADCDMA2 dma channel */
uint8 ADCDMA2_DmaHandle = DMA_INVALID_CHANNEL;

/*********************************************************************
* Function Name: uint8 ADCDMA2_DmaInitalize
**********************************************************************
* Summary:
*   Allocates and initialises a channel of the DMAC to be used by the
*   caller.
*
* Parameters:
*   BurstCount.
*       
*       
*   ReqestPerBurst.
*       
*       
*   UpperSrcAddress.
*       
*       
*   UpperDestAddress.
*       
*
* Return:
*   The channel that can be used by the caller for DMA activity.
*   DMA_INVALID_CHANNEL (0xFF) if there are no channels left. 
*
*
*******************************************************************/
uint8 ADCDMA2_DmaInitialize(uint8 BurstCount, uint8 ReqestPerBurst, uint16 UpperSrcAddress, uint16 UpperDestAddress) 
{

    /* Allocate a DMA channel. */
    ADCDMA2_DmaHandle = (uint8)ADCDMA2__DRQ_NUMBER;

    /* Configure the channel. */
    (void)CyDmaChSetConfiguration(ADCDMA2_DmaHandle,
                                  BurstCount,
                                  ReqestPerBurst,
                                  (uint8)ADCDMA2__TERMOUT0_SEL,
                                  (uint8)ADCDMA2__TERMOUT1_SEL,
                                  (uint8)ADCDMA2__TERMIN_SEL);

    /* Set the extended address for the transfers */
    (void)CyDmaChSetExtendedAddress(ADCDMA2_DmaHandle, UpperSrcAddress, UpperDestAddress);

    /* Set the priority for this channel */
    (void)CyDmaChPriority(ADCDMA2_DmaHandle, (uint8)ADCDMA2__PRIORITY);
    
    return ADCDMA2_DmaHandle;
}

/*********************************************************************
* Function Name: void ADCDMA2_DmaRelease
**********************************************************************
* Summary:
*   Frees the channel associated with ADCDMA2.
*
*
* Parameters:
*   void.
*
*
*
* Return:
*   void.
*
*******************************************************************/
void ADCDMA2_DmaRelease(void) 
{
    /* Disable the channel */
    (void)CyDmaChDisable(ADCDMA2_DmaHandle);
}

