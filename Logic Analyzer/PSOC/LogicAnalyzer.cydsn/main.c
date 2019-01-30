/* ========================================
 *
 * Copyright YOUR COMPANY, THE YEAR
 * All Rights Reserved
 * UNPUBLISHED, LICENSED SOFTWARE.
 *
 * CONFIDENTIAL AND PROPRIETARY INFORMATION
 * WHICH IS THE PROPERTY OF your company.
 *
 * ========================================
*/
#include "project.h"

uint8 DMA_Chan;
uint8 DMA_TD[8];
uint8 direction;

uint8 data[4][5000];
int sendSource = 0;
uint8 potValue[2];
uint8 oldPots[2];
//int pot2Value[1];
int i;
int dataTD = 0;
int triggerIndex = -1;
int freqFlag = 0;
int memDepth = 0;
uint8 freqSelect;
unsigned char table[256];
uint8 sendBuf[3*5000];
int sendFlag = 0;
int DMAforce = 0;

CY_ISR(DMAISR)
{
    if(DMAforce < 5)            // forces the DMA interrupt to trigger at least 5 times so that there is data before the trigger even
    {
        DMAforce++;
    }
    //LED_Write(1);
    else
    {
        if (triggerIndex == -1)         // serach for trigger Index
        {
            for (i = 0; i < 5000; i++)      // iterate through data gathered
            {
                if (table[data[dataTD][i]] == 1)            // if the we see a 1 then we found positive triggerindex
                {
                    triggerIndex = i;
                    break;
                }
                else if (table[data[dataTD][i]] == 0 && table[data[dataTD][i - 1]] == 1 && direction == 1)          // if we see a 1 then 0 then we found neg triggerindex
                {
                    triggerIndex = i;
                }
            }
        }
        else
        {
            CyDmaChDisable(DMA_Chan);           // the trigger has been found so we diable the DMA, there is no need for it anymore
            int currentBuf = dataTD - 1;        // we figure out which buffer the triggerindex was found in
            int prevBuf = dataTD - 2;
            if (currentBuf < 0)
            {
                currentBuf += 4;
            }
            if (prevBuf < 0)
            {
                prevBuf +=4;
            }
            memcpy(sendBuf, data[prevBuf], 5000);           // we take the buffers from before, during, and after the trigger and concatanate them to create one large coninuous data buffer
            memcpy(sendBuf + 5000, data[currentBuf], 5000);
            memcpy(sendBuf + 10000, data[dataTD], 5000);
            sendFlag = 1;           // set flag to one to send this data to Pi
        }
    }
    dataTD = (dataTD + 1) % 4;
}

