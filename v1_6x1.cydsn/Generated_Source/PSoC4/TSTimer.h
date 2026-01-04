/*******************************************************************************
* File Name: TSTimer.h
* Version 2.80
*
*  Description:
*     Contains the function prototypes and constants available to the timer
*     user module.
*
*   Note:
*     None
*
********************************************************************************
* Copyright 2008-2017, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
********************************************************************************/

#if !defined(CY_TIMER_TSTimer_H)
#define CY_TIMER_TSTimer_H

#include "cytypes.h"
#include "cyfitter.h"
#include "CyLib.h" /* For CyEnterCriticalSection() and CyExitCriticalSection() functions */

extern uint8 TSTimer_initVar;

/* Check to see if required defines such as CY_PSOC5LP are available */
/* They are defined starting with cy_boot v3.0 */
#if !defined (CY_PSOC5LP)
    #error Component Timer_v2_80 requires cy_boot v3.0 or later
#endif /* (CY_ PSOC5LP) */


/**************************************
*           Parameter Defaults
**************************************/

#define TSTimer_Resolution                 16u
#define TSTimer_UsingFixedFunction         0u
#define TSTimer_UsingHWCaptureCounter      0u
#define TSTimer_SoftwareCaptureMode        0u
#define TSTimer_SoftwareTriggerMode        0u
#define TSTimer_UsingHWEnable              0u
#define TSTimer_EnableTriggerMode          0u
#define TSTimer_InterruptOnCaptureCount    0u
#define TSTimer_RunModeUsed                0u
#define TSTimer_ControlRegRemoved          0u

#if defined(TSTimer_TimerUDB_sCTRLReg_SyncCtl_ctrlreg__CONTROL_REG)
    #define TSTimer_UDB_CONTROL_REG_REMOVED            (0u)
#elif  (TSTimer_UsingFixedFunction)
    #define TSTimer_UDB_CONTROL_REG_REMOVED            (0u)
#else 
    #define TSTimer_UDB_CONTROL_REG_REMOVED            (1u)
#endif /* End TSTimer_TimerUDB_sCTRLReg_SyncCtl_ctrlreg__CONTROL_REG */


/***************************************
*       Type defines
***************************************/


/**************************************************************************
 * Sleep Wakeup Backup structure for Timer Component
 *************************************************************************/
typedef struct
{
    uint8 TimerEnableState;
    #if(!TSTimer_UsingFixedFunction)

        uint16 TimerUdb;
        uint8 InterruptMaskValue;
        #if (TSTimer_UsingHWCaptureCounter)
            uint8 TimerCaptureCounter;
        #endif /* variable declarations for backing up non retention registers in CY_UDB_V1 */

        #if (!TSTimer_UDB_CONTROL_REG_REMOVED)
            uint8 TimerControlRegister;
        #endif /* variable declaration for backing up enable state of the Timer */
    #endif /* define backup variables only for UDB implementation. Fixed function registers are all retention */

}TSTimer_backupStruct;


/***************************************
*       Function Prototypes
***************************************/

void    TSTimer_Start(void) ;
void    TSTimer_Stop(void) ;

void    TSTimer_SetInterruptMode(uint8 interruptMode) ;
uint8   TSTimer_ReadStatusRegister(void) ;
/* Deprecated function. Do not use this in future. Retained for backward compatibility */
#define TSTimer_GetInterruptSource() TSTimer_ReadStatusRegister()

#if(!TSTimer_UDB_CONTROL_REG_REMOVED)
    uint8   TSTimer_ReadControlRegister(void) ;
    void    TSTimer_WriteControlRegister(uint8 control) ;
#endif /* (!TSTimer_UDB_CONTROL_REG_REMOVED) */

uint16  TSTimer_ReadPeriod(void) ;
void    TSTimer_WritePeriod(uint16 period) ;
uint16  TSTimer_ReadCounter(void) ;
void    TSTimer_WriteCounter(uint16 counter) ;
uint16  TSTimer_ReadCapture(void) ;
void    TSTimer_SoftwareCapture(void) ;

