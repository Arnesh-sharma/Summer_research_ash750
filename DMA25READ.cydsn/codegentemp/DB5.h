/*******************************************************************************
* File Name: DB5.h  
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

#if !defined(CY_PINS_DB5_H) /* Pins DB5_H */
#define CY_PINS_DB5_H

#include "cytypes.h"
#include "cyfitter.h"
#include "cypins.h"
#include "DB5_aliases.h"

/* APIs are not generated for P15[7:6] */
#if !(CY_PSOC5A &&\
	 DB5__PORT == 15 && ((DB5__MASK & 0xC0) != 0))


/***************************************
*        Function Prototypes             
***************************************/    

/**
* \addtogroup group_general
* @{
*/
void    DB5_Write(uint8 value);
void    DB5_SetDriveMode(uint8 mode);
uint8   DB5_ReadDataReg(void);
uint8   DB5_Read(void);
void    DB5_SetInterruptMode(uint16 position, uint16 mode);
uint8   DB5_ClearInterrupt(void);
/** @} general */

/***************************************
*           API Constants        
***************************************/
/**
* \addtogroup group_constants
* @{
*/
    /** \addtogroup driveMode Drive mode constants
     * \brief Constants to be passed as "mode" parameter in the DB5_SetDriveMode() function.
     *  @{
     */
        #define DB5_DM_ALG_HIZ         PIN_DM_ALG_HIZ
        #define DB5_DM_DIG_HIZ         PIN_DM_DIG_HIZ
        #define DB5_DM_RES_UP          PIN_DM_RES_UP
        #define DB5_DM_RES_DWN         PIN_DM_RES_DWN
        #define DB5_DM_OD_LO           PIN_DM_OD_LO
        #define DB5_DM_OD_HI           PIN_DM_OD_HI
        #define DB5_DM_STRONG          PIN_DM_STRONG
        #define DB5_DM_RES_UPDWN       PIN_DM_RES_UPDWN
    /** @} driveMode */
/** @} group_constants */
    
/* Digital Port Constants */
#define DB5_MASK               DB5__MASK
#define DB5_SHIFT              DB5__SHIFT
#define DB5_WIDTH              1u

/* Interrupt constants */
#if defined(DB5__INTSTAT)
/**
* \addtogroup group_constants
* @{
*/
    /** \addtogroup intrMode Interrupt constants
     * \brief Constants to be passed as "mode" parameter in DB5_SetInterruptMode() function.
     *  @{
     */
        #define DB5_INTR_NONE      (uint16)(0x0000u)
        #define DB5_INTR_RISING    (uint16)(0x0001u)
        #define DB5_INTR_FALLING   (uint16)(0x0002u)
        #define DB5_INTR_BOTH      (uint16)(0x0003u) 
    /** @} intrMode */
/** @} group_constants */

    #define DB5_INTR_MASK      (0x01u) 
#endif /* (DB5__INTSTAT) */


/***************************************
*             Registers        
***************************************/

/* Main Port Registers */
/* Pin State */
#define DB5_PS                     (* (reg8 *) DB5__PS)
/* Data Register */
#define DB5_DR                     (* (reg8 *) DB5__DR)
/* Port Number */
#define DB5_PRT_NUM                (* (reg8 *) DB5__PRT) 
/* Connect to Analog Globals */                                                  
#define DB5_AG                     (* (reg8 *) DB5__AG)                       
/* Analog MUX bux enable */
#define DB5_AMUX                   (* (reg8 *) DB5__AMUX) 
/* Bidirectional Enable */                                                        
#define DB5_BIE                    (* (reg8 *) DB5__BIE)
/* Bit-mask for Aliased Register Access */
#define DB5_BIT_MASK               (* (reg8 *) DB5__BIT_MASK)
/* Bypass Enable */
#define DB5_BYP                    (* (reg8 *) DB5__BYP)
/* Port wide control signals */                                                   
#define DB5_CTL                    (* (reg8 *) DB5__CTL)
/* Drive Modes */
#define DB5_DM0                    (* (reg8 *) DB5__DM0) 
#define DB5_DM1                    (* (reg8 *) DB5__DM1)
#define DB5_DM2                    (* (reg8 *) DB5__DM2) 
/* Input Buffer Disable Override */
#define DB5_INP_DIS                (* (reg8 *) DB5__INP_DIS)
/* LCD Common or Segment Drive */
#define DB5_LCD_COM_SEG            (* (reg8 *) DB5__LCD_COM_SEG)
/* Enable Segment LCD */
#define DB5_LCD_EN                 (* (reg8 *) DB5__LCD_EN)
/* Slew Rate Control */
#define DB5_SLW                    (* (reg8 *) DB5__SLW)

/* DSI Port Registers */
/* Global DSI Select Register */
#define DB5_PRTDSI__CAPS_SEL       (* (reg8 *) DB5__PRTDSI__CAPS_SEL) 
/* Double Sync Enable */
#define DB5_PRTDSI__DBL_SYNC_IN    (* (reg8 *) DB5__PRTDSI__DBL_SYNC_IN) 
/* Output Enable Select Drive Strength */
#define DB5_PRTDSI__OE_SEL0        (* (reg8 *) DB5__PRTDSI__OE_SEL0) 
#define DB5_PRTDSI__OE_SEL1        (* (reg8 *) DB5__PRTDSI__OE_SEL1) 
/* Port Pin Output Select Registers */
#define DB5_PRTDSI__OUT_SEL0       (* (reg8 *) DB5__PRTDSI__OUT_SEL0) 
#define DB5_PRTDSI__OUT_SEL1       (* (reg8 *) DB5__PRTDSI__OUT_SEL1) 
/* Sync Output Enable Registers */
#define DB5_PRTDSI__SYNC_OUT       (* (reg8 *) DB5__PRTDSI__SYNC_OUT) 

/* SIO registers */
#if defined(DB5__SIO_CFG)
    #define DB5_SIO_HYST_EN        (* (reg8 *) DB5__SIO_HYST_EN)
    #define DB5_SIO_REG_HIFREQ     (* (reg8 *) DB5__SIO_REG_HIFREQ)
    #define DB5_SIO_CFG            (* (reg8 *) DB5__SIO_CFG)
    #define DB5_SIO_DIFF           (* (reg8 *) DB5__SIO_DIFF)
#endif /* (DB5__SIO_CFG) */

/* Interrupt Registers */
#if defined(DB5__INTSTAT)
    #define DB5_INTSTAT            (* (reg8 *) DB5__INTSTAT)
    #define DB5_SNAP               (* (reg8 *) DB5__SNAP)
    
	#define DB5_0_INTTYPE_REG 		(* (reg8 *) DB5__0__INTTYPE)
#endif /* (DB5__INTSTAT) */

#endif /* CY_PSOC5A... */

#endif /*  CY_PINS_DB5_H */


/* [] END OF FILE */
