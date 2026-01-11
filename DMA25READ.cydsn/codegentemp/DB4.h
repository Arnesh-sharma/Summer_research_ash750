/*******************************************************************************
* File Name: DB4.h  
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

#if !defined(CY_PINS_DB4_H) /* Pins DB4_H */
#define CY_PINS_DB4_H

#include "cytypes.h"
#include "cyfitter.h"
#include "cypins.h"
#include "DB4_aliases.h"

/* APIs are not generated for P15[7:6] */
#if !(CY_PSOC5A &&\
	 DB4__PORT == 15 && ((DB4__MASK & 0xC0) != 0))


/***************************************
*        Function Prototypes             
***************************************/    

/**
* \addtogroup group_general
* @{
*/
void    DB4_Write(uint8 value);
void    DB4_SetDriveMode(uint8 mode);
uint8   DB4_ReadDataReg(void);
uint8   DB4_Read(void);
void    DB4_SetInterruptMode(uint16 position, uint16 mode);
uint8   DB4_ClearInterrupt(void);
/** @} general */

/***************************************
*           API Constants        
***************************************/
/**
* \addtogroup group_constants
* @{
*/
    /** \addtogroup driveMode Drive mode constants
     * \brief Constants to be passed as "mode" parameter in the DB4_SetDriveMode() function.
     *  @{
     */
        #define DB4_DM_ALG_HIZ         PIN_DM_ALG_HIZ
        #define DB4_DM_DIG_HIZ         PIN_DM_DIG_HIZ
        #define DB4_DM_RES_UP          PIN_DM_RES_UP
        #define DB4_DM_RES_DWN         PIN_DM_RES_DWN
        #define DB4_DM_OD_LO           PIN_DM_OD_LO
        #define DB4_DM_OD_HI           PIN_DM_OD_HI
        #define DB4_DM_STRONG          PIN_DM_STRONG
        #define DB4_DM_RES_UPDWN       PIN_DM_RES_UPDWN
    /** @} driveMode */
/** @} group_constants */
    
/* Digital Port Constants */
#define DB4_MASK               DB4__MASK
#define DB4_SHIFT              DB4__SHIFT
#define DB4_WIDTH              1u

/* Interrupt constants */
#if defined(DB4__INTSTAT)
/**
* \addtogroup group_constants
* @{
*/
    /** \addtogroup intrMode Interrupt constants
     * \brief Constants to be passed as "mode" parameter in DB4_SetInterruptMode() function.
     *  @{
     */
        #define DB4_INTR_NONE      (uint16)(0x0000u)
        #define DB4_INTR_RISING    (uint16)(0x0001u)
        #define DB4_INTR_FALLING   (uint16)(0x0002u)
        #define DB4_INTR_BOTH      (uint16)(0x0003u) 
    /** @} intrMode */
/** @} group_constants */

    #define DB4_INTR_MASK      (0x01u) 
#endif /* (DB4__INTSTAT) */


/***************************************
*             Registers        
***************************************/

/* Main Port Registers */
/* Pin State */
#define DB4_PS                     (* (reg8 *) DB4__PS)
/* Data Register */
#define DB4_DR                     (* (reg8 *) DB4__DR)
/* Port Number */
#define DB4_PRT_NUM                (* (reg8 *) DB4__PRT) 
/* Connect to Analog Globals */                                                  
#define DB4_AG                     (* (reg8 *) DB4__AG)                       
/* Analog MUX bux enable */
#define DB4_AMUX                   (* (reg8 *) DB4__AMUX) 
/* Bidirectional Enable */                                                        
#define DB4_BIE                    (* (reg8 *) DB4__BIE)
/* Bit-mask for Aliased Register Access */
#define DB4_BIT_MASK               (* (reg8 *) DB4__BIT_MASK)
/* Bypass Enable */
#define DB4_BYP                    (* (reg8 *) DB4__BYP)
/* Port wide control signals */                                                   
#define DB4_CTL                    (* (reg8 *) DB4__CTL)
/* Drive Modes */
#define DB4_DM0                    (* (reg8 *) DB4__DM0) 
#define DB4_DM1                    (* (reg8 *) DB4__DM1)
#define DB4_DM2                    (* (reg8 *) DB4__DM2) 
/* Input Buffer Disable Override */
#define DB4_INP_DIS                (* (reg8 *) DB4__INP_DIS)
/* LCD Common or Segment Drive */
#define DB4_LCD_COM_SEG            (* (reg8 *) DB4__LCD_COM_SEG)
/* Enable Segment LCD */
#define DB4_LCD_EN                 (* (reg8 *) DB4__LCD_EN)
/* Slew Rate Control */
#define DB4_SLW                    (* (reg8 *) DB4__SLW)

/* DSI Port Registers */
/* Global DSI Select Register */
#define DB4_PRTDSI__CAPS_SEL       (* (reg8 *) DB4__PRTDSI__CAPS_SEL) 
/* Double Sync Enable */
#define DB4_PRTDSI__DBL_SYNC_IN    (* (reg8 *) DB4__PRTDSI__DBL_SYNC_IN) 
/* Output Enable Select Drive Strength */
#define DB4_PRTDSI__OE_SEL0        (* (reg8 *) DB4__PRTDSI__OE_SEL0) 
#define DB4_PRTDSI__OE_SEL1        (* (reg8 *) DB4__PRTDSI__OE_SEL1) 
/* Port Pin Output Select Registers */
#define DB4_PRTDSI__OUT_SEL0       (* (reg8 *) DB4__PRTDSI__OUT_SEL0) 
#define DB4_PRTDSI__OUT_SEL1       (* (reg8 *) DB4__PRTDSI__OUT_SEL1) 
/* Sync Output Enable Registers */
#define DB4_PRTDSI__SYNC_OUT       (* (reg8 *) DB4__PRTDSI__SYNC_OUT) 

/* SIO registers */
#if defined(DB4__SIO_CFG)
    #define DB4_SIO_HYST_EN        (* (reg8 *) DB4__SIO_HYST_EN)
    #define DB4_SIO_REG_HIFREQ     (* (reg8 *) DB4__SIO_REG_HIFREQ)
    #define DB4_SIO_CFG            (* (reg8 *) DB4__SIO_CFG)
    #define DB4_SIO_DIFF           (* (reg8 *) DB4__SIO_DIFF)
#endif /* (DB4__SIO_CFG) */

/* Interrupt Registers */
#if defined(DB4__INTSTAT)
    #define DB4_INTSTAT            (* (reg8 *) DB4__INTSTAT)
    #define DB4_SNAP               (* (reg8 *) DB4__SNAP)
    
	#define DB4_0_INTTYPE_REG 		(* (reg8 *) DB4__0__INTTYPE)
#endif /* (DB4__INTSTAT) */

#endif /* CY_PSOC5A... */

#endif /*  CY_PINS_DB4_H */


/* [] END OF FILE */
