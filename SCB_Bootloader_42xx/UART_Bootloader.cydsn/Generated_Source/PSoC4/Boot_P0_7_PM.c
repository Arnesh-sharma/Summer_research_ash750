/*******************************************************************************
* File Name: Boot_P0_7.c  
* Version 2.20
*
* Description:
*  This file contains APIs to set up the Pins component for low power modes.
*
* Note:
*
********************************************************************************
* Copyright 2015, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions, 
* disclaimers, and limitations in the end user license agreement accompanying 
* the software package with which this file was provided.
*******************************************************************************/

#include "cytypes.h"
#include "Boot_P0_7.h"

static Boot_P0_7_BACKUP_STRUCT  Boot_P0_7_backup = {0u, 0u, 0u};


/*******************************************************************************
* Function Name: Boot_P0_7_Sleep
****************************************************************************//**
*
* \brief Stores the pin configuration and prepares the pin for entering chip 
*  deep-sleep/hibernate modes. This function applies only to SIO and USBIO pins.
*  It should not be called for GPIO or GPIO_OVT pins.
*
* <b>Note</b> This function is available in PSoC 4 only.
*
* \return 
*  None 
*  
* \sideeffect
*  For SIO pins, this function configures the pin input threshold to CMOS and
*  drive level to Vddio. This is needed for SIO pins when in device 
*  deep-sleep/hibernate modes.
*
* \funcusage
*  \snippet Boot_P0_7_SUT.c usage_Boot_P0_7_Sleep_Wakeup
*******************************************************************************/
void Boot_P0_7_Sleep(void)
{
    #if defined(Boot_P0_7__PC)
        Boot_P0_7_backup.pcState = Boot_P0_7_PC;
    #else
        #if (CY_PSOC4_4200L)
            /* Save the regulator state and put the PHY into suspend mode */
            Boot_P0_7_backup.usbState = Boot_P0_7_CR1_REG;
            Boot_P0_7_USB_POWER_REG |= Boot_P0_7_USBIO_ENTER_SLEEP;
            Boot_P0_7_CR1_REG &= Boot_P0_7_USBIO_CR1_OFF;
        #endif
    #endif
    #if defined(CYIPBLOCK_m0s8ioss_VERSION) && defined(Boot_P0_7__SIO)
        Boot_P0_7_backup.sioState = Boot_P0_7_SIO_REG;
        /* SIO requires unregulated output buffer and single ended input buffer */
        Boot_P0_7_SIO_REG &= (uint32)(~Boot_P0_7_SIO_LPM_MASK);
    #endif  
}


/*******************************************************************************
* Function Name: Boot_P0_7_Wakeup
****************************************************************************//**
*
* \brief Restores the pin configuration that was saved during Pin_Sleep(). This 
* function applies only to SIO and USBIO pins. It should not be called for
* GPIO or GPIO_OVT pins.
*
* For USBIO pins, the wakeup is only triggered for falling edge interrupts.
*
* <b>Note</b> This function is available in PSoC 4 only.
*
* \return 
*  None
*  
* \funcusage
*  Refer to Boot_P0_7_Sleep() for an example usage.
*******************************************************************************/
void Boot_P0_7_Wakeup(void)
{
    #if defined(Boot_P0_7__PC)
        Boot_P0_7_PC = Boot_P0_7_backup.pcState;
    #else
        #if (CY_PSOC4_4200L)
            /* Restore the regulator state and come out of suspend mode */
            Boot_P0_7_USB_POWER_REG &= Boot_P0_7_USBIO_EXIT_SLEEP_PH1;
            Boot_P0_7_CR1_REG = Boot_P0_7_backup.usbState;
            Boot_P0_7_USB_POWER_REG &= Boot_P0_7_USBIO_EXIT_SLEEP_PH2;
        #endif
    #endif
    #if defined(CYIPBLOCK_m0s8ioss_VERSION) && defined(Boot_P0_7__SIO)
        Boot_P0_7_SIO_REG = Boot_P0_7_backup.sioState;
    #endif
}


/* [] END OF FILE */