#if(!TSTimer_UsingFixedFunction) /* UDB Prototypes */
    #if (TSTimer_SoftwareCaptureMode)
        void    TSTimer_SetCaptureMode(uint8 captureMode) ;
    #endif /* (!TSTimer_UsingFixedFunction) */

    #if (TSTimer_SoftwareTriggerMode)
        void    TSTimer_SetTriggerMode(uint8 triggerMode) ;
    #endif /* (TSTimer_SoftwareTriggerMode) */

    #if (TSTimer_EnableTriggerMode)
        void    TSTimer_EnableTrigger(void) ;
        void    TSTimer_DisableTrigger(void) ;
    #endif /* (TSTimer_EnableTriggerMode) */


    #if(TSTimer_InterruptOnCaptureCount)
        void    TSTimer_SetInterruptCount(uint8 interruptCount) ;
    #endif /* (TSTimer_InterruptOnCaptureCount) */

    #if (TSTimer_UsingHWCaptureCounter)
        void    TSTimer_SetCaptureCount(uint8 captureCount) ;
        uint8   TSTimer_ReadCaptureCount(void) ;
    #endif /* (TSTimer_UsingHWCaptureCounter) */

    void TSTimer_ClearFIFO(void) ;
#endif /* UDB Prototypes */

/* Sleep Retention APIs */
void TSTimer_Init(void)          ;
void TSTimer_Enable(void)        ;
void TSTimer_SaveConfig(void)    ;
void TSTimer_RestoreConfig(void) ;
void TSTimer_Sleep(void)         ;
void TSTimer_Wakeup(void)        ;


/***************************************
*   Enumerated Types and Parameters
***************************************/

/* Enumerated Type B_Timer__CaptureModes, Used in Capture Mode */
#define TSTimer__B_TIMER__CM_NONE 0
#define TSTimer__B_TIMER__CM_RISINGEDGE 1
#define TSTimer__B_TIMER__CM_FALLINGEDGE 2
#define TSTimer__B_TIMER__CM_EITHEREDGE 3
#define TSTimer__B_TIMER__CM_SOFTWARE 4



/* Enumerated Type B_Timer__TriggerModes, Used in Trigger Mode */
#define TSTimer__B_TIMER__TM_NONE 0x00u
#define TSTimer__B_TIMER__TM_RISINGEDGE 0x04u
#define TSTimer__B_TIMER__TM_FALLINGEDGE 0x08u
#define TSTimer__B_TIMER__TM_EITHEREDGE 0x0Cu
#define TSTimer__B_TIMER__TM_SOFTWARE 0x10u


/***************************************
*    Initialial Parameter Constants
***************************************/

#define TSTimer_INIT_PERIOD             199u
#define TSTimer_INIT_CAPTURE_MODE       ((uint8)((uint8)0u << TSTimer_CTRL_CAP_MODE_SHIFT))
#define TSTimer_INIT_TRIGGER_MODE       ((uint8)((uint8)0u << TSTimer_CTRL_TRIG_MODE_SHIFT))
#if (TSTimer_UsingFixedFunction)
    #define TSTimer_INIT_INTERRUPT_MODE (((uint8)((uint8)0u << TSTimer_STATUS_TC_INT_MASK_SHIFT)) | \
                                                  ((uint8)((uint8)0 << TSTimer_STATUS_CAPTURE_INT_MASK_SHIFT)))
#else
    #define TSTimer_INIT_INTERRUPT_MODE (((uint8)((uint8)0u << TSTimer_STATUS_TC_INT_MASK_SHIFT)) | \
                                                 ((uint8)((uint8)0 << TSTimer_STATUS_CAPTURE_INT_MASK_SHIFT)) | \
                                                 ((uint8)((uint8)0 << TSTimer_STATUS_FIFOFULL_INT_MASK_SHIFT)))
#endif /* (TSTimer_UsingFixedFunction) */
#define TSTimer_INIT_CAPTURE_COUNT      (2u)
#define TSTimer_INIT_INT_CAPTURE_COUNT  ((uint8)((uint8)(1u - 1u) << TSTimer_CTRL_INTCNT_SHIFT))


/***************************************
*           Registers
***************************************/

