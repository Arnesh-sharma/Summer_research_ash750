/* ========================================
 * Fully working code: 
 * PWM      : 
 * Encoder  : 
 * ADC      :
 * USB      : port displays speed and position.
 * CMD: "PW xx"
 * Copyright Univ of Auckland, 2016
 * All Rights Reserved
 * UNPUBLISHED, LICENSED SOFTWARE.
 *
 * CONFIDENTIAL AND PROPRIETARY INFORMATION
 * WHICH IS THE PROPERTY OF Univ of Auckland.
 *
 * ========================================
*/
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <project.h>
//* ========================================
#include "defines.h"
#include "vars.h"
//* ========================================
void usbPutString(char *s);
void usbPutChar(char c);
void usbPutBin(uint16_t *buffer, uint16_t count); 
void handle_usb();
//* ========================================

//some global vars
int16_t result_of_conversion; 
char buffer[64];
uint16_t *bank1; 
//uint16_t tempBank;
uint16_t Bank1New[26];
void init_dma_1_CHAINED();
int sample_window = 100; 
float voltage_of_reading = 0.0; 
int16_t sensor_sum[25] = {0}; 
int16_t sensor_max[25] = {0};
int16_t sensor_min[25] = {0};
float current_max[25] = {0.0}; 
float current_min[25] = {0.0};
float dc_offset[25] = {0.0};
float vpp[25] = {0.0}; 
int16_t sensor_sample_count[25] = {0}; 
uint8_t flag = 10; 
uint8_t flag1 = 0;



CY_ISR(ADC_READ)
{
/* ISR Code here */
    flag1 += 1;
    //RX_Write(1);
    DB8_Write(1);
    //DB8_Write(0);
    
}


