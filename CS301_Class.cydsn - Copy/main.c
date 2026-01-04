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
void handle_usb();
//* ========================================

//some global vars
int16_t result_of_conversion; 
char buffer[64];
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

int main()
{
// --------------------------------    
// ----- INITIALIZATIONS ----------
    CYGlobalIntEnable;
    
    
    ADC_SAR_Start();
    ADC_SAR_StartConvert(); 
   
    

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
        
        
        for(int ch = 0; ch < 25; ch++){
        result_of_conversion = ADC_SAR_GetResult16();
        //sprintf(buffer, "%d \r\n",result_of_conversion); 
        
        //convert the adc value into a voltage
        voltage_of_reading = (result_of_conversion * 5 /4096);
        //sprintf(buffer, "%d.%d%d \r\n", (int16_t)voltage_of_reading/1000, (int16_t)voltage_of_reading/100%10, (int16_t)voltage_of_reading/10%10); 
        sensor_sum[ch] += voltage_of_reading;
        voltage_of_reading = 0.0;
        
        //check the max and min values and update 
        if(voltage_of_reading > current_max[ch]){
            current_max[ch] = voltage_of_reading;
        
        }
        
        if(voltage_of_reading < current_min[ch]){
            current_min[ch] = voltage_of_reading; 
        }
        
        sensor_sample_count[ch]++; 
        
        if(sensor_sample_count[ch] >= sample_window){
        
            //calc the final values   
            dc_offset[ch] = sensor_sum[ch]/sample_window; 
            vpp[ch] = current_max[ch] - current_min[ch];
            
            sprintf(buffer, "Channel %d: DC: %d.%d%d VPP: %d.%d%d \r\n", ch, (int16_t)dc_offset[ch], 1000*(int16_t)dc_offset[ch]/100 % 10, 1000*(int16_t)dc_offset[ch]/10%10, (int16_t)vpp[ch],1000*(int16_t)vpp[ch]/100%10, 1000*(int16_t)vpp[ch]/10%10 ); 
            
                  
            usbPutString(buffer);
            //reset the vars
            sensor_sum[ch] = 0; 
            current_max[ch] = 0; 
            current_min[ch] = 0;
            sensor_sample_count[ch] = 0; 
        
        }
        
        
        
        
        
        
        
        
        

        
        }
     
       
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


/* [] END OF FILE */