#if (TSTimer_UsingFixedFunction) /* Implementation Specific Registers and Register Constants */


    /***************************************
    *    Fixed Function Registers
    ***************************************/

    #define TSTimer_STATUS         (*(reg8 *) TSTimer_TimerHW__SR0 )
    /* In Fixed Function Block Status and Mask are the same register */
    #define TSTimer_STATUS_MASK    (*(reg8 *) TSTimer_TimerHW__SR0 )
    #define TSTimer_CONTROL        (*(reg8 *) TSTimer_TimerHW__CFG0)
    #define TSTimer_CONTROL2       (*(reg8 *) TSTimer_TimerHW__CFG1)
    #define TSTimer_CONTROL2_PTR   ( (reg8 *) TSTimer_TimerHW__CFG1)
    #define TSTimer_RT1            (*(reg8 *) TSTimer_TimerHW__RT1)
    #define TSTimer_RT1_PTR        ( (reg8 *) TSTimer_TimerHW__RT1)

    #if (CY_PSOC3 || CY_PSOC5LP)
        #define TSTimer_CONTROL3       (*(reg8 *) TSTimer_TimerHW__CFG2)
        #define TSTimer_CONTROL3_PTR   ( (reg8 *) TSTimer_TimerHW__CFG2)
    #endif /* (CY_PSOC3 || CY_PSOC5LP) */
    #define TSTimer_GLOBAL_ENABLE  (*(reg8 *) TSTimer_TimerHW__PM_ACT_CFG)
    #define TSTimer_GLOBAL_STBY_ENABLE  (*(reg8 *) TSTimer_TimerHW__PM_STBY_CFG)

    #define TSTimer_CAPTURE_LSB         (* (reg16 *) TSTimer_TimerHW__CAP0 )
    #define TSTimer_CAPTURE_LSB_PTR       ((reg16 *) TSTimer_TimerHW__CAP0 )
    #define TSTimer_PERIOD_LSB          (* (reg16 *) TSTimer_TimerHW__PER0 )
    #define TSTimer_PERIOD_LSB_PTR        ((reg16 *) TSTimer_TimerHW__PER0 )
    #define TSTimer_COUNTER_LSB         (* (reg16 *) TSTimer_TimerHW__CNT_CMP0 )
    #define TSTimer_COUNTER_LSB_PTR       ((reg16 *) TSTimer_TimerHW__CNT_CMP0 )


    /***************************************
    *    Register Constants
    ***************************************/

    /* Fixed Function Block Chosen */
    #define TSTimer_BLOCK_EN_MASK                     TSTimer_TimerHW__PM_ACT_MSK
    #define TSTimer_BLOCK_STBY_EN_MASK                TSTimer_TimerHW__PM_STBY_MSK

    /* Control Register Bit Locations */
    /* Interrupt Count - Not valid for Fixed Function Block */
    #define TSTimer_CTRL_INTCNT_SHIFT                  0x00u
    /* Trigger Polarity - Not valid for Fixed Function Block */
    #define TSTimer_CTRL_TRIG_MODE_SHIFT               0x00u
    /* Trigger Enable - Not valid for Fixed Function Block */
    #define TSTimer_CTRL_TRIG_EN_SHIFT                 0x00u
    /* Capture Polarity - Not valid for Fixed Function Block */
    #define TSTimer_CTRL_CAP_MODE_SHIFT                0x00u
    /* Timer Enable - As defined in Register Map, part of TMRX_CFG0 register */
    #define TSTimer_CTRL_ENABLE_SHIFT                  0x00u

    /* Control Register Bit Masks */
    #define TSTimer_CTRL_ENABLE                        ((uint8)((uint8)0x01u << TSTimer_CTRL_ENABLE_SHIFT))

    /* Control2 Register Bit Masks */
    /* As defined in Register Map, Part of the TMRX_CFG1 register */
    #define TSTimer_CTRL2_IRQ_SEL_SHIFT                 0x00u
    #define TSTimer_CTRL2_IRQ_SEL                      ((uint8)((uint8)0x01u << TSTimer_CTRL2_IRQ_SEL_SHIFT))

    #if (CY_PSOC5A)
        /* Use CFG1 Mode bits to set run mode */
        /* As defined by Verilog Implementation */
        #define TSTimer_CTRL_MODE_SHIFT                 0x01u
        #define TSTimer_CTRL_MODE_MASK                 ((uint8)((uint8)0x07u << TSTimer_CTRL_MODE_SHIFT))
    #endif /* (CY_PSOC5A) */
    #if (CY_PSOC3 || CY_PSOC5LP)
        /* Control3 Register Bit Locations */
        #define TSTimer_CTRL_RCOD_SHIFT        0x02u
        #define TSTimer_CTRL_ENBL_SHIFT        0x00u
        #define TSTimer_CTRL_MODE_SHIFT        0x00u

        /* Control3 Register Bit Masks */
        #define TSTimer_CTRL_RCOD_MASK  ((uint8)((uint8)0x03u << TSTimer_CTRL_RCOD_SHIFT)) /* ROD and COD bit masks */
        #define TSTimer_CTRL_ENBL_MASK  ((uint8)((uint8)0x80u << TSTimer_CTRL_ENBL_SHIFT)) /* HW_EN bit mask */
        #define TSTimer_CTRL_MODE_MASK  ((uint8)((uint8)0x03u << TSTimer_CTRL_MODE_SHIFT)) /* Run mode bit mask */

        #define TSTimer_CTRL_RCOD       ((uint8)((uint8)0x03u << TSTimer_CTRL_RCOD_SHIFT))
        #define TSTimer_CTRL_ENBL       ((uint8)((uint8)0x80u << TSTimer_CTRL_ENBL_SHIFT))
    #endif /* (CY_PSOC3 || CY_PSOC5LP) */

    /*RT1 Synch Constants: Applicable for PSoC3 and PSoC5LP */
    #define TSTimer_RT1_SHIFT                       0x04u
    /* Sync TC and CMP bit masks */
    #define TSTimer_RT1_MASK                        ((uint8)((uint8)0x03u << TSTimer_RT1_SHIFT))
    #define TSTimer_SYNC                            ((uint8)((uint8)0x03u << TSTimer_RT1_SHIFT))
    #define TSTimer_SYNCDSI_SHIFT                   0x00u
    /* Sync all DSI inputs with Mask  */
    #define TSTimer_SYNCDSI_MASK                    ((uint8)((uint8)0x0Fu << TSTimer_SYNCDSI_SHIFT))
    /* Sync all DSI inputs */
    #define TSTimer_SYNCDSI_EN                      ((uint8)((uint8)0x0Fu << TSTimer_SYNCDSI_SHIFT))

    #define TSTimer_CTRL_MODE_PULSEWIDTH            ((uint8)((uint8)0x01u << TSTimer_CTRL_MODE_SHIFT))
    #define TSTimer_CTRL_MODE_PERIOD                ((uint8)((uint8)0x02u << TSTimer_CTRL_MODE_SHIFT))
    #define TSTimer_CTRL_MODE_CONTINUOUS            ((uint8)((uint8)0x00u << TSTimer_CTRL_MODE_SHIFT))

    /* Status Register Bit Locations */
    /* As defined in Register Map, part of TMRX_SR0 register */
    #define TSTimer_STATUS_TC_SHIFT                 0x07u
    /* As defined in Register Map, part of TMRX_SR0 register, Shared with Compare Status */
    #define TSTimer_STATUS_CAPTURE_SHIFT            0x06u
    /* As defined in Register Map, part of TMRX_SR0 register */
    #define TSTimer_STATUS_TC_INT_MASK_SHIFT        (TSTimer_STATUS_TC_SHIFT - 0x04u)
    /* As defined in Register Map, part of TMRX_SR0 register, Shared with Compare Status */
    #define TSTimer_STATUS_CAPTURE_INT_MASK_SHIFT   (TSTimer_STATUS_CAPTURE_SHIFT - 0x04u)

    /* Status Register Bit Masks */
    #define TSTimer_STATUS_TC                       ((uint8)((uint8)0x01u << TSTimer_STATUS_TC_SHIFT))
    #define TSTimer_STATUS_CAPTURE                  ((uint8)((uint8)0x01u << TSTimer_STATUS_CAPTURE_SHIFT))
    /* Interrupt Enable Bit-Mask for interrupt on TC */
    #define TSTimer_STATUS_TC_INT_MASK              ((uint8)((uint8)0x01u << TSTimer_STATUS_TC_INT_MASK_SHIFT))
    /* Interrupt Enable Bit-Mask for interrupt on Capture */
    #define TSTimer_STATUS_CAPTURE_INT_MASK         ((uint8)((uint8)0x01u << TSTimer_STATUS_CAPTURE_INT_MASK_SHIFT))

