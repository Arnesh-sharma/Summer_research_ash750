/***************************************************************************//**
* \file CapSense_SelfTest.c
* \version 7.10
*
* \brief
*   This file provides the source code for the Built-In Self-Test library.
*
* \see CapSense v7.10 Datasheet
*
*//*****************************************************************************
* Copyright (2016-2019), Cypress Semiconductor Corporation.
********************************************************************************
* This software is owned by Cypress Semiconductor Corporation (Cypress) and is
* protected by and subject to worldwide patent protection (United States and
* foreign), United States copyright laws and international treaty provisions.
* Cypress hereby grants to licensee a personal, non-exclusive, non-transferable
* license to copy, use, modify, create derivative works of, and compile the
* Cypress Source Code and derivative works for the sole purpose of creating
* custom software in support of licensee product to be used only in conjunction
* with a Cypress integrated circuit as specified in the applicable agreement.
* Any reproduction, modification, translation, compilation, or representation of
* this software except as specified above is prohibited without the express
* written permission of Cypress.
*
* Disclaimer: CYPRESS MAKES NO WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, WITH
* REGARD TO THIS MATERIAL, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
* WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
* Cypress reserves the right to make changes without further notice to the
* materials described herein. Cypress does not assume any liability arising out
* of the application or use of any product or circuit described herein. Cypress
* does not authorize its products for use as critical components in life-support
* systems where a malfunction or failure may reasonably be expected to result in
* significant injury to the user. The inclusion of Cypress' product in a life-
* support systems application implies that the manufacturer assumes all risk of
* such use and in doing so indemnifies Cypress against all charges. Use may be
* limited by and subject to the applicable Cypress software license agreement.
*******************************************************************************/

#include "cytypes.h"
#include "CyLib.h"
#include "CapSense_Configuration.h"
#include "CapSense_Structure.h"
#include "CapSense_Sensing.h"
#include "CapSense_SelfTest.h"

/***********************************************************************************************************************
* Local definition
***********************************************************************************************************************/
#define CapSense_TST_CRC_GROUP_EN                       (CapSense_SELF_TEST_EN && \
                                                                (CapSense_TST_GLOBAL_CRC_EN || \
                                                                 CapSense_TST_WDGT_CRC_EN))

#define CapSense_TST_BASELINE_GROUP_EN                  (CapSense_SELF_TEST_EN && \
                                                                (CapSense_TST_BSLN_DUPLICATION_EN || \
                                                                 CapSense_TST_BSLN_RAW_OUT_RANGE_EN))

#define CapSense_TST_SHORT_GROUP_EN                     (CapSense_SELF_TEST_EN && \
                                                                (CapSense_TST_SNS_SHORT_EN))

#define CapSense_TST_MEASUREMENT_GROUP_EN               (CapSense_SELF_TEST_EN && \
                                                                (CapSense_TST_SNS_CAP_EN || \
                                                                 CapSense_TST_SH_CAP_EN ||\
                                                                 CapSense_TST_VDDA_EN ||\
                                                                 CapSense_TST_EXTERNAL_CAP_EN))

#define CapSense_TST_HW_GROUP_EN                        (CapSense_TST_SHORT_GROUP_EN ||\
                                                                 CapSense_TST_MEASUREMENT_GROUP_EN)



#define CapSense_TST_SW_WATCHDOG_CYCLES_NUM             (0x0000FFFFLu)
#define CapSense_MAX_16_BITS                            (65535Lu)

/***********************************************************************************************************************
* Definition of IO register modes
***********************************************************************************************************************/

/* Port Select HSIOM Register */
#define CapSense_TST_HSIOM_GPIO                         (0Lu)
#define CapSense_TST_HSIOM_CSD_SENSE                    (4Lu)
#define CapSense_TST_HSIOM_CSD_SHIELD                   (5Lu)
#define CapSense_TST_HSIOM_AMUXA                        (6Lu)
#define CapSense_TST_HSIOM_AMUXB                        (7Lu)

/* Port Configuration Register */
#define CapSense_TST_PC_HIGHZA                          (0Lu)
#define CapSense_TST_PC_PULLUP                          (2Lu)
#define CapSense_TST_PC_PULLDOWN                        (3Lu)
#define CapSense_TST_PC_STRONG                          (6Lu)

/* Port Data Register */
#define CapSense_TST_DR_PIN2GND                         (0Lu)
#define CapSense_TST_DR_PIN2VDD                         (1Lu)


/***************************************************************************//**
* \brief Defines Self-test HW Configuration
*
* \detail
* Defines the HW configuration depended on the test type. It includes the
* HW CSD block configuration, clock configuration, IDAC configuration and
* all electrode configuration.
*
*******************************************************************************/
typedef enum
{
    CapSense_BIST_HW_UNDEFINED_E        = 0x00u,
    CapSense_BIST_HW_SNS_SHORT_E        = 0x01u,
    CapSense_BIST_HW_SNS_CAP_E          = 0x03u,
    CapSense_BIST_HW_SH_CAP_E           = 0x05u,
    CapSense_BIST_HW_EXT_CAP_E          = 0x06u,
    CapSense_BIST_HW_VDDA_E             = 0x07u
} CapSense_BIST_HW_CONFIG_ENUM;


/***************************************************************************//**
* \brief Defines Self-test IO Configuration
*
* \detail
* Defines the IO configuration depends on the test type.
*
*******************************************************************************/
typedef enum
{
    /*
    * The undefined state.
    */
    CapSense_BIST_IO_UNDEFINED_E        = 0x00u,
    /*
    * The drive mode of inactive sensors is set to Strong.
    */
    CapSense_BIST_IO_STRONG_E           = 0x01u,
    /*
    * The drive mode of inactive sensors is set to High-Z.
    */
    CapSense_BIST_IO_HIGHZA_E           = 0x02u,

    /*
    * The drive mode of inactive sensors is set to High-Z.
    * The HSIOM of inactive sensors is set to CSD sense
    * connection.
    */
    CapSense_BIST_IO_SENSE_E           = 0x03u,

    /*
    * The drive mode of inactive sensors is set to High-Z.
    * The HSIOM of inactive sensors is set to CSD shield
    * connection.
    */
    CapSense_BIST_IO_SHIELD_E           = 0x04u,
    /*
    * The default state (all pins disconnected from analog busses)
    */
    CapSense_BIST_IO_DEFAULT_E          = 0x05u,
} CapSense_BIST_IO_CONFIG_ENUM;


/***********************************************************************************************************************
* Local variables
***********************************************************************************************************************/
#if (CapSense_ENABLE == CapSense_TST_HW_GROUP_EN)
    static CapSense_BIST_HW_CONFIG_ENUM CapSense_bistHwConfig = CapSense_BIST_HW_UNDEFINED_E;
    static CapSense_BIST_IO_CONFIG_ENUM CapSense_bistCurrentISC = CapSense_BIST_IO_UNDEFINED_E;
#endif /* (CapSense_ENABLE == CapSense_TST_HW_GROUP_EN) */

#if (CapSense_ENABLE == CapSense_TST_MEASUREMENT_GROUP_EN)
    static volatile uint16 CapSense_bistRawcount;
    static volatile uint32 CapSense_bistConfigCsd = CapSense_BIST_CSD_CONFIG;
    CapSense_TST_CP_CAP_IDAC_TABLE_STRUCT const CapSense_BistIdacTable[] = CapSense_TST_CP_CAP_IDAC_TABLE_INIT;

    #if (CapSense_ENABLE != CapSense_CSDV2)
        static volatile uint32 CapSense_bistCounterResolution = CapSense_BIST_SCAN_DURATION;
    #endif /* (CapSense_ENABLE == CapSense_CSDV2) */
#endif /* (CapSense_ENABLE == CapSense_TST_MEASUREMENT_GROUP_EN) */


/***********************************************************************************************************************
* Static function prototypes
***********************************************************************************************************************/

/**

* \cond SECTION_C_INTERNAL
* \addtogroup group_c_internal
* \{
*/

/* CRC group function prototypes */
#if (CapSense_ENABLE == CapSense_TST_WDGT_CRC_EN)
    static uint32 CapSense_CheckAllWidgetCRC(void);
#endif

#if (CapSense_ENABLE == CapSense_TST_CRC_GROUP_EN)
    static uint16 CapSense_DsCalculateCrc16(uint8 *ptrData, uint32 len);
#endif

/* Baseline group function prototypes */
#if (CapSense_ENABLE == CapSense_TST_BSLN_DUPLICATION_EN)
    static uint32 CapSense_CheckAllBaselineDuplication(void);
#endif

#if (CapSense_ENABLE == CapSense_TST_HW_GROUP_EN)

    #if (((CapSense_ENABLE == CapSense_CSD_EN) || (CapSense_ENABLE == CapSense_CSX_EN)) &&\
         (CapSense_ENABLE == CapSense_TST_SNS_SHORT_EN))
        static uint32 CapSense_SnsShortCheckAllSensors(void);
        static void CapSense_SnsShortUpdateTestResult(uint32 wdgtId, uint32 snsId);
        static uint32 CapSense_SnsShortCheckSensor(uint32 widgetId, uint32 sensorId, uint32 mode);
        static uint32 CapSense_SnsShortCheckElectrode(uint32 widgetId, uint32 ioSnsId, uint32 mode);
        static uint32 CapSense_SnsShortGetElectrdeList(uint32 widgetId, uint32 ioSnsId, CapSense_FLASH_IO_STRUCT const **ioPtr);
        static void CapSense_BistEnableHwSnsShort(void);
        static void CapSense_BistDisableHwSnsShort(void);
        static void CapSense_SnsShortSetDrExtCapShield(uint32 value);
        static uint32 CapSense_SnsShortReadElectrodeState(CapSense_FLASH_IO_STRUCT const *ioPtr);
    #endif /* (((CapSense_ENABLE == CapSense_CSD_EN) || (CapSense_ENABLE == CapSense_CSX_EN)) &&\
               (CapSense_ENABLE == CapSense_TST_SNS_SHORT_EN)) */

    #if (CapSense_ENABLE == CapSense_TST_MEASUREMENT_GROUP_EN)
        #if ((CapSense_ENABLE == CapSense_TST_SNS_CAP_EN) || \
             (CapSense_ENABLE == CapSense_TST_SH_CAP_EN)  ||\
             (CapSense_ENABLE == CapSense_TST_EXTERNAL_CAP_EN))
            static void CapSense_BistEnableHwSnsCap(void);
            static void CapSense_BistSetScanDuration(void);
            CY_ISR(CapSense_BistPostSingleScan);
            static void CapSense_BistStartSample(void);
            static void CapSense_BistConfigClock(uint32 snsClk);
            static void CapSense_BistSetIdac(uint32 idacValue);
            static void CapSense_BistCmodPrecharge(void);
            static void CapSense_WaitEndOfScan(uint32 timeout);
            static void CapSense_BistTriggerScan(void);

            #if (((CapSense_ENABLE == CapSense_CSD_EN) || (CapSense_ENABLE == CapSense_CSX_EN)) &&\
                 ((CapSense_ENABLE == CapSense_TST_SNS_CAP_EN) || (CapSense_ENABLE == CapSense_TST_SH_CAP_EN)))

                #if (CapSense_ENABLE == CapSense_TST_SNS_CAP_EN)
                    #if ((CapSense_ENABLE == CapSense_CSD_SHIELD_EN) &&\
                         (CapSense_ENABLE == CapSense_CSD_SHIELD_TANK_EN))
                        static void CapSense_BistEnableShieldTank(void);
                    #endif /* ((CapSense_ENABLE == CapSense_CSD_SHIELD_EN) &&\
                               (CapSense_ENABLE == CapSense_CSD_SHIELD_TANK_EN)) */
                    static uint32 CapSense_GetSensorCapacitanceExt(uint32 wdgtId,
                                                                   uint32 sensorElement,
                                                                   CapSense_TST_MEASUREMENT_STATUS_ENUM * measurementStatusPtr);
                    static void CapSense_BistConnectSensor(uint32 wdgtId, uint32 sensorElement);
                    static uint32 CapSense_MeasureAllSensorCapacitance(void);
                    static void CapSense_BistSetInactiveSensorConnection(uint32 wdgtId, uint32 sensorElement);
                #endif /* (CapSense_ENABLE == CapSense_TST_SNS_CAP_EN) */


                #if (CapSense_ENABLE == CapSense_TST_SH_CAP_EN)
                    #if (0u != CapSense_CSD_TOTAL_SHIELD_COUNT)
                        static void CapSense_SwitchAllShieldsState(CapSense_BIST_IO_CONFIG_ENUM config);
                    #endif /* (0u != CapSense_CSD_TOTAL_SHIELD_COUNT) */
                #endif /* (CapSense_ENABLE == CapSense_TST_SH_CAP_EN) */

                static uint32 CapSense_GetCapacitanceExt(CapSense_TST_MEASUREMENT_STATUS_ENUM * snsMeasStatusPtr);
                static void CapSense_BistDisableHwSnsCap(void);
                static void CapSense_BistSetIdacGain(uint32 idacValue, uint32 idacGainIndex);
                static CapSense_BIST_IO_CONFIG_ENUM CapSense_IscToBistIOConfig(uint32 iscState);

                #if (CapSense_ENABLE != CapSense_CSDV2)
                    static void CapSense_BistClockRestart(void);
                #endif /* (CapSense_ENABLE != CapSense_CSDV2) */
            #endif /* (((CapSense_ENABLE == CapSense_CSD_EN) || (CapSense_ENABLE == CapSense_CSX_EN)) &&\
                       ((CapSense_ENABLE == CapSense_TST_SNS_CAP_EN) || (CapSense_ENABLE == CapSense_TST_SH_CAP_EN))) */

            #if (CapSense_ENABLE == CapSense_TST_EXTERNAL_CAP_EN)
                static uint32 CapSense_MeasureExtCapCapacitance(uint32 extCapId);
                static void CapSense_GetExtCapCapacitanceExt(uint32 modClkDivider, uint32 iDacVal);
                static void CapSense_BistConnectExtCapacitor(uint32 extCapId);
                static void CapSense_BistDisconnectExtCapacitor(uint32 extCapId);
                static uint32 CapSense_MeasureAllExtCapCapacitance(void);
            #endif /*(CapSense_ENABLE == CapSense_TST_EXTERNAL_CAP_EN) */

        #endif /* ((CapSense_ENABLE == CapSense_TST_SNS_CAP_EN) || \
                   (CapSense_ENABLE == CapSense_TST_SH_CAP_EN)  ||\
                   (CapSense_ENABLE == CapSense_TST_EXTERNAL_CAP_EN)) */

        #if ((CapSense_ENABLE == CapSense_TST_VDDA_EN) &&\
             (CapSense_ENABLE == CapSense_CSDV2))
            static void CapSense_BistEnableVddaMeasurement(void);
            static uint16 CapSense_BistVddaRunMeasurement(void);
        #endif /* ((CapSense_ENABLE == CapSense_TST_VDDA_EN) &&\
                   (CapSense_ENABLE == CapSense_CSDV2)) */
    #endif /* (CapSense_ENABLE == CapSense_TST_MEASUREMENT_GROUP_EN) */

    static void CapSense_BistSwitchHwConfig(CapSense_BIST_HW_CONFIG_ENUM config);
    static void CapSense_SwitchAllIOState(CapSense_BIST_IO_CONFIG_ENUM config);
    static void CapSense_SetSnsHsiom(CapSense_FLASH_IO_STRUCT const *ioPtr, uint32 value);
    static void CapSense_SetSnsDr(CapSense_FLASH_IO_STRUCT const *ioPtr, uint32 value);
    static void CapSense_SetSnsPc(CapSense_FLASH_IO_STRUCT const *ioPtr, uint32 value);
    static void CapSense_SetAllSnsHsiomState(uint32 value);
    static void CapSense_SetAllSnsDrState(uint32 value);
    static void CapSense_SetAllSnsPcState(uint32 value);


    #if ((CapSense_ENABLE == CapSense_CSD_SHIELD_EN) && \
         (0u != CapSense_CSD_TOTAL_SHIELD_COUNT))
        static void CapSense_BistSetAllShieldsHsiomState(uint32 hsiomValue);
        static void CapSense_BistSetAllShieldsPcState(uint32 pcValue);
    #endif /* ((CapSense_ENABLE == CapSense_CSD_SHIELD_EN) && \
               (0u != CapSense_CSD_TOTAL_SHIELD_COUNT)) */

#endif /* (CapSense_ENABLE == CapSense_TST_HW_GROUP_EN) */

/** \}
* \endcond */

/*******************************************************************************
* Function Name: CapSense_RunSelfTest
****************************************************************************//**
*
* \brief
*  Runs built-in self-tests specified by the test enable mask.
*
* \details
*  The function performs various self-tests on all enabled widgets
*  and sensors in the Component. The required set of tests can be selected
*  using the bit-mask in the testEnMask parameter.
*
*  Use CapSense_TST_RUN_SELF_TEST_MASK to execute all the self-tests or
*  any combination of the masks (defined in testEnMask parameter) to specify the
*  test list.
*
*  To execute a single-element test (i.e. for one widget or sensor), the following
*  functions are available:
*  - CapSense_CheckGlobalCRC()
*  - CapSense_CheckWidgetCRC()
*  - CapSense_CheckBaselineDuplication()
*  - CapSense_CheckIntegritySensorPins()
*  - CapSense_GetSensorCapacitance()
*  - CapSense_GetShieldCapacitance()
*  - CapSense_GetExtCapCapacitance()
*  - CapSense_GetVdda()
*
*  Refer to these functions for detail information on a corresponding test.
*
* \param testEnMask
*  Specifies the tests to be executed. Each bit corresponds to one test. It is
*  possible to launch the function with any combination of the available tests.
*  - CapSense_TST_GLOBAL_CRC       - Verifies the RAM structure CRC of global parameters.
*  - CapSense_TST_WDGT_CRC         - Verifies the RAM widget structure CRC for all the widgets.
*  - CapSense_TST_BSLN_DUPLICATION - Verifies the baseline consistency of all the sensors (inverse copy).
*  - CapSense_TST_SNS_SHORT        - Checks all the sensors for a short to GND / VDD / other sensors.
*  - CapSense_TST_SNS_CAP          - Measures all the sensors capacitance.
*  - CapSense_TST_SH_CAP           - Measures the shield capacitance.
*  - CapSense_TST_EXTERNAL_CAP     - Measures the capacitance of the available external capacitors.
*  - CapSense_TST_VDDA             - Measures the Vdda voltage.
*  - CapSense_TST_RUN_SELF_TEST_MASK - Executes all available tests.
*
* \return
*  Returns a bit-mask with a status of execution of the specified tests:
*  - CYRET_SUCCESS - All the tests are passed.
*  - CapSense_TST_NOT_EXECUTED - The previously triggered scanning is not
*    completed.
*  - CapSense_TST_BAD_PARAM - A non-defined test was requested in the
*    testEnMask parameter.
*  - CapSense_TST_GLOBAL_CRC       - Fails the test of the RAM structure CRC of global parameters.
*  - CapSense_TST_WDGT_CRC         - Fails the test of the RAM widget structure CRC of any widget.
*  - CapSense_TST_BSLN_DUPLICATION - Fails the baseline consistency test of any sensor.
*  - CapSense_TST_SNS_SHORT        - Fails the test of the short to GND or VDD of any sensor.
*  - CapSense_TST_SNS_CAP          - Fails the execution of any sensor capacitance measurement test.
*  - CapSense_TST_SH_CAP           - Fails the execution of the shield capacitance measurement test.
*  - CapSense_TST_EXTERNAL_CAP     - Fails the execution of any external capacitor capacitance measurement test.
*  - CapSense_TST_VDDA             - Fails the execution of the Vdda voltage measurement test.
*
*******************************************************************************/
uint32 CapSense_RunSelfTest(uint32 testEnMask)
{
    #if ((CapSense_ENABLE == CapSense_TST_VDDA_EN) &&\
         (CapSense_ENABLE == CapSense_CSDV2))
        uint32 tmpVal;
    #endif

    #if (CapSense_ENABLE == CapSense_TST_SH_CAP_EN)
        CapSense_TST_MEASUREMENT_STATUS_ENUM measurementStatus;
    #endif

    uint32 result = CYRET_SUCCESS;

    if(CapSense_NOT_BUSY != CapSense_IsBusy())
    {
        result = CapSense_TST_NOT_EXECUTED;
    }
    else
    {
        if (0u == (testEnMask & (~CapSense_TST_RUN_SELF_TEST_MASK)))
        {
            #if (CapSense_ENABLE == CapSense_TST_GLOBAL_CRC_EN)
                if (0u != (CapSense_TST_GLOBAL_CRC & testEnMask))
                {
                    result |= CapSense_CheckGlobalCRC();
                }
            #endif /* (CapSense_ENABLE == CapSense_TST_GLOBAL_CRC_EN) */

            #if (CapSense_ENABLE == CapSense_TST_WDGT_CRC_EN)
                if (0u != (CapSense_TST_WDGT_CRC & testEnMask))
                {
                    result |= CapSense_CheckAllWidgetCRC();
                }
            #endif /* (CapSense_ENABLE == CapSense_TST_WDGT_CRC_EN) */

            #if (CapSense_ENABLE == CapSense_TST_BSLN_DUPLICATION_EN)
                if (0u != (CapSense_TST_BSLN_DUPLICATION & testEnMask))
                {
                    result |= CapSense_CheckAllBaselineDuplication();
                }
            #endif /* (CapSense_ENABLE == CapSense_TST_BSLN_DUPLICATION_EN) */

            /* All the next tests are hardware dependent, so requesting to switch the sense method */
            CapSense_SsSwitchSensingMode(CapSense_SENSE_METHOD_BIST_E);

            #if ((CapSense_ENABLE == CapSense_CSD_EN) ||\
                 (CapSense_ENABLE == CapSense_CSX_EN))

                #if (CapSense_ENABLE == CapSense_TST_SNS_SHORT_EN)
                    if (0u != (CapSense_TST_SNS_SHORT & testEnMask))
                    {
                        result |= CapSense_SnsShortCheckAllSensors();
                    }
                #endif /* (CapSense_ENABLE == CapSense_TST_SNS_SHORT_EN) */

                #if (CapSense_ENABLE == CapSense_TST_SNS_CAP_EN)
                    if (0u != (CapSense_TST_SNS_CAP & testEnMask))
                    {
                        result |= CapSense_MeasureAllSensorCapacitance();
                    }
                #endif /* (CapSense_ENABLE == CapSense_TST_SNS_CAP_EN) */

                #if (CapSense_ENABLE == CapSense_TST_SH_CAP_EN)
                    if (0u != (CapSense_TST_SH_CAP & testEnMask))
                    {
                        (void)CapSense_GetShieldCapacitance(&measurementStatus);
                        if (CapSense_TST_MEASUREMENT_SUCCESS != measurementStatus)
                        {
                            result |= CapSense_TST_SH_CAP;
                        }
                    }
                #endif /* (CapSense_ENABLE == CapSense_TST_SH_CAP_EN) */
            #endif /* ((CapSense_ENABLE == CapSense_CSD_EN) ||\
                       (CapSense_ENABLE == CapSense_CSX_EN)) */

                #if (CapSense_ENABLE == CapSense_TST_EXTERNAL_CAP_EN)
                    if (0u != (CapSense_TST_EXTERNAL_CAP & testEnMask))
                    {
                        result |= CapSense_MeasureAllExtCapCapacitance();
                    }
                #endif /* (CapSense_ENABLE == CapSense_TST_EXTERNAL_CAP_EN) */

                #if ((CapSense_ENABLE == CapSense_TST_VDDA_EN) &&\
                     (CapSense_ENABLE == CapSense_CSDV2))
                    if (0u != (CapSense_TST_VDDA & testEnMask))
                    {
                        tmpVal = CapSense_GetVdda();
                        if (CapSense_TST_VDDA_BAD_RESULT == tmpVal)
                        {
                            result |= CapSense_TST_VDDA;
                        }
                    }
                #endif /* ((CapSense_ENABLE == CapSense_TST_VDDA_EN) &&\
                           (CapSense_ENABLE == CapSense_CSDV2)) */
        }
        else
        {
            result |= CapSense_TST_BAD_PARAM;
        }
    }

    return result;
}


/*******************************************************************************
* Function Name: CapSense_BistDisableMode
****************************************************************************//**
*
* \brief
*  Releases captured HW resources.
*
* \details
*  Releases captured HW resources to be used by other modes.
*
*******************************************************************************/
void CapSense_BistDisableMode(void)
{
    #if (CapSense_ENABLE == CapSense_TST_HW_GROUP_EN)
        CapSense_BistSwitchHwConfig(CapSense_BIST_HW_UNDEFINED_E);

        CapSense_SwitchAllIOState(CapSense_BIST_IO_UNDEFINED_E);
        CapSense_bistCurrentISC = CapSense_BIST_IO_UNDEFINED_E;
    #endif /* (CapSense_ENABLE == CapSense_TST_HW_GROUP_EN) */
}


/*******************************************************************************
* Function Name: CapSense_BistInitialize
****************************************************************************//**
*
* \brief
*  Initialize the HW resources to Built-in Self-test mode.
*
* \details
*  Preserved the function to be consistent with other modes (CSX or CSD).
*  In practice resource capturing and initialization are performed later when
*  defined which test type should be executed. Depends on the test type HW
*  resources initialized differently. Only disconnection of all IO from AMUX
*  is common for all the test types.
*
*******************************************************************************/
void CapSense_BistInitialize(void)
{
    #if (CapSense_ENABLE == CapSense_TST_HW_GROUP_EN)
        CapSense_bistCurrentISC = CapSense_BIST_IO_UNDEFINED_E;

        #if (CapSense_ENABLE == CapSense_CSD_SHIELD_EN)
            CapSense_SetAllSnsHsiomState(CapSense_TST_HSIOM_GPIO);
        #endif
        /* Set data register to GND that is applicable for all the tests */
        CapSense_SetAllSnsDrState(CapSense_TST_DR_PIN2GND);
    #endif /* (CapSense_ENABLE == CapSense_TST_HW_GROUP_EN) */
}


#if (CapSense_ENABLE == CapSense_TST_GLOBAL_CRC_EN)

/*******************************************************************************
* Function Name: CapSense_CheckGlobalCRC
****************************************************************************//**
*
* \brief
*  Checks the stored CRC of the CapSense_RAM_STRUCT data structure.
*
* \details
*  This function validates the data integrity of the
*  CapSense_RAM_STRUCT data structure by calculating the CRC and
*  comparing it with the stored CRC value (i.e. CapSense_GLB_CRC_VALUE).
*
*  If the stored and calculated CRC values differ, the calculated CRC
*  is stored to the CapSense_GLB_CRC_CALC_VALUE register and
*  the CapSense_TST_GLOBAL_CRC bit is set in the
*  CapSense_TEST_RESULT_MASK_VALUE register. The function never
*  clears the CapSense_TST_GLOBAL_CRC bit.
*
*  It is recommended to use the CapSense_SetParam() function to
*  change a value of CapSense_RAM_STRUCT data structure register/elements
*  as CRC is updated by the CapSense_SetParam() function.
*
*  This test also can be initiated by using CapSense_RunSelfTest()
*  function with the CapSense_TST_GLOBAL_CRC mask input.
*
* \return
*  Returns a status of the executed test:
*  - CYRET_SUCCESS - The stored CRC matches the calculated CRC
*  - CapSense_TST_GLOBAL_CRC - The stored CRC is wrong.
*
*******************************************************************************/
uint32 CapSense_CheckGlobalCRC(void)
{
    uint32 result = CapSense_TST_GLOBAL_CRC;
    uint16 crcValue = CapSense_DsCalculateCrc16((uint8 *)&CapSense_dsRam +
                                    CapSense_GLOBAL_CRC_AREA_START,
                                    CapSense_GLOBAL_CRC_AREA_SIZE);
    if (CapSense_dsRam.glbCrc == crcValue)
    {
        result = CYRET_SUCCESS;
    }
    else
    {
        CapSense_dsRam.selfTest.glbCrcCalc = crcValue;
        CapSense_dsRam.selfTest.testResultMask |= CapSense_TST_GLOBAL_CRC;
    }
    return result;
}

/*******************************************************************************
* Function Name: CapSense_DsUpdateGlobalCrc
****************************************************************************//**
*
* \brief
*   Recalculates the CRC for the global Component data (certain registers)
*   and writes it to the Data Structure.
*
* \details
*  Uses the function CapSense_DsCalculateCrc16.
*
*******************************************************************************/
void CapSense_DsUpdateGlobalCrc(void)
{
    CapSense_dsRam.glbCrc = CapSense_DsCalculateCrc16((uint8 *)&CapSense_dsRam +
                                                        CapSense_GLOBAL_CRC_AREA_START,
                                                        CapSense_GLOBAL_CRC_AREA_SIZE);
}
#endif /*(CapSense_ENABLE == CapSense_TST_GLOBAL_CRC_EN) */

#if (CapSense_ENABLE == CapSense_TST_WDGT_CRC_EN)

/*******************************************************************************
* Function Name: CapSense_CheckWidgetCRC
****************************************************************************//**
*
* \brief
*  Checks the stored CRC of the CapSense_RAM_WD_BASE_STRUCT data
*  structure of the specified widget.
*
* \details
*  This function validates the data integrity of the
*  CapSense_RAM_WD_BASE_STRUCT data structure of the specified widget
*  by calculating the CRC and comparing it with the stored CRC value
*  (i.e. CapSense_<WidgetName>_CRC_VALUE).
*
*  If the stored and calculated CRC values differ:
*  1. The calculated CRC is stored to the CapSense_WDGT_CRC_CALC_VALUE register
*  2. The widget ID is stored to the CapSense_WDGT_CRC_ID_VALUE register
*  3. The CapSense_TST_WDGT_CRC bit is set in the
*     CapSense_TEST_RESULT_MASK_VALUE register.
*
*  The function never clears the CapSense_TST_WDGT_CRC bit.
*  If the CapSense_TST_WDGT_CRC bit is set, the CapSense_WDGT_CRC_CALC_VALUE
*  and CapSense_WDGT_CRC_ID_VALUE registers are not updated.
*
*  It is recommended to use the CapSense_SetParam() function to change
*  a value of CapSense_RAM_WD_BASE_STRUCT data structure register/elements
*  as the CRC is updated by CapSense_SetParam() function.
*
*  This test can be initiated by CapSense_RunSelfTest() function with
*  the CapSense_TST_WDGT_CRC mask as an input.
*
*  The function updates the wdgtWorking register
*  CapSense_WDGT_WORKING<Number>_VALUE by clearing the widget-corresponding bit.
*  Those non-working widgets are skipped by the high-level API.
*  Restoring a widget to its working state should be done by the application level.
*
* \param widgetId
*  Specifies the ID number of the widget to be processed.
*  A macro for the widget ID can be found in the CapSense Configuration
*  header file defined as CapSense_<WidgetName>_WDGT_ID.
*
* \return
*  Returns a status of the test execution:
*  - CYRET_SUCCESS - The stored CRC matches the calculated CRC.
*  - CapSense_TST_WDGT_CRC - The widget CRC is wrong.
*  - CapSense_TST_BAD_PARAM - The input parameter is invalid.
*
*******************************************************************************/
uint32 CapSense_CheckWidgetCRC(uint32 widgetId)
{
    uint32 result = CapSense_TST_WDGT_CRC;
    uint16 crcValue = 0u;
    CapSense_RAM_WD_BASE_STRUCT *ptrWdgt;

    if (CapSense_TOTAL_WIDGETS > widgetId)
    {
        /* Get pointer to RAM object data */
        ptrWdgt = (CapSense_RAM_WD_BASE_STRUCT *)CapSense_dsFlash.wdgtArray[widgetId].ptr2WdgtRam;
        crcValue = CapSense_DsCalculateCrc16((uint8 *)ptrWdgt +
                                    CapSense_WIDGET_CRC_AREA_START,
                                    CapSense_WIDGET_CRC_AREA_SIZE);
        if ((ptrWdgt->crc) != crcValue)
        {
            /* Write to self-test data structure wdgtId of the first bad tested widget */
            if (0Lu == (CapSense_dsRam.selfTest.testResultMask &
                       CapSense_TST_WDGT_CRC))
            {
                CapSense_dsRam.selfTest.wdgtCrcCalc = crcValue;
                CapSense_dsRam.selfTest.wdgtCrcId = (uint8)widgetId;
                CapSense_dsRam.selfTest.testResultMask |= CapSense_TST_WDGT_CRC;
            }
            else
            {
            }
            CapSense_dsRam.wdgtWorking[CapSense_GET_WDGT_STATUS_INDEX(widgetId)] &=
                                             ~(CapSense_GET_WDGT_STATUS_MASK(widgetId));
        }
        else
        {
            result = CYRET_SUCCESS;
        }
    }
    else
    {
        result = CapSense_TST_BAD_PARAM;
    }
    return result;
}

