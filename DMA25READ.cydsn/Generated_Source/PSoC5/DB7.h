/*******************************************************************************
* File Name: DB7.h  
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

#if !defined(CY_PINS_DB7_H) /* Pins DB7_H */
#define CY_PINS_DB7_H

#include "cytypes.h"
#include "cyfitter.h"
#include "cypins.h"
#include "DB7_aliases.h"

/* APIs are not generated for P15[7:6] */
#if !(CY_PSOC5A &&\
	 DB7__PORT == 15 && ((DB7__MASK & 0xC0) != 0))


/***************************************
*        Function Prototypes             
***************************************/    

/**
* \addtogroup group_general
* @{
*/
void    DB7_Write(uint8 value);
void    DB7_SetDriveMode(uint8 mode);
uint8   DB7_ReadDataReg(void);
uint8   DB7_Read(void);
void    DB7_SetInterruptMode(uint16 position, uint16 mode);
uint8   DB7_ClearInterrupt(void);
/** @} general */

/***************************************
*           API Constants        
***************************************/
/**
* \addtogroup group_constants
* @{
*/
    /** \addtogroup driveMode Drive mode constants
     * \brief Constants to be passed as "mode" parameter in the DB7_SetDriveMode() function.
     *  @{
     */
        #define DB7_DM_ALG_HIZ         PIN_DM_ALG_HIZ
        #define DB7_DM_DIG_HIZ         PIN_DM_DIG_HIZ
        #define DB7_DM_RES_UP          PIN_DM_RES_UP
        #define DB7_DM_RES_DWN         PIN_DM_RES_DWN
        #define DB7_DM_OD_LO           PIN_DM_OD_LO
        #define DB7_DM_OD_HI           PIN_DM_OD_HI
        #define DB7_DM_STRONG          PIN_DM_STRONG
        #define DB7_DM_RES_UPDWN       PIN_DM_RES_UPDWN
    /** @} driveMode */
/** @} group_constants */
    
/* Digital Port Constants */
#define DB7_MASK               DB7__MASK
#define DB7_SHIFT              DB7__SHIFT
#define DB7_WIDTH              1u

/* Interrupt constants */
#if defined(DB7__INTSTAT)
/**
* \addtogroup group_constants
* @{
*/
    /** \addtogroup intrMode Interrupt constants
     * \brief Constants to be passed as "mode" parameter in DB7_SetInterruptMode() function.
     *  @{
     */
        #define DB7_INTR_NONE      (uint16)(0x0000u)
        #define DB7_INTR_RISING    (uint16)(0x0001u)
        #define DB7_INTR_FALLING   (uint16)(0x0002u)
        #define DB7_INTR_BOTH      (uint16)(0x0003u) 
    /** @} intrMode */
/** @} group_constants */

    #define DB7_INTR_MASK      (0x01u) 
#endif /* (DB7__INTSTAT) */


/***************************************
*             Registers        
***************************************/

/* Main Port Registers */
/* Pin State */
#define DB7_PS                     (* (reg8 *) DB7__PS)
/* Data Register */
#define DB7_DR                     (* (reg8 *) DB7__DR)
/* Port Number */
#define DB7_PRT_NUM                (* (reg8 *) DB7__PRT) 
/* Connect to Analog Globals */                                                  
#define DB7_AG                     (* (reg8 *) DB7__AG)                       
/* Analog MUX bux enable */
#define DB7_AMUX                   (* (reg8 *) DB7__AMUX) 
/* Bidirectional Enable */                                                        
#define DB7_BIE                    (* (reg8 *) DB7__BIE)
/* Bit-mask for Aliased Register Access */
#define DB7_BIT_MASK               (* (reg8 *) DB7__BIT_MASK)
/* Bypass Enable */
#define DB7_BYP                    (* (reg8 *) DB7__BYP)
/* Port wide control signals */                                                   
#define DB7_CTL                    (* (reg8 *) DB7__CTL)
/* Drive Modes */
#define DB7_DM0                    (* (reg8 *) DB7__DM0) 
#define DB7_DM1                    (* (reg8 *) DB7__DM1)
#define DB7_DM2                    (* (reg8 *) DB7__DM2) 
/* Input Buffer Disable Override */
#define DB7_INP_DIS                (* (reg8 *) DB7__INP_DIS)
/* LCD Common or Segment Drive */
#define DB7_LCD_COM_SEG            (* (reg8 *) DB7__LCD_COM_SEG)
/* Enable Segment LCD */
#define DB7_LCD_EN                 (* (reg8 *) DB7__LCD_EN)
/* Slew Rate Control */
#define DB7_SLW                    (* (reg8 *) DB7__SLW)

/* DSI Port Registers */
/* Global DSI Select Register */
#define DB7_PRTDSI__CAPS_SEL       (* (reg8 *) DB7__PRTDSI__CAPS_SEL) 
/* Double Sync Enable */
#define DB7_PRTDSI__DBL_SYNC_IN    (* (reg8 *) DB7__PRTDSI__DBL_SYNC_IN) 
/* Output Enable Select Drive Strength */
#define DB7_PRTDSI__OE_SEL0        (* (reg8 *) DB7__PRTDSI__OE_SEL0) 
#define DB7_PRTDSI__OE_SEL1        (* (reg8 *) DB7__PRTDSI__OE_SEL1) 
/* Port Pin Output Select Registers */
#define DB7_PRTDSI__OUT_SEL0       (* (reg8 *) DB7__PRTDSI__OUT_SEL0) 
#define DB7_PRTDSI__OUT_SEL1       (* (reg8 *) DB7__PRTDSI__OUT_SEL1) 
/* Sync Output Enable Registers */
#define DB7_PRTDSI__SYNC_OUT       (* (reg8 *) DB7__PRTDSI__SYNC_OUT) 

/* SIO registers */
#if defined(DB7__SIO_CFG)
    #define DB7_SIO_HYST_EN        (* (reg8 *) DB7__SIO_HYST_EN)
    #define DB7_SIO_REG_HIFREQ     (* (reg8 *) DB7__SIO_REG_HIFREQ)
    #define DB7_SIO_CFG            (* (reg8 *) DB7__SIO_CFG)
    #define DB7_SIO_DIFF           (* (reg8 *) DB7__SIO_DIFF)
#endif /* (DB7__SIO_CFG) */

/* Interrupt Registers */
#if defined(DB7__INTSTAT)
    #define DB7_INTSTAT            (* (reg8 *) DB7__INTSTAT)
    #define DB7_SNAP               (* (reg8 *) DB7__SNAP)
    
	#define DB7_0_INTTYPE_REG 		(* (reg8 *) DB7__0__INTTYPE)
#endif /* (DB7__INTSTAT) */

#endif /* CY_PSOC5A... */

#endif /*  CY_PINS_DB7_H */


/* [] END OF FILE */