#else   /* UDB Registers and Register Constants */


    /***************************************
    *           UDB Registers
    ***************************************/

    #define TSTimer_STATUS              (* (reg8 *) TSTimer_TimerUDB_rstSts_stsreg__STATUS_REG )
    #define TSTimer_STATUS_MASK         (* (reg8 *) TSTimer_TimerUDB_rstSts_stsreg__MASK_REG)
    #define TSTimer_STATUS_AUX_CTRL     (* (reg8 *) TSTimer_TimerUDB_rstSts_stsreg__STATUS_AUX_CTL_REG)
    #define TSTimer_CONTROL             (* (reg8 *) TSTimer_TimerUDB_sCTRLReg_SyncCtl_ctrlreg__CONTROL_REG )
    
    #if(TSTimer_Resolution <= 8u) /* 8-bit Timer */
        #define TSTimer_CAPTURE_LSB         (* (reg8 *) TSTimer_TimerUDB_sT16_timerdp_u0__F0_REG )
        #define TSTimer_CAPTURE_LSB_PTR       ((reg8 *) TSTimer_TimerUDB_sT16_timerdp_u0__F0_REG )
        #define TSTimer_PERIOD_LSB          (* (reg8 *) TSTimer_TimerUDB_sT16_timerdp_u0__D0_REG )
        #define TSTimer_PERIOD_LSB_PTR        ((reg8 *) TSTimer_TimerUDB_sT16_timerdp_u0__D0_REG )
        #define TSTimer_COUNTER_LSB         (* (reg8 *) TSTimer_TimerUDB_sT16_timerdp_u0__A0_REG )
        #define TSTimer_COUNTER_LSB_PTR       ((reg8 *) TSTimer_TimerUDB_sT16_timerdp_u0__A0_REG )
    #elif(TSTimer_Resolution <= 16u) /* 8-bit Timer */
        #if(CY_PSOC3) /* 8-bit addres space */
            #define TSTimer_CAPTURE_LSB         (* (reg16 *) TSTimer_TimerUDB_sT16_timerdp_u0__F0_REG )
            #define TSTimer_CAPTURE_LSB_PTR       ((reg16 *) TSTimer_TimerUDB_sT16_timerdp_u0__F0_REG )
            #define TSTimer_PERIOD_LSB          (* (reg16 *) TSTimer_TimerUDB_sT16_timerdp_u0__D0_REG )
            #define TSTimer_PERIOD_LSB_PTR        ((reg16 *) TSTimer_TimerUDB_sT16_timerdp_u0__D0_REG )
            #define TSTimer_COUNTER_LSB         (* (reg16 *) TSTimer_TimerUDB_sT16_timerdp_u0__A0_REG )
            #define TSTimer_COUNTER_LSB_PTR       ((reg16 *) TSTimer_TimerUDB_sT16_timerdp_u0__A0_REG )
        #else /* 16-bit address space */
            #define TSTimer_CAPTURE_LSB         (* (reg16 *) TSTimer_TimerUDB_sT16_timerdp_u0__16BIT_F0_REG )
            #define TSTimer_CAPTURE_LSB_PTR       ((reg16 *) TSTimer_TimerUDB_sT16_timerdp_u0__16BIT_F0_REG )
            #define TSTimer_PERIOD_LSB          (* (reg16 *) TSTimer_TimerUDB_sT16_timerdp_u0__16BIT_D0_REG )
            #define TSTimer_PERIOD_LSB_PTR        ((reg16 *) TSTimer_TimerUDB_sT16_timerdp_u0__16BIT_D0_REG )
            #define TSTimer_COUNTER_LSB         (* (reg16 *) TSTimer_TimerUDB_sT16_timerdp_u0__16BIT_A0_REG )
            #define TSTimer_COUNTER_LSB_PTR       ((reg16 *) TSTimer_TimerUDB_sT16_timerdp_u0__16BIT_A0_REG )
        #endif /* CY_PSOC3 */
    #elif(TSTimer_Resolution <= 24u)/* 24-bit Timer */
        #define TSTimer_CAPTURE_LSB         (* (reg32 *) TSTimer_TimerUDB_sT16_timerdp_u0__F0_REG )
        #define TSTimer_CAPTURE_LSB_PTR       ((reg32 *) TSTimer_TimerUDB_sT16_timerdp_u0__F0_REG )
        #define TSTimer_PERIOD_LSB          (* (reg32 *) TSTimer_TimerUDB_sT16_timerdp_u0__D0_REG )
        #define TSTimer_PERIOD_LSB_PTR        ((reg32 *) TSTimer_TimerUDB_sT16_timerdp_u0__D0_REG )
        #define TSTimer_COUNTER_LSB         (* (reg32 *) TSTimer_TimerUDB_sT16_timerdp_u0__A0_REG )
        #define TSTimer_COUNTER_LSB_PTR       ((reg32 *) TSTimer_TimerUDB_sT16_timerdp_u0__A0_REG )
    #else /* 32-bit Timer */
        #if(CY_PSOC3 || CY_PSOC5) /* 8-bit address space */
            #define TSTimer_CAPTURE_LSB         (* (reg32 *) TSTimer_TimerUDB_sT16_timerdp_u0__F0_REG )
            #define TSTimer_CAPTURE_LSB_PTR       ((reg32 *) TSTimer_TimerUDB_sT16_timerdp_u0__F0_REG )
            #define TSTimer_PERIOD_LSB          (* (reg32 *) TSTimer_TimerUDB_sT16_timerdp_u0__D0_REG )
            #define TSTimer_PERIOD_LSB_PTR        ((reg32 *) TSTimer_TimerUDB_sT16_timerdp_u0__D0_REG )
            #define TSTimer_COUNTER_LSB         (* (reg32 *) TSTimer_TimerUDB_sT16_timerdp_u0__A0_REG )
            #define TSTimer_COUNTER_LSB_PTR       ((reg32 *) TSTimer_TimerUDB_sT16_timerdp_u0__A0_REG )
        #else /* 32-bit address space */
            #define TSTimer_CAPTURE_LSB         (* (reg32 *) TSTimer_TimerUDB_sT16_timerdp_u0__32BIT_F0_REG )
            #define TSTimer_CAPTURE_LSB_PTR       ((reg32 *) TSTimer_TimerUDB_sT16_timerdp_u0__32BIT_F0_REG )
            #define TSTimer_PERIOD_LSB          (* (reg32 *) TSTimer_TimerUDB_sT16_timerdp_u0__32BIT_D0_REG )
            #define TSTimer_PERIOD_LSB_PTR        ((reg32 *) TSTimer_TimerUDB_sT16_timerdp_u0__32BIT_D0_REG )
            #define TSTimer_COUNTER_LSB         (* (reg32 *) TSTimer_TimerUDB_sT16_timerdp_u0__32BIT_A0_REG )
            #define TSTimer_COUNTER_LSB_PTR       ((reg32 *) TSTimer_TimerUDB_sT16_timerdp_u0__32BIT_A0_REG )
        #endif /* CY_PSOC3 || CY_PSOC5 */ 
    #endif

    #define TSTimer_COUNTER_LSB_PTR_8BIT       ((reg8 *) TSTimer_TimerUDB_sT16_timerdp_u0__A0_REG )
    
    #if (TSTimer_UsingHWCaptureCounter)
        #define TSTimer_CAP_COUNT              (*(reg8 *) TSTimer_TimerUDB_sCapCount_counter__PERIOD_REG )
        #define TSTimer_CAP_COUNT_PTR          ( (reg8 *) TSTimer_TimerUDB_sCapCount_counter__PERIOD_REG )
        #define TSTimer_CAPTURE_COUNT_CTRL     (*(reg8 *) TSTimer_TimerUDB_sCapCount_counter__CONTROL_AUX_CTL_REG )
        #define TSTimer_CAPTURE_COUNT_CTRL_PTR ( (reg8 *) TSTimer_TimerUDB_sCapCount_counter__CONTROL_AUX_CTL_REG )
    #endif /* (TSTimer_UsingHWCaptureCounter) */


    /***************************************
    *       Register Constants
    ***************************************/

    /* Control Register Bit Locations */
    #define TSTimer_CTRL_INTCNT_SHIFT              0x00u       /* As defined by Verilog Implementation */
    #define TSTimer_CTRL_TRIG_MODE_SHIFT           0x02u       /* As defined by Verilog Implementation */
    #define TSTimer_CTRL_TRIG_EN_SHIFT             0x04u       /* As defined by Verilog Implementation */
    #define TSTimer_CTRL_CAP_MODE_SHIFT            0x05u       /* As defined by Verilog Implementation */
    #define TSTimer_CTRL_ENABLE_SHIFT              0x07u       /* As defined by Verilog Implementation */

    /* Control Register Bit Masks */
    #define TSTimer_CTRL_INTCNT_MASK               ((uint8)((uint8)0x03u << TSTimer_CTRL_INTCNT_SHIFT))
    #define TSTimer_CTRL_TRIG_MODE_MASK            ((uint8)((uint8)0x03u << TSTimer_CTRL_TRIG_MODE_SHIFT))
    #define TSTimer_CTRL_TRIG_EN                   ((uint8)((uint8)0x01u << TSTimer_CTRL_TRIG_EN_SHIFT))
    #define TSTimer_CTRL_CAP_MODE_MASK             ((uint8)((uint8)0x03u << TSTimer_CTRL_CAP_MODE_SHIFT))
    #define TSTimer_CTRL_ENABLE                    ((uint8)((uint8)0x01u << TSTimer_CTRL_ENABLE_SHIFT))

    /* Bit Counter (7-bit) Control Register Bit Definitions */
    /* As defined by the Register map for the AUX Control Register */
    #define TSTimer_CNTR_ENABLE                    0x20u

    /* Status Register Bit Locations */
    #define TSTimer_STATUS_TC_SHIFT                0x00u  /* As defined by Verilog Implementation */
    #define TSTimer_STATUS_CAPTURE_SHIFT           0x01u  /* As defined by Verilog Implementation */
    #define TSTimer_STATUS_TC_INT_MASK_SHIFT       TSTimer_STATUS_TC_SHIFT
    #define TSTimer_STATUS_CAPTURE_INT_MASK_SHIFT  TSTimer_STATUS_CAPTURE_SHIFT
    #define TSTimer_STATUS_FIFOFULL_SHIFT          0x02u  /* As defined by Verilog Implementation */
    #define TSTimer_STATUS_FIFONEMP_SHIFT          0x03u  /* As defined by Verilog Implementation */
    #define TSTimer_STATUS_FIFOFULL_INT_MASK_SHIFT TSTimer_STATUS_FIFOFULL_SHIFT

    /* Status Register Bit Masks */
    /* Sticky TC Event Bit-Mask */
    #define TSTimer_STATUS_TC                      ((uint8)((uint8)0x01u << TSTimer_STATUS_TC_SHIFT))
    /* Sticky Capture Event Bit-Mask */
    #define TSTimer_STATUS_CAPTURE                 ((uint8)((uint8)0x01u << TSTimer_STATUS_CAPTURE_SHIFT))
    /* Interrupt Enable Bit-Mask */
    #define TSTimer_STATUS_TC_INT_MASK             ((uint8)((uint8)0x01u << TSTimer_STATUS_TC_SHIFT))
    /* Interrupt Enable Bit-Mask */
    #define TSTimer_STATUS_CAPTURE_INT_MASK        ((uint8)((uint8)0x01u << TSTimer_STATUS_CAPTURE_SHIFT))
    /* NOT-Sticky FIFO Full Bit-Mask */
    #define TSTimer_STATUS_FIFOFULL                ((uint8)((uint8)0x01u << TSTimer_STATUS_FIFOFULL_SHIFT))
    /* NOT-Sticky FIFO Not Empty Bit-Mask */
    #define TSTimer_STATUS_FIFONEMP                ((uint8)((uint8)0x01u << TSTimer_STATUS_FIFONEMP_SHIFT))
    /* Interrupt Enable Bit-Mask */
    #define TSTimer_STATUS_FIFOFULL_INT_MASK       ((uint8)((uint8)0x01u << TSTimer_STATUS_FIFOFULL_SHIFT))

    #define TSTimer_STATUS_ACTL_INT_EN             0x10u   /* As defined for the ACTL Register */

    /* Datapath Auxillary Control Register definitions */
    #define TSTimer_AUX_CTRL_FIFO0_CLR             0x01u   /* As defined by Register map */
    #define TSTimer_AUX_CTRL_FIFO1_CLR             0x02u   /* As defined by Register map */
    #define TSTimer_AUX_CTRL_FIFO0_LVL             0x04u   /* As defined by Register map */
    #define TSTimer_AUX_CTRL_FIFO1_LVL             0x08u   /* As defined by Register map */
    #define TSTimer_STATUS_ACTL_INT_EN_MASK        0x10u   /* As defined for the ACTL Register */

#endif /* Implementation Specific Registers and Register Constants */

#endif  /* CY_TIMER_TSTimer_H */


/* [] END OF FILE */