/*******************************************************************************
* Function Name: CapSense_CheckAllWidgetCRC
****************************************************************************//**
*
* \brief
*  The internal function that checks CRC of all widget structures.
*
* \details
*  The function calculates CRC of all widget structures and compare it
*  to the stored CRCs.
*  In case of failed comparison the function updates
*  testResultMask and returns the status.
*  The function use CapSense_CheckWidgetCRC.
*
* \return
*  Returns the status of the test processing:
*  - CYRET_SUCCESS if all widget CRC are OK;
*  - CapSense_TST_WDGT_CRC if any widget CRC is wrong.
*  - CapSense_TST_NOT_EXECUTED if any widget is unavailable.
*
*******************************************************************************/
static uint32 CapSense_CheckAllWidgetCRC(void)
{
    uint32 result = CYRET_SUCCESS;
    uint32 wdgtIndex = 0u;

    for (wdgtIndex = 0u;
         wdgtIndex < CapSense_TOTAL_WIDGETS;
         wdgtIndex++)
    {
        if (0u != (CapSense_CheckWidgetCRC(wdgtIndex)))
        {
            result |= CapSense_TST_WDGT_CRC;
        }
    }
    return result;
}

/*******************************************************************************
* Function Name: CapSense_DsUpdateWidgetCrc
****************************************************************************//**
*
* \brief
*   Recalculates the CRC for the specified widgetId and
*   writes it to the Data Structure.
*
* \details
*  The maximum size of the RAM Widget Object is 192 bits. So, the best suitable
*  CRC polynomial is 0xAC9A that has hamming distance 5 for the data up to 241 bits.
*
*  Reference:  "P. Koopman, T. Chakravarthy,
*  "Cyclic Redundancy Code (CRC) Polynomial Selection for Embedded Networks",
*  The International Conference on Dependable Systems and Networks, DSN-2004"
*
* \param widgetId  The widget index number.
*
*******************************************************************************/
void CapSense_DsUpdateWidgetCrc(uint32 widgetId)
{
    uint32 len;
    uint8 *ptrData;
    CapSense_RAM_WD_BASE_STRUCT *ptrWdgt;

    if (widgetId < CapSense_TOTAL_WIDGETS)
    {
        /* Get pointer to RAM object data */
        ptrWdgt = (CapSense_RAM_WD_BASE_STRUCT *)CapSense_dsFlash.wdgtArray[widgetId].ptr2WdgtRam;
        ptrData = (uint8 *)CapSense_dsFlash.wdgtArray[widgetId].ptr2WdgtRam;
        len = (uint32)CapSense_WIDGET_CRC_AREA_SIZE;

        /* Skip CRC in RAM object structure for CRC calculation */
        ptrData += CapSense_WIDGET_CRC_AREA_START;

        ptrWdgt->crc = CapSense_DsCalculateCrc16(ptrData, len);
    }
}
#endif /* (CapSense_ENABLE == CapSense_TST_WDGT_CRC_EN) */

#if (CapSense_ENABLE == CapSense_TST_CRC_GROUP_EN)

/*******************************************************************************
* Function Name: DsCalculateCrc16
****************************************************************************//**
*
* \brief
*   Calculates CRC for the specified buffer and length. CRC Poly: 0xAC9A
*
* \details
*  This API is used for the CRC protection of the RAM Widget Object.
*  The maximum size of the RAM Widget Object is 192 bits. So, the best suitable
*  CRC polynomial is 0xAC9A. It has a Hamming distance 5 for data words up to 241 bits.
*
*  Reference:  "P. Koopman, T. Chakravarthy,
*  "Cyclic Redundancy Code (CRC) Polynomial Selection for Embedded Networks",
*  The International Conference on Dependable Systems and Networks, DSN-2004"
*
* \param ptrData The pointer to the data.
* \param len     The length of the data in bytes.
*
* \return A calculated CRC-16 value.
*
*******************************************************************************/
static uint16 CapSense_DsCalculateCrc16(uint8 *ptrData, uint32 len)
{
    uint32 idx;
    uint32 actualCrc = 0Lu;
    const uint16 crcTable[] =
    {
        0x0000u, 0xAC9Au, 0xF5AEu, 0x5934u, 0x47C6u, 0xEB5Cu, 0xB268u, 0x1EF2u,
        0x8F8Cu, 0x2316u, 0x7A22u, 0xD6B8u, 0xC84Au, 0x64D0u, 0x3DE4u, 0x917Eu
    };

    for (;len-- > 0u;)
    {
        /* Process HI Nibble */
        idx = ((actualCrc >> 12u) ^ (((uint32)*ptrData) >> 4u)) & 0xFLu;
        actualCrc = crcTable[idx] ^ (actualCrc << 4u);

        /* Process LO Nibble */
        idx = ((actualCrc >> 12u) ^ (uint32)*ptrData) & 0xFLu;
        actualCrc = crcTable[idx] ^ (actualCrc << 4u);

        ptrData++;
    }

    return (uint16)actualCrc;
}
#endif /* (CapSense_ENABLE == CapSense_TST_CRC_GROUP_EN) */

#if (CapSense_ENABLE == CapSense_TST_BSLN_DUPLICATION_EN)
/*******************************************************************************
* Function Name: CapSense_CheckBaselineDuplication
****************************************************************************//**
*
* \brief
*  Checks that the baseline of the specified widget/sensor is not corrupted by
*  comparing it with a baseline inverse copy.
*
* \details
*  This function validates the integrity of baseline of sensor by comparing
*  the conformity of the baseline and its inversion.
*
*  If the baseline does not match its inverse copy:
*  1. The widget ID is stored to the CapSense_INV_BSLN_WDGT_ID_VALUE register
*  2. The sensor ID is stored to the CapSense_INV_BSLN_SNS_ID_VALUE register
*  3. The CapSense_TST_BSLN_DUPLICATION bit is set in the
*     CapSense_TEST_RESULT_MASK_VALUE register.
*
*  The function never clears the CapSense_TST_BSLN_DUPLICATION bit.
*  If the CapSense_TST_BSLN_DUPLICATION bit is set, the CapSense_INV_BSLN_WDGT_ID_VALUE
*  and CapSense_INV_BSLN_SNS_ID_VALUE registers are not updated.
*
*  It is possible to execute a test for all the widgets using CapSense_RunSelfTest() function
*  with the CapSense_TST_BSLN_DUPLICATION mask.
*  In this case, the CapSense_INV_BSLN_WDGT_ID_VALUE and
*  CapSense_INV_BSLN_SNS_ID_VALUE registers contain the widget and
*  sensor ID of the first detected fail.
*
*  The function updates the wdgtWorking register CapSense_WDGT_WORKING<Number>_VALUE
*  by clearing the widget-corresponding bit. Those non-working widgets are skipped by
*  the high-level API. Restoring a widget to its working state should be done by the application level.
*
*  The test is integrated into the CapSense Component. All CapSense
*  processing functions like CapSense_ProcessAllWidgets()
*  or CapSense_UpdateSensorBaseline() automatically verify the baseline
*  value before using it and update its inverse copy after processing.
*  If fail is detected during a baseline update a CYRET_BAD_DATA result
*  is returned. The baseline initialization functions do not verify the
*  baseline and update the baseline inverse copy.
*
* \param widgetId
*  Specifies the ID number of the widget to be processed.
*  A macro for the widget ID can be found in the
*  CapSense Configuration header file defined as
*  CapSense_<WidgetName>_WDGT_ID.
*
* \param sensorId
*  Specifies the ID number of the sensor within the widget.
*  A macro for the sensor ID within the specified widget can be found in
*  the CapSense Configuration header file defined as
*  CapSense_<WidgetName>_SNS<SensorNumber>_ID.
*
* \return
*  Returns the status of the test execution:
*  - CY_RET_SUCCESS - The baseline matches its inverse copy.
*  - CapSense_TST_BSLN_DUPLICATION - The test failed.
*  - CapSense_TST_BAD_PARAM - The input parameters are invalid.
*
*******************************************************************************/
uint32 CapSense_CheckBaselineDuplication(uint32 widgetId, uint32 sensorId)
{
    uint32 result = CYRET_SUCCESS;
    uint32 freqChannel;
    CapSense_RAM_SNS_STRUCT *ptrSensor = NULL;
    CapSense_FLASH_WD_STRUCT const *ptrWidget = NULL;

    if ((CapSense_TOTAL_WIDGETS > widgetId) && ((CapSense_GET_SENSOR_COUNT(widgetId)) > sensorId))
    {
        ptrWidget = &CapSense_dsFlash.wdgtArray[widgetId];
        /* Find pointer to specified sensor object */
        ptrSensor = ptrWidget->ptr2SnsRam;
        ptrSensor = &ptrSensor[sensorId];
        /* Check baseline inversion of every channel in sensor */
        for(freqChannel = 0u; freqChannel < CapSense_NUM_SCAN_FREQS; freqChannel++)
        {
            if (ptrSensor->bsln[freqChannel] != ((uint16) ~(ptrSensor->bslnInv[freqChannel])))
            {
                CapSense_UpdateTestResultBaselineDuplication(widgetId, sensorId);
                result |= CapSense_TST_BSLN_DUPLICATION;
            }
        }
    }
    else
    {
        result |= CapSense_TST_BAD_PARAM;
    }
    return result;
}

/*******************************************************************************
* Function Name: CapSense_CheckAllBaselineDuplication
****************************************************************************//**
*
* \brief
*  The internal function that checks baseline duplication of all the sensors.
*
* \details
*  The function checks baseline duplication of all the sensors by using
*  CapSense_CheckBaselineDuplication() function.
*
* \return
*  Returns the status of the test processing:
*    - CYRET_SUCCESS if the test is successfully completed;
*    - CapSense_TST_BSLN_DUPLICATION if any sensor baseline duplication
       is wrong;
*******************************************************************************/
static uint32 CapSense_CheckAllBaselineDuplication(void)
{
    uint32 result = CYRET_SUCCESS;
    uint32 wdgtIndex;
    uint32 snsIndex;

    for (wdgtIndex = 0u; wdgtIndex < CapSense_TOTAL_WIDGETS; wdgtIndex++)
    {
        for (snsIndex = 0u; snsIndex < (CapSense_GET_SENSOR_COUNT(wdgtIndex)); snsIndex++)
        {
            if (CYRET_SUCCESS != CapSense_CheckBaselineDuplication(wdgtIndex, snsIndex))
            {
                result |= CapSense_TST_BSLN_DUPLICATION;
                break;
            }
        }
    }
    return result;
}

/*******************************************************************************
* Function Name: CapSense_UpdateTestResultBaselineDuplication
****************************************************************************//**
*
* \brief
*  The internal function that updates SefTest data structure and
*  widgetWorking register.
*
* \details
*  The function resets a corresponding bit in widgetWorking register,
*  checks TestResultMask for CapSense_TST_BSLN_DUPLICATION
*  bit and if it was not setted to 1 sets it and memorizes wdgtId and snsId
*  in registers invBslnWdgtId and invBslnSnsId of the SefTest data structure.
*
* \param widgetId
*  Specifies the ID number of the widget to be processed.
*
* \param sensorId
*  Specifies the ID number of the sensor within the widget which
*  will be processed.
*
*******************************************************************************/
void CapSense_UpdateTestResultBaselineDuplication(uint32 widgetId, uint32 sensorId)
{
    CapSense_dsRam.wdgtWorking[CapSense_GET_WDGT_STATUS_INDEX(widgetId)] &=
                                     ~(CapSense_GET_WDGT_STATUS_MASK(widgetId));
    /* Write to self-test data structure wdgtId and snsId of the first sensor with a damaged baseline inversion */
    if (0Lu == (CapSense_dsRam.selfTest.testResultMask &
                CapSense_TST_BSLN_DUPLICATION))
    {
        CapSense_dsRam.selfTest.testResultMask |= CapSense_TST_BSLN_DUPLICATION;
        CapSense_dsRam.selfTest.invBslnWdgtId = (uint8)widgetId;
        CapSense_dsRam.selfTest.invBslnSnsId = (uint8)sensorId;
    }
}
#endif /*(CapSense_ENABLE == CapSense_TST_BSLN_DUPLICATION_EN)*/

#if (CapSense_ENABLE == CapSense_TST_BSLN_RAW_OUT_RANGE_EN)

/*******************************************************************************
* Function Name: CapSense_CheckBaselineRawcountRange
****************************************************************************//**
*
* \brief
*  Checks that raw count and baseline of the specified widget/sensor are
*  within the specified range.
*
* \details
*  The baseline and raw count shall be within specific range (based on
*  calibration target) for good units. The function checks whether or not
*  the baseline and raw count are within the limits defined by the user in the
*  ranges function argument. If baseline or raw count are out of limits this
*  function sets the CapSense_TST_BSLN_RAW_OUT_RANGE bit in the
*  CapSense_TEST_RESULT_MASK_VALUE register.
*
*  Unlike other tests, this test does not update
*  CapSense_WDGT_WORKING<Number>_VALUE register and is not available in
*  the CapSense_RunSelfTest() function.
*
*  Use this function to verify the uniformity of sensors, for example, at
*  mass-production or during an operation phase.
*
* \param widgetId
*  Specifies the ID number of the widget.
*  A macro for the widget ID can be found in the
*  CapSense Configuration header file defined as
*  CapSense_<WidgetName>_WDGT_ID.
*
* \param sensorId
*  Specifies the ID number of the sensor within the widget.
*  A macro for the sensor ID within the specified widget can be found in
*  the CapSense Configuration header file defined as
*  CapSense_<WidgetName>_SNS<SensorNumber>_ID.
*
* \param *ranges
*  Specifies the pointer to the CapSense_BSLN_RAW_RANGE_STRUCT structure
*  with valid ranges for the raw count and baseline.
*
* \return
*  Returns a status of the test execution:
*  - CYRET_SUCCESS - The raw count and baseline are within the specified range
*  - CapSense_TST_BSLN_RAW_OUT_RANGE - The test failed and baseline or
     raw count or both are out of the specified limit.
*  - CapSense_TST_BAD_PARAM - The input parameters are invalid.
*
*******************************************************************************/
uint32 CapSense_CheckBaselineRawcountRange(uint32 widgetId, uint32 sensorId,CapSense_BSLN_RAW_RANGE_STRUCT *ranges)
{
    uint32 result = CYRET_SUCCESS;
    uint32 freqChannel;
    CapSense_RAM_SNS_STRUCT *ptrSensor = NULL;
    CapSense_FLASH_WD_STRUCT const *ptrWidget = NULL;


    if ((CapSense_TOTAL_WIDGETS > widgetId) &&
       ((CapSense_GET_SENSOR_COUNT(widgetId)) > sensorId))
    {
        ptrWidget = &CapSense_dsFlash.wdgtArray[widgetId];
        /* Find pointer to specified sensor object */
        ptrSensor = ptrWidget->ptr2SnsRam;
        ptrSensor = &ptrSensor[sensorId];

        #if(CapSense_ENABLE == CapSense_MULTI_FREQ_SCAN_EN)
        /* Check baseline and rawcont of every channel in sensor */
            for(freqChannel = 0u;
                freqChannel < CapSense_NUM_SCAN_FREQS;
                freqChannel++)
            {
        #else
            freqChannel = 0u;
        #endif /* (CapSense_ENABLE == CapSense_MULTI_FREQ_SCAN_EN) */

                if ((ptrSensor->bsln[freqChannel] > ranges->bslnHiLim) ||
                    (ptrSensor->bsln[freqChannel] < ranges->bslnLoLim) ||
                    (ptrSensor->raw[freqChannel]  > ranges->rawHiLim)  ||
                    (ptrSensor->raw[freqChannel]  < ranges->rawLoLim))
                {
                    CapSense_dsRam.selfTest.testResultMask |= CapSense_TST_BSLN_RAW_OUT_RANGE;
                    result = CapSense_TST_BSLN_RAW_OUT_RANGE;
                }

        #if(CapSense_ENABLE == CapSense_MULTI_FREQ_SCAN_EN)
            }
        #endif /* (CapSense_ENABLE == CapSense_MULTI_FREQ_SCAN_EN) */

    }
    else
    {
        result |= CapSense_TST_BAD_PARAM;
    }
    return result;
}
#endif /* (CapSense_ENABLE == CapSense_TST_BSLN_RAW_OUT_RANGE_EN) */