int main()
{
    
// --------------------------------    
// ----- INITIALIZATIONS ----------
    CYGlobalIntEnable;
    
    bank1 = &Bank1New[1];
    Bank1New[8] = 10; 
    Bank1New[0] = 0xFFFF; 
    #ifdef USE_DMA_CHAINED
    init_dma_1_CHAINED();    
    #endif
    
    ADC_Start();
    Timer_1_Start();
    Timer_1_WritePeriod(10);
    isr_eoc_StartEx(ADC_READ); 
    

   
    

// ------USB SETUP ----------------    
#ifdef USE_USB    
    USBUART_Start(0,USBUART_5V_OPERATION);
#endif        
    for(;;)
    {
       
    
    
        /* Place your application code here. */
        handle_usb();
        if (flag_KB_string == 1)
        {
            usbPutString(line);
            flag_KB_string = 0;
        }  
        
        
//        for(int ch = 0; ch < 25; ch++){
//        result_of_conversion = ADC_SAR_GetResult16();
//        //sprintf(buffer, "%d \r\n",result_of_conversion); 
//        
//        //convert the adc value into a voltage
//        voltage_of_reading = (result_of_conversion * 5 /4096);
//        //sprintf(buffer, "%d.%d%d \r\n", (int16_t)voltage_of_reading/1000, (int16_t)voltage_of_reading/100%10, (int16_t)voltage_of_reading/10%10); 
//        sensor_sum[ch] += voltage_of_reading;
//        voltage_of_reading = 0.0;
//        
//        //check the max and min values and update 
//        if(voltage_of_reading > current_max[ch]){
//            current_max[ch] = voltage_of_reading;
//        
//        }
//        
//        if(voltage_of_reading < current_min[ch]){
//            current_min[ch] = voltage_of_reading; 
//        }
//        
//        sensor_sample_count[ch]++; 
//        
//        if(sensor_sample_count[ch] >= sample_window){
//        
//            //calc the final values   
//            dc_offset[ch] = sensor_sum[ch]/sample_window; 
//            vpp[ch] = current_max[ch] - current_min[ch];
//            
//            sprintf(buffer, "Channel %d: DC: %d.%d%d VPP: %d.%d%d \r\n", ch, (int16_t)dc_offset[ch], 1000*(int16_t)dc_offset[ch]/100 % 10, 1000*(int16_t)dc_offset[ch]/10%10, (int16_t)vpp[ch],1000*(int16_t)vpp[ch]/100%10, 1000*(int16_t)vpp[ch]/10%10 ); 
//            
//                  
//            usbPutString(buffer);
//            //reset the vars
//            sensor_sum[ch] = 0; 
//            current_max[ch] = 0; 
//            current_min[ch] = 0;
//            sensor_sample_count[ch] = 0; 
//        
//        }
//        
//        }
        
        uint16_t i = 0; 
        uint16_t min = 4096; 
        uint16_t max = 0; 
        uint16_t diff = 0; 
        uint16_t diff1 = 0; 
        uint16_t min1 = 4096; 
        uint16_t max1 = 0;
        uint16_t new_diff = 0; 
        if(flag == 1){
        while(i<400){
        //calculate the voltage readings
        voltage_of_reading = ADC_GetResult16(8); 
        uint16_t bank_voltage = bank1[8]; 
        //sprintf(buffer,"SEN%d:%d(%d)-- %d.%d%dV (%d.%d%dV)\n",8,ADC_GetResult16(8), bank1[8], (int16_t)voltage_of_reading,  1000*(int16_t)voltage_of_reading/100%10, 1000*(int16_t)voltage_of_reading/10%10,(int16_t)bank_voltage,  1000*(int16_t)bank_voltage/100%10, 1000*(int16_t)bank_voltage/10%10 );
        if(min > bank_voltage){
            min = bank_voltage; 
        }
        
   
        
        if(max<bank_voltage){
            max = bank_voltage; 
        }
        
         if(min1 > voltage_of_reading){
            min1 = voltage_of_reading; 
        }
        
      
        
        if(max1<voltage_of_reading){
            max1 = voltage_of_reading; 
        }
        
        diff = max - min; 
        
        diff1 = max1 - min1; 
        
        new_diff = diff - diff1;
        
        
        
        sprintf(buffer, "MAX: %d.%d%d -- MIN: %d.%d%d -- DIFF: %d.%d%d \n", (int16_t)max,  1000*(int16_t)max/100%10, 1000*(int16_t)max/10%10,(int16_t)min,  1000*(int16_t)min/100%10, 1000*(int16_t)min/10%10, (int16_t)diff,  1000*(int16_t)diff/100%10, 1000*(int16_t)diff/10%10); 
        usbPutString(buffer);
        sprintf(buffer, "MAX1: %d.%d%d -- MIN1: %d.%d%d -- DIFF1: %d.%d%d \n", (int16_t)max1,  1000*(int16_t)max1/100%10, 1000*(int16_t)max1/10%10,(int16_t)min1,  1000*(int16_t)min1/100%10, 1000*(int16_t)min1/10%10, (int16_t)diff,  1000*(int16_t)diff1/100%10, 1000*(int16_t)diff1/10%10);
        usbPutString(buffer);
        sprintf(buffer, "DIFFDIFF: %d.%d%d\n", (int16_t)new_diff,  1000*(int16_t)new_diff/100%10, 1000*(int16_t)new_diff/10%10);
        usbPutString(buffer);
        i++; 
       
        }    
        }else{
             
            if(flag1 >= 1){
                
             
                
             //have a flag/special character as an acknowledgement of the whole packet 
            // tempBank[0] = 0xFFFF; 
     
            //make a copy of the DMA data 
            //memcpy(&tempBank[1], bank1, 50);
        
            //send the entire array as raw binary, so with 26 elements thats about 52 bytes since 2 bytes each 
            usbPutBin(Bank1New, PACKET_ELEMENTS); 
            //sprintf(buffer, "HERE %d \n", DB8_Read() );
            DB8_Write(0);
            //sprintf(buffer, "DONE %d \n", DB8_Read());
            //usbPutString(buffer); 
            flag1 = 0; 
        }
            
        }
        
        //CyDelay(1000);  
        
        
        
    }   
    
}
//* ========================================
void usbPutString(char *s)
{
// !! Assumes that *s is a string with allocated space >=64 chars     
//  Since USB implementation retricts data packets to 64 chars, this function truncates the
//  length to 62 char (63rd char is a '!')

#ifdef USE_USB     
    while (USBUART_CDCIsReady() == 0);
    s[63]='\0';
    s[62]='!';
    USBUART_PutData((uint8*)s,strlen(s));
#endif
}
//* ========================================
void usbPutChar(char c)
{
#ifdef USE_USB     
    while (USBUART_CDCIsReady() == 0);
    USBUART_PutChar(c);
#endif    
}

//* ========================================