int main(void)
{
    CyGlobalIntEnable; /* Enable global interrupts. */
    USBFS_Start(0,USBFS_5V_OPERATION);
    while (0u == USBFS_GetConfiguration())
    {
        
    }
    USBFS_EnableOutEP(2);
    DMAInt_StartEx(DMAISR);
    ADC_DelSig_Start();
    potSelect_Start();
    ADC_DelSig_StartConvert();
    
    /* Defines for DMA */
    #define DMA_BYTES_PER_BURST 1
    #define DMA_REQUEST_PER_BURST 1
    #define DMA_SRC_BASE (CYDEV_PERIPH_BASE)
    #define DMA_DST_BASE (CYDEV_SRAM_BASE)


    /* DMA Configuration for DMA */
    DMA_Chan = DMA_DmaInitialize(DMA_BYTES_PER_BURST, DMA_REQUEST_PER_BURST, 
                                 HI16(DMA_SRC_BASE), HI16(DMA_DST_BASE));
    DMA_TD[0] = CyDmaTdAllocate();
    DMA_TD[1] = CyDmaTdAllocate();
    DMA_TD[2] = CyDmaTdAllocate();
    DMA_TD[3] = CyDmaTdAllocate();
    DMA_TD[4] = CyDmaTdAllocate();
    DMA_TD[5] = CyDmaTdAllocate();
    DMA_TD[6] = CyDmaTdAllocate();
    DMA_TD[7] = CyDmaTdAllocate();
    
    CyDmaTdSetConfiguration(DMA_TD[0], 2500, DMA_TD[1],  CY_DMA_TD_INC_DST_ADR);
    CyDmaTdSetConfiguration(DMA_TD[1], 2500, DMA_TD[2], DMA__TD_TERMOUT_EN | CY_DMA_TD_INC_DST_ADR);
    CyDmaTdSetConfiguration(DMA_TD[2], 2500, DMA_TD[3],  CY_DMA_TD_INC_DST_ADR);
    CyDmaTdSetConfiguration(DMA_TD[3], 2500, DMA_TD[4], DMA__TD_TERMOUT_EN | CY_DMA_TD_INC_DST_ADR);
    CyDmaTdSetConfiguration(DMA_TD[4], 2500, DMA_TD[5], CY_DMA_TD_INC_DST_ADR);
    CyDmaTdSetConfiguration(DMA_TD[5], 2500, DMA_TD[6], DMA__TD_TERMOUT_EN | CY_DMA_TD_INC_DST_ADR);
    CyDmaTdSetConfiguration(DMA_TD[6], 2500, DMA_TD[7], CY_DMA_TD_INC_DST_ADR);
    CyDmaTdSetConfiguration(DMA_TD[7], 2500, DMA_TD[0], DMA__TD_TERMOUT_EN | CY_DMA_TD_INC_DST_ADR);
    
    CyDmaTdSetAddress(DMA_TD[0], LO16((uint32)Status_Reg_1_Status_PTR), LO16((uint32)data[0]));
    CyDmaTdSetAddress(DMA_TD[1], LO16((uint32)Status_Reg_1_Status_PTR), LO16((uint32)data[0] + 2500));
    CyDmaTdSetAddress(DMA_TD[2], LO16((uint32)Status_Reg_1_Status_PTR), LO16((uint32)data[1]));
    CyDmaTdSetAddress(DMA_TD[3], LO16((uint32)Status_Reg_1_Status_PTR), LO16((uint32)data[1] + 2500));
    CyDmaTdSetAddress(DMA_TD[4], LO16((uint32)Status_Reg_1_Status_PTR), LO16((uint32)data[2]));
    CyDmaTdSetAddress(DMA_TD[5], LO16((uint32)Status_Reg_1_Status_PTR), LO16((uint32)data[2] + 2500));
    CyDmaTdSetAddress(DMA_TD[6], LO16((uint32)Status_Reg_1_Status_PTR), LO16((uint32)data[3]));
    CyDmaTdSetAddress(DMA_TD[7], LO16((uint32)Status_Reg_1_Status_PTR), LO16((uint32)data[3] + 2500));
    
    while(USBFS_GetEPState(2) == USBFS_OUT_BUFFER_EMPTY)
    {
        if (0u != USBFS_IsConfigurationChanged())
        {
            /* Re-enable endpoint when device is configured. */
            if (0u != USBFS_GetConfiguration())
            {
                /* Enable OUT endpoint to receive data from host. */
                USBFS_EnableOutEP(2);
            }
        }
    }
    USBFS_ReadOutEP(2,&freqSelect, 1);          // receive frequency from Pi
    freqFlag = 1;
    
    
    while(USBFS_GetEPState(2) == USBFS_OUT_BUFFER_EMPTY)
    {
        if (0u != USBFS_IsConfigurationChanged())
        {
            /* Re-enable endpoint when device is configured. */
            if (0u != USBFS_GetConfiguration())
            {
                /* Enable OUT endpoint to receive data from host. */
                USBFS_EnableOutEP(2);
            }
        }
    }
    USBFS_ReadOutEP(2,(unsigned char*)&memDepth, 4);            // receive memDepth from Pi
    
    int j;
    for (j = 0; j < 256; j+=64)
    {
        while(USBFS_GetEPState(2) == USBFS_OUT_BUFFER_EMPTY)
        {
            if (0u != USBFS_IsConfigurationChanged())
            {
                /* Re-enable endpoint when device is configured. */
                if (0u != USBFS_GetConfiguration())
                {
                    /* Enable OUT endpoint to receive data from host. */
                    USBFS_EnableOutEP(2);
                }
            }
        }
        USBFS_ReadOutEP(2,table + j, 64);           // receive Truthtable from Pi
    }
    
    while(USBFS_GetEPState(2) == USBFS_OUT_BUFFER_EMPTY)
    {
        if (0u != USBFS_IsConfigurationChanged())
        {
            /* Re-enable endpoint when device is configured. */
            if (0u != USBFS_GetConfiguration())
            {
                /* Enable OUT endpoint to receive data from host. */
                USBFS_EnableOutEP(2);
            }
        }
    }
    USBFS_ReadOutEP(2,&direction, 4);           // receive triggerDirection from Pi
    
    muxSelect_Write(freqSelect);            // set frequency of DMA according to frequency sent from Pi
    CyDmaChSetInitialTd(DMA_Chan, DMA_TD[0]);
    CyDmaChEnable(DMA_Chan, 1);             // start the DMA
    
    for(;;)
    {
        if (sendFlag == 1)          // flag is set to one so trigger was found and data put together
        {
            for(i = 0; i < memDepth; i+=64)         // loop to send all the data to the Pi
            {
                while(USBFS_GetEPState(1) == USBFS_IN_BUFFER_FULL)
                {
                }
                if (memDepth - i < 64)
                {
                    USBFS_LoadInEP(1, sendBuf + i + triggerIndex - memDepth/2, memDepth - i);
                }
                else
                {
                    USBFS_LoadInEP(1, sendBuf + i + triggerIndex - memDepth/2, 64);
                }
            }
            sendFlag = 0;           // set flag back to zero so we dont keep sending data
        }
        
        potSelect_Select(0);        // read the pots and then send them
        potValue[0] = ADC_DelSig_Read16();
        if (potValue[0] > 255)
        {
            potValue[0] = 255;
        }
        if (potValue[0] < 0)
        {
            potValue[0] = 0;
        }
        
        
        potSelect_Select(1);
        CyDelay(5);
        potValue[1] = ADC_DelSig_Read16();
        if (potValue[1] > 255)
        {
            potValue[1] = 255;
        }
        if (potValue[1] < 0)
        {
            potValue[1] = 0;
        }
        // check to see if pots have changed
        if (oldPots[0] -  potValue[0] > 5 ||oldPots[1] -  potValue[1] > 5 || potValue[0] -  oldPots[0] > 5 || potValue[1] -  oldPots[1] > 5)
        {
            oldPots[0] = potValue[0];
            oldPots[1] = potValue[1];    
            if (USBFS_GetEPState(3) == USBFS_IN_BUFFER_EMPTY)
            {
                    USBFS_LoadInEP(3, potValue, 2);         // send the pot data to the Pi continuously
            }
        }
    
    }
}

/* [] END OF FILE */
