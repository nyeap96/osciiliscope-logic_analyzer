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

uint8 source [2][2][64];            // ping pong buffer arrays
int probe1TXTD = 0;                 // ping pong selector for channel 1 data
int probe2TXTD = 0;                 // ping pong selector for channel 1 data
uint8 pot1Value[1];
uint8 pot2Value[1];



CY_ISR(ADCDMAISR1)
{
    if (USBFS_GetEPState(1) == USBFS_IN_BUFFER_EMPTY)           // send data if the buffer is empty
    {
        USBFS_LoadInEP(1,source[0][probe1TXTD],64);
    }   
    probe1TXTD = 1 - probe1TXTD;            // switch buffer to send from
    
    potSelect_Select(0);            // select channel 1 pot
    pot1Value[0] = ADC_DelSig_Read16();
    if (pot1Value[0] >= 255)
    {
        pot1Value[0] = 255;
    }
    else if (pot1Value[0] < 0)
    {
        pot1Value[0] = 0;
    }
    if (USBFS_GetEPState(3) == USBFS_IN_BUFFER_EMPTY)           // send pot value to Pi
    {
        USBFS_LoadInEP(3,pot1Value,1);
    }
}

CY_ISR(ADCDMAISR2)
{
    if (USBFS_GetEPState(2) == USBFS_IN_BUFFER_EMPTY)           // send data if the buffer is empty
    {
        USBFS_LoadInEP(2,source[1][probe2TXTD],64);
    }
    probe2TXTD = 1 - probe2TXTD;            // switch buffer to send from
    
    potSelect_Select(1);            // select channel 1 pot
    pot2Value[0] = ADC_DelSig_Read16();
    if (pot2Value[0] >= 255)
    {
        pot2Value[0] = 255;
    }
    else if (pot2Value[0] < 0)
    {
        pot2Value[0] = 0;
    }
    if (USBFS_GetEPState(4) == USBFS_IN_BUFFER_EMPTY)           // send pot value to Pi
    {
        USBFS_LoadInEP(4,pot2Value,1);
    }
}