void usbPutBin(uint16_t *buffer, uint16_t count)
{
    uint16_t totalBytes = count * 2; // 16-bit numbers = 2 bytes each

    #ifdef USE_USB
        // 1. TIMEOUT CHECK
        // Don't wait forever. If USB is stuck, skip this packet and keep running.
        // (Simple loop counter as a timeout)
        uint32_t timeout = 10000; 
        
        while (USBUART_CDCIsReady() == 0)
        {
            timeout--;
            if(timeout == 0) return; // USB is busy/unplugged. Abort to save the system.
        }

        // 2. SEND DATA
        USBUART_PutData((uint8*)buffer, totalBytes);
    #endif
}


//* ========================================
void handle_usb()
{
    // handles input at terminal, echos it back to the terminal
    // turn echo OFF, key emulation: only CR
    // entered string is made available in 'line' and 'flag_KB_string' is set
    
    static uint8 usbStarted = FALSE;
    static uint16 usbBufCount = 0;
    uint8 c; 
    

    if (!usbStarted)
    {
        if (USBUART_GetConfiguration())
        {
            USBUART_CDC_Init();
            usbStarted = TRUE;
        }
    }
    else
    {
        if (USBUART_DataIsReady() != 0)
        {  
            c = USBUART_GetChar();

            if ((c == 13) || (c == 10))
            {
//                if (usbBufCount > 0)
                {
                    entry[usbBufCount]= '\0';
                    strcpy(line,entry);
                    usbBufCount = 0;
                    flag_KB_string = 1;
                }
            }
            else 
            {
                if (((c == CHAR_BACKSP) || (c == CHAR_DEL) ) && (usbBufCount > 0) )
                    usbBufCount--;
                else
                {
                    if (usbBufCount > (BUF_SIZE-2) ) // one less else strtok triggers a crash
                    {
                       USBUART_PutChar('!');        
                    }
                    else
                        entry[usbBufCount++] = c;  
                }  
            }
        }
    }    
}
#ifdef USE_DMA_CHAINED
void init_dma_1_CHAINED(void)
{
/* Defines for DMA_1 */
#define DMA_1_BYTES_PER_BURST 2
#define DMA_1_REQUEST_PER_BURST 1
#define DMA_1_SRC_BASE (CYDEV_SRAM_BASE)
#define DMA_1_DST_BASE (CYDEV_SRAM_BASE)

/* Variable declarations for DMA_1 */
/* Move these variable declarations to the top of the function */
uint8 DMA_1_Chan;
uint8 DMA_1_TD[25];

/* DMA Configuration for DMA_1 */
DMA_1_Chan = DMA_1_DmaInitialize(DMA_1_BYTES_PER_BURST, DMA_1_REQUEST_PER_BURST, 
    HI16(DMA_1_SRC_BASE), HI16(DMA_1_DST_BASE));
DMA_1_TD[0] = CyDmaTdAllocate();
DMA_1_TD[1] = CyDmaTdAllocate();
DMA_1_TD[2] = CyDmaTdAllocate();
DMA_1_TD[3] = CyDmaTdAllocate();
DMA_1_TD[4] = CyDmaTdAllocate();
DMA_1_TD[5] = CyDmaTdAllocate();
DMA_1_TD[6] = CyDmaTdAllocate();
DMA_1_TD[7] = CyDmaTdAllocate();
DMA_1_TD[8] = CyDmaTdAllocate();
DMA_1_TD[9] = CyDmaTdAllocate();
DMA_1_TD[10] = CyDmaTdAllocate();
DMA_1_TD[11] = CyDmaTdAllocate();
DMA_1_TD[12] = CyDmaTdAllocate();
DMA_1_TD[13] = CyDmaTdAllocate();
DMA_1_TD[14] = CyDmaTdAllocate();
DMA_1_TD[15] = CyDmaTdAllocate();
DMA_1_TD[16] = CyDmaTdAllocate();
DMA_1_TD[17] = CyDmaTdAllocate();
DMA_1_TD[18] = CyDmaTdAllocate();
DMA_1_TD[19] = CyDmaTdAllocate();
DMA_1_TD[20] = CyDmaTdAllocate();
DMA_1_TD[21] = CyDmaTdAllocate();
DMA_1_TD[22] = CyDmaTdAllocate();
DMA_1_TD[23] = CyDmaTdAllocate();
DMA_1_TD[24] = CyDmaTdAllocate();





//CyDmaTdSetConfiguration(DMA_1_TD[0],  2, DMA_1_TD[1], TD_AUTO_EXEC_NEXT | DMA_1__TD_TERMOUT_EN);
//CyDmaTdSetConfiguration(DMA_1_TD[1],  2, DMA_1_TD[2], TD_AUTO_EXEC_NEXT | DMA_1__TD_TERMOUT_EN);
//CyDmaTdSetConfiguration(DMA_1_TD[2],  2, DMA_1_TD[3], TD_AUTO_EXEC_NEXT | DMA_1__TD_TERMOUT_EN);
//CyDmaTdSetConfiguration(DMA_1_TD[3],  2, DMA_1_TD[4], TD_AUTO_EXEC_NEXT | DMA_1__TD_TERMOUT_EN);
//CyDmaTdSetConfiguration(DMA_1_TD[4],  2, DMA_1_TD[5], TD_AUTO_EXEC_NEXT | DMA_1__TD_TERMOUT_EN);
//CyDmaTdSetConfiguration(DMA_1_TD[5],  2, DMA_1_TD[6], TD_AUTO_EXEC_NEXT | DMA_1__TD_TERMOUT_EN);
//CyDmaTdSetConfiguration(DMA_1_TD[6],  2, DMA_1_TD[7], TD_AUTO_EXEC_NEXT | DMA_1__TD_TERMOUT_EN);
//CyDmaTdSetConfiguration(DMA_1_TD[7],  2, DMA_1_TD[8], TD_AUTO_EXEC_NEXT | DMA_1__TD_TERMOUT_EN);
//CyDmaTdSetConfiguration(DMA_1_TD[8],  2, DMA_1_TD[9], TD_AUTO_EXEC_NEXT | DMA_1__TD_TERMOUT_EN);
//CyDmaTdSetConfiguration(DMA_1_TD[9],  2, DMA_1_TD[10],TD_AUTO_EXEC_NEXT | DMA_1__TD_TERMOUT_EN);
//CyDmaTdSetConfiguration(DMA_1_TD[10], 2, DMA_1_TD[11],TD_AUTO_EXEC_NEXT | DMA_1__TD_TERMOUT_EN);
//CyDmaTdSetConfiguration(DMA_1_TD[11], 2, DMA_1_TD[12],TD_AUTO_EXEC_NEXT | DMA_1__TD_TERMOUT_EN);
//CyDmaTdSetConfiguration(DMA_1_TD[12],  2, DMA_1_TD[13], TD_AUTO_EXEC_NEXT | DMA_1__TD_TERMOUT_EN);
//CyDmaTdSetConfiguration(DMA_1_TD[13],  2, DMA_1_TD[14], TD_AUTO_EXEC_NEXT | DMA_1__TD_TERMOUT_EN);
//CyDmaTdSetConfiguration(DMA_1_TD[14],  2, DMA_1_TD[15], TD_AUTO_EXEC_NEXT | DMA_1__TD_TERMOUT_EN);
//CyDmaTdSetConfiguration(DMA_1_TD[15],  2, DMA_1_TD[16], TD_AUTO_EXEC_NEXT | DMA_1__TD_TERMOUT_EN);
//CyDmaTdSetConfiguration(DMA_1_TD[16],  2, DMA_1_TD[17], TD_AUTO_EXEC_NEXT | DMA_1__TD_TERMOUT_EN);
//CyDmaTdSetConfiguration(DMA_1_TD[17],  2, DMA_1_TD[18], TD_AUTO_EXEC_NEXT | DMA_1__TD_TERMOUT_EN);
//CyDmaTdSetConfiguration(DMA_1_TD[18],  2, DMA_1_TD[19], TD_AUTO_EXEC_NEXT | DMA_1__TD_TERMOUT_EN);
//CyDmaTdSetConfiguration(DMA_1_TD[19],  2, DMA_1_TD[20], TD_AUTO_EXEC_NEXT | DMA_1__TD_TERMOUT_EN);
//CyDmaTdSetConfiguration(DMA_1_TD[20],  2, DMA_1_TD[21], TD_AUTO_EXEC_NEXT | DMA_1__TD_TERMOUT_EN);
//CyDmaTdSetConfiguration(DMA_1_TD[21],  2, DMA_1_TD[22],TD_AUTO_EXEC_NEXT | DMA_1__TD_TERMOUT_EN);
//CyDmaTdSetConfiguration(DMA_1_TD[22], 2, DMA_1_TD[23],TD_AUTO_EXEC_NEXT | DMA_1__TD_TERMOUT_EN);
//CyDmaTdSetConfiguration(DMA_1_TD[23], 2, DMA_1_TD[24],TD_AUTO_EXEC_NEXT | DMA_1__TD_TERMOUT_EN);
//CyDmaTdSetConfiguration(DMA_1_TD[24], 2, DMA_1_TD[0], DMA_1__TD_TERMOUT_EN);

CyDmaTdSetConfiguration(DMA_1_TD[0],  2, DMA_1_TD[1], TD_AUTO_EXEC_NEXT);
CyDmaTdSetConfiguration(DMA_1_TD[1],  2, DMA_1_TD[2], TD_AUTO_EXEC_NEXT);
CyDmaTdSetConfiguration(DMA_1_TD[2],  2, DMA_1_TD[3], TD_AUTO_EXEC_NEXT);
CyDmaTdSetConfiguration(DMA_1_TD[3],  2, DMA_1_TD[4], TD_AUTO_EXEC_NEXT);
CyDmaTdSetConfiguration(DMA_1_TD[4],  2, DMA_1_TD[5], TD_AUTO_EXEC_NEXT);
CyDmaTdSetConfiguration(DMA_1_TD[5],  2, DMA_1_TD[6], TD_AUTO_EXEC_NEXT);
CyDmaTdSetConfiguration(DMA_1_TD[6],  2, DMA_1_TD[7], TD_AUTO_EXEC_NEXT);
CyDmaTdSetConfiguration(DMA_1_TD[7],  2, DMA_1_TD[8], TD_AUTO_EXEC_NEXT);
CyDmaTdSetConfiguration(DMA_1_TD[8],  2, DMA_1_TD[9], TD_AUTO_EXEC_NEXT);
CyDmaTdSetConfiguration(DMA_1_TD[9],  2, DMA_1_TD[10],TD_AUTO_EXEC_NEXT);
CyDmaTdSetConfiguration(DMA_1_TD[10], 2, DMA_1_TD[11],TD_AUTO_EXEC_NEXT);
CyDmaTdSetConfiguration(DMA_1_TD[11], 2, DMA_1_TD[12],TD_AUTO_EXEC_NEXT);
CyDmaTdSetConfiguration(DMA_1_TD[12],  2, DMA_1_TD[13], TD_AUTO_EXEC_NEXT);
CyDmaTdSetConfiguration(DMA_1_TD[13],  2, DMA_1_TD[14], TD_AUTO_EXEC_NEXT);
CyDmaTdSetConfiguration(DMA_1_TD[14],  2, DMA_1_TD[15], TD_AUTO_EXEC_NEXT);
CyDmaTdSetConfiguration(DMA_1_TD[15],  2, DMA_1_TD[16], TD_AUTO_EXEC_NEXT);
CyDmaTdSetConfiguration(DMA_1_TD[16],  2, DMA_1_TD[17], TD_AUTO_EXEC_NEXT);
CyDmaTdSetConfiguration(DMA_1_TD[17],  2, DMA_1_TD[18], TD_AUTO_EXEC_NEXT);
CyDmaTdSetConfiguration(DMA_1_TD[18],  2, DMA_1_TD[19], TD_AUTO_EXEC_NEXT);
CyDmaTdSetConfiguration(DMA_1_TD[19],  2, DMA_1_TD[20], TD_AUTO_EXEC_NEXT);
CyDmaTdSetConfiguration(DMA_1_TD[20],  2, DMA_1_TD[21], TD_AUTO_EXEC_NEXT);
CyDmaTdSetConfiguration(DMA_1_TD[21],  2, DMA_1_TD[22],TD_AUTO_EXEC_NEXT);
CyDmaTdSetConfiguration(DMA_1_TD[22], 2, DMA_1_TD[23],TD_AUTO_EXEC_NEXT);
CyDmaTdSetConfiguration(DMA_1_TD[23], 2, DMA_1_TD[24],TD_AUTO_EXEC_NEXT);
CyDmaTdSetConfiguration(DMA_1_TD[24], 2, DMA_1_TD[0], DMA_1__TD_TERMOUT_EN);

CyDmaTdSetAddress(DMA_1_TD[0],   LO16((uint32)&ADC_finalArray[0]),  LO16((uint32)&bank1[24]));
CyDmaTdSetAddress(DMA_1_TD[1],   LO16((uint32)&ADC_finalArray[1]),  LO16((uint32)&bank1[23]));
CyDmaTdSetAddress(DMA_1_TD[2],   LO16((uint32)&ADC_finalArray[2]),  LO16((uint32)&bank1[22]));
CyDmaTdSetAddress(DMA_1_TD[3],   LO16((uint32)&ADC_finalArray[3]),  LO16((uint32)&bank1[21]));
CyDmaTdSetAddress(DMA_1_TD[4],   LO16((uint32)&ADC_finalArray[4]),  LO16((uint32)&bank1[20]));
CyDmaTdSetAddress(DMA_1_TD[5],   LO16((uint32)&ADC_finalArray[5]),  LO16((uint32)&bank1[19]));
CyDmaTdSetAddress(DMA_1_TD[6],   LO16((uint32)&ADC_finalArray[6]),  LO16((uint32)&bank1[18]));
CyDmaTdSetAddress(DMA_1_TD[7],   LO16((uint32)&ADC_finalArray[7]),  LO16((uint32)&bank1[17]));
CyDmaTdSetAddress(DMA_1_TD[8],   LO16((uint32)&ADC_finalArray[8]),  LO16((uint32)&bank1[16]));
CyDmaTdSetAddress(DMA_1_TD[9],   LO16((uint32)&ADC_finalArray[9]),  LO16((uint32)&bank1[15]));
CyDmaTdSetAddress(DMA_1_TD[10],  LO16((uint32)&ADC_finalArray[10]), LO16((uint32)&bank1[14]));
CyDmaTdSetAddress(DMA_1_TD[11],  LO16((uint32)&ADC_finalArray[11]), LO16((uint32)&bank1[13]));
CyDmaTdSetAddress(DMA_1_TD[12],  LO16((uint32)&ADC_finalArray[12]), LO16((uint32)&bank1[12]));
CyDmaTdSetAddress(DMA_1_TD[13],  LO16((uint32)&ADC_finalArray[13]),  LO16((uint32)&bank1[11]));
CyDmaTdSetAddress(DMA_1_TD[14],  LO16((uint32)&ADC_finalArray[14]),  LO16((uint32)&bank1[10]));
CyDmaTdSetAddress(DMA_1_TD[15],  LO16((uint32)&ADC_finalArray[15]),  LO16((uint32)&bank1[9]));
CyDmaTdSetAddress(DMA_1_TD[16],  LO16((uint32)&ADC_finalArray[16]),  LO16((uint32)&bank1[8]));
CyDmaTdSetAddress(DMA_1_TD[17],  LO16((uint32)&ADC_finalArray[17]),  LO16((uint32)&bank1[7]));
CyDmaTdSetAddress(DMA_1_TD[18],  LO16((uint32)&ADC_finalArray[18]),  LO16((uint32)&bank1[6]));
CyDmaTdSetAddress(DMA_1_TD[19],  LO16((uint32)&ADC_finalArray[19]),  LO16((uint32)&bank1[5]));
CyDmaTdSetAddress(DMA_1_TD[20],  LO16((uint32)&ADC_finalArray[20]),  LO16((uint32)&bank1[4]));
CyDmaTdSetAddress(DMA_1_TD[21],  LO16((uint32)&ADC_finalArray[21]),  LO16((uint32)&bank1[3]));
CyDmaTdSetAddress(DMA_1_TD[22],  LO16((uint32)&ADC_finalArray[22]),  LO16((uint32)&bank1[2]));
CyDmaTdSetAddress(DMA_1_TD[23],  LO16((uint32)&ADC_finalArray[23]), LO16((uint32)&bank1[1]));
CyDmaTdSetAddress(DMA_1_TD[24],  LO16((uint32)&ADC_finalArray[24]), LO16((uint32)&bank1[0]));


CyDmaChSetInitialTd(DMA_1_Chan, DMA_1_TD[0]);
CyDmaChEnable(DMA_1_Chan, 1);

}
#endif





/* [] END OF FILE */
