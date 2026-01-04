
#include "project.h"


#define TOTAL_RX_TX_PINS  CapSense_TOTAL_ELECTRODES

#define CAP_MEASUREMENT_RATE 5 //1 sec >= 5, timer tick rate is 0.2 second

volatile uint8 flag_sns = 0, flag_cp = 0;
uint32 c;

//---------------------------
// Uncomment only one
//#define MATLAB_ONLY_SENSOR_VALUE_MEASUREMENT
//#define MATLAB_ALL_DATA
#define MATLAB_EXTENDED_SET_MEASUREMENT
//#define MATLAB_CAPACITOR_MEASUREMENT
//#define TUNER
//---------------------------

#ifdef TUNER
uint8 header[] = {0x0Du, 0x0Au};
uint8 tail[] = {0x00u, 0xFFu, 0xFFu};
#endif

#ifdef MATLAB_EXTENDED_SET_MEASUREMENT
uint8 header[] = {0xffu, 0xffu};    
uint8 tail[] = {0x00u, 0xFFu};

uint16 sns_value[CapSense_TOTAL_CSX_NODES];
uint8 idac_comp[CapSense_TOTAL_CSX_NODES];
uint32 sns_capacitance[CapSense_TOTAL_CSX_NODES];
uint32 ext_capacitance[3]; // CMOD, CINTA, CINTB
uint8 i;
#endif

CY_ISR(MY_ISR_TS)
{
/* ISR Code here */
    flag_sns++;
    flag_cp++;
    isr_TS_ClearPending();
}

int main()
{
    uint32 *ptr32;
    CapSense_TST_MEASUREMENT_STATUS_ENUM StatusPtr;
    
    CyGlobalIntEnable; /* Enable global interrupts. */
//    __enable_irq(); /* Enable global interrupts. */
    UART_Start(); /* Start EZI2C Component */
    L1_Write(0);
    L2_Write(0);
    
// Timer for Cap sensing    
    isr_TS_StartEx(MY_ISR_TS);
    isr_TS_ClearPending();
    TSTimer_Start();

// TIMER TEST
//    while(1)
//    {
//        if (flag_sns > 0)
//        {
//            L2_Write(!L2_Read());
//            flag_sns = 0;
//        }
//        if (flag_cp > 5)
//        {
//            L1_Write(!L1_Read());
//            flag_cp = 0;
//        } 
//    }
    
    
    
 /* 
 * Set up communication and initialize data buffer to CapSense data structure
 * to use Tuner application
 */
// EZI2C_EzI2CSetBuffer1(sizeof(CapSense_dsRam), sizeof(CapSense_dsRam), (uint8_t *)&(CapSense_dsRam));
    CapSense_Start(); /* Initialize Component */
    CapSense_ScanAllWidgets(); /* Scan all widgets */
    ptr32 = (uint32*)(&CapSense_dsRam) + 336;
     for(;;)
     {
     /* Do this only when a scan is done */
         if(CapSense_NOT_BUSY == CapSense_IsBusy())
         {
            CapSense_ProcessAllWidgets(); /* Process all widgets */
            CapSense_RunTuner(); /* To sync with Tuner application */

#ifdef TUNER                        
            UART_SpiUartPutArray((uint8 *)(&header), sizeof(header));
            UART_SpiUartPutArray((uint8 *)(&CapSense_dsRam), sizeof(CapSense_dsRam));
            UART_SpiUartPutArray((uint8 *)(&tail), sizeof(tail));
#endif

    #ifdef MATLAB_EXTENDED_SET_MEASUREMENT
            if (flag_sns > 0)
            {
                for (i = 0; i < CapSense_TOTAL_CSX_NODES; i++)
                {
                    sns_value[i] = CapSense_dsRam.snsList.button0[i].raw[0u];
                    idac_comp[i] = CapSense_dsRam.snsList.button0[i].idacComp[0u];
                }
                ptr32 = (uint32*)(&CapSense_dsRam) + 336;                     
    //            UART_SpiUartPutArray((uint8 *)(&header), sizeof(header));
                UART_SpiUartPutArray((uint8 *)(&sns_value[0]), CapSense_TOTAL_CSX_NODES*sizeof(uint16));
                UART_SpiUartPutArray((uint8 *)(&idac_comp[0]), CapSense_TOTAL_CSX_NODES*sizeof(uint8));
    //            UART_SpiUartPutArray((uint8 *)(&sns_capacitance[0]), TOTAL_RX_TX_PINS*sizeof(uint32));
                UART_SpiUartPutArray((uint8 *)(ptr32), TOTAL_RX_TX_PINS*sizeof(uint32));
                UART_SpiUartPutArray((uint8 *)(&ext_capacitance[0]), 3*sizeof(uint32));
    //            UART_SpiUartPutArray((uint8 *)(&tail), sizeof(tail));
                UART_UartPutCRLF(0xff);
                L1_Write(!L1_Read());
                flag_sns = 0;
            }
            if (flag_cp >= CAP_MEASUREMENT_RATE)
            {
//                ptr32 = (uint32*)(&CapSense_dsRam) + 336;
//                for (i = 0; i < TOTAL_RX_TX_PINS; i++)
//                {
//                    *(ptr32+i)  = CapSense_GetSensorCapacitance(CapSense_MATRIXBUTTONS0_WDGT_ID, i, &StatusPtr);
//                    while(CapSense_NOT_BUSY != CapSense_IsBusy()){}
//                }            
                
                for (i = 0; i < 6; i++)
                {
                    while(CapSense_NOT_BUSY != CapSense_IsBusy()){};
                    *(ptr32++) =  CapSense_GetSensorCapacitance(i, 0, &StatusPtr);
                    
                    while(CapSense_NOT_BUSY != CapSense_IsBusy()){};
                    *(ptr32++) =  CapSense_GetSensorCapacitance(i, 1, &StatusPtr);
                    
                    while(CapSense_NOT_BUSY != CapSense_IsBusy()){};
                    *(ptr32++) =  CapSense_GetSensorCapacitance(i, 2, &StatusPtr);
                }
                
//                CapSense_RunSelfTest(CapSense_TST_RUN_SELF_TEST_MASK);
//                while(CapSense_NOT_BUSY != CapSense_IsBusy()){}
//                CapSense_RunSelfTest(CapSense_TST_SNS_CAP);
                
                
                // NOTE: MEasuring CMOD not available in library - contrary to datasheet!
                ext_capacitance[0] = 0u;    //CapSense_GetExtCapCapacitance(CapSense_TST_CMOD_ID); 
                ext_capacitance[1] = CapSense_GetExtCapCapacitance(CapSense_TST_CINTA_ID);
                ext_capacitance[2] = CapSense_GetExtCapCapacitance(CapSense_TST_CINTB_ID);
                
                flag_cp = 0;                
                L2_Write(!L2_Read());
            }
            
    #endif



            if (CapSense_IsAnyWidgetActive()) /* Scan result verification */
            {
     /* add custom tasks to execute when touch detected */
            }
     
            CapSense_ScanAllWidgets(); /* Start next scan */
         }
     }
}
