/*******************************************************************************
* File Name: myPin.h  
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

#if !defined(CY_PINS_myPin_H) /* Pins myPin_H */
#define CY_PINS_myPin_H

#include "cytypes.h"
#include "cyfitter.h"
#include "cypins.h"
#include "myPin_aliases.h"

/* APIs are not generated for P15[7:6] */
#if !(CY_PSOC5A &&\
	 myPin__PORT == 15 && ((myPin__MASK & 0xC0) != 0))


/***************************************
*        Function Prototypes             
***************************************/    

/**
* \addtogroup group_general
* @{
*/
void    myPin_Write(uint8 value);
void    myPin_SetDriveMode(uint8 mode);
uint8   myPin_ReadDataReg(void);
uint8   myPin_Read(void);
void    myPin_SetInterruptMode(uint16 position, uint16 mode);
uint8   myPin_ClearInterrupt(void);
/** @} general */

/***************************************
*           API Constants        
***************************************/
/**
* \addtogroup group_constants
* @{
*/
    /** \addtogroup driveMode Drive mode constants
     * \brief Constants to be passed as "mode" parameter in the myPin_SetDriveMode() function.
     *  @{
     */
        #define myPin_DM_ALG_HIZ         PIN_DM_ALG_HIZ
        #define myPin_DM_DIG_HIZ         PIN_DM_DIG_HIZ
        #define myPin_DM_RES_UP          PIN_DM_RES_UP
        #define myPin_DM_RES_DWN         PIN_DM_RES_DWN
        #define myPin_DM_OD_LO           PIN_DM_OD_LO
        #define myPin_DM_OD_HI           PIN_DM_OD_HI
        #define myPin_DM_STRONG          PIN_DM_STRONG
        #define myPin_DM_RES_UPDWN       PIN_DM_RES_UPDWN
    /** @} driveMode */
/** @} group_constants */
    
/* Digital Port Constants */
#define myPin_MASK               myPin__MASK
#define myPin_SHIFT              myPin__SHIFT
#define myPin_WIDTH              1u

/* Interrupt constants */
#if defined(myPin__INTSTAT)
/**
* \addtogroup group_constants
* @{
*/
    /** \addtogroup intrMode Interrupt constants
     * \brief Constants to be passed as "mode" parameter in myPin_SetInterruptMode() function.
     *  @{
     */
        #define myPin_INTR_NONE      (uint16)(0x0000u)
        #define myPin_INTR_RISING    (uint16)(0x0001u)
        #define myPin_INTR_FALLING   (uint16)(0x0002u)
        #define myPin_INTR_BOTH      (uint16)(0x0003u) 
    /** @} intrMode */
/** @} group_constants */

    #define myPin_INTR_MASK      (0x01u) 
#endif /* (myPin__INTSTAT) */


/***************************************
*             Registers        
***************************************/

/* Main Port Registers */
/* Pin State */
#define myPin_PS                     (* (reg8 *) myPin__PS)
/* Data Register */
#define myPin_DR                     (* (reg8 *) myPin__DR)
/* Port Number */
#define myPin_PRT_NUM                (* (reg8 *) myPin__PRT) 
/* Connect to Analog Globals */                                                  
#define myPin_AG                     (* (reg8 *) myPin__AG)                       
/* Analog MUX bux enable */
#define myPin_AMUX                   (* (reg8 *) myPin__AMUX) 
/* Bidirectional Enable */                                                        
#define myPin_BIE                    (* (reg8 *) myPin__BIE)
/* Bit-mask for Aliased Register Access */
#define myPin_BIT_MASK               (* (reg8 *) myPin__BIT_MASK)
/* Bypass Enable */
#define myPin_BYP                    (* (reg8 *) myPin__BYP)
/* Port wide control signals */                                                   
#define myPin_CTL                    (* (reg8 *) myPin__CTL)
/* Drive Modes */
#define myPin_DM0                    (* (reg8 *) myPin__DM0) 
#define myPin_DM1                    (* (reg8 *) myPin__DM1)
#define myPin_DM2                    (* (reg8 *) myPin__DM2) 
/* Input Buffer Disable Override */
#define myPin_INP_DIS                (* (reg8 *) myPin__INP_DIS)
/* LCD Common or Segment Drive */
#define myPin_LCD_COM_SEG            (* (reg8 *) myPin__LCD_COM_SEG)
/* Enable Segment LCD */
#define myPin_LCD_EN                 (* (reg8 *) myPin__LCD_EN)
/* Slew Rate Control */
#define myPin_SLW                    (* (reg8 *) myPin__SLW)

/* DSI Port Registers */
/* Global DSI Select Register */
#define myPin_PRTDSI__CAPS_SEL       (* (reg8 *) myPin__PRTDSI__CAPS_SEL) 
/* Double Sync Enable */
#define myPin_PRTDSI__DBL_SYNC_IN    (* (reg8 *) myPin__PRTDSI__DBL_SYNC_IN) 
/* Output Enable Select Drive Strength */
#define myPin_PRTDSI__OE_SEL0        (* (reg8 *) myPin__PRTDSI__OE_SEL0) 
#define myPin_PRTDSI__OE_SEL1        (* (reg8 *) myPin__PRTDSI__OE_SEL1) 
/* Port Pin Output Select Registers */
#define myPin_PRTDSI__OUT_SEL0       (* (reg8 *) myPin__PRTDSI__OUT_SEL0) 
#define myPin_PRTDSI__OUT_SEL1       (* (reg8 *) myPin__PRTDSI__OUT_SEL1) 
/* Sync Output Enable Registers */
#define myPin_PRTDSI__SYNC_OUT       (* (reg8 *) myPin__PRTDSI__SYNC_OUT) 

/* SIO registers */
#if defined(myPin__SIO_CFG)
    #define myPin_SIO_HYST_EN        (* (reg8 *) myPin__SIO_HYST_EN)
    #define myPin_SIO_REG_HIFREQ     (* (reg8 *) myPin__SIO_REG_HIFREQ)
    #define myPin_SIO_CFG            (* (reg8 *) myPin__SIO_CFG)
    #define myPin_SIO_DIFF           (* (reg8 *) myPin__SIO_DIFF)
#endif /* (myPin__SIO_CFG) */

/* Interrupt Registers */
#if defined(myPin__INTSTAT)
    #define myPin_INTSTAT            (* (reg8 *) myPin__INTSTAT)
    #define myPin_SNAP               (* (reg8 *) myPin__SNAP)
    
	#define myPin_0_INTTYPE_REG 		(* (reg8 *) myPin__0__INTTYPE)
#endif /* (myPin__INTSTAT) */

#endif /* CY_PSOC5A... */

#endif /*  CY_PINS_myPin_H */


/* [] END OF FILE */
