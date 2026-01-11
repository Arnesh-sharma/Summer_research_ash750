/*******************************************************************************
* File Name: PinTest.h  
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

#if !defined(CY_PINS_PinTest_H) /* Pins PinTest_H */
#define CY_PINS_PinTest_H

#include "cytypes.h"
#include "cyfitter.h"
#include "cypins.h"
#include "PinTest_aliases.h"

/* APIs are not generated for P15[7:6] */
#if !(CY_PSOC5A &&\
	 PinTest__PORT == 15 && ((PinTest__MASK & 0xC0) != 0))


/***************************************
*        Function Prototypes             
***************************************/    

/**
* \addtogroup group_general
* @{
*/
void    PinTest_Write(uint8 value);
void    PinTest_SetDriveMode(uint8 mode);
uint8   PinTest_ReadDataReg(void);
uint8   PinTest_Read(void);
void    PinTest_SetInterruptMode(uint16 position, uint16 mode);
uint8   PinTest_ClearInterrupt(void);
/** @} general */

/***************************************
*           API Constants        
***************************************/
/**
* \addtogroup group_constants
* @{
*/
    /** \addtogroup driveMode Drive mode constants
     * \brief Constants to be passed as "mode" parameter in the PinTest_SetDriveMode() function.
     *  @{
     */
        #define PinTest_DM_ALG_HIZ         PIN_DM_ALG_HIZ
        #define PinTest_DM_DIG_HIZ         PIN_DM_DIG_HIZ
        #define PinTest_DM_RES_UP          PIN_DM_RES_UP
        #define PinTest_DM_RES_DWN         PIN_DM_RES_DWN
        #define PinTest_DM_OD_LO           PIN_DM_OD_LO
        #define PinTest_DM_OD_HI           PIN_DM_OD_HI
        #define PinTest_DM_STRONG          PIN_DM_STRONG
        #define PinTest_DM_RES_UPDWN       PIN_DM_RES_UPDWN
    /** @} driveMode */
/** @} group_constants */
    
/* Digital Port Constants */
#define PinTest_MASK               PinTest__MASK
#define PinTest_SHIFT              PinTest__SHIFT
#define PinTest_WIDTH              1u

/* Interrupt constants */
#if defined(PinTest__INTSTAT)
/**
* \addtogroup group_constants
* @{
*/
    /** \addtogroup intrMode Interrupt constants
     * \brief Constants to be passed as "mode" parameter in PinTest_SetInterruptMode() function.
     *  @{
     */
        #define PinTest_INTR_NONE      (uint16)(0x0000u)
        #define PinTest_INTR_RISING    (uint16)(0x0001u)
        #define PinTest_INTR_FALLING   (uint16)(0x0002u)
        #define PinTest_INTR_BOTH      (uint16)(0x0003u) 
    /** @} intrMode */
/** @} group_constants */

    #define PinTest_INTR_MASK      (0x01u) 
#endif /* (PinTest__INTSTAT) */


/***************************************
*             Registers        
***************************************/

/* Main Port Registers */
/* Pin State */
#define PinTest_PS                     (* (reg8 *) PinTest__PS)
/* Data Register */
#define PinTest_DR                     (* (reg8 *) PinTest__DR)
/* Port Number */
#define PinTest_PRT_NUM                (* (reg8 *) PinTest__PRT) 
/* Connect to Analog Globals */                                                  
#define PinTest_AG                     (* (reg8 *) PinTest__AG)                       
/* Analog MUX bux enable */
#define PinTest_AMUX                   (* (reg8 *) PinTest__AMUX) 
/* Bidirectional Enable */                                                        
#define PinTest_BIE                    (* (reg8 *) PinTest__BIE)
/* Bit-mask for Aliased Register Access */
#define PinTest_BIT_MASK               (* (reg8 *) PinTest__BIT_MASK)
/* Bypass Enable */
#define PinTest_BYP                    (* (reg8 *) PinTest__BYP)
/* Port wide control signals */                                                   
#define PinTest_CTL                    (* (reg8 *) PinTest__CTL)
/* Drive Modes */
#define PinTest_DM0                    (* (reg8 *) PinTest__DM0) 
#define PinTest_DM1                    (* (reg8 *) PinTest__DM1)
#define PinTest_DM2                    (* (reg8 *) PinTest__DM2) 
/* Input Buffer Disable Override */
#define PinTest_INP_DIS                (* (reg8 *) PinTest__INP_DIS)
/* LCD Common or Segment Drive */
#define PinTest_LCD_COM_SEG            (* (reg8 *) PinTest__LCD_COM_SEG)
/* Enable Segment LCD */
#define PinTest_LCD_EN                 (* (reg8 *) PinTest__LCD_EN)
/* Slew Rate Control */
#define PinTest_SLW                    (* (reg8 *) PinTest__SLW)

/* DSI Port Registers */
/* Global DSI Select Register */
#define PinTest_PRTDSI__CAPS_SEL       (* (reg8 *) PinTest__PRTDSI__CAPS_SEL) 
/* Double Sync Enable */
#define PinTest_PRTDSI__DBL_SYNC_IN    (* (reg8 *) PinTest__PRTDSI__DBL_SYNC_IN) 
/* Output Enable Select Drive Strength */
#define PinTest_PRTDSI__OE_SEL0        (* (reg8 *) PinTest__PRTDSI__OE_SEL0) 
#define PinTest_PRTDSI__OE_SEL1        (* (reg8 *) PinTest__PRTDSI__OE_SEL1) 
/* Port Pin Output Select Registers */
#define PinTest_PRTDSI__OUT_SEL0       (* (reg8 *) PinTest__PRTDSI__OUT_SEL0) 
#define PinTest_PRTDSI__OUT_SEL1       (* (reg8 *) PinTest__PRTDSI__OUT_SEL1) 
/* Sync Output Enable Registers */
#define PinTest_PRTDSI__SYNC_OUT       (* (reg8 *) PinTest__PRTDSI__SYNC_OUT) 

/* SIO registers */
#if defined(PinTest__SIO_CFG)
    #define PinTest_SIO_HYST_EN        (* (reg8 *) PinTest__SIO_HYST_EN)
    #define PinTest_SIO_REG_HIFREQ     (* (reg8 *) PinTest__SIO_REG_HIFREQ)
    #define PinTest_SIO_CFG            (* (reg8 *) PinTest__SIO_CFG)
    #define PinTest_SIO_DIFF           (* (reg8 *) PinTest__SIO_DIFF)
#endif /* (PinTest__SIO_CFG) */

/* Interrupt Registers */
#if defined(PinTest__INTSTAT)
    #define PinTest_INTSTAT            (* (reg8 *) PinTest__INTSTAT)
    #define PinTest_SNAP               (* (reg8 *) PinTest__SNAP)
    
	#define PinTest_0_INTTYPE_REG 		(* (reg8 *) PinTest__0__INTTYPE)
#endif /* (PinTest__INTSTAT) */

#endif /* CY_PSOC5A... */

#endif /*  CY_PINS_PinTest_H */


/* [] END OF FILE */