int main(void)
{
    CyGlobalIntEnable; /* Enable global interrupts. */
    USBFS_Start(0,USBFS_5V_OPERATION);
    while (0u == USBFS_GetConfiguration())
    {
        
    }
    USBFS_EnableOutEP(5);
    
    ADCInt1_StartEx(ADCDMAISR1);
    ADCInt2_StartEx(ADCDMAISR2);

    //WaveDAC8_1_Start();
    ADC_SAR_1_Start();
    ADC_SAR_1_StartConvert();
    
    //WaveDAC8_2_Start();
    ADC_SAR_2_Start();
    ADC_SAR_2_StartConvert();
    
    potSelect_Start();
    potSelect_Select(0);
    ADC_DelSig_Start();
    
    /* Defines for DMA_1 */
    #define DMA_1_BYTES_PER_BURST 1
    #define DMA_1_REQUEST_PER_BURST 1
    #define DMA_1_SRC_BASE (CYDEV_PERIPH_BASE)
    #define DMA_1_DST_BASE (CYDEV_SRAM_BASE)

    /* Variable declarations for DMA_1 */
    /* Move these variable declarations to the top of the function */
    
    uint8 DMA_1_Chan;
    uint8 DMA_1_TD[2];

    /* DMA Configuration for DMA_1 */
    
    DMA_1_Chan = ADCDMA1_DmaInitialize(DMA_1_BYTES_PER_BURST, DMA_1_REQUEST_PER_BURST, 
                                     HI16(DMA_1_SRC_BASE), HI16(DMA_1_DST_BASE));
    DMA_1_TD[0] = CyDmaTdAllocate();
    DMA_1_TD[1] = CyDmaTdAllocate();
    CyDmaTdSetConfiguration(DMA_1_TD[0], 64, DMA_1_TD[1], ADCDMA1__TD_TERMOUT_EN | CY_DMA_TD_INC_DST_ADR);
    CyDmaTdSetConfiguration(DMA_1_TD[1], 64, DMA_1_TD[0], ADCDMA1__TD_TERMOUT_EN | CY_DMA_TD_INC_DST_ADR);
    CyDmaTdSetAddress(DMA_1_TD[0], LO16((uint32)ADC_SAR_1_SAR_WRK0_PTR), LO16((uint32)&source[0][0]));
    CyDmaTdSetAddress(DMA_1_TD[1], LO16((uint32)ADC_SAR_1_SAR_WRK0_PTR), LO16((uint32)&source[0][1]));
    CyDmaChSetInitialTd(DMA_1_Chan, DMA_1_TD[0]);
    CyDmaChEnable(DMA_1_Chan, 1);
    
    
    /* Defines for ADCDMA2 */
    #define ADCDMA2_BYTES_PER_BURST 1
    #define ADCDMA2_REQUEST_PER_BURST 1
    #define ADCDMA2_SRC_BASE (CYDEV_PERIPH_BASE)
    #define ADCDMA2_DST_BASE (CYDEV_SRAM_BASE)

    /* Variable declarations for ADCDMA2 */
    /* Move these variable declarations to the top of the function */
    uint8 ADCDMA2_Chan;
    uint8 ADCDMA2_TD[2];

    /* DMA Configuration for ADCDMA2 */
    ADCDMA2_Chan = ADCDMA2_DmaInitialize(ADCDMA2_BYTES_PER_BURST, ADCDMA2_REQUEST_PER_BURST, 
                                         HI16(ADCDMA2_SRC_BASE), HI16(ADCDMA2_DST_BASE));
    ADCDMA2_TD[0] = CyDmaTdAllocate();
    ADCDMA2_TD[1] = CyDmaTdAllocate();
    CyDmaTdSetConfiguration(ADCDMA2_TD[0], 64, ADCDMA2_TD[1], ADCDMA2__TD_TERMOUT_EN | CY_DMA_TD_INC_DST_ADR);
    CyDmaTdSetConfiguration(ADCDMA2_TD[1], 64, ADCDMA2_TD[0], ADCDMA2__TD_TERMOUT_EN | CY_DMA_TD_INC_DST_ADR);
    CyDmaTdSetAddress(ADCDMA2_TD[0], LO16((uint32)ADC_SAR_2_SAR_WRK0_PTR), LO16((uint32)&source[1][0]));
    CyDmaTdSetAddress(ADCDMA2_TD[1], LO16((uint32)ADC_SAR_2_SAR_WRK0_PTR), LO16((uint32)&source[1][1]));
    CyDmaChSetInitialTd(ADCDMA2_Chan, ADCDMA2_TD[0]);
    CyDmaChEnable(ADCDMA2_Chan, 1);

    
    uint8 run[1];       // variable for starting /stopiing data collection
    int i,j,k;
    
    for(;;)
    {
        /* Place your application code here. */
        if (USBFS_GetEPState(5) != USBFS_OUT_BUFFER_EMPTY)          // receive data on run argument from Pi
        {
            USBFS_ReadOutEP(5,run,1);
        }
        if (run[0] == 1)            // if run is 1 then enable DMAs and send data to Pi
        {
            CyDmaChEnable(ADCDMA2_Chan,1);
            CyDmaChEnable(DMA_1_Chan,1);
            run[0] = 2;
        }
        if (run[0] == 0)            // if run is 0 stop the DMAs
        {
            CyDmaChDisable(ADCDMA2_Chan);
            CyDmaChDisable(DMA_1_Chan);
            for (i = 0; i < 2; i++)         // clears the send buffer when run is 0
            {
                for (j = 0; j < 2; j++)
                {
                    for (k = 0; k < 64; k++)
                    {
                       source[i][j][k] = 0;
                    }
                }
            }
            run[0] = 3;         // set run 3 so that i can continuously send zeros to the Pi
        }
        if (run[0] == 3)            // send zeros to Pi
        {
            if (USBFS_GetEPState(1) == USBFS_IN_BUFFER_EMPTY)
            {
                USBFS_LoadInEP(1,source[1][0],64);
            }
            if (USBFS_GetEPState(2) == USBFS_IN_BUFFER_EMPTY)
            {
                USBFS_LoadInEP(2,source[1][0],64);
            } 
        }
        if (0u != USBFS_IsConfigurationChanged())
        {
            /* Re-enable endpoint when device is configured. */
            if (0u != USBFS_GetConfiguration())
            {
                /* Enable OUT endpoint to receive data from host. */
                USBFS_EnableOutEP(1);
                USBFS_EnableOutEP(2);
                USBFS_EnableOutEP(3);
                USBFS_EnableOutEP(4);
                USBFS_EnableOutEP(5);
            }
        }
    }
}

/* [] END OF FILE */