#if (CapSense_ENABLE == CapSense_TST_HW_GROUP_EN)
    #if (((CapSense_ENABLE == CapSense_CSD_EN) || (CapSense_ENABLE == CapSense_CSX_EN)) &&\
         (CapSense_ENABLE == CapSense_TST_SNS_SHORT_EN))
        /*******************************************************************************
        * Function Name: CapSense_CheckIntegritySensorPins
        ****************************************************************************//**
        *
        * \brief
        *  Checks the specified widget/sensor for shorts to GND, VDD or other sensors.
        *
        * \details
        *  This function performs several tests to verify a specified sensor that
        *  is not electrically shorted and in good condition to reliably detect
        *  user interactions.
        *
        *  This function performs tests to check if the specified sensor is shorted to:
        *   * GND, VDD
        *   * Other GPIOs used by CapSense (such as sensors, Tx, Rx,
        *     shield electrodes, and external capacitors)
        *   * Other non-CapSense GPIOs (only if they are configured
        *     in a strong high or low state during the test execution).
        *
        *  The absolute resistance of an electrical short must be less than 1500 Ohm
        *  including all series resistors on the sensor for the short to be detected
        *  to GND, VDD or GPIOs. For example, if a series resistor on a sensor is
        *  560 Ohm (as recommended) and the sensor is shorted with another sensor,
        *  the function can detect a short of 380 Ohm or less as there are two
        *  560 ohm resistors between the shorted sensor GPIOs.
        *
        *  The function executes the following flow to detect a short:
        *   * Configures all the Component controlled GPIOs to strong-drive-low,
        *     and the specified sensor GPIO to resistive Pull up mode.
        *   * A delay of CapSense_SNS_SHORT_TIME_VALUE in microseconds.
        *   * Checks the status of the specified sensor for the expected state
        *     (logic high).
        *   * Configures all CapSense controlled GPIOs to strong-drive-high,
        *     and the specified sensor GPIO to resistive Pull down mode.
        *   * A delay of CapSense_SNS_SHORT_TIME_VALUE in microseconds.
        *   * Checks the status of the specified sensor for the expected state
        *     (logic low).
        *   * The test result is stored in CapSense Data Structure.
        *     A short is reported only when the sensor status check returns
        *     an unexpected state.
        *
        *  Due to the sensor parasitic capacitance and internal pull-up/down resistor,
        *  logic high-to-low (and vice versa) transitions require setting time before
        *  checking the sensor status. A 2-us delay is used as the setting time and can
        *  that be changed using the CapSense_SNS_SHORT_TIME_VALUE parameter.
        *
        *  This function updates the following statuses if a short is detected:
        *   * The widget ID is stored to the
        *     CapSense_SHORTED_WDGT_ID_VALUE register.
        *   * The sensor ID is stored to the CapSense_SHORTED_SNS_ID_VALUE
        *     register.
        *   * The CapSense_TST_SNS_SHORT bit is set in the
        *     CapSense_TEST_RESULT_MASK_VALUE register.
        *   * If CapSense_TST_SNS_SHORT is already set due to a previously
        *     detected fault on any of the sensor, this function does not update
        *     the CapSense_SHORTED_WDGT_ID_VALUE and
        *     CapSense_SHORTED_SNS_ID_VALUE registers. For this reason,
        *     clear CapSense_TST_SNS_SHORT prior to calling this function.
        *   * The widget is disabled by clearing the correcting bit
        *     (CapSense_WDGT_WORKING<Number>_VALUE) in the wdgtWorking
        *     register. The widget is ignored by a high-level function during
        *     further scans and data processing tasks. The application layer can
        *     be set to the corresponding bit to restore the widget operation.
        *   * To check all the Component sensor at once, use the
        *     CapSense_RunSelfTest() function with the
        *     CapSense_TST_SNS_SHORT mask.
        *   * To detect an electrical short or fault condition with resistance
        *     higher than 1500 ohm, the CapSense_GetSensorCapacitance()
        *     function can be used as the fault condition changes the
        *     sensor capacitance, hence it can be detected.
        *
        * \param widgetId
        *  Specifies the ID number of the widget to be processed.
        *  A macro for the widget ID is in the CapSense Configuration
        *  header file defined as CapSense_<WidgetName>_WDGT_ID.
        *
        * \param sensorId
        *  Specifies the ID number of the sensor within the widget.
        *  A macro for the sensor ID within the specified widget is in
        *  the CapSense Configuration header file defined as
        *  CapSense_<WidgetName>_SNS<SensorNumber>_ID.
        *
        * \return
        *  Returns a status of the test execution:
        *  - CYRET_SUCCESS - The sensor does not have a short and is in the
        *    working condition.
        *  - CapSense_TST_SNS_SHORT - A short is detected on
        *    the specified sensor.
        *  - CapSense_TST_BAD_PARAM - The input parameters are invalid.
        *
        *******************************************************************************/
        uint32 CapSense_CheckIntegritySensorPins(uint32 widgetId, uint32 sensorId)
        {
            uint32 result = CYRET_SUCCESS;

            if ((CapSense_TOTAL_WIDGETS > widgetId) &&
               ((CapSense_GET_SENSOR_COUNT(widgetId)) > sensorId))
            {
                /* Release previously captured HW resources by other mode and capture them for BIST */
                CapSense_SsSwitchSensingMode(CapSense_SENSE_METHOD_BIST_E);
                /* Switch HW resource configuration to sensor short test */
                CapSense_BistSwitchHwConfig(CapSense_BIST_HW_SNS_SHORT_E);
                CapSense_SwitchAllIOState(CapSense_BIST_IO_STRONG_E);

                /* Set all sensors to strong-low */
                CapSense_SetAllSnsDrState(CapSense_TST_DR_PIN2GND);
                CapSense_SnsShortSetDrExtCapShield(CapSense_TST_DR_PIN2GND);
                /* Execute the test */
                result = CapSense_SnsShortCheckSensor(widgetId, sensorId, CapSense_TST_DR_PIN2GND);

                if (CYRET_SUCCESS == result)
                {
                    /* Set all sensors to strong-high */
                    CapSense_SetAllSnsDrState(CapSense_TST_DR_PIN2VDD);
                    CapSense_SnsShortSetDrExtCapShield(CapSense_TST_DR_PIN2VDD);
                    /* Execute the test */
                    result = CapSense_SnsShortCheckSensor(widgetId, sensorId, CapSense_TST_DR_PIN2VDD);
                }
            }
            else
            {
                result = CapSense_TST_BAD_PARAM;
            }

            CapSense_bistCurrentISC = CapSense_BIST_IO_UNDEFINED_E;
            return result;
        }


        /*******************************************************************************
        * Function Name: CapSense_SnsShortCheckAllSensors
        ****************************************************************************//**
        *
        * \brief
        *  The internal function that checks all the sensors short.
        *
        * \details
        *  The function that checks for shorts on Vdd/GND of all sensor (not
        *  electrode) by using CapSense_ExecuteSnsShort. The function returns
        *  the status and updates testResultMask and wdgtWorking register
        *  in a case of any shorts detection.
        *
        * \return
        *  Returns the status of the test processing:
        *  - CYRET_SUCCESS if test passed successfully;
        *  - CapSense_TST_SNS_SHORT if any sensor of any widget is
        *    shorted to Vdd or GND.
        *
        *******************************************************************************/
        static uint32 CapSense_SnsShortCheckAllSensors(void)
        {
            uint32 wdgtIndex;
            uint32 snsIndex;
            uint32 result = CYRET_SUCCESS;
            uint32 resultWidget = CYRET_SUCCESS;

            /* Releasing of previously captured HW resources by other mode was done in RunSelfTest function */
            /* Switch HW resource configuration to sensor short test */
            CapSense_BistSwitchHwConfig(CapSense_BIST_HW_SNS_SHORT_E);
            CapSense_SwitchAllIOState(CapSense_BIST_IO_STRONG_E);

            /* For performance reason the CapSense_CheckIntegritySensorPins() function is not used */

            /* Set all sensors to strong-low */
            CapSense_SetAllSnsDrState(CapSense_TST_DR_PIN2GND);
            CapSense_SnsShortSetDrExtCapShield(CapSense_TST_DR_PIN2GND);
            for (wdgtIndex = 0u; wdgtIndex < CapSense_TOTAL_WIDGETS; wdgtIndex++)
            {
                resultWidget = CYRET_SUCCESS;
                for (snsIndex = 0u; snsIndex < (CapSense_GET_SENSOR_COUNT(wdgtIndex)); snsIndex++)
                {
                    resultWidget = CapSense_SnsShortCheckSensor(wdgtIndex, snsIndex, CapSense_TST_DR_PIN2GND);
                    if (CYRET_SUCCESS != resultWidget)
                    {
                        break;
                    }
                }
                result |= resultWidget;
            }

            /* Set all sensors to strong-high */
            CapSense_SetAllSnsDrState(CapSense_TST_DR_PIN2VDD);
            CapSense_SnsShortSetDrExtCapShield(CapSense_TST_DR_PIN2VDD);
            for (wdgtIndex = 0u; wdgtIndex < CapSense_TOTAL_WIDGETS; wdgtIndex++)
            {
                resultWidget = CYRET_SUCCESS;
                for (snsIndex = 0u; snsIndex < (CapSense_GET_SENSOR_COUNT(wdgtIndex)); snsIndex++)
                {
                    resultWidget = CapSense_SnsShortCheckSensor(wdgtIndex, snsIndex, CapSense_TST_DR_PIN2VDD);
                    if (CYRET_SUCCESS != resultWidget)
                    {
                        break;
                    }
                }
                result |= resultWidget;
            }

            CapSense_bistCurrentISC = CapSense_BIST_IO_UNDEFINED_E;
            return result;
        }


        /*******************************************************************************
        * Function Name: CapSense_SnsShortUpdateTestResult
        ****************************************************************************//**
        * \brief
        *  Updates SefTest data structure and widgetWorking register with
        *  the test result.
        *
        * \details
        *  The function resets a corresponding bit in widgetWorking register,
        *  checks TestResultMask for CapSense_TST_SNS_SHORT bit and if
        *  it was not setted to 1 sets it and memorizes wdgtId and snsId
        *  in corresponding registers of the SefTest data structure.
        *
        * \param wdgtId
        *  Specifies the ID number of the widget to be processed.
        *
        * \param snsId
        *  Specifies the ID number of the sensor within the widget which
        *  will be processed.
        *******************************************************************************/
        static void CapSense_SnsShortUpdateTestResult(uint32 wdgtId, uint32 snsId)
        {
            CapSense_dsRam.wdgtWorking[CapSense_GET_WDGT_STATUS_INDEX(wdgtId)] &=
                                             ~(CapSense_GET_WDGT_STATUS_MASK(wdgtId));
            if (0Lu == (CapSense_dsRam.selfTest.testResultMask & CapSense_TST_SNS_SHORT))
            {
                /* Write to self-test data structure wdgtId and snsId of the first shorted sensor */
                CapSense_dsRam.selfTest.testResultMask |= CapSense_TST_SNS_SHORT;
                CapSense_dsRam.selfTest.shortedWdgtId = (uint8)wdgtId;
                CapSense_dsRam.selfTest.shortedSnsId = (uint8)snsId;
            }
        }


        /*******************************************************************************
        * Function Name: CapSense_SnsShortCheckSensor
        ****************************************************************************//**
        *
        * \brief
        *  Check if the specified sensor is shorted to VDD or GND level by configuring
        *  each of its electrode to pull-up or pull-down and check its state.
        *
        * \details
        *  Check if the specified sensor is shorted to VDD or GND level by connfiguring
        *  each of its electrodes to pull-up or pull-down and check its state.
        *  An additional delay is added between configuring the electrode and
        *  reading its state to establish the transition process for cases
        *  with big capacitance and short resistance.
        *  The function assumes all rest electrodes are set to strong drive mode,
        *  so the sensor-to-sensor short condition is also detected.
        *
        * \param widgetId
        *  Specifies the ID number of the widget to be processed.
        *
        * \param sensorId
        *  Specifies the ID number of the sensor within the widget
        *  to be processed.
        *
        * \param mode
        *  Specifies the test mode, either:
        *  * CapSense_TST_DR_PIN2GND means sensor is configured
        *    to pull-up and checked for logical 0
        *  * CapSense_TST_DR_PIN2VDD means sensor is configured
        *    to pull-down and checked for logical 1
        *
        * \return
        *  Returns the status of the test processing:
        *  - CYRET_SUCCESS if the sensor of the widget is not shorted.
        *  - CapSense_TST_SNS_SHORT if the sensor is shorted.
        *
        *******************************************************************************/
        static uint32 CapSense_SnsShortCheckSensor(uint32 widgetId, uint32 sensorId, uint32 mode)
        {
            uint32 result = CYRET_SUCCESS;

            #if (CapSense_ENABLE == CapSense_CSX_EN)
                uint32 ioSnsId;
                if (CapSense_SENSE_METHOD_CSX_E == CapSense_GET_SENSE_METHOD(&CapSense_dsFlash.wdgtArray[widgetId]))
                {
                    /* For CSX widgets get an index of Rx electrode */
                    ioSnsId = sensorId / CapSense_dsFlash.wdgtArray[widgetId].numRows;
                    result = CapSense_SnsShortCheckElectrode(widgetId, ioSnsId, mode);
                    if (CYRET_SUCCESS == result)
                    {
                        /* For CSX widgets get an index of Tx electrode */
                        ioSnsId = (uint32)(sensorId % CapSense_dsFlash.wdgtArray[widgetId].numRows) +
                                  (uint32)CapSense_dsFlash.wdgtArray[widgetId].numCols;
                        result = CapSense_SnsShortCheckElectrode(widgetId, ioSnsId, mode);
                    }
                }
                else
                {
                    result = CapSense_SnsShortCheckElectrode(widgetId, sensorId, mode);
                }
            #else
                result = CapSense_SnsShortCheckElectrode(widgetId, sensorId, mode);
            #endif

            if (CYRET_SUCCESS != result)
            {
                CapSense_SnsShortUpdateTestResult(widgetId, sensorId);
            }
            return result;
        }


        /*******************************************************************************
        * Function Name: CapSense_SnsShortCheckElectrode
        ****************************************************************************//**
        *
        * \brief
        *  Check if a sensor or Rx or Tx are shorted to VDD or GND level by configuring
        *  each of its electrode to pull-up or pull-down and check its state.
        *
        * \details
        *  Check if a sensor or Rx or Tx are shorted to VDD or GND level by connfiguring
        *  each of its electrodes to pull-up or pull-down and check its state.
        *  An additional delay is added between configuring the electrode and
        *  reading its state to establish the transition process for cases
        *  with big capacitance and short resistance.
        *  The function assumes all rest electrodes are set to strong drive mode,
        *  so the sensor-to-sensor short condition is also detected.
        *
        * \param wdgtId
        *  Specifies the ID number of the widget to be processed.
        *
        * \param ioSnsId
        *  Specifies the ID number of the sensor (electrode) within the widget
        *  to be processed.
        *
        * \param mode
        *  Specifies the test mode, either:
        *  * CapSense_TST_DR_PIN2GND means sensor is configured
        *    to pull-up and checked for logical 0
        *  * CapSense_TST_DR_PIN2VDD means sensor is configured
        *    to pull-down and checked for logical 1
        *
        * \return
        *  Returns the status of the test processing:
        *  - CYRET_SUCCESS if the electrode of the widget is not shorted.
        *  - CapSense_TST_SNS_SHORT if the electrode is shorted.
        *
        *******************************************************************************/
        static uint32 CapSense_SnsShortCheckElectrode(uint32 widgetId, uint32 ioSnsId, uint32 mode)
        {
            uint32 i;
            uint32 eltdNum;
            uint32 eltdState;
            uint32 eltdDM = CapSense_TST_PC_PULLDOWN;
            uint32 result = CYRET_SUCCESS;
            CapSense_FLASH_IO_STRUCT const *ioPtr;

            if (CapSense_TST_DR_PIN2GND == mode)
            {
                eltdDM = CapSense_TST_PC_PULLUP;
            }

            eltdNum = CapSense_SnsShortGetElectrdeList(widgetId, ioSnsId, &ioPtr);

            /* Loop through all electrodes of specified sensor */
            for (i = 0u; i < eltdNum; i++)
            {
                /* Set electrode drive mode */
                CapSense_SetSnsDr(ioPtr, (mode ^ 0x01u));
                CapSense_SetSnsPc(ioPtr, eltdDM);
                /* Wait for establishing the transition process */
                CyDelayUs((uint16)CapSense_dsRam.selfTestConfig.snsShortTimeUs);
                /* Read the electrode state */
                eltdState = CapSense_SnsShortReadElectrodeState(ioPtr);
                if (CapSense_TST_DR_PIN2GND != eltdState)
                {
                    eltdState = CapSense_TST_DR_PIN2VDD;
                }
                /* Revert electrode to default drive mode */
                CapSense_SetSnsDr(ioPtr, mode);
                CapSense_SetSnsPc(ioPtr, CapSense_TST_PC_STRONG);
                /* Check the electrode state */
                if (mode == eltdState)
                {
                    result = CapSense_TST_SNS_SHORT;
                    break;
                }
                /* Get the next electrode */
                ioPtr++;
            }
            return result;
        }


        /*******************************************************************************
        * Function Name: CapSense_SnsShortGetElectrdeList
        ****************************************************************************//**
        *
        * \brief
        *  Returns the number of electrodes in a specified sensor or Rx or Tx
        *  and a pointer to the first electrode in the flash IO structure.
        *
        * \details
        *  Returns the number of electrodes in a specified sensor or Rx or Tx
        *  and a pointer to the first electrode in the flash IO structure.
        *  The function considers is an input object is ganged or not.
        *
        * \param widgetId
        *  Specifies the ID number of the widget to be processed.
        *
        * \param ioSnsId
        *  Specifies the ID number of the sensor (electrode) within the widget
        *  to be processed.
        *
        * \param **ioPtr
        *  A pointer to a pointer to the electrode in the flash IO structure.
        *
        * \return
        *  Returns the number of electrodes in a sensor or Rx or Tx.
        *
        *******************************************************************************/
        static uint32 CapSense_SnsShortGetElectrdeList(uint32 widgetId, uint32 ioSnsId, CapSense_FLASH_IO_STRUCT const **ioPtr)
        {
            uint32 eltdNum;
            CapSense_FLASH_SNS_STRUCT const *curFlashSnsPtr;

            /* Check ganged sns flag */
            if (CapSense_GANGED_SNS_MASK == (CapSense_dsFlash.wdgtArray[widgetId].staticConfig & CapSense_GANGED_SNS_MASK))
            {
                curFlashSnsPtr = (CapSense_FLASH_SNS_STRUCT const *)CapSense_dsFlash.wdgtArray[widgetId].ptr2SnsFlash;
                curFlashSnsPtr += ioSnsId;
                *ioPtr = &CapSense_ioList[curFlashSnsPtr->firstPinId];
                eltdNum = curFlashSnsPtr->numPins;
            }
            else
            {
                *ioPtr = (CapSense_FLASH_IO_STRUCT const *)CapSense_dsFlash.wdgtArray[widgetId].ptr2SnsFlash + ioSnsId;
                eltdNum = 1u;
            }

            return eltdNum;
        }


        /*******************************************************************************
        * Function Name: CapSense_BistEnableHwSnsShort
        ****************************************************************************//**
        *
        * \brief
        *  This function configures external capacitors and shield to strong drive mode.
        *
        * \details
        *  This function configures external capacitors and shield to strong drive mode.
        *
        *******************************************************************************/
        static void CapSense_BistEnableHwSnsShort(void)
        {
            uint32 regValue;
            uint8 interruptState;
            #if ((CapSense_ENABLE == CapSense_CSD_SHIELD_EN) && \
                 (0u != CapSense_CSD_TOTAL_SHIELD_COUNT))
                uint32 shieldIndex;
            #endif


            #if (CapSense_ENABLE == CapSense_CSD_EN)
                /* Configure Cmod to strong drive mode */
                interruptState = CyEnterCriticalSection();
                regValue = CY_GET_REG32(CapSense_CMOD_PC_PTR);
                regValue &= (uint32)(~((uint32)CapSense_GPIO_PC_MASK << CapSense_CMOD_PC_SHIFT));
                regValue |= (CapSense_GPIO_STRGDRV << CapSense_CMOD_PC_SHIFT);
                CY_SET_REG32(CapSense_CMOD_PC_PTR, regValue);
                CyExitCriticalSection(interruptState);

                #if ((CapSense_ENABLE == CapSense_CSD_SHIELD_EN) && \
                     (CapSense_ENABLE == CapSense_CSD_SHIELD_TANK_EN))
                    /* Configure Csh to strong drive mode */
                    interruptState = CyEnterCriticalSection();
                    regValue = CY_GET_REG32(CapSense_CSH_PC_PTR);
                    regValue &= (uint32)(~((uint32)CapSense_GPIO_PC_MASK << CapSense_CSH_PC_SHIFT));
                    regValue |= (CapSense_GPIO_STRGDRV << CapSense_CSH_PC_SHIFT);
                    CY_SET_REG32(CapSense_CSH_PC_PTR, regValue);
                    CyExitCriticalSection(interruptState);
                #endif

                #if ((CapSense_ENABLE == CapSense_CSD_SHIELD_EN) && \
                     (0u != CapSense_CSD_TOTAL_SHIELD_COUNT))
                    /* Configure all shield electrodes to strong drive mode */
                    for (shieldIndex = 0u; shieldIndex < CapSense_CSD_TOTAL_SHIELD_COUNT; shieldIndex++)
                    {
                        interruptState = CyEnterCriticalSection();
                        regValue = CY_GET_REG32(CapSense_shieldIoList[shieldIndex].pcPtr);
                        regValue &= ~(CapSense_GPIO_PC_MASK << CapSense_shieldIoList[shieldIndex].shift);
                        regValue |= (CapSense_SNS_GROUND_CONNECT << CapSense_shieldIoList[shieldIndex].shift);
                        CY_SET_REG32(CapSense_shieldIoList[shieldIndex].pcPtr, regValue);
                        CyExitCriticalSection(interruptState);
                    }
                #endif
            #endif

            #if (CapSense_ENABLE == CapSense_CSX_EN)
                /* Configure CintA to strong drive mode */
                interruptState = CyEnterCriticalSection();
                regValue = CY_GET_REG32(CapSense_CintA_PC_PTR);
                regValue &= (uint32)(~((uint32)CapSense_GPIO_PC_MASK << (CapSense_CintA_SHIFT * CapSense_GPIO_PC_BIT_SIZE)));
                regValue |= (CapSense_GPIO_STRGDRV << (CapSense_CintA_SHIFT * CapSense_GPIO_PC_BIT_SIZE));
                CY_SET_REG32(CapSense_CintA_PC_PTR, regValue);
                CyExitCriticalSection(interruptState);

                /* Configure CintB to strong drive mode */
                interruptState = CyEnterCriticalSection();
                regValue = CY_GET_REG32(CapSense_CintB_PC_PTR);
                regValue &= (uint32)(~((uint32)CapSense_GPIO_PC_MASK << (CapSense_CintB_SHIFT * CapSense_GPIO_PC_BIT_SIZE)));
                regValue |= (CapSense_GPIO_STRGDRV << (CapSense_CintB_SHIFT * CapSense_GPIO_PC_BIT_SIZE));
                CY_SET_REG32(CapSense_CintB_PC_PTR, regValue);
                CyExitCriticalSection(interruptState);
            #endif
        }


        /*******************************************************************************
        * Function Name: CapSense_BistDisableHwSnsShort
        ****************************************************************************//**
        *
        * \brief
        *  This function configures external capacitors to High-Z drive mode.
        *
        * \details
        *  This function configures external capacitors to High-Z drive mode.
        *
        *******************************************************************************/
        static void CapSense_BistDisableHwSnsShort(void)
        {
            uint8 interruptState;
            uint32 newRegValue;

            #if (CapSense_ENABLE == CapSense_CSD_EN)
                /* Set port configuration register (drive mode) for Cmod in HIGHZ-A mode (PC=000) */
                interruptState = CyEnterCriticalSection();
                newRegValue = CY_GET_REG32(CapSense_CMOD_PC_PTR);
                newRegValue &= (uint32)(~((uint32)CapSense_GPIO_PC_MASK << CapSense_CMOD_PC_SHIFT));
                newRegValue |= (CapSense_GPIO_HIGHZ << CapSense_CMOD_PC_SHIFT);
                CY_SET_REG32(CapSense_CMOD_PC_PTR, newRegValue);
                CyExitCriticalSection(interruptState);

                #if (CapSense_ENABLE == CapSense_CSDV2)
                    interruptState = CyEnterCriticalSection();
                    /* Disconnect CSDCMP from CSDBUS-A (parallel pass via SFCA switch) */
                    newRegValue = CY_GET_REG32(CapSense_SW_CMP_P_SEL_PTR);
                    newRegValue &= (uint32) (~CapSense_SW_CMP_P_SEL_SW_SFCA_MASK);
                    CY_SET_REG32(CapSense_SW_CMP_P_SEL_PTR, newRegValue);
                    CyExitCriticalSection(interruptState);
                #endif /* (CapSense_ENABLE == CapSense_CSDV2) */

                /* Set port data register for Cmod in 0 (DR=0) */
                interruptState = CyEnterCriticalSection();
                newRegValue = CY_GET_REG32(CapSense_CMOD_DR_PTR);
                newRegValue &= (uint32)(~((uint32)1u << CapSense_CMOD_DR_SHIFT));
                CY_SET_REG32(CapSense_CMOD_DR_PTR, newRegValue);
                CyExitCriticalSection(interruptState);

                #if ((CapSense_ENABLE == CapSense_CSD_SHIELD_EN) && \
                     (CapSense_ENABLE == CapSense_CSD_SHIELD_TANK_EN))
                    /* Set port configuration register (drive mode) for CTANK in HIGHZ-A mode */
                    interruptState = CyEnterCriticalSection();
                    newRegValue = CY_GET_REG32(CapSense_CSH_PC_PTR);
                    newRegValue &= (uint32)(~((uint32)CapSense_GPIO_PC_MASK << CapSense_CSH_PC_SHIFT));
                    newRegValue |= (CapSense_GPIO_HIGHZ << CapSense_CMOD_PC_SHIFT);
                    CY_SET_REG32(CapSense_CSH_PC_PTR, newRegValue);
                    /* Set port data register for CTANK in 0 (DR=0) */
                    newRegValue = CY_GET_REG32(CapSense_CSH_DR_PTR);
                    newRegValue &= (uint32)(~((uint32)1u << CapSense_CSH_SHIFT));
                    CY_SET_REG32(CapSense_CSH_DR_PTR, newRegValue);
                    CyExitCriticalSection(interruptState);
                #endif /* ((CapSense_ENABLE == CapSense_CSD_SHIELD_EN) && \
                           (CapSense_ENABLE == CapSense_CSD_SHIELD_TANK_EN)) */
            #endif /* (CapSense_ENABLE == CapSense_CSD_EN) */

            #if (CapSense_ENABLE == CapSense_CSX_EN)
                /* Set port configuration register (drive mode) for CintA in HIGHZ-A mode (PC=000) */
                interruptState = CyEnterCriticalSection();
                newRegValue = CY_GET_REG32(CapSense_CintA_PC_PTR);
                newRegValue &= (uint32)(~((uint32)CapSense_GPIO_PC_MASK << (CapSense_CintA_SHIFT * CapSense_GPIO_PC_BIT_SIZE)));
                newRegValue |= (CapSense_GPIO_HIGHZ << (CapSense_CintA_SHIFT * CapSense_GPIO_PC_BIT_SIZE));
                CY_SET_REG32(CapSense_CintA_PC_PTR, newRegValue);
                /* Set port data register for CintA in 0 (DR=0) */
                newRegValue = CY_GET_REG32(CapSense_CintA_DR_PTR);
                newRegValue &= (uint32)(~((uint32)1u << CapSense_CintA_SHIFT));
                CY_SET_REG32(CapSense_CintA_DR_PTR, newRegValue);
                CyExitCriticalSection(interruptState);

                /* Set port configuration register (drive mode) for CintB in HIGHZ-A mode */
                interruptState = CyEnterCriticalSection();
                newRegValue = CY_GET_REG32(CapSense_CintB_PC_PTR);
                newRegValue &= (uint32)(~((uint32)CapSense_GPIO_PC_MASK << (CapSense_CintB_SHIFT * CapSense_GPIO_PC_BIT_SIZE)));
                newRegValue |= (CapSense_GPIO_HIGHZ << (CapSense_CintB_SHIFT * CapSense_GPIO_PC_BIT_SIZE));
                CY_SET_REG32(CapSense_CintB_PC_PTR, newRegValue);
                /* Set port data register for CintB in 0 (DR=0) */
                newRegValue = CY_GET_REG32(CapSense_CintB_DR_PTR);
                newRegValue &= (uint32)(~((uint32)1u << CapSense_CintB_SHIFT));
                CY_SET_REG32(CapSense_CintB_DR_PTR, newRegValue);
                CyExitCriticalSection(interruptState);
            #endif /* (CapSense_ENABLE == CapSense_CSX_EN) */
        }


        /*******************************************************************************
        * Function Name: CapSense_SnsShortSetDrExtCapShield
        ****************************************************************************//**
        *
        * \brief
        *  This function configures external capacitors and shield to either
        *  strong-low or strong-high.
        *
        * \details
        *  This function configures external capacitors and shield to either
        *  strong-low or strong-high.
        *
        * \param value
        *  Specifies the strong state, either:
        *  * CapSense_TST_DR_PIN2GND means electrodes are configured to GND
        *  * CapSense_TST_DR_PIN2VDD means electrodes are configured to VDD
        *
        *******************************************************************************/
        static void CapSense_SnsShortSetDrExtCapShield(uint32 value)
        {
            uint32 regValue;
            uint8 interruptState;
            #if ((CapSense_ENABLE == CapSense_CSD_SHIELD_EN) && \
                 (0u != CapSense_CSD_TOTAL_SHIELD_COUNT))
                uint32 shieldIndex;
            #endif

            #if (CapSense_ENABLE == CapSense_CSD_EN)
                /* Configure Cmod to strong drive mode */
                interruptState = CyEnterCriticalSection();
                regValue = CY_GET_REG32(CapSense_CMOD_DR_PTR);
                regValue &= (uint32)(~(uint32) (1uL << CapSense_CMOD_DR_SHIFT));
                regValue |= (uint32)(value << CapSense_CMOD_DR_SHIFT);
                CY_SET_REG32(CapSense_CMOD_DR_PTR, regValue);
                CyExitCriticalSection(interruptState);

                #if ((CapSense_ENABLE == CapSense_CSD_SHIELD_EN) && \
                     (CapSense_ENABLE == CapSense_CSD_SHIELD_TANK_EN))
                    /* Configure Csh to strong drive mode */
                    interruptState = CyEnterCriticalSection();
                    regValue = CY_GET_REG32(CapSense_CSH_DR_PTR);
                    regValue &= (uint32)(~(uint32) (1uL << CapSense_CSH_SHIFT));
                    regValue |= (uint32)(value << CapSense_CSH_SHIFT);
                    CY_SET_REG32(CapSense_CSH_DR_PTR, regValue);
                    CyExitCriticalSection(interruptState);
                #endif

                #if ((CapSense_ENABLE == CapSense_CSD_SHIELD_EN) && \
                     (0u != CapSense_CSD_TOTAL_SHIELD_COUNT))
                    /* Configure all shield electrodes to strong drive mode */
                    for (shieldIndex = 0u; shieldIndex < CapSense_CSD_TOTAL_SHIELD_COUNT; shieldIndex++)
                    {
                        interruptState = CyEnterCriticalSection();
                        regValue = CY_GET_REG32(CapSense_shieldIoList[shieldIndex].drPtr);
                        regValue &= (uint32)(~(uint32) (1uL << CapSense_shieldIoList[shieldIndex].drShift));
                        regValue |= (uint32)(value << CapSense_shieldIoList[shieldIndex].drShift);
                        CY_SET_REG32(CapSense_shieldIoList[shieldIndex].drPtr, regValue);
                        CyExitCriticalSection(interruptState);
                    }
                #endif
            #endif

            #if (CapSense_ENABLE == CapSense_CSX_EN)
                /* Configure CintA to strong-low */
                interruptState = CyEnterCriticalSection();
                regValue = CY_GET_REG32(CapSense_CintA_DR_PTR);
                regValue &= (uint32)(~(uint32) (1uL << CapSense_CintA_SHIFT));
                regValue |= (uint32)(value << CapSense_CintA_SHIFT);
                CY_SET_REG32(CapSense_CintA_DR_PTR, regValue);
                CyExitCriticalSection(interruptState);

                /* Configure CintB to strong-low */
                interruptState = CyEnterCriticalSection();
                regValue = CY_GET_REG32(CapSense_CintB_DR_PTR);
                regValue &= (uint32)(~(uint32) (1uL << CapSense_CintB_SHIFT));
                regValue |= (uint32)(value << CapSense_CintB_SHIFT);
                CY_SET_REG32(CapSense_CintB_DR_PTR, regValue);
                CyExitCriticalSection(interruptState);
            #endif
        }


        /*******************************************************************************
        * Function Name: CapSense_SnsShortReadElectrodeState
        ****************************************************************************//**
        *
        * \brief
        *  Reads a certain electrode state.
        *
        * \details
        *  The function activates an input buffer for the specified electrode and reads
        *  the correspondent input bit. After that it disables the input buffer.
        *
        * \param *ioPtr
        *  A pointer to IO data structure of the specified electrode.
        *
        * \return
        *  Returns the state of the input buffer of the electrode (0 or non 0).
        *
        *******************************************************************************/
        static uint32 CapSense_SnsShortReadElectrodeState(CapSense_FLASH_IO_STRUCT const *ioPtr)
        {
            uint32 newRegisterValue;
            uint8  interruptState;
            uint32 result;

            /* Enable the input buffer for the specified electrode */
            interruptState = CyEnterCriticalSection();
            newRegisterValue = CY_GET_REG32(ioPtr->pc2Ptr);
            newRegisterValue &= ~(ioPtr->mask);
            CY_SET_REG32(ioPtr->pc2Ptr, newRegisterValue);
            CyExitCriticalSection(interruptState);

            /* Read the bit from the IO pad state register (PS) */
            result = CY_GET_REG32(ioPtr->psPtr) & ioPtr->mask;

            /* Disable the input buffer for this pin (set PC2 bit to logic 1) */
            interruptState = CyEnterCriticalSection();
            newRegisterValue = CY_GET_REG32(ioPtr->pc2Ptr);
            newRegisterValue |= ioPtr->mask;
            CY_SET_REG32(ioPtr->pc2Ptr, newRegisterValue);
            CyExitCriticalSection(interruptState);

            return(result);
        }
    #endif /* (((CapSense_ENABLE == CapSense_CSD_EN) || (CapSense_ENABLE == CapSense_CSX_EN)) &&\
               (CapSense_ENABLE == CapSense_TST_SNS_SHORT_EN)) */


    #if (CapSense_ENABLE == CapSense_TST_MEASUREMENT_GROUP_EN)
        #if ((CapSense_ENABLE == CapSense_TST_SNS_CAP_EN) || \
             (CapSense_ENABLE == CapSense_TST_SH_CAP_EN)  ||\
             (CapSense_ENABLE == CapSense_TST_EXTERNAL_CAP_EN))
            /*******************************************************************************
            * Function Name: CapSense_BistEnableHwSnsCap
            ****************************************************************************//**
            *
            * \brief
            *  Sets up the CapSense block to perform a capacitanse measuring.
            *
            * \details
            *   This function prepare HW of the CapSense block to CSD sensing mode
            *   with BIST defined parameters.
            *   The function performs the following tasks:
            *   1. Sets the default CSD configuration for BIST scans
            *   2. Enables shield electrodes and Csh (if defined)
            *   3. Sets Idac config for BIST scans
            *   4. Sets a sensor clock frequency (only for a sensor capacitance measuring)
            *   5. Sets the ISR handler for BIST scans.
            *
            *******************************************************************************/
            static void CapSense_BistEnableHwSnsCap(void)
            {
                uint32 newRegValue;
                uint8  interruptState;

                CapSense_DischargeExtCapacitors(CapSense_EXT_CAP_DISCHARGE_TIME);

                #if (((CapSense_ENABLE == CapSense_CSD_EN) || (CapSense_ENABLE == CapSense_CSX_EN)) &&\
                     (CapSense_ENABLE == CapSense_TST_SNS_CAP_EN))
                    /* Verification of the bist mode */
                    if (CapSense_BIST_HW_SNS_CAP_E == CapSense_bistHwConfig)
                    {
                        #if ((CapSense_ENABLE == CapSense_CSD_SHIELD_EN) &&\
                             (CapSense_ENABLE == CapSense_CSD_SHIELD_TANK_EN))
                            /* Configure Csh */
                            CapSense_BistEnableShieldTank();
                        #endif
                    }
                #endif /* (((CapSense_ENABLE == CapSense_CSD_EN) || (CapSense_ENABLE == CapSense_CSX_EN)) &&\
                           (CapSense_ENABLE == CapSense_TST_SNS_CAP_EN)) */

                #if (CapSense_ENABLE == CapSense_CSDV2)

                    /* Initialize the unused CSD registers to defaut state */
                    CY_SET_REG32(CapSense_INTR_SET_PTR,         CapSense_BIST_INTR_SET_CFG);
                    CY_SET_REG32(CapSense_SW_FW_TANK_SEL_PTR,   CapSense_BIST_SW_FW_TANK_SEL_CFG);
                    CY_SET_REG32(CapSense_SW_DSI_SEL_PTR,       CapSense_BIST_DEFAULT_SW_DSI_SEL);
                    CY_SET_REG32(CapSense_ADC_CTL_PTR,          CapSense_BIST_ADC_CTL_CFG);
                    CY_SET_REG32(CapSense_AMBUF_PTR,            CapSense_BIST_AMBUF_PWR_MODE_OFF);
                    CY_SET_REG32(CapSense_SW_SHIELD_SEL_PTR,    CapSense_BIST_SW_SHIELD_SEL_CFG);
                    CY_SET_REG32(CapSense_SW_HS_P_SEL_PTR,      CapSense_BIST_SW_HS_P_SEL_CFG);
                    CY_SET_REG32(CapSense_SW_HS_N_SEL_PTR,      CapSense_BIST_SW_HS_N_SEL_CFG);
                    CY_SET_REG32(CapSense_HSCMP_PTR,            CapSense_BIST_HSCMP_CFG);
                    CY_SET_REG32(CapSense_SEQ_TIME_PTR,         CapSense_BIST_AUTO_ZERO_TIME);
                    CY_SET_REG32(CapSense_SW_RES_PTR,           CapSense_BIST_SW_RES_INIT);

                    /* Connect AMUXBUS-A to CSDBUS-A */
                    interruptState = CyEnterCriticalSection();
                    newRegValue = CY_GET_REG32(CapSense_SW_BYP_SEL_PTR);
                    newRegValue |= CapSense_SW_BYP_SEL_SW_BYA_MASK;
                    CY_SET_REG32(CapSense_SW_BYP_SEL_PTR, newRegValue);
                    CyExitCriticalSection(interruptState);

                    /* Verification of the bist mode */
                    if (CapSense_BIST_HW_EXT_CAP_E != CapSense_bistHwConfig)
                    {
                        #if (CapSense_ENABLE == CapSense_CSD_EN)
                            /* Connect CMOD to AMUXBUS-A */
                            interruptState = CyEnterCriticalSection();
                            newRegValue = CY_GET_REG32(CapSense_CMOD_HSIOM_PTR);
                            newRegValue &= (uint32)(~(uint32)CapSense_CMOD_HSIOM_MASK);
                            newRegValue |= (CapSense_HSIOM_SEL_AMUXA << CapSense_CMOD_HSIOM_SHIFT);
                            CY_SET_REG32(CapSense_CMOD_HSIOM_PTR, newRegValue);
                            CyExitCriticalSection(interruptState);

                            /* Connect CMOD to (sense path) to CSDCOMP */
                            #if (CapSense_CSD__CMOD_PAD == CapSense_CMOD_CONNECTION)
                                CY_SET_REG32(CapSense_SW_CMP_P_SEL_PTR, CapSense_SW_CMP_P_SEL_SW_SFPM_STATIC_CLOSE);
                            #elif (CapSense_CSD__CSHIELD_PAD == CapSense_CMOD_CONNECTION)
                                CY_SET_REG32(CapSense_SW_CMP_P_SEL_PTR, CapSense_SW_CMP_P_SEL_SW_SFPS_STATIC_CLOSE);
                            #else
                                CY_SET_REG32(CapSense_SW_CMP_P_SEL_PTR, CapSense_SW_CMP_P_SEL_SW_SFPT_STATIC_CLOSE);
                            #endif /* (CapSense_CSD__CMOD_PAD == CapSense_CMOD_CONNECTION) */
                        #else
                            interruptState = CyEnterCriticalSection();
                            newRegValue = CY_GET_REG32(CapSense_CintA_HSIOM_PTR);
                            newRegValue &= (uint32)(~(uint32)CapSense_CintA_HSIOM_MASK);
                            newRegValue |= (CapSense_HSIOM_SEL_AMUXA << CapSense_CintA_HSIOM_SHIFT);
                            CY_SET_REG32(CapSense_CintA_HSIOM_PTR, newRegValue);
                            CyExitCriticalSection(interruptState);

                            interruptState = CyEnterCriticalSection();
                            newRegValue = CY_GET_REG32(CapSense_CintB_HSIOM_PTR);
                            newRegValue &= (uint32)(~(uint32)CapSense_CintB_HSIOM_MASK);
                            newRegValue |= (CapSense_HSIOM_SEL_AMUXA << CapSense_CintB_HSIOM_SHIFT);
                            CY_SET_REG32(CapSense_CintB_HSIOM_PTR, newRegValue);
                            CyExitCriticalSection(interruptState);

                            CY_SET_REG32(CapSense_SW_CMP_P_SEL_PTR, CapSense_SW_CMP_P_SEL_SW_SFPT_STATIC_CLOSE);
                        #endif /* (CapSense_ENABLE == CapSense_CSD_EN) */
                    }

                    /* Verification of the bist mode */
                    if (CapSense_BIST_HW_SNS_CAP_E == CapSense_bistHwConfig)
                    {
                        /* Configure shield driving path */
                        #if (CapSense_ENABLE == CapSense_CSD_SHIELD_EN)
                            /* Connect AMUXBUS-B to CSDBUS-B (and AMUXBUS-A to CSDBUS-A ) */
                            CY_SET_REG32(CapSense_SW_BYP_SEL_PTR, CapSense_SW_BYP_SEL_SW_BYA_MASK |
                                                                          CapSense_SW_BYP_SEL_SW_BYB_MASK);
                        #endif /* (CapSense_ENABLE == CapSense_CSD_SHIELD_EN) */
                    }

                    /* Connect VREF to REFGEN (IAIB switch is open) */
                    CY_SET_REG32(CapSense_SW_REFGEN_SEL_PTR, CapSense_BIST_SW_REFGEN_SEL_CFG);

                    CY_SET_REG32(CapSense_SW_CMP_N_SEL_PTR, CapSense_SW_CMP_N_SEL_SW_SCRH_STATIC_CLOSE);
                    #if (CapSense_2000_MV > CapSense_CYDEV_VDDA_MV)

                        /* Configure REFGEN. Set reference voltage when Vdda < 2 V */
                        CY_SET_REG32(CapSense_REFGEN_PTR, CapSense_TST_REFGEN_LV);

                        #if (CapSense_ENABLE == CapSense_CSD_SHIELD_EN)
                            CY_SET_REG32(CapSense_AMBUF_PTR, CapSense_AMBUF_PWR_MODE_NORM);
                            CY_SET_REG32(CapSense_SW_AMUXBUF_SEL_PTR, CapSense_SW_AMUXBUF_SEL_SW_IRH_STATIC_CLOSE |
                                                                              CapSense_SW_AMUXBUF_SEL_SW_IRLB_STATIC_CLOSE |
                                                                              CapSense_SW_AMUXBUF_SEL_SW_ICB_PHI2);
                        #else
                            CY_SET_REG32(CapSense_AMBUF_PTR, CapSense_AMBUF_PWR_MODE_NORM);
                            CY_SET_REG32(CapSense_SW_AMUXBUF_SEL_PTR, CapSense_SW_AMUXBUF_SEL_SW_IRH_STATIC_CLOSE |
                                                                              CapSense_SW_AMUXBUF_SEL_SW_IRLB_STATIC_CLOSE);
                        #endif /* (CapSense_ENABLE == CapSense_CSD_SHIELD_EN) */
                    #else


                        /* Configure REFGEN. Set reference voltage when Vdda >= 2 V */
                        #if (((CapSense_ENABLE == CapSense_CSD_EN) || (CapSense_ENABLE == CapSense_CSX_EN)) &&\
                             ((0u != CapSense_TST_SNS_CAP_EN) || (0u != CapSense_TST_SH_CAP_EN)))
                            if ((CapSense_BIST_HW_SNS_CAP_E == CapSense_bistHwConfig) ||
                                (CapSense_BIST_HW_SH_CAP_E == CapSense_bistHwConfig))
                            {
                            newRegValue = CapSense_TST_REFGEN_HV |
                                    ((uint32)CapSense_dsRam.selfTestConfig.snsCapVrefGain << CYFLD_CSD_GAIN__OFFSET);
                            }
                        #endif /* (((CapSense_ENABLE == CapSense_CSD_EN) || (CapSense_ENABLE == CapSense_CSX_EN)) &&\
                                   ((0u != CapSense_TST_SNS_CAP_EN) || (0u != CapSense_TST_SH_CAP_EN))) */

                        #if (0u != CapSense_TST_EXTERNAL_CAP_EN)
                            if ((CapSense_BIST_HW_EXT_CAP_E == CapSense_bistHwConfig))
                            {
                            newRegValue = CapSense_TST_REFGEN_HV |
                                    ((uint32)CapSense_dsRam.selfTestConfig.extCapVrefGain << CYFLD_CSD_GAIN__OFFSET);
                            }
                        #endif /* #if (0u != CapSense_TST_EXTERNAL_CAP_EN) */

                        CY_SET_REG32(CapSense_REFGEN_PTR, newRegValue);

                        #if (CapSense_ENABLE == CapSense_CSD_SHIELD_EN)
                            /* Turn on CSD_AMBUF high power level when Vdaa >= 2V */
                            CY_SET_REG32(CapSense_AMBUF_PTR, CapSense_AMBUF_PWR_MODE_HI);
                            CY_SET_REG32(CapSense_SW_AMUXBUF_SEL_PTR, CapSense_SW_AMUXBUF_SEL_SW_IRH_STATIC_CLOSE |
                                                                              CapSense_SW_AMUXBUF_SEL_SW_ICB_PHI2);
                        #else

                            CY_SET_REG32(CapSense_AMBUF_PTR, CapSense_AMBUF_PWR_MODE_OFF);

                            /* Configure REFGEN. Set reference voltage when Vdda >= 2 V */
                            CY_SET_REG32(CapSense_SW_AMUXBUF_SEL_PTR, CapSense_SW_AMUXBUF_SEL_SW_DEFAULT);
                        #endif /* (CapSense_ENABLE == CapSense_CSD_SHIELD_EN) */
                    #endif /* (CapSense_2000_MV > CapSense_CYDEV_VDDA_MV) */

                    #if ((CapSense_ENABLE == CapSense_TST_SNS_CAP_EN) ||\
                         (CapSense_ENABLE == CapSense_TST_SH_CAP_EN))
                        if (CapSense_BIST_HW_EXT_CAP_E != CapSense_bistHwConfig)
                        {
                            /* Configure ModClk */
                            CapSense_SsSetModClkClockDivider((uint32)CapSense_dsRam.selfTestConfig.snsCapModClkDivider);
                        }
                    #endif /* ((CapSense_ENABLE == CapSense_TST_SNS_CAP_EN) ||\
                               (CapSense_ENABLE == CapSense_TST_SH_CAP_EN)) */

                    /* Verification of the bist mode */
                    if (CapSense_BIST_HW_SNS_CAP_E == CapSense_bistHwConfig)
                    {
                        #if (CapSense_ENABLE == CapSense_CSD_SHIELD_EN)
                            /* Update CSD config register shield delay bits with shield delay value */
                            CapSense_bistConfigCsd &= (uint32)(~CapSense_CONFIG_SHIELD_DELAY_MASK);
                            CapSense_bistConfigCsd |= ((uint32)CapSense_CSD_SHIELD_DELAY <<
                                                                                   CYFLD_CSD_SHIELD_DELAY__OFFSET);
                        #endif /* (CapSense_ENABLE == CapSense_CSD_SHIELD_EN) */
                    }

                    /* Configure HW block filter delay */
                    CapSense_bistConfigCsd &= (uint32)(~CapSense_CONFIG_FILTER_DELAY_MASK);
                    CapSense_bistConfigCsd |= CapSense_FILTER_DELAY_CFG;

                    /* Enable power to sub-blocks */
                    CY_SET_REG32(CapSense_CONFIG_PTR,     CapSense_bistConfigCsd |
                                                                  CapSense_CONFIG_SENSE_EN_MASK |
                                                                  CapSense_CONFIG_ENABLE_MASK |
                                                                  CapSense_CONFIG_SAMPLE_SYNC_MASK);

                    #if(CapSense_TST_ANALOG_STARTUP_DELAY_US > 0uL)
                        CyDelayUs(CapSense_TST_ANALOG_STARTUP_DELAY_US);
                    #endif /* (CapSense_TST_ANALOG_STARTUP_DELAY_US > 0uL) */


                    /* Set the number of dummy fine initialization cycles depending of the bist mode */
                    /* Set Phi1 and Phi2 switches to non-overlapping (no low EMI) and width depending of the bist mode */
                    if (CapSense_BIST_HW_EXT_CAP_E != CapSense_bistHwConfig)
                    {
                        #if (((CapSense_ENABLE == CapSense_CSD_EN) || (CapSense_ENABLE == CapSense_CSX_EN)) &&\
                             ((0u != CapSense_TST_SNS_CAP_EN) || (0u != CapSense_TST_SH_CAP_EN)))
                            CY_SET_REG32(CapSense_SEQ_INIT_CNT_PTR, (uint32)CapSense_dsRam.selfTestConfig.fineInitTime);
                            CY_SET_REG32(CapSense_SENSE_DUTY_PTR, CapSense_SENSE_DUTY_OVERLAP_PHI1_MASK |
                                                                          CapSense_SENSE_DUTY_OVERLAP_PHI2_MASK);
                        #endif /* (((CapSense_ENABLE == CapSense_CSD_EN) || (CapSense_ENABLE == CapSense_CSX_EN)) &&\
                                   ((0u != CapSense_TST_SNS_CAP_EN) || (0u != CapSense_TST_SH_CAP_EN))) */
                    }
                    else
                    {
                        #if (0u != CapSense_TST_EXTERNAL_CAP_EN)
                            CY_SET_REG32(CapSense_SEQ_INIT_CNT_PTR, CapSense_SEQ_INIT_CNT_FINE_INIT_SKIP);
                            CY_SET_REG32(CapSense_SENSE_DUTY_PTR, CapSense_SENSE_DUTY_OVERLAP_PHI1_MASK |
                                                                          CapSense_SENSE_DUTY_OVERLAP_PHI2_MASK |
                                                                          CapSense_SENSE_DUTY_TST_EXT_CAP_WIDTH);
                        #endif /* #if (0u != CapSense_TST_EXTERNAL_CAP_EN) */
                    }
                #endif /* (CapSense_ENABLE == CapSense_CSDV2) */

                #if (CapSense_DISABLE == CapSense_CSDV2)

                    if (CapSense_BIST_HW_EXT_CAP_E != CapSense_bistHwConfig)
                    {
                        #if (CapSense_ENABLE == CapSense_CSD_EN)
                            /* Connect Cmod to AMUXBUS-A using the HSIOM registers */
                            interruptState = CyEnterCriticalSection();
                            newRegValue = CY_GET_REG32(CapSense_CMOD_HSIOM_PTR);
                            newRegValue &= (uint32)(~(uint32)CapSense_CMOD_HSIOM_MASK);
                            newRegValue |= (CapSense_HSIOM_SEL_AMUXA << CapSense_CMOD_HSIOM_SHIFT);
                            CY_SET_REG32(CapSense_CMOD_HSIOM_PTR, newRegValue);
                            CyExitCriticalSection(interruptState);
                        #else
                            interruptState = CyEnterCriticalSection();
                            newRegValue = CY_GET_REG32(CapSense_CintB_HSIOM_PTR);
                            newRegValue &= ~CapSense_CintB_HSIOM_MASK;
                            newRegValue |= (CapSense_HSIOM_SEL_AMUXA << CapSense_CintB_HSIOM_SHIFT);
                            CY_SET_REG32(CapSense_CintB_HSIOM_PTR, newRegValue);
                            CyExitCriticalSection(interruptState);
                        #endif /* (CapSense_ENABLE == CapSense_CSD_EN) */
                    }
                    #if (CapSense_ENABLE == CapSense_CSD_SHIELD_EN)
                        /* Verification of the BIST mode */
                        if (CapSense_BIST_HW_SNS_CAP_E == CapSense_bistHwConfig)
                        {
                            /* Update CSD config register shield delay bits with a shield delay value */
                            CapSense_bistConfigCsd &= (uint32)(~CapSense_CONFIG_SHIELD_DELAY_MASK);
                            CapSense_bistConfigCsd |= ((uint32)CapSense_CSD_SHIELD_DELAY << CapSense_SHIELD_DELAY_SHIFT);
                        }
                    #endif /* (CapSense_ENABLE == CapSense_CSD_SHIELD_EN) */

                    if (CapSense_BIST_HW_EXT_CAP_E != CapSense_bistHwConfig)
                    {
                        /* Enable the CSD block. Connect Vref buffer to AMUX bus to make sure that Cmod is charged before scanning in the active mode */
                        #if (CapSense_ENABLE == CapSense_BLOCK_OFF_AFTER_SCAN_EN)
                            CY_SET_REG32(CapSense_CONFIG_PTR, CapSense_bistConfigCsd);
                        #else
                            CY_SET_REG32(CapSense_CONFIG_PTR, CapSense_bistConfigCsd | CapSense_BIST_CTANK_PRECHARGE_CONFIG_CSD_EN);
                        #endif /* (CapSense_ENABLE == CapSense_BLOCK_OFF_AFTER_SCAN_EN) */
                    }
                #endif /* (CapSense_DISABLE == CapSense_CSDV2) */

                /* Configure IDAC */
                CapSense_BistSetIdac(0uL);

                #if ((CapSense_ENABLE == CapSense_TST_SNS_CAP_EN) ||\
                     (CapSense_ENABLE == CapSense_TST_SH_CAP_EN))
                if (CapSense_BIST_HW_EXT_CAP_E != CapSense_bistHwConfig)
                {
                    /* Set clock dividers and clock source mode */
                    CapSense_BistConfigClock((uint32)CapSense_dsRam.selfTestConfig.snsCapSnsClkDivider);
                }
                else
                #endif
                {
                    #if (CapSense_DISABLE == CapSense_CSDV2)
                        /* Enable the CSD block. Connect Vref buffer to AMUX bus to make sure that Cmod is charged before scanning in the active mode */
                        CY_SET_REG32(CapSense_CONFIG_PTR, CapSense_bistConfigCsd | CapSense_CONFIG_ENABLE_MASK);
                    #endif /* (CapSense_DISABLE == CapSense_CSDV2) */
                }
                CapSense_ISR_StartEx(&CapSense_BistPostSingleScan);
            }


            /*******************************************************************************
            * Function Name: CapSense_BistSetScanDuration
            ****************************************************************************//**
            *
            * \brief
            *   Calculates Scan Duration which is defined by scan resolution
            *
            * \details
            *   For Fourth-generation HW block: The function calculates the number of conversions and updates the
            *   SEQ_NORM_CNT register. The number of conversions depends on the resolution and
            *   snsClk divider.
            *   For Third-generation HW block: The function recalculates the resolution using the following equation:
            *   2^resolution - 1. The calculated value is contained in the
            *   CapSense_bistCounterResolution global variable and used in the
            *   CapSense_bistStartSample() function to trigger a scan process.
            *
            *******************************************************************************/
            static void CapSense_BistSetScanDuration(void)
            {
                uint32 scanDuration;

                if (CapSense_bistHwConfig == CapSense_BIST_HW_EXT_CAP_E)
                {
                    #if (CapSense_ENABLE == CapSense_TST_EXTERNAL_CAP_EN)
                        #if (CapSense_ENABLE == CapSense_CSDV2)
                            scanDuration = CapSense_TST_EXT_CAP_DURATION;
                            CY_SET_REG32(CapSense_SEQ_NORM_CNT_PTR, scanDuration & CapSense_SEQ_NORM_CNT_CONV_CNT_MASK);
                        #else
                            scanDuration = ((uint32)CapSense_TST_EXT_CAP_MEASURE_WND << 1u) - 1u;
                            scanDuration <<= CapSense_RESOLUTION_OFFSET;
                            CapSense_bistCounterResolution = scanDuration;
                        #endif
                    #endif
                }
                else
                {
                #if ((CapSense_ENABLE == CapSense_TST_SNS_CAP_EN) ||\
                     (CapSense_ENABLE == CapSense_TST_SH_CAP_EN))
                    #if (CapSense_ENABLE == CapSense_CSDV2)
                        /* Calculate scanning resolution value in register */
                        scanDuration = (uint32)(((0x01uL << CapSense_dsRam.selfTestConfig.snsCapResolution) /
                                                            CapSense_dsRam.selfTestConfig.snsCapSnsClkDivider) &
                                                            CapSense_SEQ_NORM_CNT_CONV_CNT_MASK);
                        CY_SET_REG32(CapSense_SEQ_NORM_CNT_PTR, scanDuration & CapSense_SEQ_NORM_CNT_CONV_CNT_MASK);
                    #else
                        scanDuration = (((0x01uL << CapSense_dsRam.selfTestConfig.snsCapResolution) - 1uL) <<
                                                    CapSense_RESOLUTION_OFFSET);
                        CapSense_bistCounterResolution = scanDuration;
                    #endif
                #endif /* ((CapSense_ENABLE == CapSense_TST_SNS_CAP_EN) ||\
                           (CapSense_ENABLE == CapSense_TST_SH_CAP_EN)) */
                }
            }


            /*******************************************************************************
            * Function Name: CapSense_BistPostSingleScan
            ****************************************************************************//**
            *
            * \brief
            *  This is an internal ISR function for the single-sensor scanning implementation.
            *
            * \details
            *  This ISR handler is triggered when the CSD scan for a capacitance measuring
            *  is finished.
            *
            *  The following tasks are performed:
            *    1. Disable the CSD interrupt.
            *    2. Read the Counter register and update the CapSense_bistRawcount with raw data.
            *    3. Reset the BUSY flag.
            *    4. Connect the Vref buffer to the AMUX bus (for Third-generation HW block).
            *
            *******************************************************************************/
            CY_ISR(CapSense_BistPostSingleScan)
            {
                uint32 tmpRawData;

                /* Clear pending interrupt */
                CY_SET_REG32(CapSense_INTR_PTR, CapSense_INTR_SET_MASK);
                /* Read SlotResult from Raw Counter */
                tmpRawData = (uint16)CY_GET_REG32(CapSense_COUNTER_PTR);

                CapSense_bistRawcount = LO16(tmpRawData);

                /* Sensor is totally scanned. Reset BUSY flag */
                CapSense_dsRam.status &= ~(CapSense_SW_STS_BUSY | CapSense_WDGT_SW_STS_BUSY);
                #if (CapSense_ENABLE == CapSense_CSDV2)
                    #if (CapSense_ENABLE == CapSense_CSD_SHIELD_EN)
                    /* Open HCBV and HCBG switches */
                        CY_SET_REG32(CapSense_SW_SHIELD_SEL_PTR, CapSense_SW_SHIELD_SEL_SW_HCBV_STATIC_OPEN |
                                                                         CapSense_SW_SHIELD_SEL_SW_HCBG_STATIC_OPEN);
                    #endif /* (CapSense_ENABLE == CapSense_CSD_SHIELD_EN) */
                    #if (CapSense_ENABLE == CapSense_BLOCK_OFF_AFTER_SCAN_EN)
                        /* Disable the Fourth-generation HW block */
                        CY_SET_REG32(CapSense_CONFIG_PTR, CapSense_bistConfigCsd);
                    #endif /* (CapSense_ENABLE == CapSense_BLOCK_OFF_AFTER_SCAN_EN) */
                #else /* (CapSense_ENABLE == CapSense_CSDV2) */
                    #if (CapSense_ENABLE == CapSense_BLOCK_OFF_AFTER_SCAN_EN)
                        /* Disable the Third-generation HW block. Connect the Vref buffer to AMUX bus */
                        #if ((CapSense_CSH_PRECHARGE_IO_BUF == CapSense_CSD_CSH_PRECHARGE_SRC) &&\
                             (CapSense_ENABLE == CapSense_CSD_SHIELD_TANK_EN))
                            CY_SET_REG32(CapSense_CONFIG_PTR, CapSense_bistConfigCsd | CapSense_BIST_CMOD_PRECHARGE_CONFIG);
                        #else
                            CY_SET_REG32(CapSense_CONFIG_PTR, CapSense_bistConfigCsd | CapSense_BIST_CTANK_PRECHARGE_CONFIG);
                        #endif /* ((CapSense_CSH_PRECHARGE_IO_BUF == CapSense_CSD_CSH_PRECHARGE_SRC) &&\
                                   (CapSense_ENABLE == CapSense_CSD_SHIELD_TANK_EN)) */
                    #else
                        /* Connect the Vref buffer to AMUX bus. The Third-generation HW block is enabled */
                        CY_SET_REG32(CapSense_CONFIG_PTR, CapSense_bistConfigCsd | CapSense_BIST_CTANK_PRECHARGE_CONFIG_CSD_EN);
                    #endif /* (CapSense_ENABLE == CapSense_BLOCK_OFF_AFTER_SCAN_EN) */
                #endif /* (CapSense_ENABLE == CapSense_CSDV2) */
            }


            /*******************************************************************************
            * Function Name: CapSense_BistStartSample
            ****************************************************************************//**
            *
            * \brief
            *   Starts the CSD conversion for a capacitance measuring.
            *
            * \details
            *   This function assumes that the CSD block is already set up using
            *   the CapSense_BistEnableHwSnsCap function and the capacitance
            *   is connected to the CSD block.
            *   The function performs the following tasks:
            *   1. Disables the CSD interrupt.
            *   2. Enables CSD block if it was disabled.
            *   3. Restarts the clock (only for the Third-generation HW block and only for sensor capacitance measuring).
            *   3. Pre charges Cmod (only for a sensor capacitance measuring or for the Third-generation HW block).
            *   4. Starts the scan.
            *
            *******************************************************************************/
            static void CapSense_BistStartSample(void)
            {
                CapSense_DischargeExtCapacitors(0u);

                #if (CapSense_ENABLE == CapSense_CSDV2)
                    /* Fourth-generation HW block section */

                    /* Disable the CSD interrupt to prevent it during coarse initialization */
                    CyIntDisable(CapSense_ISR_NUMBER);
                    /* Enable power to sub-blocks */
                    CY_SET_REG32(CapSense_HSCMP_PTR, CapSense_HSCMP_EN_MASK);

                    #if (CapSense_ENABLE == CapSense_BLOCK_OFF_AFTER_SCAN_EN)
                        CY_SET_REG32(CapSense_CONFIG_PTR, CapSense_bistConfigCsd |
                                                                  CapSense_CONFIG_SENSE_EN_MASK |
                                                                  CapSense_CONFIG_ENABLE_MASK);

                        /* Wait until CSD block power is ON */
                        CyDelayCycles(CapSense_BIST_BLOCK_ON_DELAY);
                    #else
                        /* Wait until HSCMP power is ON */
                        CyDelayCycles(CapSense_BIST_HSCMP_ON_DELAY);
                    #endif /* (CapSense_ENABLE == CapSense_BLOCK_OFF_AFTER_SCAN_EN) */

                    if (CapSense_bistHwConfig != CapSense_BIST_HW_EXT_CAP_E)
                    {
                        /* Precharging Cmod and Csh */
                        CapSense_BistCmodPrecharge();
                    }

                    /* Trigger Scan */
                    CapSense_BistTriggerScan();

                #else
                    /* Third-generation HW block section */
                    uint8 interruptState;

                    interruptState = CyEnterCriticalSection();

                    /* Enable the CSD block. Disconnect the Vref buffer from AMUX */
                    #if ((CapSense_CSH_PRECHARGE_IO_BUF == CapSense_CSD_CSH_PRECHARGE_SRC) &&\
                         (CapSense_ENABLE == CapSense_CSD_SHIELD_TANK_EN))
                        CY_SET_REG32(CapSense_CONFIG_PTR, CapSense_bistConfigCsd | CapSense_BIST_CMOD_PRECHARGE_CONFIG_CSD_EN);
                    #else
                        CY_SET_REG32(CapSense_CONFIG_PTR, CapSense_bistConfigCsd | CapSense_BIST_CTANK_PRECHARGE_CONFIG_CSD_EN);
                    #endif

                    #if (((CapSense_ENABLE == CapSense_CSD_EN) || (CapSense_ENABLE == CapSense_CSX_EN)) &&\
                         ((0u != CapSense_TST_SNS_CAP_EN) || (0u != CapSense_TST_SH_CAP_EN)))
                        /* Restart the clocks. Scan one cycle to reset the flip-flop for the Direct clock mode */
                        if (CapSense_bistHwConfig != CapSense_BIST_HW_EXT_CAP_E)
                        {
                            CapSense_BistClockRestart();
                        }
                    #endif /* (((CapSense_ENABLE == CapSense_CSD_EN) || (CapSense_ENABLE == CapSense_CSX_EN)) &&\
                               ((0u != CapSense_TST_SNS_CAP_EN) || (0u != CapSense_TST_SH_CAP_EN))) */

                    /* Pre-charging Cmod and Csh */
                    CapSense_BistCmodPrecharge();
                    /* Trigger Scan */
                    CapSense_BistTriggerScan();

                    CyExitCriticalSection(interruptState);
                #endif /* (CapSense_ENABLE == CapSense_CSDV2) */
            }


            /*******************************************************************************
            * Function Name: CapSense_BistConfigClock
            ****************************************************************************//**
            *
            * \brief
            *   Sets the sensor clock frequency for BIST scans
            *
            * \details
            *   For a sensor or shield capacitance measurement sets a calculated value of
            *   the sensor clock frequency and for an external capacitance measurement sets
            *   the value for a one period scan.
            *
            *******************************************************************************/
            static void CapSense_BistConfigClock(uint32 snsClk)
            {
                #if (CapSense_ENABLE == CapSense_CSDV2)
                    uint32 newRegValue;

                    /*
                     * Set divider value for sense clock.
                     * 1u is subtracted from snsClk because SENSE_DIV value 0 corresponds
                     * to dividing by 1.
                     */
                    newRegValue = (snsClk - 1u) & CapSense_SENSE_PERIOD_SENSE_DIV_MASK;
                    CY_SET_REG32(CapSense_SENSE_PERIOD_PTR, newRegValue);
                #else
                    /* Set a sense clock divider */
                    CapSense_SsSetSnsClockDivider(snsClk);
                #endif /* (CapSense_ENABLE == CapSense_CSDV2) */
            }


            /*******************************************************************************
            * Function Name: CapSense_BistSetIdac
            ****************************************************************************//**
            *
            * \brief
            *   This internal function changes the IDACs values for BIST scan.
            *
            * \details
            *   The functions disables the compensation IDAC if enabled and sets
            *   desired Idac value.
            *
            * \param idacValue
            *   The Idac code to be set.
            *
            *******************************************************************************/
            static void CapSense_BistSetIdac(uint32 idacValue)
            {
                #if ((CapSense_ENABLE != CapSense_CSDV2) && \
                    (CapSense_ENABLE != CapSense_CSD_IDAC_COMP_EN))
                    uint32 regValue;
                    uint8  interruptState;
                #endif

                #if (CapSense_ENABLE == CapSense_CSDV2)
                    CY_SET_REG32(CapSense_CSD_IDACA_PTR, CapSense_BIST_IDACA_CFG | (idacValue & CapSense_IDAC_MOD_VAL_MASK));
                    #if((CapSense_ENABLE == CapSense_ADC_EN) || (CapSense_ENABLE == CapSense_CSD_IDAC_COMP_EN))
                        CY_SET_REG32(CapSense_CSD_IDACB_PTR, CapSense_BIST_IDACB_CFG);
                    #endif
                #else
                    #if (CapSense_ENABLE == CapSense_CSD_IDAC_COMP_EN)
                        CY_SET_REG32(CapSense_IDAC_PTR, CapSense_BIST_IDAC_CFG | (idacValue & CapSense_BIST_IDAC_MOD_VAL_MASK));
                    #else
                        interruptState = CyEnterCriticalSection();
                        regValue = CY_GET_REG32(CapSense_IDAC_PTR);
                        regValue &= ~(CapSense_BIST_IDAC_MOD_MASK);
                        regValue |= (CapSense_BIST_IDAC_CFG | (idacValue & CapSense_BIST_IDAC_MOD_VAL_MASK));
                        CY_SET_REG32(CapSense_IDAC_PTR, regValue);
                        CyExitCriticalSection(interruptState);
                    #endif /* (CapSense_ENABLE == CapSense_CSD_IDAC_COMP_EN) */
                #endif /* (CapSense_ENABLE == CapSense_CSDV2) */
            }


            /*******************************************************************************
            * Function Name: CapSense_BistCmodPrecharge
            ****************************************************************************//**
            *
            * \brief
            *  This function initializes the Cmod charging to Vref.
            *
            * \details
            *  For Third-generation HW block:
            *  The function initializes the Cmod charging to Vref.
            *  Then it waits until Cmod is completely charged
            *  to Vref to have stable raw counts. The Software Watchdog Counter is implemented to
            *  prevent project hanging.
            *
            *  For Fourth-generation HW block:
            *  Coarse initialization for CMOD and Cch.
            *  The coarse initialization is performed by HSCOMP.
            *  The HSCOMP monitors the Cmod voltage via the Cmod sense path
            *  and charges the Cmod using the HCAV switch via CSDBUS-A, AMUXBUS-A
            *  and static connection of Cmod to AMUXBUS-A.
            *
            *******************************************************************************/
            static void CapSense_BistCmodPrecharge(void)
            {
                #if (CapSense_ENABLE == CapSense_CSDV2)
                    /* Fourth-generation HW block section */

                    volatile uint32 watchdogCounter;
                    uint32 nonVolWDC;

                    #if ((CapSense_ENABLE == CapSense_CSD_SHIELD_EN) && (CapSense_ENABLE == CapSense_CSD_EN))
                        #if(0u != CapSense_CSD_SHIELD_TANK_EN)
                            uint32 tmpRegValue;
                        #endif
                    #endif

                    CY_SET_REG32(CapSense_CSDCMP_PTR, CapSense_BIST_CSDCMP_INIT);
                    CY_SET_REG32(CapSense_SW_FW_MOD_SEL_PTR,  CapSense_BIST_SW_FW_MOD_SEL_INIT);
                    CY_SET_REG32(CapSense_SW_FW_TANK_SEL_PTR, CapSense_BIST_SW_FW_TANK_SEL_INIT);
                    CY_SET_REG32(CapSense_SW_SHIELD_SEL_PTR,  CapSense_BIST_SW_SHIELD_SEL_INIT);

                    /* Connect CMOD to (sense path) to HSCOMP: HMPM or HMPS or HMPT switches depend on Cmod connects to certain pad */
                    CY_SET_REG32(CapSense_SW_HS_P_SEL_PTR, CapSense_BIST_SW_HS_P_SEL_COARSE);
                    CY_SET_REG32(CapSense_SW_HS_N_SEL_PTR, CapSense_SW_HS_N_SEL_SW_HCRH_STATIC_CLOSE);

                    #if ((CapSense_ENABLE == CapSense_CSD_SHIELD_EN) && (CapSense_ENABLE == CapSense_CSD_EN))
                        #if(CapSense_ENABLE == CapSense_CSD_SHIELD_TANK_EN)
                            /* Verification of the bist mode */
                            if (CapSense_BIST_HW_SNS_CAP_E == CapSense_bistHwConfig)
                            {
                                /* Connect CTANK to AMUXBUS-A */
                                tmpRegValue = CY_GET_REG32(CapSense_CSH_HSIOM_PTR);
                                tmpRegValue &= (uint32)(~(uint32)CapSense_CSH_HSIOM_MASK);
                                tmpRegValue |= (CapSense_HSIOM_SEL_AMUXA << CapSense_CSH_HSIOM_SHIFT);
                                CY_SET_REG32(CapSense_CSH_HSIOM_PTR, tmpRegValue);
                            }
                        #endif /* (CapSense_ENABLE == CapSense_CSD_SHIELD_TANK_EN) */
                    #endif /* ((CapSense_ENABLE == CapSense_CSD_SHIELD_EN) && (CapSense_ENABLE == CapSense_CSD_EN)) */

                    CY_SET_REG32(CapSense_SW_RES_PTR, CapSense_BIST_SW_RES_INIT);

                    /* Clear all interrupt pending requests */
                    CY_SET_REG32(CapSense_INTR_PTR, CapSense_INTR_SET_MASK);
                    (void)CY_GET_REG32(CapSense_INTR_PTR);

                    /* Unmask INIT interrupt (enable interrupt) */
                    CY_SET_REG32(CapSense_INTR_MASK_PTR, CapSense_INTR_MASK_CLEAR_MASK);

                    /* Start SEQUENCER for coarse initialization for Cmod */
                    CY_SET_REG32(CapSense_SEQ_START_PTR, CapSense_SEQ_START_SEQ_MODE_MASK |
                                                                 CapSense_SEQ_START_START_MASK);

                    /* Init Watchdog Counter to prevent a hang */
                    watchdogCounter = CapSense_BIST_PRECHARGE_WATCHDOG_CYCLES_NUM;
                    nonVolWDC = watchdogCounter;
                    /* Wait for INTR.INIT goes high */
                    while((0u == (CY_GET_REG32(CapSense_INTR_PTR) & CapSense_INTR_INIT_MASK)) && (0u != nonVolWDC))
                    {
                        watchdogCounter--;
                        nonVolWDC = watchdogCounter;
                    }

                    if (0u == watchdogCounter)
                    {
                        /* Set sequencer to idle state if coarse initialization fails */
                        CY_SET_REG32(CapSense_SEQ_START_PTR, CapSense_SEQ_START_ABORT_MASK);
                    }

                    /* Set scanning configuration for switches */
                    CY_SET_REG32(CapSense_SW_FW_MOD_SEL_PTR, CapSense_BIST_SW_FW_MOD_SEL_SCAN);
                    CY_SET_REG32(CapSense_SW_FW_TANK_SEL_PTR, CapSense_BIST_SW_FW_TANK_SEL_SCAN);

                #else /* (CapSense_ENABLE == CapSense_CSDV2) */

                    uint32 newRegValue;
                    volatile uint32 watchdogCounter;
                    uint32 nonVolWDC;
                    uint16 fineInitTimeUs;
                    const uint32 fineInitiCyclesNum = 5u;

                    /* Save the current IDAC configuration */
                    newRegValue = CY_GET_REG32(CapSense_IDAC_PTR);

                    /* Wait until Cmod discharges below Vref */
                    CyDelayCycles(CapSense_CMOD_DISCHARGE_CYCLES);

                    /* Set the IDAC configuration for fast Cmod pre-charge to Vref */
                    CY_SET_REG32(CapSense_IDAC_PTR, CapSense_CSD_IDAC_PRECHARGE_CONFIG);

                    /* Initialize the watchdog counter to prevent a hang */
                    watchdogCounter = CapSense_BIST_PRECHARGE_WATCHDOG_CYCLES_NUM;
                    nonVolWDC = watchdogCounter;

                    /* The Sense Clock divider is multiplied by 2 to take into account the DIV_BY_2
                     * divider that is embedded into the CSD HW block.
                    */
                    fineInitTimeUs = CapSense_dsRam.selfTestConfig.snsCapSnsClkDivider * 2u;
                    #if(CapSense_ENABLE != CapSense_IS_M0S8PERI_BLOCK)
                        fineInitTimeUs *= CapSense_dsRam.selfTestConfig.snsCapModClkDivider;
                    #endif
                    fineInitTimeUs = (uint16)(((fineInitiCyclesNum * fineInitTimeUs) / CYDEV_BCLK__HFCLK__MHZ) + 1u);

                    /* Wait until Cmod reaches Vref */
                    while((0u == (CY_GET_REG32(CapSense_STATUS_PTR) & CapSense_STATUS_SAMPLE)) && (0u != nonVolWDC))
                    {
                        watchdogCounter--;
                        nonVolWDC = watchdogCounter;
                    }

                    /* Enable the sense modulator output */
                    CY_SET_REG32(CapSense_CONFIG_PTR, CapSense_bistConfigCsd | CapSense_BIST_CTANK_PRECHARGE_CONFIG_CSD_EN | CapSense_CONFIG_SENSE_EN_MASK);

                    /* Restore the current IDAC configuration */
                    CY_SET_REG32(CapSense_IDAC_PTR, newRegValue);

                    CyDelayUs(fineInitTimeUs);

                    CyIntEnable(CapSense_ISR_NUMBER);

                #endif /* (CapSense_ENABLE == CapSense_CSDV2) */
            }


            /*******************************************************************************
            * Function Name: CapSense_WaitEndOfScan
            ****************************************************************************//**
            *
            * \brief
            *   Waits the end of scan for capacitance measurements.
            *
            * \details
            *   Apart the waiting end of scan the function sets a watchdog to prevent
            *   a hangout
            *
            *******************************************************************************/
            static void CapSense_WaitEndOfScan(uint32 timeout)
            {
                volatile uint32 watchdogCounter = timeout;
                uint32 nonVolWDC = watchdogCounter;
                while (((*(volatile uint8 *)&CapSense_dsRam.status & CapSense_SW_STS_BUSY) != 0u) &&
                       (0u != nonVolWDC))
                {
                    watchdogCounter--;
                    nonVolWDC = watchdogCounter;
                }
                (void)watchdogCounter;
            }


            /*******************************************************************************
            * Function Name: CapSense_BistTriggerScan
            ****************************************************************************//**
            *
            * \brief
            *  This function triggers the scanning.
            *
            * \details
            *  For Third-generation HW block:
            *  Writes a resolution to start scanning.
            *
            *  For Fourth-generation HW block:
            *  Trigger the fine initialization (scan some dummy cycles only for sensor or
            *  shield capacitance meterings) and start sampling.
            *  For the fine initialization and sampling the measured capacitance is charged
            *  from Idac and discharged to ground using the switches in GPIO cell.
            *  The CSDCOMP monitors voltage on the measured capacitance using the sense path
            *  and charges it to Vref using IDAC.
            *
            *******************************************************************************/
            static void CapSense_BistTriggerScan(void)
            {
                #if (CapSense_ENABLE == CapSense_CSDV2)
                    /* Fourth-generation HW block section */
                    volatile uint32 watchdogCounter;
                    uint32 nonVolWDC;

                    /* Clear previous interrupts */
                    CY_SET_REG32(CapSense_INTR_PTR, CapSense_INTR_SET_MASK);
                    (void)CY_GET_REG32(CapSense_INTR_PTR);

                    /* Clear pending interrupts  */
                    CyIntClearPending(CapSense_ISR_NUMBER);
                    /* Enable the CSD interrupt */
                    CyIntEnable(CapSense_ISR_NUMBER);

                    CY_SET_REG32(CapSense_SW_HS_P_SEL_PTR, CapSense_BIST_HS_P_SEL_SCAN);
                    #if (((CapSense_ENABLE == CapSense_CSD_EN) || (CapSense_ENABLE == CapSense_CSX_EN)) &&\
                         (0u != CapSense_TST_SNS_CAP_EN))
                        /* Verification of the bist mode */
                        if (CapSense_BIST_HW_SNS_CAP_E == CapSense_bistHwConfig)
                        {
                            #if (CapSense_ENABLE == CapSense_CSD_SHIELD_EN)
                                CY_SET_REG32(CapSense_SW_SHIELD_SEL_PTR,  CapSense_BIST_SW_SHIELD_SEL_CFG);
                                #if (CapSense_ENABLE == CapSense_CSD_SHIELD_TANK_EN)
                                    CapSense_BistEnableShieldTank();
                                #endif /* (CapSense_ENABLE == CapSense_CSD_SHIELD_TANK_EN) */
                            #else
                                CY_SET_REG32(CapSense_SW_SHIELD_SEL_PTR, CapSense_STATIC_OPEN);
                                CY_SET_REG32(CapSense_SW_HS_P_SEL_PTR, CapSense_STATIC_OPEN);
                            #endif /* (CapSense_ENABLE == CapSense_CSD_SHIELD_EN) */
                        }
                        else
                        {
                            CY_SET_REG32(CapSense_SW_SHIELD_SEL_PTR, CapSense_STATIC_OPEN);
                            CY_SET_REG32(CapSense_SW_HS_P_SEL_PTR,   CapSense_STATIC_OPEN);
                        }
                    #else
                        CY_SET_REG32(CapSense_SW_SHIELD_SEL_PTR, CapSense_STATIC_OPEN);
                        CY_SET_REG32(CapSense_SW_HS_P_SEL_PTR,   CapSense_STATIC_OPEN);
                    #endif /* (((CapSense_ENABLE == CapSense_CSD_EN) || (CapSense_ENABLE == CapSense_CSX_EN)) &&\
                               (0u != CapSense_TST_SNS_CAP_EN)) */

                    CY_SET_REG32(CapSense_SW_RES_PTR, CapSense_BIST_SW_RES_SCAN);

                    /* Enable CSDCMP */
                    if (CapSense_BIST_HW_EXT_CAP_E == CapSense_bistHwConfig)
                    {
                        CY_SET_REG32(CapSense_CSDCMP_PTR,         CapSense_CSDCMP_TST_CAP_MEAS);
                        CY_SET_REG32(CapSense_SW_FW_MOD_SEL_PTR,  CapSense_STATIC_OPEN);
                        CY_SET_REG32(CapSense_SW_FW_TANK_SEL_PTR, CapSense_STATIC_OPEN);
                    }
                    else
                    {
                        CY_SET_REG32(CapSense_CSDCMP_PTR, CapSense_CSDCMP_CSDCMP_EN_MASK);
                    }

                    /* Verification of the bist mode */
                    if (CapSense_BIST_HW_SNS_CAP_E == CapSense_bistHwConfig)
                    {
                        #if (CapSense_ENABLE == CapSense_CSD_SHIELD_EN)
                            CY_SET_REG32(CapSense_HSCMP_PTR, CapSense_BIST_HSCMP_SCAN_MASK);
                        #else
                            /* Disable HSCOMP during the sampling phase when shield is disabled */
                            CY_SET_REG32(CapSense_HSCMP_PTR, 0uL);
                        #endif /* (CapSense_DISABLE == CapSense_CSD_SHIELD_EN) */
                    }
                    else
                    {
                        /* Disable HSCOMP during the sampling phase when shield is disabled */
                        CY_SET_REG32(CapSense_HSCMP_PTR, 0uL);
                    }
                    /* Enable SAMPLE interrupt */
                    CY_SET_REG32(CapSense_INTR_MASK_PTR, CapSense_INTR_MASK_SAMPLE_MASK);

                    /* Init Watchdog Counter to prevent a hang */
                    watchdogCounter = CapSense_BIST_PRECHARGE_WATCHDOG_CYCLES_NUM;
                    nonVolWDC = watchdogCounter;
                    /* Wait for IDLE state of the Fourth-generation HW block sequencer */
                    while((0u != (CapSense_STAT_SEQ_REG & CapSense_STAT_SEQ_SEQ_STATE_MASK)) && (0u != nonVolWDC))
                    {
                        watchdogCounter--;
                        nonVolWDC = watchdogCounter;
                    }

                    /* Start SEQUENCER for fine initialization scan for Cmod and then for normal scan */
                    CY_SET_REG32(CapSense_SEQ_START_PTR, CapSense_SEQ_START_AZ0_SKIP_MASK |
                                                                 CapSense_SEQ_START_AZ1_SKIP_MASK |
                                                                 CapSense_SEQ_START_START_MASK);

                #else

                    /* Third-generation HW block section */

                    /* Set a resolution to the counter register to start scanning */
                    CY_SET_REG32(CapSense_COUNTER_PTR, CapSense_bistCounterResolution);

                #endif /* (CapSense_ENABLE == CapSense_CSDV2) */
            }

            #if (((CapSense_ENABLE == CapSense_CSD_EN) || (CapSense_ENABLE == CapSense_CSX_EN)) &&\
                 ((CapSense_ENABLE == CapSense_TST_SNS_CAP_EN) || (CapSense_ENABLE == CapSense_TST_SH_CAP_EN)))

                #if (CapSense_ENABLE == CapSense_TST_SNS_CAP_EN)
                    #if ((CapSense_ENABLE == CapSense_CSD_SHIELD_EN) &&\
                         (CapSense_ENABLE == CapSense_CSD_SHIELD_TANK_EN))
                        /*******************************************************************************
                        * Function Name: CapSense_BistEnableShieldTank
                        ****************************************************************************//**
                        *
                        * \brief
                        *   This function configures Csh pin
                        *
                        * \details
                        *   The function updates the CSH_HSIOM register to connect Csh to AMUXBUS-B.
                        *   For Third-generation HW block it additionally sets Strong drive mode for Csh
                        *   and applies High level to Csh pin when precharge is set to IO buffer.
                        *
                        *******************************************************************************/
                        static void CapSense_BistEnableShieldTank(void)
                        {
                            /* Update the CSH_HSIOM register to connect Csh to AMUXBUS-B */
                            CapSense_WriteBitsSafe(CapSense_CSH_HSIOM_PTR, CapSense_CSH_HSIOM_MASK,
                                (uint32)(CapSense_HSIOM_SEL_AMUXB << CapSense_CSH_HSIOM_SHIFT));
                            #if ((CapSense_CSH_PRECHARGE_IO_BUF == CapSense_CSD_CSH_PRECHARGE_SRC) &&\
                                 (CapSense_DISABLE == CapSense_CSDV2))
                                /* Set Strong drive mode for Csh */
                                CapSense_WriteBitsSafe(CapSense_CSH_PC_PTR,
                                            (uint32)(CapSense_CSH_PC_MASK << CapSense_CSH_PC_SHIFT),
                                            (uint32)(CapSense_CSH_PC_STRONG_MODE << CapSense_CSH_PC_SHIFT));
                                /* Appliy High level to Csh pin for Csh */
                                CapSense_WriteBitsSafe(CapSense_CSH_DR_PTR, (uint32)(CapSense_DR_MASK << CapSense_CSH_SHIFT), CapSense_CSH_DR_CONFIG);
                            #else
                                /* Set output port register to 0 to connect to GND */
                                CapSense_WriteBitsSafe(CapSense_CSH_DR_PTR, (uint32)(CapSense_DR_MASK << CapSense_CSH_SHIFT), 0u);
                            #endif
                        }
                    #endif /* ((CapSense_ENABLE == CapSense_CSD_SHIELD_EN) &&\
                               (CapSense_ENABLE == CapSense_CSD_SHIELD_TANK_EN)) */


                    /*******************************************************************************
                    * Function Name: CapSense_GetSensorCapacitance
                    ****************************************************************************//**
                    *
                    * \brief
                    *  Measures the specified widget/sensor capacitance.
                    *
                    * \details
                    *  This function measures the capacitance of the specified sensor
                    *  element in femtofarads.
                    *
                    *  For a CSX sensor, measurement is done on either Rx or Tx electrodes.
                    *  For a CSD sensor, measurement is done on a sensor (refer to the
                    *  sensorElement description). If the specified sensor element is
                    *  a ganged one, the capacitance is measured for all the electrodes
                    *  that belong to the sensor.
                    *
                    *  For the latest measurement, the result is stored in the
                    *  CapSense_RAM_SNS_CP_STRUCT data structure and
                    *  the CapSense_<WidgetName>_PTR2SNS_CP_VALUE register
                    *  is the pointer to the array with the measured
                    *  capacitance of all sensors in the widget.
                    *
                    *  In addition to the measuring sensor capacitance, this function
                    *  is used to identify various fault conditions with sensors such
                    *  as electrically opened sensors. For example, the PCB track is broken
                    *  or shorted to other nodes in the system - in all of these conditions,
                    *  the sensor capacitance is changed which can be compared
                    *  against pre determined capacitance for the sensor to detect a
                    *  fault condition.
                    *
                    *  This function must not be called if the Component is in
                    *  the busy state.
                    *
                    *  Capacitance is measured independently of sensor scan configuration.
                    *  To measure sensor capacitance, the CSD sensing method is used and
                    *  the capacitance is calculated as follows:
                    *
                    *  Cs = R * IGain * ICode / ((2^Res - 1) * Vref * SnsClk)
                    *
                    *  where (default value):
                    *  * Cs - the sensor capacitance.
                    *  * R - the measured raw count value.
                    *  * IGain - idac gain
                    *  * ICode - idac code (Compensation IDAC is disabled).
                    *  * Res - the scanning resolution.
                    *  * Vref - the reference voltage.
                    *  * SnsClk - the sensor clock frequency.
                    *
                    *  The measurement is performed differently for the 3rd and 4th generation
                    *  CSD HW blocks.
                    *
                    *  * 3rd generation CSD HW block:
                    *    The function performs a successive approximation search algorithm
                    *    to find appropriate IDAC code for the sensor in the specified widget
                    *    that provides a raw count to the target of the 85% level
                    *    of the raw count maximum value.
                    *    Then Cs is calculated. If the raw count is within +/- 10% of the
                    *    desired target the function returns through measurementStatusPtr
                    *    CapSense_TST_MEASUREMENT_SUCCESS, otherwise
                    *    CapSense_TST_MEASUREMENT_LOW_LIMIT or
                    *    CapSense_TST_MEASUREMENT_HIGH_LIMIT is returned.
                    *    The measured Cp range for the 3rd generation CSD HW block is from 5 pF
                    *    to 65 pF.
                    *    The default meaurement parameter values are:
                    *    * IGain = 1.2 uA
                    *    * Res = 12 bits
                    *    * Vref = 1.2 V
                    *    * SnsClk = 1.5 MHz
                    *
                    *  * 4th generation CSD HW block:
                    *    The function performs up to 4 scans
                    *    to reach the raw count in a range between 7.5% and 45% of the maximum
                    *    value (2^Res - 1).
                    *    If a raw count is less than 7.5% of the maximum limit (2^Res - 1), the
                    *    function returns CapSense_TST_LOW_LIMIT through the
                    *    measurementStatus pointer.
                    *    If a raw count is between 7.5% and 45% of the maximum, the function
                    *    calculates the sensor capacitance, updates the register map and
                    *    returns CapSense_TST_SUCCESS. If a raw count is above 45% of
                    *    the maximum, the function measures again with a 4x increased
                    *    current (I), and repeats the measurement until the raw count is
                    *    within 7.5% to 45% of the maximum value or gets 4x bigger.
                    *    The minimum measurable input by this function for the 4th generation
                    *    CSD HW block is 1pF and the
                    *    maximum is 384pF limited by the RC time constant
                    *    (Cs < 1 / (2*5*SnsClk*R), where R is the total sensor series
                    *    resistance which includes on-chip GPIO resistance ~500 Ohm and
                    *    external series resistance).
                    *    The default meaurement parameter values are:
                    *    * IGain = 1.2 uA
                    *    * Res = 12 bits
                    *    * Vref = 1.2 V
                    *    * SnsClk = 375 kHz
                    *
                    *  The measurement accuracy for both generation is about 15%.
                    *
                    *  By default, sensors that are not being measured are configured to
                    *  strong-drive-low during the measurement. This state can be changed to
                    *  High-Z or Shield using the CapSense_SetInactiveElectrodeState()
                    *  function. CSD and CSX sensors have independent parameters to configure
                    *  inactive electrode states.
                    *
                    *  A Cmod capacitor is required for the measurement. If a dedicated Cmod
                    *  is not available (e.g. the design has CSX widgets only), CintA and CintB
                    *  capacitors are combined by the Component to form Cmod.
                    *
                    *  The sensor measurement is done on all the sensors using
                    *  the CapSense_RunSelfTest() function along with
                    *  the CapSense_TST_SNS_CAP mask. The measurement operation
                    *  requires reconfiguration of the hardware, hence measurement of
                    *  all sensors together is recommended to avoid hardware reconfiguration
                    *  for optimized firmware execution.
                    *
                    * \param widgetId
                    *  Specifies the ID number of the widget to be processed.
                    *  A macro for the widget ID is in the CapSense Configuration
                    *  header file defined as CapSense_<WidgetName>_WDGT_ID.
                    *
                    * \param sensorElement
                    *  Specifies the ID of the sensor element within the widget to be measured.
                    *
                    *  For the CSD widgets, sensorElement is the sensor ID and is in
                    *  the CapSense Configuration header file defined as:
                    *  * CapSense_<WidgetName>_SNS<SensorNumber>_ID.
                    *
                    *  For the CSX widgets, sensorElement is defined either as Rx ID or Tx ID.
                    *  The first Rx in a widget corresponds to sensorElement = 0, the
                    *  second Rx in a widget corresponds to sensorElement = 1, and so on.
                    *  The last Tx in a widget corresponds to
                    *  sensorElement = (RxNum + TxNum - 1). Macros for Rx and Tx IDs can
                    *  be found in the CapSense Configuration header file defined as:
                    *  * CapSense_<WidgetName>_RX<RXNumber>_ID
                    *  * CapSense_<WidgetName>_TX<TXNumber>_ID.
                    *
                    * \param measurementStatusPtr
                    *  Specifies the pointer to the CapSense_TST_MEASUREMENT_STATUS_ENUM
                    *  variable where the result of the function execution is stored:
                    *  - CapSense_TST_MEASUREMENT_SUCCESS - The measurement completes
                    *                                        successfully, the result is valid.
                    *  - CapSense_TST_MEASUREMENT_BAD_PARAM - The input parameter is invalid.
                    *  - CapSense_TST_MEASUREMENT_LOW_LIMIT - The measured capacitance is below
                    *                                         the minimum possible value.
                    *                                         The measurement result is invalid.
                    *                                         It is possible that the sensor was
                    *                                         shorted to VDD or a sensor PCB track
                    *                                         was broken (open sensor).
                    *  - CapSense_TST_MEASUREMENT_HIGH_LIMIT - The measured capacitance is above the
                    *                                          maximum possible value.
                    *                                          The measurement result is invalid.
                    *                                          It is possible that the sensor was
                    *                                          shorted to GND.
                    *  - CapSense_TST_MEASUREMENT_ERROR - an unexpected fault occurred during
                    *                                     the measurement, the measurement may need
                    *                                     to be repeated.
                    *
                    * \return
                    *  Returns the capacitance of a sensor element in femtofarads. The status
                    *  of the measurement is returned through the measurementStatus pointer.
                    *  If measurementStatus is not equal to CapSense_TST_MEASUREMENT_SUCCESS,
                    *  a fail occurs and the returned capacitance should be ignored.
                    *  For more detail, refer to the measurementStatus description.
                    *
                    *******************************************************************************/
                    uint32 CapSense_GetSensorCapacitance(uint32 widgetId,
                                                         uint32 sensorElement,
                                                         CapSense_TST_MEASUREMENT_STATUS_ENUM * measurementStatusPtr)
                    {
                        uint32 totalNumSnsElements;
                        CapSense_BIST_IO_CONFIG_ENUM bistIoConfig;

                        uint32 result = CapSense_TST_BAD_PARAM;
                        CapSense_FLASH_WD_STRUCT const *ptrFlashWidget = &CapSense_dsFlash.wdgtArray[widgetId];
                        CapSense_SENSE_METHOD_ENUM senseMethod = CapSense_GET_SENSE_METHOD(ptrFlashWidget);

                        if ((CapSense_SENSE_METHOD_CSX_E == senseMethod) || (CapSense_SENSE_METHOD_CSD_E == senseMethod))
                        {
                            /* Get a total number of widget elements: for CSX it is numRows + numCols, for CSD it is totalNumSns */
                            #if (CapSense_ENABLE == CapSense_CSD_CSX_EN)
                                if (CapSense_SENSE_METHOD_CSX_E == senseMethod)
                                {
                                    totalNumSnsElements = (uint32)ptrFlashWidget->numRows + ptrFlashWidget->numCols;
                                }
                                else
                                {
                                    totalNumSnsElements = (uint32)ptrFlashWidget->totalNumSns;
                                }
                            #elif (CapSense_ENABLE == CapSense_CSX_EN)
                                totalNumSnsElements = (uint32)ptrFlashWidget->numRows + ptrFlashWidget->numCols;
                            #else
                                totalNumSnsElements = (uint32)ptrFlashWidget->totalNumSns;
                            #endif

                            if ((CapSense_TOTAL_WIDGETS > widgetId) && (totalNumSnsElements > sensorElement))
                            {
                                CapSense_SsSwitchSensingMode(CapSense_SENSE_METHOD_BIST_E);
                                CapSense_BistSwitchHwConfig(CapSense_BIST_HW_SNS_CAP_E);

                                switch (senseMethod)
                                {
                                #if (CapSense_ENABLE == CapSense_CSD_EN)
                                    case CapSense_SENSE_METHOD_CSD_E:
                                         bistIoConfig = CapSense_IscToBistIOConfig((uint32)CapSense_dsRam.selfTestConfig.bistCsdSnsCapISC);
                                         CapSense_bistCurrentISC = bistIoConfig;
                                         CapSense_SwitchAllIOState(bistIoConfig);
                                         result = CapSense_GetSensorCapacitanceExt(widgetId, sensorElement, measurementStatusPtr);
                                        break;
                                #endif
                                #if (CapSense_ENABLE == CapSense_CSX_EN)
                                    case CapSense_SENSE_METHOD_CSX_E:
                                         bistIoConfig = CapSense_IscToBistIOConfig((uint32)CapSense_dsRam.selfTestConfig.bistCsxSnsCapISC);
                                         CapSense_bistCurrentISC = bistIoConfig;
                                         CapSense_SwitchAllIOState(bistIoConfig);
                                         result = CapSense_GetSensorCapacitanceExt(widgetId, sensorElement, measurementStatusPtr);
                                        break;
                                #endif
                                default:
                                    /* Update the measurement status */
                                    *measurementStatusPtr = CapSense_TST_MEASUREMENT_BAD_PARAM;
                                    break;
                                }
                            }
                        }
                        else
                        {
                            /* Update the measurement status */
                            *measurementStatusPtr = CapSense_TST_MEASUREMENT_BAD_PARAM;
                        }

                        return result;
                    }


                    /*******************************************************************************
                    * Function Name: CapSense_GetSensorCapacitanceExt
                    ****************************************************************************//**
                    *
                    * \brief
                    *  The internal function that measures a certain sensor
                    *  (electrode for CSX widgets) capacitance.
                    *
                    * \details
                    *  The function connects a certain sensor (electrode for CSX widgets)
                    *  to the previously configured CapSense HW block by using the CSD mode.
                    *  Then measures a Cp by using the CapSense_GetCapacitanceExt function.
                    *  After measuring the function sets an inactive connection of the sensor
                    *  (electrode for CSX widgets) and stores the Cp value in the corresponding
                    *  register of the CapSense_RAM_SNS_STRUCT.
                    *
                    * \param wdgtId
                    *  Specifies the ID number of the widget to be processed.
                    *
                    * \param sensorElement
                    *  Specifies the ID number of the sensor element within the widget
                    *  which capacitance will be measured.
                    *
                    * \return
                    *  - Cp of a certain capacitor in pF.
                    *
                    *******************************************************************************/
                    static uint32 CapSense_GetSensorCapacitanceExt(uint32 wdgtId,
                                                                   uint32 sensorElement,
                                                                   CapSense_TST_MEASUREMENT_STATUS_ENUM * measurementStatusPtr)
                    {
                        uint32 result;
                        uint32 * cpPtr = CapSense_dsFlash.wdgtArray[wdgtId].ptr2SnsCpArr + sensorElement;

                        /* Connect the specified sensor */
                        CapSense_BistConnectSensor(wdgtId, sensorElement);
                        result = CapSense_GetCapacitanceExt(measurementStatusPtr);
                        /* Disconnect the sensor */
                        CapSense_BistSetInactiveSensorConnection(wdgtId, sensorElement);
                        /* Save cp to data structure */
                        *cpPtr = result;

                        return result;
                    }


                    /*******************************************************************************
                    * Function Name: CapSense_BistConnectSensor
                    ****************************************************************************//**
                    *
                    * \brief
                    *  Connects a sensor (including ganged) port-pin to the HW block via the AMUX
                    *  bus.
                    *
                    * \details
                    *   The function gets the IO configuration registers addresses, their shifts and
                    *   masks from the FLASH_IO_STRUCT object. Basing on this data, it updates the HSIOM and
                    *   PC registers.
                    *
                    * \param
                    *   wdgtIndex Specifies ID of the widget.
                    *   sensorElement Specifies ID of the sensor element in the widget.
                    *
                    *******************************************************************************/
                    static void CapSense_BistConnectSensor(uint32 wdgtId, uint32 sensorElement)
                    {
                        uint32 eltdNum;
                        uint32 eltdIndex;
                        CapSense_FLASH_IO_STRUCT const *ioPtr;
                        CapSense_FLASH_SNS_STRUCT const *curFlashSnsPtr;

                        /* Check ganged sns flag */
                        if (CapSense_GANGED_SNS_MASK == (CapSense_dsFlash.wdgtArray[wdgtId].staticConfig & CapSense_GANGED_SNS_MASK))
                        {
                            curFlashSnsPtr = (CapSense_FLASH_SNS_STRUCT const *)CapSense_dsFlash.wdgtArray[wdgtId].ptr2SnsFlash;
                            curFlashSnsPtr += sensorElement;
                            ioPtr = &CapSense_ioList[curFlashSnsPtr->firstPinId];
                            eltdNum = curFlashSnsPtr->numPins;
                        }
                        else
                        {
                            ioPtr = (CapSense_FLASH_IO_STRUCT const *)CapSense_dsFlash.wdgtArray[wdgtId].ptr2SnsFlash + sensorElement;
                            eltdNum = 1u;
                        }

                        /* Loop through all electrodes of specified sensor */
                        for (eltdIndex = 0u; eltdIndex < eltdNum; eltdIndex++)
                        {
                            /* Set electrode to CSD_SENSE mode to be scanned */
                            CapSense_SetSnsPc(ioPtr, CapSense_TST_PC_HIGHZA);
                            CapSense_SetSnsHsiom(ioPtr, CapSense_TST_HSIOM_CSD_SENSE);
                            ioPtr++;
                        }
                    }


                    /*******************************************************************************
                    * Function Name: CapSense_MeasureAllSensorCapacitance
                    ****************************************************************************//**
                    *
                    * \brief
                    *  The internal function that measures all the sensors Cp capacitance.
                    *
                    * \details
                    *  The function that measures Cp of all the sensors by using
                    *  CapSense_GetSensorCapacitance function.
                    *  The function stores the Cp values in the corresponding BIST data
                    *  structure registers.
                    *
                    * \return
                    *  Returns the status of the measuring process:
                    *  - CYRET_SUCCESS.
                    *
                    *******************************************************************************/
                    static uint32 CapSense_MeasureAllSensorCapacitance(void)
                    {
                        uint32 wdgtId;
                        uint32 sensorElement;
                        uint32 totalNumIoSns;
                        CapSense_SENSE_METHOD_ENUM senseMethod;
                        CapSense_BIST_IO_CONFIG_ENUM bistIoConfig;
                        CapSense_TST_MEASUREMENT_STATUS_ENUM measurementStatus;
                        uint32 result = CYRET_SUCCESS;

                        CapSense_BistSwitchHwConfig(CapSense_BIST_HW_SNS_CAP_E);

                        /* Loop through all the widgets */
                        for (wdgtId = 0u; wdgtId < CapSense_TOTAL_WIDGETS; wdgtId++)
                        {
                            senseMethod = CapSense_GET_SENSE_METHOD(&CapSense_dsFlash.wdgtArray[wdgtId]);
                            if((CapSense_SENSE_METHOD_CSD_E == senseMethod) ||
                               (CapSense_SENSE_METHOD_CSX_E == senseMethod))
                            {
                                switch (senseMethod)
                                {
                                #if (CapSense_ENABLE == CapSense_CSD_EN)
                                    case CapSense_SENSE_METHOD_CSD_E:
                                         bistIoConfig = CapSense_IscToBistIOConfig((uint32)CapSense_dsRam.selfTestConfig.bistCsdSnsCapISC);
                                         CapSense_bistCurrentISC = bistIoConfig;
                                        CapSense_SwitchAllIOState(bistIoConfig);
                                         break;
                                #endif
                                #if (CapSense_ENABLE == CapSense_CSX_EN)
                                    case CapSense_SENSE_METHOD_CSX_E:
                                         bistIoConfig = CapSense_IscToBistIOConfig((uint32)CapSense_dsRam.selfTestConfig.bistCsxSnsCapISC);
                                         CapSense_bistCurrentISC = bistIoConfig;
                                        CapSense_SwitchAllIOState(bistIoConfig);
                                         break;
                                #endif
                                default:
                                    break;
                                }

                                /* Get a total number of widget elements: for CSX it is numRows + numCols, for CSD it is totalNumSns */
                                #if (CapSense_ENABLE == CapSense_CSD_CSX_EN)
                                    if (CapSense_SENSE_METHOD_CSX_E == CapSense_GET_SENSE_METHOD(&CapSense_dsFlash.wdgtArray[wdgtId]))
                                    {
                                        totalNumIoSns = (uint32)CapSense_dsFlash.wdgtArray[wdgtId].numRows + CapSense_dsFlash.wdgtArray[wdgtId].numCols;
                                    }
                                    else
                                    {
                                        totalNumIoSns = (uint32)CapSense_dsFlash.wdgtArray[wdgtId].totalNumSns;
                                    }
                                #elif (CapSense_ENABLE == CapSense_CSX_EN)
                                    totalNumIoSns = (uint32)CapSense_dsFlash.wdgtArray[wdgtId].numRows + CapSense_dsFlash.wdgtArray[wdgtId].numCols;
                                #else
                                    totalNumIoSns = (uint32)CapSense_dsFlash.wdgtArray[wdgtId].totalNumSns;
                                #endif

                                /* Loop through all the sensors */
                                for (sensorElement = 0u; sensorElement < totalNumIoSns; sensorElement++)
                                {
                                    (void)CapSense_GetSensorCapacitanceExt(wdgtId, sensorElement, &measurementStatus);
                                    if (measurementStatus != CapSense_TST_MEASUREMENT_SUCCESS)
                                    {
                                        result |= CapSense_TST_SNS_CAP;
                                    }
                                }
                            }
                        }

                        return result;
                    }


                    /*******************************************************************************
                    * Function Name: CapSense_BistSetInactiveSensorConnection
                    ****************************************************************************//**
                    *
                    * \brief
                    *  Sets an inactive state for a certain sensor (electrode).
                    *
                    * \details
                    *  The function updates appropriate bits in the IO HSIOM and PC registers
                    *  depending on the Inactive sensor connection parameter.
                    *  The function proceeds ganged sensors(electrodes).
                    *
                    * \param wdgtId
                    *  Specifies the ID number of the widget to be processed.
                    *
                    * \param sensorElement
                    *  Specifies the ID number of the sensor element within the widget to be processed.
                    *
                    *******************************************************************************/
                    static void CapSense_BistSetInactiveSensorConnection(uint32 wdgtId, uint32 sensorElement)
                    {
                        uint32 eltdNum;
                        uint32 eltdIndex;

                        uint32 newPinPcVal;
                        uint32 newPinHsiomVal;

                        CapSense_FLASH_IO_STRUCT const *ioPtr;
                        CapSense_FLASH_SNS_STRUCT const *curFlashSnsPtr;

                        /* Check ganged sns flag */
                        if (CapSense_GANGED_SNS_MASK == (CapSense_dsFlash.wdgtArray[wdgtId].staticConfig & CapSense_GANGED_SNS_MASK))
                        {
                            curFlashSnsPtr = (CapSense_FLASH_SNS_STRUCT const *)CapSense_dsFlash.wdgtArray[wdgtId].ptr2SnsFlash;
                            curFlashSnsPtr += sensorElement;
                            ioPtr = &CapSense_ioList[curFlashSnsPtr->firstPinId];
                            eltdNum = curFlashSnsPtr->numPins;
                        }
                        else
                        {
                            ioPtr = (CapSense_FLASH_IO_STRUCT const *)CapSense_dsFlash.wdgtArray[wdgtId].ptr2SnsFlash + sensorElement;
                            eltdNum = 1uL;
                        }

                        switch(CapSense_bistCurrentISC)
                        {
                            case CapSense_BIST_IO_STRONG_E:
                                newPinPcVal = CapSense_TST_PC_STRONG;
                                newPinHsiomVal = CapSense_TST_HSIOM_GPIO;
                                break;
                            case CapSense_BIST_IO_HIGHZA_E:
                                newPinPcVal = CapSense_TST_PC_HIGHZA;
                                newPinHsiomVal = CapSense_TST_HSIOM_GPIO;
                                break;
                            case CapSense_BIST_IO_SHIELD_E:
                                newPinPcVal = CapSense_TST_PC_HIGHZA;
                                newPinHsiomVal = CapSense_TST_HSIOM_CSD_SHIELD;
                                break;
                            default:
                                newPinPcVal = CapSense_TST_PC_STRONG;
                                newPinHsiomVal = CapSense_TST_HSIOM_GPIO;
                                break;
                        }

                        /* Loop through all electrodes of specified sensor */
                        for (eltdIndex = 0uL; eltdIndex < eltdNum; eltdIndex++)
                        {
                            CapSense_SetSnsHsiom(ioPtr, CapSense_TST_HSIOM_GPIO);
                            CapSense_SetSnsPc(ioPtr, newPinPcVal);
                            CapSense_SetSnsHsiom(ioPtr, newPinHsiomVal);

                            ioPtr++;
                        }
                    }
                #endif /* (CapSense_ENABLE == CapSense_TST_SNS_CAP_EN) */


                #if (CapSense_ENABLE == CapSense_TST_SH_CAP_EN)
                    /*******************************************************************************
                    * Function Name: CapSense_GetShieldCapacitance
                    ****************************************************************************//**
                    *
                    * \brief
                    *  Measures the shield electrode capacitance.
                    *
                    * \details
                    *  This function measures the capacitance of the shield electrode and
                    *  returns a result. If the shield consists of several electrodes, the
                    *  total capacitance of all shield electrodes is reported. The measured
                    *  capacitance is stored in the CapSense_SHIELD_CAP_VALUE register
                    *  of the data structure.
                    *
                    *  This function uses an algorithm identical to the sensor capacitance
                    *  measurement (for more detail, refer
                    *  to CapSense_GetSensorCapacitance()).
                    *
                    *  In addition to measuring the shield capacitance, this function is used to
                    *  identify various fault conditions with a shield electrode such as an
                    *  electrically open shield electrode, e.g. the PCB track is broken or
                    *  shorted to other nodes in the system – in all of these conditions,
                    *  this function returns elevated capacitance that can be compared
                    *  against pre-determined capacitance for the shield electrode to
                    *  detect a fault condition.
                    *
                    *  By default, all sensors are configured to Strong-drive-low mode
                    *  while measuring the shield capacitance. This state can be changed to
                    *  High-Z or Shield using the CapSense_SetInactiveElectrodeState()
                    *  function and repeat the measurement.
                    *
                    *  This test can be executed using the CapSense_RunSelfTest()
                    *  function with the CapSense_TST_SH_CAP mask.
                    *
                    * \param measurementStatusPtr
                    *  Specifies the pointer to the CapSense_TST_MEASUREMENT_STATUS_ENUM
                    *  variable where the result of the function execution is stored:
                    *  - CapSense_TST_MEASUREMENT_SUCCESS - The measurement completes
                    *                                       successfully, the result is valid.
                    *  - CapSense_TST_MEASUREMENT_BAD_PARAM - The input parameter is invalid.
                    *  - CapSense_TST_MEASUREMENT_LOW_LIMIT - The measured capacitance is below
                    *                                         the minimum possible value.
                    *                                         The measurement result is invalid.
                    *                                         It is possible that the sensor was
                    *                                         shorted to VDD or a sensor PCB track
                    *                                         was broken (open sensor).
                    *  - CapSense_TST_MEASUREMENT_HIGH_LIMIT - The measured capacitance is above the
                    *                                          maximum possible value.
                    *                                          The measurement result is invalid.
                    *                                          It is possible that the sensor was
                    *                                          shorted to GND.
                    *  - CapSense_TST_MEASUREMENT_ERROR - an unexpected fault occurred during
                    *                                     the measurement, the measurement may need
                    *                                     to be repeated.
                    *
                    * \return
                    *  Returns the capacitance of a shield in femtofarads. The status of
                    *  measurement is returned through the measurementStatus pointer.
                    *  If measurementStatus is not CapSense_TST_MEASUREMENT_SUCCESS, the returned
                    *  value should be ignored. For more details, refer to the measurementStatus
                    *  description.
                    *
                    *******************************************************************************/
                    uint32 CapSense_GetShieldCapacitance(CapSense_TST_MEASUREMENT_STATUS_ENUM * measurementStatusPtr)
                    {
                        uint32 result;
                        CapSense_BIST_IO_CONFIG_ENUM bistIoConfig;

                        CapSense_SsSwitchSensingMode(CapSense_SENSE_METHOD_BIST_E);
                        CapSense_BistSwitchHwConfig(CapSense_BIST_HW_SH_CAP_E);

                        if((0u != CapSense_CSD_TOTAL_SHIELD_COUNT) ||
                           (CapSense_SNS_CONNECTION_SHIELD == CapSense_dsRam.selfTestConfig.bistCsdShCapISC))
                        {
                            bistIoConfig = CapSense_IscToBistIOConfig((uint32)CapSense_dsRam.selfTestConfig.bistCsdShCapISC);
                            if(CapSense_BIST_IO_SHIELD_E == bistIoConfig)
                            {
                                CapSense_SwitchAllIOState(CapSense_BIST_IO_SENSE_E);
                            }
                            else
                            {
                                CapSense_SwitchAllIOState(bistIoConfig);
                                #if (0u != CapSense_CSD_TOTAL_SHIELD_COUNT)
                                    CapSense_SwitchAllShieldsState(CapSense_BIST_IO_SENSE_E);
                                #endif
                            }
                            CapSense_bistCurrentISC = bistIoConfig;

                            result = CapSense_GetCapacitanceExt(measurementStatusPtr);

                            if(CapSense_BIST_IO_SHIELD_E == bistIoConfig)
                            {
                                CapSense_SwitchAllIOState(bistIoConfig);
                            }
                            else
                            {
                                #if (0u != CapSense_CSD_TOTAL_SHIELD_COUNT)
                                    CapSense_SwitchAllShieldsState(bistIoConfig);
                                #endif
                            }

                            /* Save a capacitance value to the data structure */
                            CapSense_dsRam.selfTest.shieldCap = result;
                        }
                        else
                        {
                            result = 0u;
                            *measurementStatusPtr = CapSense_TST_MEASUREMENT_BAD_PARAM;
                        }
                        return result;
                    }


                    #if (0u != CapSense_CSD_TOTAL_SHIELD_COUNT)
                        /*******************************************************************************
                        * Function Name: CapSense_SwitchAllShieldsState
                        ****************************************************************************//**
                        *
                        * \brief
                        *   This internal function changes HSIOM selection and drive mode values for
                        *   all dedicated shield electrodes.
                        *
                        * \details
                        *   The function changes shield electrodes pin functionality and drive mode
                        *   by updating corresponding HSIOM and GPIO port configuration registers.
                        *   The shield electrodes port and pin configuration registers addresses are
                        *   stored in the CapSense_shieldIoList structure.
                        *
                        * \param config Desired shield electrode IO configuration.
                        *
                        *******************************************************************************/
                        static void CapSense_SwitchAllShieldsState(CapSense_BIST_IO_CONFIG_ENUM config)
                        {
                            /* Re-configure the IOs if mode is changed */
                            if (CapSense_bistCurrentISC != config)
                            {
                                switch(config)
                                {
                                    case CapSense_BIST_IO_STRONG_E:
                                    {
                                        CapSense_BistSetAllShieldsHsiomState(CapSense_TST_HSIOM_GPIO);
                                        CapSense_BistSetAllShieldsPcState(CapSense_TST_PC_STRONG);
                                        break;
                                    }
                                    case CapSense_BIST_IO_HIGHZA_E:
                                    {
                                        CapSense_BistSetAllShieldsHsiomState(CapSense_TST_HSIOM_GPIO);
                                        CapSense_BistSetAllShieldsPcState(CapSense_TST_PC_HIGHZA);
                                        break;
                                    }
                                    case CapSense_BIST_IO_SENSE_E:
                                    {
                                        CapSense_BistSetAllShieldsPcState(CapSense_TST_PC_HIGHZA);
                                        CapSense_BistSetAllShieldsHsiomState(CapSense_TST_HSIOM_CSD_SENSE);
                                        break;
                                    }
                                    case CapSense_BIST_IO_SHIELD_E:
                                    {
                                        CapSense_BistSetAllShieldsPcState(CapSense_TST_PC_HIGHZA);
                                        CapSense_BistSetAllShieldsHsiomState(CapSense_TST_HSIOM_CSD_SHIELD);
                                        break;
                                    }
                                    case CapSense_BIST_IO_DEFAULT_E:
                                    {
                                        CapSense_BistSetAllShieldsHsiomState(CapSense_TST_HSIOM_GPIO);
                                        break;
                                    }
                                    default:
                                    {
                                        break;
                                    }
                                }
                                /* Save the configured state */
                                CapSense_bistCurrentISC = config;
                            }
                        }
                    #endif /* (0u != CapSense_CSD_TOTAL_SHIELD_COUNT) */
                #endif /* (CapSense_ENABLE == CapSense_TST_SH_CAP_EN) */

                #if (CapSense_CSDV1)
                    /*******************************************************************************
                    * Function Name: CapSense_GetCapacitanceExt
                    ****************************************************************************//**
                    *
                    *  The internal function that measures a capacitance attached to AMUXBUS.
                    *
                    *  The function measures Cp of a certain sensor or shield by using the
                    *  CSD mode and defined Idac mode, sense clock frequency and resolution.
                    *  The range for sensor measuring is 5 to 250 pF, for shield 10 to 250 pf.
                    *  The function calibrates the Idac for reach a defined target of raw counts.
                    *  If reached Idac will be lower then a CapSense_BIST_MIN_IDAC_VALUE
                    *  the function sets the CapSense_TST_MEASUREMENT_LOW_LIMIT
                    *  measurement status.
                    *  The function stores the Cp value in the corresponding register of
                    *  CapSense_RAM_SNS_STRUCT.
                    *
                    * \param measurementStatusPtr
                    *  Specifies the pointer to the CapSense_TST_MEASUREMENT_STATUS_ENUM
                    *  variable where the result of the function execution is stored:
                    *  - CapSense_TST_MEASUREMENT_SUCCESS - The measurement completes
                    *                                       successfully, the result is valid.
                    *  - CapSense_TST_MEASUREMENT_LOW_LIMIT - The measured capacitance is below
                    *                                         the minimum possible value.
                    *                                         The measurement result is invalid.
                    *                                         It is possible that the sensor was
                    *                                         shorted to VDD or a sensor PCB track
                    *                                         was broken (open sensor).
                    *  - CapSense_TST_MEASUREMENT_HIGH_LIMIT - The measured capacitance is above the
                    *                                          maximum possible value.
                    *                                          The measurement result is invalid.
                    *                                          It is possible that the sensor was
                    *                                          shorted to GND.
                    *
                    * \return
                    *  - Cp of  a certain sensor or shield in pF.
                    *
                    *******************************************************************************/
                    static uint32 CapSense_GetCapacitanceExt(
                                    CapSense_TST_MEASUREMENT_STATUS_ENUM * snsMeasStatusPtr)
                    {
                        CapSense_TST_MEASUREMENT_STATUS_ENUM snsMeasStatus = CapSense_TST_MEASUREMENT_ERROR;
                        uint64 cp;
                        uint32 rawcountMax = (0x01uL << CapSense_dsRam.selfTestConfig.snsCapResolution) - 1uL;
                        uint32 rawCountError = (rawcountMax * CapSense_TST_SNS_CAP_RAW_ERROR) /
                                                                            CapSense_PERCENTAGE_100;
                        uint32 idacCode = 0uL;
                        uint32 idacIdx = 0u;
                        uint32 idacMask = ((uint32)CapSense_BIST_CAL_MIDDLE_BIT << 1uL);
                        uint32 rawCountCalibrTarget = (rawcountMax * CapSense_TST_RAW_TARGET) / CapSense_PERCENTAGE_100;

                        /* Configure sense clock to the BIST sensor measurement fixed value (default - 375 kHz) */
                        CapSense_BistConfigClock((uint32)CapSense_dsRam.selfTestConfig.snsCapSnsClkDivider);
                        /* Configure scanning resolution (default - 12 bit) */
                        CapSense_BistSetScanDuration();
                        /* Initialize raw count for first cycle pass */
                        CapSense_bistRawcount = 0u;
                        /* Finding optimal IDAC and Frequency */
                        while (0uL != idacMask)
                        {
                            /* Decrease IDAC until rawData reaches the calibration target */
                            if (rawCountCalibrTarget > (uint32)CapSense_bistRawcount)
                            {
                                idacCode &= (uint32)(~idacMask);
                            }
                            idacMask >>= 1uL;
                            idacCode |= idacMask;
                            if (0uL == idacCode)
                            {
                                idacCode++;
                            }
                            /*
                            * Setup scan parameters:
                            * - IDAC
                            * - SnsClk
                            * - Resolution (conversion number)
                            */
                            CapSense_BistSetIdacGain(idacCode, idacIdx);
                            /* Perform scanning */
                            CapSense_dsRam.status |= CapSense_SW_STS_BUSY;
                            CapSense_BistStartSample();
                            CapSense_WaitEndOfScan(CapSense_BIST_MEASURE_WATCHDOG_CYCLES_NUM);
                        }
                        /* Check for measurement result status */
                        if (CapSense_bistRawcount < (rawCountCalibrTarget - rawCountError))
                        {
                            snsMeasStatus = CapSense_TST_MEASUREMENT_LOW_LIMIT;
                        }
                        else if (CapSense_bistRawcount > (rawCountCalibrTarget + rawCountError))
                        {
                            snsMeasStatus = CapSense_TST_MEASUREMENT_HIGH_LIMIT;
                        }
                        else
                        {
                            snsMeasStatus = CapSense_TST_MEASUREMENT_SUCCESS;
                        }
                        /*
                        * Capacitance calculation:
                        *   cp(fF) = IdacGain(pA) * IdacCode * Rawcount * 1,000,000 / (MaxRaw * SnsClk(Hz) * Vref(mV))
                        */
                        cp = (uint64)CapSense_idacGainTable[idacIdx].gainValue * (uint64)idacCode;
                        cp *= CapSense_bistRawcount;
                        cp *= CapSense_BIST_NF_TO_FF_FACTOR;
                        cp /= CapSense_dsRam.selfTestConfig.snsCapVrefVoltage;
                        cp /= rawcountMax;
                        cp /= CapSense_dsRam.selfTestConfig.snsCapSnsClkHz;
                        /* Store the measurement status */
                        *snsMeasStatusPtr = snsMeasStatus;

                        return (uint32)cp;
                    }
                #endif /* (CapSense_CSDV1) */

                #if (CapSense_CSDV2)
                    /*******************************************************************************
                    * Function Name: CapSense_GetCapacitanceExt
                    ****************************************************************************//**
                    *
                    * \brief
                    *  The internal function that measures a capacitance attached to AMUXBUS.
                    *
                    * \details
                    *  The function measures Cp of a certain sensor or shield by using the
                    *  CSD mode and defined Idac mode, sense clock frequency and resolution.
                    *  The range for sensor measuring is 1 to 360 pF.
                    *  The function performes up to 4 CSD scans with fixed Idac values
                    *  to reach a defined target of raw counts in range from 7% to 45%
                    *  of the maximum raw count value. This range provides a possibility of
                    *  classical rwcounts formula usage for a capacity calculation.
                    *  The function stores the Cp value in the corresponding register of
                    *  CapSense_RAM_SNS_STRUCT and returns a measurement status.
                    *
                    * \param measurementStatusPtr
                    *  Specifies the pointer to the CapSense_TST_MEASUREMENT_STATUS_ENUM
                    *  variable where the result of the function execution is stored:
                    *  - CapSense_TST_MEASUREMENT_SUCCESS - The measurement completes
                    *                                       successfully, the result is valid.
                    *  - CapSense_TST_MEASUREMENT_LOW_LIMIT - The measured capacitance is below
                    *                                         the minimum possible value.
                    *                                         The measurement result is invalid.
                    *                                         It is possible that the sensor was
                    *                                         shorted to VDD or a sensor PCB track
                    *                                         was broken (open sensor).
                    *  - CapSense_TST_MEASUREMENT_HIGH_LIMIT - The measured capacitance is above the
                    *                                          maximum possible value.
                    *                                          The measurement result is invalid.
                    *                                          It is possible that the sensor was
                    *                                          shorted to GND.
                    *  - CapSense_TST_MEASUREMENT_ERROR - an unexpected fault occurred during
                    *                                     the measurement, the measurement may need
                    *                                     to be repeated.
                    *
                    * \return
                    *  - Cp of  a certain sensor or shield in pF.
                    *
                    *******************************************************************************/
                    static uint32 CapSense_GetCapacitanceExt(CapSense_TST_MEASUREMENT_STATUS_ENUM * snsMeasStatusPtr)
                    {
                        CapSense_TST_MEASUREMENT_STATUS_ENUM snsMeasStatus = CapSense_TST_MEASUREMENT_ERROR;
                        uint64 cp;
                        uint32 rawcountMax = (0x01uL << CapSense_dsRam.selfTestConfig.snsCapResolution) - 1uL;
                        uint32 idacCode = 0uL;
                        uint32 idacIdx = 0u;
                        uint32 i;
                        uint32 counter;
                        uint32 rawCountPromille;

                        /* Configure sense clock to the BIST sensor measurement fixed value (default - 375 kHz) */
                        CapSense_BistConfigClock((uint32)CapSense_dsRam.selfTestConfig.snsCapSnsClkDivider);
                        /* Configure scanning resolution (default - 12 bit) */
                        CapSense_BistSetScanDuration();
                        /* Perform up to 4 scans to measure Cp */
                        for (i = 0u; i < 4u; i++)
                        {
                            /* Store the cycle index to use it outside */
                            counter = i;
                            /* Setup scan parameters: IDAC and Gain */
                            idacCode = CapSense_BistIdacTable[i].code;
                            idacIdx = CapSense_BistIdacTable[i].gainIdx;
                            CapSense_BistSetIdacGain(idacCode, idacIdx);
                            /* Set BUSY status */
                            CapSense_dsRam.status |= CapSense_SW_STS_BUSY;
                            /* Perform scanning */
                            CapSense_BistStartSample();
                            CapSense_WaitEndOfScan(CapSense_BIST_MEASURE_WATCHDOG_CYCLES_NUM);
                            /* Check fo raw counts value */
                            rawCountPromille = (((uint32)CapSense_bistRawcount) * CapSense_BIST_PROMILLE_FACTOR) / rawcountMax;
                            if (rawCountPromille <= CapSense_BIST_CP_CAP_MAX_RAW_PROMILLE)
                            {
                                break;
                            }
                        }
                        /* Check for measurement result status */
                        if ((rawCountPromille < CapSense_BIST_CP_CAP_MIN_RAW_PROMILLE) && (counter == 0u))
                        {
                            snsMeasStatus = CapSense_TST_MEASUREMENT_LOW_LIMIT;
                        }
                        else if ((rawCountPromille > CapSense_BIST_CP_CAP_MAX_RAW_PROMILLE) && (counter == 3u))
                        {
                            snsMeasStatus = CapSense_TST_MEASUREMENT_HIGH_LIMIT;
                        }
                        else if ((rawCountPromille >= CapSense_BIST_CP_CAP_MIN_RAW_PROMILLE) &&
                                 (rawCountPromille <= CapSense_BIST_CP_CAP_MAX_RAW_PROMILLE))
                        {
                            snsMeasStatus = CapSense_TST_MEASUREMENT_SUCCESS;
                        }
                        else
                        {
                            snsMeasStatus = CapSense_TST_MEASUREMENT_ERROR;
                        }
                        /*
                        * Capacitance calculation:
                        *   cp(fF) = IdacGain(pA) * IdacCode * Rawcount * 1,000,000 / (MaxRaw * SnsClk(Hz) * Vref(mV))
                        */
                        cp = (uint64)CapSense_idacGainTable[idacIdx].gainValue * (uint64)idacCode;
                        /* Continue capacitance calculation */
                        cp *= CapSense_bistRawcount;
                        cp *= CapSense_BIST_NF_TO_FF_FACTOR;
                        cp /= CapSense_dsRam.selfTestConfig.snsCapVrefVoltage;
                        cp /= rawcountMax;
                        cp /= CapSense_dsRam.selfTestConfig.snsCapSnsClkHz;
                        /* Store the measurement status */
                        *snsMeasStatusPtr = snsMeasStatus;

                        return (uint32)cp;
                    }
                #endif /* (CapSense_CSDV2) */


                /*******************************************************************************
                * Function Name: CapSense_BistDisableHwSnsCap
                ****************************************************************************//**
                *
                * \brief
                *  This function disables CSD mode.
                *
                * \details
                *  To disable CSD mode the following tasks are performed:
                *  1. Disconnect Cmod from AMUXBUS-A.
                *  2. Disconnect previous CSX electrode if it has been connected.
                *  3. Disconnect Csh from AMUXBUS-B.
                *  4. Disable Shield Electrodes.
                *
                *******************************************************************************/
                static void CapSense_BistDisableHwSnsCap(void)
                {
                    uint32 regValue;
                    uint8  interruptState;

                    /* Disconnect Cmod from AMUXBUS-A using HSIOM registers */
                    #if (CapSense_ENABLE == CapSense_CSD_EN)
                        interruptState = CyEnterCriticalSection();
                        regValue = CY_GET_REG32(CapSense_CMOD_HSIOM_PTR);
                        regValue &= (uint32)(~(uint32)CapSense_CMOD_HSIOM_MASK);
                        CY_SET_REG32(CapSense_CMOD_HSIOM_PTR, regValue);
                        CyExitCriticalSection(interruptState);
                    #else
                        interruptState = CyEnterCriticalSection();
                        regValue = CY_GET_REG32(CapSense_CintA_HSIOM_PTR);
                        regValue &= (uint32)(~(uint32)CapSense_CintA_HSIOM_MASK);
                        regValue |= (CapSense_HSIOM_SEL_GPIO << CapSense_CintA_HSIOM_SHIFT);
                        CY_SET_REG32(CapSense_CintA_HSIOM_PTR, regValue);
                        CyExitCriticalSection(interruptState);

                        interruptState = CyEnterCriticalSection();
                        regValue = CY_GET_REG32(CapSense_CintB_HSIOM_PTR);
                        regValue &= (uint32)(~(uint32)CapSense_CintB_HSIOM_MASK);
                        regValue |= (CapSense_HSIOM_SEL_GPIO << CapSense_CintB_HSIOM_SHIFT);
                        CY_SET_REG32(CapSense_CintB_HSIOM_PTR, regValue);
                        CyExitCriticalSection(interruptState);
                    #endif /* (CapSense_ENABLE == CapSense_CSD_EN) */

                    #if (CapSense_ENABLE == CapSense_TST_SNS_CAP_EN)
                        /* Verification of the bist mode */
                        if (CapSense_BIST_HW_SNS_CAP_E == CapSense_bistHwConfig)
                        {
                            /* Disconnect Csh from AMUXBUS-B */
                            #if ((CapSense_ENABLE == CapSense_CSD_SHIELD_EN) && \
                                 (CapSense_ENABLE == CapSense_CSD_SHIELD_TANK_EN))
                                interruptState = CyEnterCriticalSection();
                                regValue = CY_GET_REG32(CapSense_CSH_HSIOM_PTR);
                                regValue &= (uint32)(~CapSense_CSH_HSIOM_MASK);
                                CY_SET_REG32(CapSense_CSH_HSIOM_PTR, regValue);
                                CyExitCriticalSection(interruptState);
                            #endif /* ((CapSense_ENABLE == CapSense_CSD_SHIELD_EN) && \
                                       (CapSense_ENABLE == CapSense_CSD_SHIELD_TANK_EN)) */

                            #if (CapSense_ENABLE == CapSense_CSD_SHIELD_EN)
                                /*
                                * Disconnect all IOs from AMUXBUS_B as all rest tests suppose the electrodes are not
                                * connected to any bus. It is done for performance optiomization:
                                * not to write HSIOM register each time at test type switching.
                                */
                                CapSense_SetAllSnsHsiomState(CapSense_TST_HSIOM_GPIO);
                            #endif
                        }
                    #endif /* (CapSense_ENABLE == CapSense_TST_SNS_CAP_EN) */
                }


                /*******************************************************************************
                * Function Name: CapSense_BistSetIdacGain
                ****************************************************************************//**
                *
                * \brief
                *   This internal function changes the IDACs values and gains for BIST measurement.
                *
                * \details
                *   The functions disables the compensation IDAC if enabled and sets
                *   desired idac code and idac gain.
                *
                * \param idacValue
                *   The Idac code to be set.
                *
                * \param idacGainIndex
                *   The Idac gain index.
                *
                *******************************************************************************/
                static void CapSense_BistSetIdacGain(uint32 idacValue, uint32 idacGainIndex)
                {
                    #if ((CapSense_ENABLE != CapSense_CSDV2) && \
                        (CapSense_ENABLE != CapSense_CSD_IDAC_COMP_EN))
                        uint32 regValue;
                        uint8 interruptState;
                    #endif

                    uint32 gainRegValue = CapSense_idacGainTable[idacGainIndex].gainReg;

                    #if (CapSense_ENABLE == CapSense_CSDV2)
                        CY_SET_REG32(CapSense_CSD_IDACA_PTR, gainRegValue | CapSense_BIST_IDACA_MODE | \
                                    (idacValue & CapSense_IDAC_MOD_VAL_MASK));
                        #if((CapSense_ENABLE == CapSense_ADC_EN) || (CapSense_ENABLE == CapSense_CSD_IDAC_COMP_EN))
                            CY_SET_REG32(CapSense_CSD_IDACB_PTR, CapSense_BIST_IDACB_CFG);
                        #endif
                    #else
                        #if (CapSense_ENABLE == CapSense_CSD_IDAC_COMP_EN)
                            CY_SET_REG32(CapSense_IDAC_PTR, CapSense_BIST_IDAC_CFG | gainRegValue | \
                                        (idacValue & CapSense_BIST_IDAC_MOD_VAL_MASK));
                        #else
                            interruptState = CyEnterCriticalSection();
                            regValue = CY_GET_REG32(CapSense_IDAC_PTR);
                            regValue &= ~(CapSense_BIST_IDAC_MOD_MASK);
                            regValue |= (CapSense_BIST_IDAC_CFG | gainRegValue | (idacValue & CapSense_BIST_IDAC_MOD_VAL_MASK));
                            CY_SET_REG32(CapSense_IDAC_PTR, regValue);
                            CyExitCriticalSection(interruptState);
                        #endif /* (CapSense_ENABLE == CapSense_CSD_IDAC_COMP_EN) */
                    #endif /* (CapSense_ENABLE == CapSense_CSDV2) */
                }


                /*******************************************************************************
                * Function Name: CapSense_IscToBistIOConfig
                ****************************************************************************//**
                *
                * \brief
                *   Converts inactive sensor connection state to corresponding
                *   CapSense_BIST_IO_CONFIG_ENUM value. Refer to iscState parameter
                *   for the list of input states.
                *
                * \param iscState State of the inactive sensor connection. Available states:
                *   - CapSense_SNS_CONNECTION_GROUND
                *   - CapSense_SNS_CONNECTION_HIGHZ
                *   - CapSense_SNS_CONNECTION_SHIELD
                *   - CapSense_SNS_CONNECTION_UNDEFINED
                *
                * \return corresponding CapSense_BIST_IO_CONFIG_ENUM value.
                *
                *******************************************************************************/
                static CapSense_BIST_IO_CONFIG_ENUM CapSense_IscToBistIOConfig(uint32 iscState)
                {
                    CapSense_BIST_IO_CONFIG_ENUM retVal;

                    switch(iscState)
                    {
                        case CapSense_SNS_CONNECTION_GROUND:
                            retVal = CapSense_BIST_IO_STRONG_E;
                            break;
                        case CapSense_SNS_CONNECTION_HIGHZ:
                            retVal = CapSense_BIST_IO_HIGHZA_E;
                            break;
                        case CapSense_SNS_CONNECTION_SHIELD:
                            retVal = CapSense_BIST_IO_SHIELD_E;
                            break;
                        case CapSense_SNS_CONNECTION_UNDEFINED:
                            retVal = CapSense_BIST_IO_UNDEFINED_E;
                            break;
                        default:
                            retVal = CapSense_BIST_IO_UNDEFINED_E;
                            break;
                    }
                    return(retVal);
                }


                #if (CapSense_ENABLE != CapSense_CSDV2)
                    /*******************************************************************************
                    * Function Name: CapSense_BistClockRestart
                    ****************************************************************************//**
                    *
                    * \brief
                    *  This function restarts the clocks.
                    *
                    * \details
                    *  The function performs the following:
                    *  1) Stops, sets dividers and starts ModClk clock.
                    *  2) Stops and starts the SnsClk clock.
                    *  3) Scans one cycle to reset the flip-flop for the Direct clock mode.
                    *
                    *******************************************************************************/
                    static void CapSense_BistClockRestart(void)
                    {
                        /* Stop clocks, set dividers, and start the clock to align the clock phase */
                        CapSense_SsSetModClkClockDivider((uint32)CapSense_dsRam.selfTestConfig.snsCapModClkDivider);

                        #if (CapSense_ENABLE == CapSense_IS_M0S8PERI_BLOCK)
                            /* Stop the sense clock   */
                            CY_SET_REG32(CapSense_SNSCLK_CMD_PTR,
                                         ((uint32)CapSense_SnsClk__DIV_ID <<
                                         CapSense_SNSCLK_CMD_DIV_SHIFT)|
                                         ((uint32)CapSense_SNSCLK_CMD_DISABLE_MASK));

                            /* Check whether the previous sense clock start command has completed */
                            while(0u != (CY_GET_REG32(CapSense_SNSCLK_CMD_PTR) & CapSense_SNSCLK_CMD_ENABLE_MASK))
                            {
                                /* Wait until the previous sense clock start command has completed */
                            }

                            /* Start the sense clock aligned to a previously started modulator clock */
                            CY_SET_REG32(CapSense_SNSCLK_CMD_PTR,
                                         (uint32)(((uint32)CapSense_SnsClk__DIV_ID << CapSense_SNSCLK_CMD_DIV_SHIFT) |
                                          ((uint32)CapSense_ModClk__PA_DIV_ID << CapSense_SNSCLK_CMD_PA_DIV_SHIFT) |
                                          CapSense_SNSCLK_CMD_ENABLE_MASK));
                        #else
                            /* Clear the bit to disable the SnsClk clock */
                            CY_SET_REG32(CapSense_SNSCLK_CMD_PTR,
                                         CY_GET_REG32(CapSense_SNSCLK_CMD_PTR) &
                                         (uint32)(~(uint32)CapSense_SnsClk__ENABLE_MASK));

                            /* Set a bit to enable the SnsClk clock */
                            CY_SET_REG32(CapSense_SNSCLK_CMD_PTR,
                                         CY_GET_REG32(CapSense_SNSCLK_CMD_PTR) |
                                        CapSense_SnsClk__ENABLE_MASK);
                        #endif /* (CapSense_ENABLE == CapSense_IS_M0S8PERI_BLOCK) */

                        /* Scan one cycle to reset the flip-flop for the Direct clock mode */
                        CyIntDisable(CapSense_ISR_NUMBER);
                        CY_SET_REG32(CapSense_COUNTER_PTR, CapSense_ONE_CYCLE);

                        while(0u != (CY_GET_REG32(CapSense_COUNTER_PTR) & CapSense_RESOLUTION_16_BITS))
                        {
                            /* Wait until scanning is complete */
                        }
                        CY_SET_REG32(CapSense_INTR_PTR, CapSense_INTR_SET_MASK);
                        CyIntClearPending(CapSense_ISR_NUMBER);
                    }

                #endif /* (CapSense_ENABLE != CapSense_CSDV2) */
            #endif /* (((CapSense_ENABLE == CapSense_CSD_EN) || (CapSense_ENABLE == CapSense_CSX_EN)) &&\
                       ((CapSense_ENABLE == CapSense_TST_SNS_CAP_EN) || (CapSense_ENABLE == CapSense_TST_SH_CAP_EN))) */


            #if (CapSense_ENABLE == CapSense_TST_EXTERNAL_CAP_EN)
                /*******************************************************************************
                * Function Name: CapSense_GetExtCapCapacitance
                ****************************************************************************//**
                *
                * \brief
                *  Measures the capacitance of the specified external capacitor.
                *
                * \details
                *  The function measures the capacitance of the specified external capacitor
                *  such as Cmod and returns the result, alternatively the result is stored in
                *  the CapSense_EXT_CAP<EXT_CAP_ID>_VALUE register in data structure.
                *
                *  The measurable capacitance range using this function is
                *  from 200pF to 60,000pF with measurement accuracy of 10%.
                *
                *  This test can be executed for all the external capacitors at once using
                *  the CapSense_RunSelfTest() function with the
                *  CapSense_TST_EXTERNAL_CAP mask.
                *
                * \param extCapId
                *  Specifies the ID number of the external capacitor to be measured:
                *  - CapSense_TST_CMOD_ID - Cmod capacitor
                *  - CapSense_TST_CSH_ID - Csh capacitor
                *  - CapSense_TST_CINTA_ID - CintA capacitor
                *  - CapSense_TST_CINTB_ID - CintB capacitor
                *
                * \return
                *  Returns a status of the test execution:
                *  - The capacitance (in pF) of the specified external capacitor
                *  - CapSense_TST_BAD_PARAM if the input parameter is invalid.
                *
                *******************************************************************************/
                uint32 CapSense_GetExtCapCapacitance(uint32 extCapId)
                {
                    uint32 result = CYRET_SUCCESS;

                    if (CapSense_TST_EXT_CAPS_NUM > extCapId)
                    {
                        CapSense_SsSwitchSensingMode(CapSense_SENSE_METHOD_BIST_E);
                        CapSense_BistSwitchHwConfig(CapSense_BIST_HW_EXT_CAP_E);

                        CapSense_SwitchAllIOState(CapSense_BIST_IO_HIGHZA_E);
                        CapSense_bistCurrentISC = CapSense_BIST_IO_HIGHZA_E;

                        /* Set resolution to fixed value */
                        CapSense_BistSetScanDuration();
                        result = CapSense_MeasureExtCapCapacitance(extCapId);
                    }
                    else
                    {
                        result |= CapSense_TST_BAD_PARAM;
                    }
                    return result;
                }

                /*******************************************************************************
                * Function Name: CapSense_MeasureExtCapCapacitance
                ****************************************************************************//**
                *
                * \brief
                *  The internal function that measures a capacitance of an external capacitor
                *  when HW block is configured for an external capacitors measuring.
                *
                * \details
                *  The function connect an external capacitor (Cmod, Csh, CintA/CintB)
                *  to CSD block, defines a modclock frequency divider and an Idac code
                *  and measures a capacitance in range of 0,5 to 5,0 nF.
                *  If the result exceeds 75 % of a range high bound the function divides modClk,
                *  multiply Idac code to the fixed value to wide a measurement range and performs
                *  another measuring cycle for a capacitance in range of 5,0 to 60,0 nF.
                *  After that the function disconnect the external capacitor, calculate a result
                *  and stores it to a correspondent register of the self-test data structure.
                *
                * \param extCapId
                *  An index of the external cap to measure.
                *
                * \return
                *  - Cext of the external capacitor in pF units.
                *
                *******************************************************************************/
                static uint32 CapSense_MeasureExtCapCapacitance(uint32 extCapId)
                {
                    uint32 modClkDivider;
                    uint32 iDacVal;
                    uint32 result = 0uL;
                    uint32 extCapRangeSwitchTh;

                    /* 75% of measurement window. */
                    extCapRangeSwitchTh  = (CapSense_TST_EXT_CAP_MEASURE_WND >> 1uL);
                    extCapRangeSwitchTh += (CapSense_TST_EXT_CAP_MEASURE_WND >> 2uL);
                    modClkDivider = (CYDEV_BCLK__HFCLK__MHZ > CapSense_TST_EXT_CAP_MOD_CLK_FREQ_MHZ) ?
                                    (CYDEV_BCLK__HFCLK__MHZ / CapSense_TST_EXT_CAP_MOD_CLK_FREQ_MHZ) : 1u;

                    /* Connect an extCap */
                    CapSense_BistConnectExtCapacitor(extCapId);
                     /* Define Idac for a first scan (capacitance range up to CapSense_TST_EXT_CAP_LOW_RANGE in pF) */
                    iDacVal = ((CapSense_TST_EXT_CAP_LOW_RANGE * CapSense_dsRam.selfTestConfig.extCapVrefVoltage * CYDEV_BCLK__HFCLK__MHZ) /
                               (CapSense_TST_SNS_CAP_IDAC_GAIN * CapSense_TST_EXT_CAP_MEASURE_WND * modClkDivider)) + 1u;

                    CapSense_GetExtCapCapacitanceExt(modClkDivider, iDacVal);

                    /* For results more then 75 % of the resolution limit perform a second wide range scan */
                    if (extCapRangeSwitchTh < CapSense_bistRawcount)
                    {
                         /* Divide modClk and Multiply Idac to the fixed value to wide a measurement range */
                        modClkDivider <<= 2u;
                        iDacVal <<= 2u;
                        if (CapSense_BIST_MAX_IDAC_VALUE < iDacVal)
                        {
                            iDacVal = CapSense_BIST_MAX_IDAC_VALUE;
                        }

                        CapSense_GetExtCapCapacitanceExt(modClkDivider, iDacVal);
                    }
                    /* Disconnect ext capacitor */
                    CapSense_BistDisconnectExtCapacitor(extCapId);
                    /* Saturate raw count */
                    if (CapSense_TST_EXT_CAP_MEASURE_WND < CapSense_bistRawcount)
                    {
                        CapSense_bistRawcount = (uint16)CapSense_TST_EXT_CAP_MEASURE_WND;
                    }
                    /* Calculate result in 2 steps to prevent overflow */
                    result = (iDacVal * modClkDivider * CapSense_bistRawcount) / CYDEV_BCLK__HFCLK__MHZ;
                    result = (result * CapSense_TST_SNS_CAP_IDAC_GAIN) /
                                CapSense_dsRam.selfTestConfig.extCapVrefVoltage;
                    if (CapSense_MAX_16_BITS < result)
                    {
                        result = CapSense_MAX_16_BITS;
                    }
                    CapSense_dsRam.selfTest.extCap[extCapId] = (uint16)result;

                    return result;
                }

                /*******************************************************************************
                * Function Name: CapSense_GetExtCapCapacitanceExt
                ****************************************************************************//**
                *
                * \brief
                *  The internal function that performs a scan for an external capacitor measuring.
                *
                * \details
                *  The function sets the modclock and Idac to defined values and performs a scan
                *  by using CSD mode for an external capacitor (Cmod, Csh, or CintA/CintB).
                *
                * \param modClkDivider
                *  Specifies a divider for a modulation clock frequency.
                *
                * \param iDacVal
                *  Specifies an Idac modulation code.
                *
                *******************************************************************************/
                static void CapSense_GetExtCapCapacitanceExt(uint32 modClkDivider, uint32 iDacVal)
                {
                    uint32 snsClkDivTmp;

                    #if(CapSense_ENABLE == CapSense_CSDV2)
                        snsClkDivTmp = CapSense_TST_EXT_CAP_MEASURE_WND + CapSense_SENSE_DUTY_TST_EXT_CAP_WIDTH;
                    #else
                        snsClkDivTmp = CapSense_TST_EXT_CAP_MEASURE_WND;
                        #if(CapSense_ENABLE == CapSense_IS_M0S8PERI_BLOCK)
                            snsClkDivTmp *= modClkDivider;
                        #endif
                    #endif

                    CapSense_SsSetModClkClockDivider(modClkDivider);
                    CapSense_BistConfigClock(snsClkDivTmp);

                    CapSense_BistSetIdac(iDacVal);
                    /* Start the scan */
                    CapSense_dsRam.status |= CapSense_SW_STS_BUSY;
                    CapSense_BistStartSample();
                    /* Initialize Watchdog Counter with time interval which is enough to charge 100 nF capacitor */
                    CapSense_WaitEndOfScan(CapSense_BIST_MEASURE_WATCHDOG_CYCLES_NUM);
                }

                /*******************************************************************************
                * Function Name: CapSense_BistConnectExtCapacitor
                ****************************************************************************//**
                *
                * \brief
                *  The internal function that connects an external capacitor to the
                *  CapSense block.
                *
                * \details
                *  The function that that connects an external capacitor to the CapSense
                *  block by using HSIOM in CSD mode.
                *
                * \param extCapId
                *  An index of the external cap to connect.
                *
                *******************************************************************************/
                static void CapSense_BistConnectExtCapacitor(uint32 extCapId)
                {
                    uint32 newRegValue;
                    uint8  interruptState;

                    #if (CapSense_ENABLE == CapSense_CSDV2)
                        /* Connect AMUXBUS-A to CSDCOMP(sense path) */
                    CY_SET_REG32(CapSense_SW_CMP_P_SEL_PTR, CapSense_SW_CMP_P_SEL_SW_SFMA_STATIC_CLOSE);
                    #endif /* (CapSense_ENABLE == CapSense_CSDV2) */

                    switch(CapSense_extCapMap[extCapId])
                    {
                        #if (CapSense_ENABLE == CapSense_CSD_EN)
                            case CapSense_TST_CMOD_MAP:
                            {
                                /* Connect Cmod pin using HSIOM registers */
                                interruptState = CyEnterCriticalSection();
                                newRegValue = CY_GET_REG32(CapSense_CMOD_HSIOM_PTR);
                                newRegValue &= (uint32)(~(uint32)CapSense_CMOD_HSIOM_MASK);
                                newRegValue |= (CapSense_HSIOM_SEL_CSD_SENSE << CapSense_CMOD_HSIOM_SHIFT);
                                CY_SET_REG32(CapSense_CMOD_HSIOM_PTR, newRegValue);
                                CyExitCriticalSection(interruptState);
                                break;
                            }
                                #if ((CapSense_ENABLE == CapSense_CSD_SHIELD_EN) && \
                                     (CapSense_ENABLE == CapSense_CSD_SHIELD_TANK_EN))
                                    case CapSense_TST_CSH_MAP:
                                    {
                                        /* Connect CTANK to AMUXBUS-A */
                                        interruptState = CyEnterCriticalSection();
                                        newRegValue = CY_GET_REG32(CapSense_CSH_HSIOM_PTR);
                                        newRegValue &= (uint32)(~(uint32)CapSense_CSH_HSIOM_MASK);
                                        newRegValue |= (CapSense_HSIOM_SEL_CSD_SENSE << CapSense_CSH_HSIOM_SHIFT);
                                        CY_SET_REG32(CapSense_CSH_HSIOM_PTR, newRegValue);
                                        CyExitCriticalSection(interruptState);
                                        break;
                                    }
                                #endif /* ((CapSense_ENABLE == CapSense_CSD_SHIELD_EN) && \
                                      (CapSense_ENABLE == CapSense_CSD_SHIELD_TANK_EN)) */

                        #endif /* (CapSense_ENABLE == CapSense_CSD_EN) */

                        #if (  (CapSense_ENABLE == CapSense_CSX_EN) \
                            || (CapSense_ENABLE == CapSense_ISX_EN))
                            case CapSense_TST_CINTA_MAP:
                            {
                                /* Connect the CintA pin using HSIOM registers */
                                interruptState = CyEnterCriticalSection();
                                newRegValue = CY_GET_REG32(CapSense_CintA_HSIOM_PTR);
                                newRegValue &= ~CapSense_CintA_HSIOM_MASK;
                                newRegValue |= (CapSense_HSIOM_SEL_CSD_SENSE << CapSense_CintA_HSIOM_SHIFT);
                                CY_SET_REG32(CapSense_CintA_HSIOM_PTR, newRegValue);
                                CyExitCriticalSection(interruptState);
                                break;
                            }
                            case CapSense_TST_CINTB_MAP:
                            {
                                /* Connect CintB to AMUXBUS-A using HSIOM registers */
                                interruptState = CyEnterCriticalSection();
                                newRegValue = CY_GET_REG32(CapSense_CintB_HSIOM_PTR);
                                newRegValue &= ~CapSense_CintB_HSIOM_MASK;
                                newRegValue |= (CapSense_HSIOM_SEL_CSD_SENSE << CapSense_CintB_HSIOM_SHIFT);
                                CY_SET_REG32(CapSense_CintB_HSIOM_PTR, newRegValue);
                                CyExitCriticalSection(interruptState);
                                break;
                            }
                        #endif /* ((CapSense_ENABLE == CapSense_CSX_EN) || (CapSense_ENABLE == CapSense_ISX_EN)) */

                        default:
                        {
                            break;
                        }
                    }
                }

                /*******************************************************************************
                * Function Name: CapSense_BistDisconnectExtCapacitor
                ****************************************************************************//**
                *
                * \brief
                *  The internal function that disconnects an external capacitor from the
                *  CapSense block.
                *
                * \details
                *  The function that that disconnects an external capacitor from the
                *  CapSense block.
                *  After that the function dicharges the specified capacitor to GND to provide
                *  next regular scans.
                *
                * \param extCapId
                *  An index of the external cap to disconnect.
                *
                *******************************************************************************/
                static void CapSense_BistDisconnectExtCapacitor(uint32 extCapId)
                {
                    uint32 newRegValue;
                    uint8  interruptState;

                    #if (CapSense_ENABLE == CapSense_CSDV2)
                        /* Disconnect AMUXBUS-A from CSDCOMP(sense path) */
                        CY_SET_REG32(CapSense_SW_CMP_P_SEL_PTR, CapSense_SW_CMP_P_SEL_SW_SFMA_STATIC_OPEN);
                    #endif /* (CapSense_ENABLE == CapSense_CSDV2) */

                    switch(CapSense_extCapMap[extCapId])
                    {
                        #if (CapSense_ENABLE == CapSense_CSD_EN)
                            case CapSense_TST_CMOD_MAP:
                            {
                                /* Disconnect Cmod from AMUXBUS-A using HSIOM registers */
                                interruptState = CyEnterCriticalSection();
                                newRegValue = CY_GET_REG32(CapSense_CMOD_HSIOM_PTR);
                                newRegValue &= (uint32)(~(uint32)CapSense_CMOD_HSIOM_MASK);
                                CY_SET_REG32(CapSense_CMOD_HSIOM_PTR, newRegValue);
                                CyExitCriticalSection(interruptState);

                                #if (CapSense_ENABLE == CapSense_CSDV2)
                                    /* Disconnect CSDCMP from CSDBUS-A (parallel pass via SFCA switch) */
                                    newRegValue = CY_GET_REG32(CapSense_SW_CMP_P_SEL_PTR);
                                    newRegValue &= (uint32) (~CapSense_SW_CMP_P_SEL_SW_SFCA_MASK);
                                    CY_SET_REG32(CapSense_SW_CMP_P_SEL_PTR, newRegValue);
                                #endif /* (CapSense_ENABLE == CapSense_CSDV2) */

                                /* Discharge CMOD by using GPIO mode */
                                interruptState = CyEnterCriticalSection();
                                newRegValue = CY_GET_REG32(CapSense_CMOD_HSIOM_PTR);
                                newRegValue &= (uint32)(~(uint32)CapSense_CMOD_HSIOM_MASK);
                                newRegValue |= (CapSense_HSIOM_SEL_GPIO << CapSense_CMOD_HSIOM_SHIFT);
                                CY_SET_REG32(CapSense_CMOD_HSIOM_PTR, newRegValue);
                                CyExitCriticalSection(interruptState);

                                /* Set output port register for Cmod to 0 to connect Cmod to GND */
                                interruptState = CyEnterCriticalSection();
                                newRegValue = CY_GET_REG32(CapSense_CMOD_DR_PTR);
                                newRegValue &= (uint32)(~(uint32) (1uL << CapSense_CMOD_DR_SHIFT));
                                CY_SET_REG32(CapSense_CMOD_DR_PTR, newRegValue);
                                CyExitCriticalSection(interruptState);

                                /* Set port configuration register (drive mode) for Cmod in STRONG mode */
                                interruptState = CyEnterCriticalSection();
                                newRegValue = CY_GET_REG32(CapSense_CMOD_PC_PTR);
                                newRegValue &= (uint32)(~((uint32)CapSense_GPIO_PC_MASK << CapSense_CMOD_PC_SHIFT));
                                newRegValue |= (CapSense_GPIO_STRGDRV << CapSense_CMOD_PC_SHIFT);
                                CY_SET_REG32(CapSense_CMOD_PC_PTR, newRegValue);
                                CyExitCriticalSection(interruptState);

                                /* Delay 10 uS to discharge Cmod */
                                CyDelayUs(10u);

                                /* Set port configuration register (drive mode) for Cmod in HIGHZ-A mode (PC=000) */
                                interruptState = CyEnterCriticalSection();
                                newRegValue = CY_GET_REG32(CapSense_CMOD_PC_PTR);
                                newRegValue &= (uint32)(~((uint32)CapSense_GPIO_PC_MASK << CapSense_CMOD_PC_SHIFT));
                                CY_SET_REG32(CapSense_CMOD_PC_PTR, newRegValue);
                                CyExitCriticalSection(interruptState);

                                break;
                            }

                            #if ((CapSense_ENABLE == CapSense_CSD_SHIELD_EN) && \
                                 (CapSense_ENABLE == CapSense_CSD_SHIELD_TANK_EN))
                                case CapSense_TST_CSH_MAP:
                                {
                                    /* Disconnect CTANK from AMUXBUS-A */
                                    interruptState = CyEnterCriticalSection();
                                    newRegValue = CY_GET_REG32(CapSense_CSH_HSIOM_PTR);
                                    newRegValue &= (uint32)(~(uint32)CapSense_CSH_HSIOM_MASK);
                                    CY_SET_REG32(CapSense_CSH_HSIOM_PTR, newRegValue);
                                    CyExitCriticalSection(interruptState);

                                    /* Discharge CTANK by using GPIO mode */
                                    interruptState = CyEnterCriticalSection();
                                    newRegValue = CY_GET_REG32(CapSense_CSH_HSIOM_PTR);
                                    newRegValue &= (uint32)(~(uint32)CapSense_CSH_HSIOM_MASK);
                                    newRegValue |= (CapSense_HSIOM_SEL_GPIO << CapSense_CSH_HSIOM_SHIFT);
                                    CY_SET_REG32(CapSense_CSH_HSIOM_PTR, newRegValue);
                                    CyExitCriticalSection(interruptState);

                                    /* Set output port register for Cmod to 0 to connect Cmod to GND */
                                    interruptState = CyEnterCriticalSection();
                                    newRegValue = CY_GET_REG32(CapSense_CSH_DR_PTR);
                                    newRegValue &= (uint32)(~(uint32) (1uL << CapSense_CSH_SHIFT));
                                    CY_SET_REG32(CapSense_CSH_DR_PTR, newRegValue);
                                    CyExitCriticalSection(interruptState);

                                    /* Set port configuration register (drive mode) for CTANK in STRONG mode */
                                    interruptState = CyEnterCriticalSection();
                                    newRegValue = CY_GET_REG32(CapSense_CSH_PC_PTR);
                                    newRegValue &= (uint32)(~((uint32)CapSense_GPIO_PC_MASK << CapSense_CSH_PC_SHIFT));
                                    newRegValue |= (CapSense_GPIO_STRGDRV << CapSense_CSH_PC_SHIFT);
                                    CY_SET_REG32(CapSense_CSH_PC_PTR, newRegValue);
                                    CyExitCriticalSection(interruptState);

                                    /* Delay 10 uS to discharge CTANK */
                                    CyDelayUs(10u);

                                    /* Set port configuration register (drive mode) for CTANK in HIGHZ-A mode */
                                    interruptState = CyEnterCriticalSection();
                                    newRegValue = CY_GET_REG32(CapSense_CSH_PC_PTR);
                                    newRegValue &= (uint32)(~((uint32)CapSense_GPIO_PC_MASK << CapSense_CSH_PC_SHIFT));
                                    CY_SET_REG32(CapSense_CSH_PC_PTR, newRegValue);
                                    CyExitCriticalSection(interruptState);

                                    break;
                                }
                            #endif /* ((CapSense_ENABLE == CapSense_CSD_SHIELD_EN) && \
                                       (CapSense_ENABLE == CapSense_CSD_SHIELD_TANK_EN)) */

                        #endif /* (CapSense_ENABLE == CapSense_CSD_EN) */

                        #if (  (CapSense_ENABLE == CapSense_CSX_EN) \
                            || (CapSense_ENABLE == CapSense_ISX_EN))
                            case CapSense_TST_CINTA_MAP:
                            {
                                /* Disconnect CintA from AMUXBUS-A using HSIOM registers. */
                                interruptState = CyEnterCriticalSection();
                                newRegValue = CY_GET_REG32(CapSense_CintA_HSIOM_PTR);
                                newRegValue &= (uint32)(~(uint32)CapSense_CintA_HSIOM_MASK);
                                CY_SET_REG32(CapSense_CintA_HSIOM_PTR, newRegValue);
                                CyExitCriticalSection(interruptState);

                                /* Discharge CintA by using GPIO mode */
                                interruptState = CyEnterCriticalSection();
                                newRegValue = CY_GET_REG32(CapSense_CintA_HSIOM_PTR);
                                newRegValue &= (uint32)(~(uint32)CapSense_CintA_HSIOM_MASK);
                                newRegValue |= (CapSense_HSIOM_SEL_GPIO << CapSense_CintA_HSIOM_SHIFT);
                                CY_SET_REG32(CapSense_CintA_HSIOM_PTR, newRegValue);
                                CyExitCriticalSection(interruptState);

                                /* Set output port register for CintA to 0 to connect CintA to GND */
                                interruptState = CyEnterCriticalSection();
                                newRegValue = CY_GET_REG32(CapSense_CintA_DR_PTR);
                                newRegValue &= (uint32)(~(uint32) (1uL << CapSense_CintA_SHIFT));
                                CY_SET_REG32(CapSense_CintA_DR_PTR, newRegValue);
                                CyExitCriticalSection(interruptState);

                                /* Set port configuration register (drive mode) for CintA in STRONG mode */
                                interruptState = CyEnterCriticalSection();
                                newRegValue = CY_GET_REG32(CapSense_CintA_PC_PTR);
                                newRegValue &= (uint32)(~((uint32)CapSense_GPIO_PC_MASK << (CapSense_CintA_SHIFT * 3uL)));
                                newRegValue |= (CapSense_GPIO_STRGDRV << (CapSense_CintA_SHIFT * 3uL));
                                CY_SET_REG32(CapSense_CintA_PC_PTR, newRegValue);
                                CyExitCriticalSection(interruptState);

                                /* Delay 10 uS to discharge CintA */
                                CyDelayUs(10u);

                                /* Set port configuration register (drive mode) for CintA in HIGHZ-A mode (PC=000) */
                                interruptState = CyEnterCriticalSection();
                                newRegValue = CY_GET_REG32(CapSense_CintA_PC_PTR);
                                newRegValue &= (uint32)(~((uint32)CapSense_GPIO_PC_MASK << (CapSense_CintA_SHIFT * 3uL)));
                                CY_SET_REG32(CapSense_CintA_PC_PTR, newRegValue);
                                CyExitCriticalSection(interruptState);

                                break;
                            }
                            case CapSense_TST_CINTB_MAP:
                            {
                                /* Discharge CintB by using GPIO mode */
                                interruptState = CyEnterCriticalSection();
                                newRegValue = CY_GET_REG32(CapSense_CintB_HSIOM_PTR);
                                newRegValue &= (uint32)(~(uint32)CapSense_CintB_HSIOM_MASK);
                                CY_SET_REG32(CapSense_CintB_HSIOM_PTR, newRegValue);
                                CyExitCriticalSection(interruptState);

                                /* Set a CintB HSIOM register to the GPIO mode */
                                interruptState = CyEnterCriticalSection();
                                newRegValue = CY_GET_REG32(CapSense_CintB_HSIOM_PTR);
                                newRegValue &= (uint32)(~(uint32)CapSense_CintB_HSIOM_MASK);
                                newRegValue |= (CapSense_HSIOM_SEL_GPIO << CapSense_CintB_HSIOM_SHIFT);
                                CY_SET_REG32(CapSense_CintB_HSIOM_PTR, newRegValue);
                                CyExitCriticalSection(interruptState);

                                /* Set output port register for CintB to 0 to connect CintB to GND */
                                interruptState = CyEnterCriticalSection();
                                newRegValue = CY_GET_REG32(CapSense_CintB_DR_PTR);
                                newRegValue &= (uint32)(~(uint32) (1uL << CapSense_CintB_SHIFT));
                                CY_SET_REG32(CapSense_CintB_DR_PTR, newRegValue);
                                CyExitCriticalSection(interruptState);

                                /* Set port configuration register (drive mode) for CintB in STRONG mode */
                                interruptState = CyEnterCriticalSection();
                                newRegValue = CY_GET_REG32(CapSense_CintB_PC_PTR);
                                newRegValue &= (uint32)(~((uint32)CapSense_GPIO_PC_MASK << (CapSense_CintB_SHIFT * 3uL)));
                                newRegValue |= (CapSense_GPIO_STRGDRV << (CapSense_CintB_SHIFT * 3uL));
                                CY_SET_REG32(CapSense_CintB_PC_PTR, newRegValue);
                                CyExitCriticalSection(interruptState);

                                /* Delay 10 uS to discharge CintB */
                                CyDelayUs(10u);

                                /* Set port configuration register (drive mode) for CintB in HIGHZ-A mode */
                                interruptState = CyEnterCriticalSection();
                                newRegValue = CY_GET_REG32(CapSense_CintB_PC_PTR);
                                newRegValue &= (uint32)(~((uint32)CapSense_GPIO_PC_MASK << (CapSense_CintB_SHIFT * 3uL)));
                                CY_SET_REG32(CapSense_CintB_PC_PTR, newRegValue);
                                CyExitCriticalSection(interruptState);

                                break;
                            }
                        #endif /* ((CapSense_ENABLE == CapSense_CSX_EN) || (CapSense_ENABLE == CapSense_ISX_EN)) */

                        default:
                        {
                            break;
                        }
                    }
                }

                /*******************************************************************************
                * Function Name: CapSense_MeasureAllExtCapCapacitance
                ****************************************************************************//**
                *
                * \brief
                *  The internal function that measures a capacitance of all external
                *  capacitors.
                *
                * \details
                *  The function measures capacitances of all external  capacitors (Cmod,
                *  Csh, CintA, CintB).
                *  The function stores cap values in the corresponding registers.
                *  The function does not affect other Component parameters or
                *  functionalities.
                *
                * \return
                *  Returns the status of the measuring process:
                *  - CYRET_SUCCESS if all measurings is successful;
                *  - CapSense_TST_EXTERNAL_CAP if any measuring is failed.
                *
                *******************************************************************************/
                static uint32 CapSense_MeasureAllExtCapCapacitance(void)
                {
                    uint32 extCapIndex;

                    CapSense_SsSwitchSensingMode(CapSense_SENSE_METHOD_BIST_E);
                    CapSense_BistSwitchHwConfig(CapSense_BIST_HW_EXT_CAP_E);

                    CapSense_SwitchAllIOState(CapSense_BIST_IO_HIGHZA_E);
                    CapSense_bistCurrentISC = CapSense_BIST_IO_HIGHZA_E;

                    /* Set resolution to fixed value) */
                    CapSense_BistSetScanDuration();

                    for (extCapIndex = 0u; extCapIndex < CapSense_TST_EXT_CAPS_NUM; extCapIndex++)
                    {
                        (void)CapSense_MeasureExtCapCapacitance(extCapIndex);
                    }

                    return (CYRET_SUCCESS);
                }
            #endif /*(CapSense_ENABLE == CapSense_TST_EXTERNAL_CAP_EN) */
        #endif /* ((CapSense_ENABLE == CapSense_TST_SNS_CAP_EN) || \
                   (CapSense_ENABLE == CapSense_TST_SH_CAP_EN)  ||\
                   (CapSense_ENABLE == CapSense_TST_EXTERNAL_CAP_EN)) */

        #if ((CapSense_ENABLE == CapSense_TST_VDDA_EN) &&\
             (CapSense_ENABLE == CapSense_CSDV2))
            /*******************************************************************************
            * Function Name: CapSense_GetVdda
            ****************************************************************************//**
            *
            * \brief
            *  Measures and returns the VDDA voltage.
            *
            * \details
            *  This function measures voltage on VDDA terminal of the chip and returns
            *  the result, alternatively the result is stored in the
            *  CapSense_VDDA_VOLTAGE_VALUE register of data structure.
            *
            * \return
            *  The VDDA voltage in mV.
            *
            *******************************************************************************/
            uint16 CapSense_GetVdda(void)
            {
                uint16 tmpResult;

                CapSense_SsSwitchSensingMode(CapSense_SENSE_METHOD_BIST_E);
                CapSense_BistSwitchHwConfig(CapSense_BIST_HW_VDDA_E);
                tmpResult = CapSense_BistVddaRunMeasurement();
                CapSense_dsRam.selfTest.vddaVoltage = tmpResult;

                return (tmpResult);
            }


            /*******************************************************************************
            * Function Name: CapSense_BistEnableVddaMeasurement
            ****************************************************************************//**
            *
            * \brief
            *  Configures the CSD block to be used for VDDA measurement.
            *
            * \details
            *  Configures the IDACB, HW block internal switches, REFGEN, HSCOMP and
            *  interrupt.
            *
            *******************************************************************************/
            static void CapSense_BistEnableVddaMeasurement(void)
            {
                uint32 modClkDivider = CapSense_dsRam.selfTestConfig.vddaModClkDivider;
                uint32 totalClkDivider = modClkDivider * CapSense_TST_VDDA_SENSE_DIV_DEFAULT;
                uint32 seqTime =   (((CYDEV_BCLK__HFCLK__HZ * CapSense_TST_VDDA_ADC_AZ_TIME) / totalClkDivider) / 1000000uL);

                seqTime = (0u == seqTime) ? 1u : seqTime;

                /* Configure clocks */
                CapSense_SsSetModClkClockDivider(modClkDivider);
                CY_SET_REG32(CapSense_SENSE_PERIOD_PTR, (CapSense_TST_VDDA_SENSE_DIV_DEFAULT - 1uL));

                /* Configure the IDAC */
                CY_SET_REG32(CapSense_CONFIG_PTR, CapSense_TST_VDDA_CONFIG_DEFAULT_CFG);

                #if(CapSense_TST_ANALOG_STARTUP_DELAY_US > 0uL)
                    CyDelayUs(CapSense_TST_ANALOG_STARTUP_DELAY_US);
                #endif /* (CapSense_TST_ANALOG_STARTUP_DELAY_US > 0uL) */

                CY_SET_REG32(CapSense_CSD_IDACB_PTR, CapSense_TST_VDDA_IDACB_DEFAULT_CFG);

                /* Configure AZ Time */
                CY_SET_REG32(CapSense_SEQ_TIME_PTR, (seqTime - 1u));

                CY_SET_REG32(CapSense_CSD_IDACA_PTR, CapSense_TST_VDDA_IDACA_DEFAULT_CFG);
                CY_SET_REG32(CapSense_CSDCMP_PTR, CapSense_TST_CSDCMP_DEFAULT_CFG);
                CY_SET_REG32(CapSense_SW_DSI_SEL_PTR, CapSense_TST_SW_DSI_SEL_DEFAULT_CFG);

                CY_SET_REG32(CapSense_SENSE_DUTY_PTR, CapSense_TST_SENSE_DUTY_VDDA_CFG);
                CY_SET_REG32(CapSense_SEQ_INIT_CNT_PTR, CapSense_TST_SEQ_INIT_CNT_DEFAULT_CFG);
                CY_SET_REG32(CapSense_SEQ_NORM_CNT_PTR, CapSense_TST_SEQ_NORM_CNT_DEFAULT_CFG);

                /* Configure the block-level routing */
                CY_SET_REG32(CapSense_SW_HS_P_SEL_PTR, CapSense_TST_SW_HS_P_SEL_DEFAULT_CFG);
                CY_SET_REG32(CapSense_SW_HS_N_SEL_PTR, CapSense_TST_SW_HS_N_SEL_DEFAULT_CFG);
                CY_SET_REG32(CapSense_SW_SHIELD_SEL_PTR, CapSense_TST_SW_SHIELD_SEL_DEFAULT_CFG);
                CY_SET_REG32(CapSense_SW_BYP_SEL_PTR, CapSense_TST_SW_BYP_SEL_DEFAULT_CFG);
                CY_SET_REG32(CapSense_SW_CMP_P_SEL_PTR, CapSense_TST_SW_CMP_P_SEL_DEFAULT_CFG);
                CY_SET_REG32(CapSense_SW_CMP_N_SEL_PTR, CapSense_TST_SW_CMP_N_SEL_DEFAULT_CFG);
                CY_SET_REG32(CapSense_SW_FW_MOD_SEL_PTR, CapSense_TST_SW_FW_MOD_SEL_DEFAULT_CFG);
                CY_SET_REG32(CapSense_SW_FW_TANK_SEL_PTR, CapSense_TST_SW_FW_TANK_SEL_DEFAULT_CFG);
                CY_SET_REG32(CapSense_SW_REFGEN_SEL_PTR, CapSense_TST_SW_REFGEN_SEL_DEFAULT_CFG);

                /* Config RefGen */
                #if (CapSense_2000_MV > CapSense_CYDEV_VDDA_MV)
                    CY_SET_REG32(CapSense_REFGEN_PTR, CapSense_TST_REFGEN_LV);
                #else
                    CY_SET_REG32(CapSense_REFGEN_PTR, CapSense_TST_REFGEN_HV |
                                 (((uint32)CapSense_dsRam.selfTestConfig.vddaVrefGain) << CYFLD_CSD_GAIN__OFFSET));
                #endif

                CY_SET_REG32(CapSense_SW_AMUXBUF_SEL_PTR, CapSense_TST_SW_AMUXBUF_SEL_DEFAULT_CFG);

                /* Configure HSCOMP */
                CY_SET_REG32(CapSense_HSCMP_PTR, CapSense_TST_HSCMP_DEFAULT_CFG);
                CY_SET_REG32(CapSense_INTR_MASK_PTR, CapSense_INTR_MASK_CLEAR_MASK);
            }


            /*******************************************************************************
            * Function Name: CapSense_BistVddaRunMeasurement
            ****************************************************************************//**
            *
            * \brief
            *  This function triggers the VDDA measurement.
            *
            * \details
            *  The function initiates a conversion, waits for completion and
            *  returns the result in mV.
            *  The function returns voltage in millivolts or
            *  CapSense_TST_VDDA_BAD_RESULT if:
            *   - The ADC conversion is not started
            *   - The ADC conversion watch-dog triggered.
            *
            *******************************************************************************/
            static uint16 CapSense_BistVddaRunMeasurement(void)
            {
                uint16 tVdda2Vref;
                uint16 tVssa2Vref;
                uint16 tRecover;
                uint16 tmpResult;
                uint32 tmpVrefCal;
                uint32 registerValue;
                uint32 watchdogCounter;
                uint32 nonVolWDC;
                uint8 interruptState;
                uint8 tmpFlashByte0;
                uint8 tmpFlashByte1;
                uint16 totalClkDivider = (uint16)CapSense_dsRam.selfTestConfig.vddaModClkDivider * (uint16)CapSense_TST_VDDA_SENSE_DIV_DEFAULT;
                uint16 acquisionBase = ((uint16)CapSense_VDDA_ACQUISITION_TIME_US  * (uint16)CYDEV_BCLK__HFCLK__MHZ) / totalClkDivider;

                watchdogCounter = CapSense_TST_SW_WATCHDOG_CYCLES_NUM;
                nonVolWDC = watchdogCounter;
                CY_SET_REG32(CapSense_ADC_CTL_PTR, (CapSense_TST_MEASMODE_VREF | ((uint32)acquisionBase - 1u)));
                CY_SET_REG32(CapSense_SEQ_START_PTR, CapSense_TST_VDDA_SEQ_START_MEASURE_CFG);

                /* Wait for INTR.INIT goes high */
                while((0u == (CY_GET_REG32(CapSense_INTR_PTR) & CapSense_INTR_ADC_RES_MASK)) && (0u != nonVolWDC))
                {
                    watchdogCounter--;
                    nonVolWDC = watchdogCounter;
                }

                /* Clear all interrupt pending requests */
                CY_SET_REG32(CapSense_INTR_PTR, CapSense_INTR_SET_MASK);
                (void)CY_GET_REG32(CapSense_INTR_PTR);

                tVssa2Vref = (uint16)CY_GET_REG32(CapSense_ADC_RES_PTR);

                if(0uL != watchdogCounter)
                {
                    watchdogCounter = CapSense_TST_SW_WATCHDOG_CYCLES_NUM;
                    nonVolWDC = watchdogCounter;
                    CY_SET_REG32(CapSense_ADC_CTL_PTR, (CapSense_TST_MEASMODE_VREFBY2 | ((uint32)acquisionBase - 1u)));
                    CY_SET_REG32(CapSense_SEQ_START_PTR, CapSense_TST_VDDA_SEQ_START_MEASURE_CFG);

                    /* Wait for INTR.INIT goes high */
                    while((0u == (CY_GET_REG32(CapSense_INTR_PTR) & CapSense_INTR_ADC_RES_MASK)) && (0u != nonVolWDC))
                    {
                        watchdogCounter--;
                        nonVolWDC = watchdogCounter;
                    }

                    CY_SET_REG32(CapSense_INTR_PTR, CapSense_INTR_SET_MASK);
                    (void)CY_GET_REG32(CapSense_INTR_PTR);
                }
                else
                {
                    /* Do nothing. */
                }

                tRecover = (uint16)CY_GET_REG32(CapSense_ADC_RES_PTR);
                if(0uL != watchdogCounter)
                {
                    /* Disconnect amuxbusB, Connect VDDA to csdbusB */
                    interruptState = CyEnterCriticalSection();
                    registerValue = CY_GET_REG32(CapSense_SW_BYP_SEL_PTR);
                    registerValue &= (uint32)(~(CapSense_SW_BYP_SEL_SW_BYA_MASK | CapSense_SW_BYP_SEL_SW_BYB_MASK));
                    CY_SET_REG32(CapSense_SW_BYP_SEL_PTR, registerValue);
                    CyExitCriticalSection(interruptState);
                    CY_SET_REG32(CapSense_SW_SHIELD_SEL_PTR, CapSense_SW_SHIELD_SEL_SW_HCBV_STATIC_CLOSE);

                    watchdogCounter = CapSense_TST_SW_WATCHDOG_CYCLES_NUM;
                    nonVolWDC = watchdogCounter;
                    CY_SET_REG32(CapSense_ADC_CTL_PTR, (CapSense_TST_MEASMODE_VIN | ((uint32)acquisionBase - 1u)));
                    CY_SET_REG32(CapSense_SEQ_START_PTR, CapSense_TST_VDDA_SEQ_START_MEASURE_CFG);

                    /* Wait for INTR.INIT goes high */
                    while((0u == (CY_GET_REG32(CapSense_INTR_PTR) & CapSense_INTR_ADC_RES_MASK)) && (0u != nonVolWDC))
                    {
                        watchdogCounter--;
                        nonVolWDC = watchdogCounter;
                    }

                    CY_SET_REG32(CapSense_INTR_PTR, CapSense_INTR_SET_MASK);
                    (void)CY_GET_REG32(CapSense_INTR_PTR);

                    tVdda2Vref = (uint16)CY_GET_REG32(CapSense_ADC_RES_PTR);
                    /* Test whether the full range is 2-bits higher than adc resolution */

                    tVdda2Vref = (uint16)(((((uint32)tVdda2Vref << 1u) * tRecover) + ((uint32)tVssa2Vref >> 1u)) / tVssa2Vref);

                    /* Get Vref trim-value */
                    tmpFlashByte0 = CY_GET_REG8(CYREG_SFLASH_CSDV2_CSD0_ADC_TRIM1);
                    tmpFlashByte1 = CY_GET_REG8(CYREG_SFLASH_CSDV2_CSD0_ADC_TRIM2);

                    tmpVrefCal = (uint32)tmpFlashByte0 | (((uint32)tmpFlashByte1) << 8u);

                    /* Update nominal Vref to real Vref */
                    tmpVrefCal *= CapSense_dsRam.selfTestConfig.vddaVrefVoltage;
                    tmpVrefCal /= CapSense_TST_VDDA_VREF_CALIB_USED;

                    tmpResult = (uint16)tmpVrefCal;

                    /* Calculate deviation of trim register */
                    if (tmpVrefCal > CapSense_dsRam.selfTestConfig.vddaVrefVoltage)
                    {
                        tmpVrefCal = tmpVrefCal - CapSense_dsRam.selfTestConfig.vddaVrefVoltage;
                    }
                    else
                    {
                        tmpVrefCal = CapSense_dsRam.selfTestConfig.vddaVrefVoltage - tmpVrefCal;
                    }
                    tmpVrefCal = (tmpVrefCal * CapSense_PERCENTAGE_100) / CapSense_dsRam.selfTestConfig.vddaVrefVoltage;

                    /* Use nominal Vref if trim-value is not within the allowed range */
                    if (CapSense_TST_VDDA_VREF_TRIM_MAX_DEVIATION < tmpVrefCal)
                    {
                        tmpResult = CapSense_dsRam.selfTestConfig.vddaVrefVoltage;
                    }

                    tmpResult = tmpResult + (((tmpResult * tVdda2Vref) + (tVssa2Vref >> 1uL)) / tVssa2Vref);
                }
                else
                {
                    tmpResult = CapSense_TST_VDDA_BAD_RESULT;
                }

                return(tmpResult);
            }
        #endif /* ((CapSense_ENABLE == CapSense_TST_VDDA_EN) &&\
                   (CapSense_ENABLE == CapSense_CSDV2)) */
    #endif /* (CapSense_ENABLE == CapSense_TST_MEASUREMENT_GROUP_EN) */


    /*******************************************************************************
    * Function Name: CapSense_BistSwitchHwConfig
    ****************************************************************************//**
    *
    * \brief
    *   The function switches HW configuration.
    *
    * \details
    *   The function checks the current HW configuration of CapSense block.
    *   If it differs from a desired configuration the function disables the current
    *   configuration and sets the desired one.
    *
    * \param config
    *   Specifies desired configuration.
    *
    *******************************************************************************/
    static void CapSense_BistSwitchHwConfig(CapSense_BIST_HW_CONFIG_ENUM config)
    {
        if (CapSense_bistHwConfig != config)
        {
            /* The requested config differes to the current one. Disable the current config */
            switch(CapSense_bistHwConfig)
            {
                #if ((CapSense_ENABLE == CapSense_CSD_EN) || (CapSense_ENABLE == CapSense_CSX_EN))
                    #if (CapSense_ENABLE == CapSense_TST_SNS_SHORT_EN)
                        case CapSense_BIST_HW_SNS_SHORT_E:
                        {
                            CapSense_BistDisableHwSnsShort();
                            break;
                        }
                    #endif /* (CapSense_ENABLE == CapSense_TST_SNS_SHORT_EN) */
                    #if (CapSense_ENABLE == CapSense_TST_SNS_CAP_EN)
                        case CapSense_BIST_HW_SNS_CAP_E:
                        {
                            CapSense_BistDisableHwSnsCap();
                            break;
                        }
                    #endif /* (CapSense_ENABLE == CapSense_TST_SNS_CAP_EN) */
                    #if (CapSense_ENABLE == CapSense_TST_SH_CAP_EN)
                        case CapSense_BIST_HW_SH_CAP_E:
                        {
                            CapSense_BistDisableHwSnsCap();
                            break;
                        }
                    #endif /* (CapSense_ENABLE == CapSense_TST_SH_CAP_EN) */
                #endif /* ((CapSense_ENABLE == CapSense_CSD_EN) || (CapSense_ENABLE == CapSense_CSX_EN)) */
                #if (CapSense_ENABLE == CapSense_TST_EXTERNAL_CAP_EN)
                    case CapSense_BIST_HW_EXT_CAP_E:
                    {
                        /* Nothing to do */
                        break;
                    }
                #endif /* (CapSense_ENABLE == CapSense_TST_EXTERNAL_CAP_EN) */
                #if (CapSense_ENABLE == CapSense_TST_VDDA_EN)
                    case CapSense_BIST_HW_VDDA_E:
                    {
                        /* Nothing to do */
                        break;
                    }
                #endif /* (CapSense_ENABLE == CapSense_TST_VDDA_EN) */
                default:
                {
                    break;
                }
            }

            CapSense_bistHwConfig = config;

            /* Enable the specified mode */
            switch(config)
            {
                #if ((CapSense_ENABLE == CapSense_CSD_EN) || (CapSense_ENABLE == CapSense_CSX_EN))
                    #if (CapSense_ENABLE == CapSense_TST_SNS_SHORT_EN)
                        case CapSense_BIST_HW_SNS_SHORT_E:
                        {
                            CapSense_BistEnableHwSnsShort();
                            break;
                        }
                    #endif /* (CapSense_ENABLE == CapSense_TST_SNS_SHORT_EN) */
                    #if (CapSense_ENABLE == CapSense_TST_SNS_CAP_EN)
                        case CapSense_BIST_HW_SNS_CAP_E:
                        {
                            CapSense_BistEnableHwSnsCap();
                            break;
                        }
                    #endif /* (CapSense_ENABLE == CapSense_TST_SNS_CAP_EN) */
                    #if (CapSense_ENABLE == CapSense_TST_SH_CAP_EN)
                        case CapSense_BIST_HW_SH_CAP_E:
                        {
                            CapSense_BistEnableHwSnsCap();
                            break;
                        }
                    #endif /* (CapSense_ENABLE == CapSense_TST_SH_CAP_EN) */
                #endif /* ((CapSense_ENABLE == CapSense_CSD_EN) || (CapSense_ENABLE == CapSense_CSX_EN)) */
                #if (CapSense_ENABLE == CapSense_TST_EXTERNAL_CAP_EN)
                    case CapSense_BIST_HW_EXT_CAP_E:
                    {
                        CapSense_BistEnableHwSnsCap();
                        break;
                    }
                #endif /* (CapSense_ENABLE == CapSense_TST_EXTERNAL_CAP_EN) */
                #if ((CapSense_ENABLE == CapSense_TST_VDDA_EN) &&\
                     (CapSense_ENABLE == CapSense_CSDV2))
                    case CapSense_BIST_HW_VDDA_E:
                    {
                        CapSense_BistEnableVddaMeasurement();
                        break;
                    }
                #endif /* ((CapSense_ENABLE == CapSense_TST_VDDA_EN) &&\
                           (CapSense_ENABLE == CapSense_CSDV2)) */
                default:
                {
                    break;
                }
            }
        }
    }


    /*******************************************************************************
    * Function Name: CapSense_SwitchAllIOState
    ****************************************************************************//**
    *
    * \brief
    *   Sets all the sensors pins in a desired state.
    *
    * \details
    *   Sets all the sensors and shield (if defined) pins in a desired state.
    *
    * \param config
    *   Specifies desired IO configuration.
    *
    *******************************************************************************/
    static void CapSense_SwitchAllIOState(CapSense_BIST_IO_CONFIG_ENUM config)
    {
        /* Re-configure the IOs if mode is changed */
        if (CapSense_bistCurrentISC != config)
        {
            switch(config)
            {
                case CapSense_BIST_IO_STRONG_E:
                {
                    CapSense_SetAllSnsHsiomState(CapSense_TST_HSIOM_GPIO);
                    CapSense_SetAllSnsPcState(CapSense_TST_PC_STRONG);
                    #if ((CapSense_ENABLE == CapSense_CSD_SHIELD_EN) && \
                         (0u != CapSense_CSD_TOTAL_SHIELD_COUNT))
                        CapSense_BistSetAllShieldsHsiomState(CapSense_TST_HSIOM_GPIO);
                        CapSense_BistSetAllShieldsPcState(CapSense_TST_PC_STRONG);
                    #endif
                    break;
                }
                case CapSense_BIST_IO_HIGHZA_E:
                {
                    CapSense_SetAllSnsHsiomState(CapSense_TST_HSIOM_GPIO);
                    CapSense_SetAllSnsPcState(CapSense_TST_PC_HIGHZA);
                    #if ((CapSense_ENABLE == CapSense_CSD_SHIELD_EN) && \
                         (0u != CapSense_CSD_TOTAL_SHIELD_COUNT))
                        CapSense_BistSetAllShieldsHsiomState(CapSense_TST_HSIOM_GPIO);
                        CapSense_BistSetAllShieldsPcState(CapSense_TST_PC_HIGHZA);
                    #endif
                    break;
                }
                case CapSense_BIST_IO_SENSE_E:
                {
                    CapSense_SetAllSnsPcState(CapSense_TST_PC_HIGHZA);
                    CapSense_SetAllSnsHsiomState(CapSense_TST_HSIOM_CSD_SENSE);
                    #if ((CapSense_ENABLE == CapSense_CSD_SHIELD_EN) && \
                         (0u != CapSense_CSD_TOTAL_SHIELD_COUNT))
                        CapSense_BistSetAllShieldsPcState(CapSense_TST_PC_HIGHZA);
                        CapSense_BistSetAllShieldsHsiomState(CapSense_TST_HSIOM_CSD_SENSE);
                    #endif
                    break;
                }
                #if (CapSense_ENABLE == CapSense_TST_SH_CAP_EN)
                    case CapSense_BIST_IO_SHIELD_E:
                    {
                        CapSense_SetAllSnsPcState(CapSense_TST_PC_HIGHZA);
                        CapSense_SetAllSnsHsiomState(CapSense_TST_HSIOM_CSD_SHIELD);
                        #if ((CapSense_ENABLE == CapSense_CSD_SHIELD_EN) && \
                             (0u != CapSense_CSD_TOTAL_SHIELD_COUNT))
                            CapSense_BistSetAllShieldsPcState(CapSense_TST_PC_HIGHZA);
                            CapSense_BistSetAllShieldsHsiomState(CapSense_TST_HSIOM_CSD_SHIELD);
                        #endif
                        break;
                    }
                    case CapSense_BIST_IO_DEFAULT_E:
                    {
                        /* Disconnect all IOs from analog busses */
                        CapSense_SetAllSnsHsiomState(CapSense_TST_HSIOM_GPIO);
                        #if ((CapSense_ENABLE == CapSense_CSD_SHIELD_EN) && \
                             (0u != CapSense_CSD_TOTAL_SHIELD_COUNT))
                            CapSense_BistSetAllShieldsHsiomState(CapSense_TST_HSIOM_GPIO);
                        #endif
                        break;
                    }
                #endif /* (CapSense_ENABLE == CapSense_TST_SH_CAP_EN) */
                default:
                {
                    break;
                }
            }
            /* Save the configured state */
            CapSense_bistCurrentISC = config;
        }
    }


    /*******************************************************************************
    * Function Name: CapSense_SetSnsHsiom
    ****************************************************************************//**
    *
    * \brief
    *  The internal function that sets a certain electrode in a demand HSIOM mode.
    *
    * \details
    *  The function sets an electrode to a demand HSIOM mode.
    *
    * \param *ioPtr
    *  A pointer to the specified electrode in the flash IO structure.
    *
    * \param value
    *  A demand HSIOM mode.
    *
    *******************************************************************************/
    static void CapSense_SetSnsHsiom(CapSense_FLASH_IO_STRUCT const *ioPtr, uint32 value)
    {
        uint32 regValue;
        uint8  interruptState;

        /* Update HSIOM port select register */
        interruptState = CyEnterCriticalSection();
        regValue = CY_GET_REG32(ioPtr->hsiomPtr);
        regValue &= ~(ioPtr->hsiomMask);
        regValue |= (value << ioPtr->hsiomShift);
        CY_SET_REG32(ioPtr->hsiomPtr, regValue);
        CyExitCriticalSection(interruptState);
    }


    /*******************************************************************************
    * Function Name: CapSense_SetSnsDr
    ****************************************************************************//**
    *
    * \brief
    *  The internal function that sets a certain electrode port output data.
    *
    * \details
    *  The function sets an electrode port output data register (DR) in a demand state.
    *
    * \param *ioPtr
    *  A pointer to the specified electrode in the flash IO structure.
    *
    * \param value
    *  A port output data which will be setted for an electrode.
    *
    *******************************************************************************/
    static void CapSense_SetSnsDr(CapSense_FLASH_IO_STRUCT const *ioPtr, uint32 value)
    {
        uint32 regValue;
        uint8  interruptState;

        /* Set a data register */
        interruptState = CyEnterCriticalSection();
        regValue = CY_GET_REG32(ioPtr->drPtr);
        regValue &= ~(ioPtr->mask);
        regValue |= value << ioPtr->drShift;
        CY_SET_REG32(ioPtr->drPtr, regValue);
        CyExitCriticalSection(interruptState);
    }


    /*******************************************************************************
    * Function Name: CapSense_SetSnsPc
    ****************************************************************************//**
    *
    * \brief
    *  The internal function that sets a certain electrode drive mode.
    *
    * \details
    *  The function sets an electrode drive mode (PC register) in a demand state.
    *
    * \param *ioPtr
    *  A pointer to the specified electrode in the flash IO structure.
    *
    * \param value
    *  A drive mode which will be setted for an electrode.
    *
    *******************************************************************************/
    static void CapSense_SetSnsPc(CapSense_FLASH_IO_STRUCT const *ioPtr, uint32 value)
    {
        uint32 regValue;
        uint8  interruptState;

        /* Update port configuration register (drive mode) */
        interruptState = CyEnterCriticalSection();
        regValue = CY_GET_REG32(ioPtr->pcPtr);
        regValue &= ~(CapSense_GPIO_PC_MASK << ioPtr->shift);
        regValue |= value << ioPtr->shift;
        CY_SET_REG32(ioPtr->pcPtr, regValue);
        CyExitCriticalSection(interruptState);
    }


    /*******************************************************************************
    * Function Name: CapSense_SetAllSnsHsiomState
    ****************************************************************************//**
    *
    * \brief
    *  The internal function that sets all pins in a demand HSIOM mode.
    *
    * \details
    *  The function sets all pins to a demand HSIOM mode.
    *
    * \param value
    *  A demand HSIOM mode.
    *
    *******************************************************************************/
    static void CapSense_SetAllSnsHsiomState(uint32 value)
    {
        uint32 loopIndex;
        CapSense_FLASH_IO_STRUCT const *ioPtr = &CapSense_ioList[0u];

        /* Loop through all electrodes */
        for (loopIndex = 0u; loopIndex < CapSense_TOTAL_ELECTRODES; loopIndex++)
        {
            CapSense_SetSnsHsiom(ioPtr, value);
            ioPtr++;
        }
    }


    /*******************************************************************************
    * Function Name: CapSense_SetAllSnsDrState
    ****************************************************************************//**
    *
    * \brief
    *  The internal function that sets a specific port output data for all electrodes.
    *
    * \details
    *  The function sets a specific port output data register (DR) in a demand state for all electrodes.
    *
    * \param value
    *  A port output data which will be set for all electrodes.
    *
    *******************************************************************************/
    static void CapSense_SetAllSnsDrState(uint32 value)
    {
        uint32 loopIndex;
        CapSense_FLASH_IO_STRUCT const *ioPtr = &CapSense_ioList[0u];

        /* Loop through all electrodes */
        for (loopIndex = 0u; loopIndex < CapSense_TOTAL_ELECTRODES; loopIndex++)
        {
            CapSense_SetSnsDr(ioPtr, value);
            ioPtr++;
        }
    }


    /*******************************************************************************
    * Function Name: CapSense_SetAllSnsPcState
    ****************************************************************************//**
    *
    * \brief
    *  The internal function that sets a specific drive mode for all electrodes.
    *
    * \details
    *  The function sets a specific drive mode (PC register) in a demand state for all electrodes.
    *
    * \param value
    *  A drive mode which will be set for all electrodes.
    *
    *******************************************************************************/
    static void CapSense_SetAllSnsPcState(uint32 value)
    {
        uint32 loopIndex;
        CapSense_FLASH_IO_STRUCT const *ioPtr = &CapSense_ioList[0u];

        /* Loop through all electrodes */
        for (loopIndex = 0u; loopIndex < CapSense_TOTAL_ELECTRODES; loopIndex++)
        {
            CapSense_SetSnsPc(ioPtr, value);
            ioPtr++;
        }
    }


    #if ((CapSense_ENABLE == CapSense_CSD_SHIELD_EN) && \
         (0u != CapSense_CSD_TOTAL_SHIELD_COUNT))
            /*******************************************************************************
            * Function Name: CapSense_BistSetAllShieldsHsiomState
            ****************************************************************************//**
            *
            * \brief
            *   This internal function connects specified HSIOM selection for all dedicated
            *   shield electrodes.
            *
            * \details
            *   The function changes shield electrodes pin functionality by updating HSIOM
            *   registers value. The shield electrodes port and pin configuration registers
            *   addresses are stored in the CapSense_shieldIoList structure.
            *
            * \param hsiomValue Desired HSIOM selection. Available options:
            *   - CapSense_TST_HSIOM_GPIO
            *   - CapSense_TST_HSIOM_CSD_SENSE
            *   - CapSense_TST_HSIOM_CSD_SHIELD
            *   - CapSense_TST_HSIOM_AMUXA
            *   - CapSense_TST_HSIOM_AMUXB
            *
            *******************************************************************************/
            static void CapSense_BistSetAllShieldsHsiomState(uint32 hsiomValue)
            {
                uint8  interruptState;
                uint32 loopIndex;
                uint32 regValue;

                CapSense_SHIELD_IO_STRUCT const *shieldIoPtr;

                shieldIoPtr = &CapSense_shieldIoList[0u];
                for (loopIndex = 0u; loopIndex < CapSense_CSD_TOTAL_SHIELD_COUNT; loopIndex++)
                {
                    interruptState = CyEnterCriticalSection();
                    regValue = CY_GET_REG32(shieldIoPtr->hsiomPtr);
                    regValue &= ~(shieldIoPtr->hsiomMask);
                    regValue |= (hsiomValue << shieldIoPtr->hsiomShift);
                    CY_SET_REG32(shieldIoPtr->hsiomPtr, regValue);
                    CyExitCriticalSection(interruptState);

                    /* Get next electrode */
                    shieldIoPtr++;
                }
            }


            /*******************************************************************************
            * Function Name: CapSense_BistSetAllShieldsPcState
            ****************************************************************************//**
            *
            * \brief
            *   This internal function sets desired drive mode value for all dedicated
            *   shield electrodes.
            *
            * \details
            *   The function changes shield electrodes drive mode by changing value of
            *   corresponding GPIO port configuration registers. The shield electrodes
            *   port and pin configuration registers addresses are stored in the
            *   CapSense_shieldIoList structure.
            *
            * \param pcValue Desired shield electrodes drive mode. Available values:
            *   - CapSense_TST_PC_HIGHZA
            *   - CapSense_TST_PC_PULLUP
            *   - CapSense_TST_PC_PULLDOWN
            *   - CapSense_TST_PC_STRONG
            *
            *******************************************************************************/
            static void CapSense_BistSetAllShieldsPcState(uint32 pcValue)
            {
                uint8  interruptState;
                uint32 loopIndex;
                uint32 regValue;

                CapSense_SHIELD_IO_STRUCT const *shieldIoPtr;

                shieldIoPtr = &CapSense_shieldIoList[0u];
                for (loopIndex = 0u; loopIndex < CapSense_CSD_TOTAL_SHIELD_COUNT; loopIndex++)
                {
                    interruptState = CyEnterCriticalSection();
                    regValue = CY_GET_REG32(shieldIoPtr->pcPtr);
                    regValue &= ~(CapSense_GPIO_PC_MASK << shieldIoPtr->shift);
                    regValue |=  (pcValue << shieldIoPtr->shift);
                    CY_SET_REG32(shieldIoPtr->pcPtr, regValue);
                    CyExitCriticalSection(interruptState);

                    /* Get next electrode */
                    shieldIoPtr++;
                }
            }

    #endif /* ((CapSense_ENABLE == CapSense_CSD_SHIELD_EN) && \
               (0u != CapSense_CSD_TOTAL_SHIELD_COUNT)) */

#endif /* (CapSense_ENABLE == CapSense_TST_HW_GROUP_EN) */


/* [] END OF FILE */
