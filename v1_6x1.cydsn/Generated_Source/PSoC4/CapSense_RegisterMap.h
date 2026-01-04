/***************************************************************************//**
* \file CapSense_RegisterMap.h
* \version 7.10
*
* \brief
*   This file provides the definitions for the Component data structure register.
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

#if !defined(CY_SENSE_CapSense_REGISTER_MAP_H)
#define CY_SENSE_CapSense_REGISTER_MAP_H

#include <cytypes.h>
#include "CapSense_Configuration.h"
#include "CapSense_Structure.h"

/*****************************************************************************/
/* RAM Data structure register definitions                                   */
/*****************************************************************************/
#define CapSense_CONFIG_ID_VALUE                            (CapSense_dsRam.configId)
#define CapSense_CONFIG_ID_OFFSET                           (0u)
#define CapSense_CONFIG_ID_SIZE                             (2u)
#define CapSense_CONFIG_ID_PARAM_ID                         (0x87000000u)

#define CapSense_DEVICE_ID_VALUE                            (CapSense_dsRam.deviceId)
#define CapSense_DEVICE_ID_OFFSET                           (2u)
#define CapSense_DEVICE_ID_SIZE                             (2u)
#define CapSense_DEVICE_ID_PARAM_ID                         (0x8B000002u)

#define CapSense_HW_CLOCK_VALUE                             (CapSense_dsRam.hwClock)
#define CapSense_HW_CLOCK_OFFSET                            (4u)
#define CapSense_HW_CLOCK_SIZE                              (2u)
#define CapSense_HW_CLOCK_PARAM_ID                          (0x86000004u)

#define CapSense_TUNER_CMD_VALUE                            (CapSense_dsRam.tunerCmd)
#define CapSense_TUNER_CMD_OFFSET                           (6u)
#define CapSense_TUNER_CMD_SIZE                             (2u)
#define CapSense_TUNER_CMD_PARAM_ID                         (0xA1000006u)

#define CapSense_SCAN_COUNTER_VALUE                         (CapSense_dsRam.scanCounter)
#define CapSense_SCAN_COUNTER_OFFSET                        (8u)
#define CapSense_SCAN_COUNTER_SIZE                          (2u)
#define CapSense_SCAN_COUNTER_PARAM_ID                      (0x85000008u)

#define CapSense_STATUS_VALUE                               (CapSense_dsRam.status)
#define CapSense_STATUS_OFFSET                              (12u)
#define CapSense_STATUS_SIZE                                (4u)
#define CapSense_STATUS_PARAM_ID                            (0xCB00000Cu)

#define CapSense_WDGT_ENABLE0_VALUE                         (CapSense_dsRam.wdgtEnable[0u])
#define CapSense_WDGT_ENABLE0_OFFSET                        (16u)
#define CapSense_WDGT_ENABLE0_SIZE                          (4u)
#define CapSense_WDGT_ENABLE0_PARAM_ID                      (0xE6000010u)

#define CapSense_WDGT_WORKING0_VALUE                        (CapSense_dsRam.wdgtWorking[0u])
#define CapSense_WDGT_WORKING0_OFFSET                       (20u)
#define CapSense_WDGT_WORKING0_SIZE                         (4u)
#define CapSense_WDGT_WORKING0_PARAM_ID                     (0xCC000014u)

#define CapSense_WDGT_STATUS0_VALUE                         (CapSense_dsRam.wdgtStatus[0u])
#define CapSense_WDGT_STATUS0_OFFSET                        (24u)
#define CapSense_WDGT_STATUS0_SIZE                          (4u)
#define CapSense_WDGT_STATUS0_PARAM_ID                      (0xCF000018u)

#define CapSense_SNS_STATUS0_VALUE                          (CapSense_dsRam.snsStatus[0u])
#define CapSense_SNS_STATUS0_OFFSET                         (28u)
#define CapSense_SNS_STATUS0_SIZE                           (1u)
#define CapSense_SNS_STATUS0_PARAM_ID                       (0x4900001Cu)

#define CapSense_SNS_STATUS1_VALUE                          (CapSense_dsRam.snsStatus[1u])
#define CapSense_SNS_STATUS1_OFFSET                         (29u)
#define CapSense_SNS_STATUS1_SIZE                           (1u)
#define CapSense_SNS_STATUS1_PARAM_ID                       (0x4F00001Du)

#define CapSense_SNS_STATUS2_VALUE                          (CapSense_dsRam.snsStatus[2u])
#define CapSense_SNS_STATUS2_OFFSET                         (30u)
#define CapSense_SNS_STATUS2_SIZE                           (1u)
#define CapSense_SNS_STATUS2_PARAM_ID                       (0x4500001Eu)

#define CapSense_SNS_STATUS3_VALUE                          (CapSense_dsRam.snsStatus[3u])
#define CapSense_SNS_STATUS3_OFFSET                         (31u)
#define CapSense_SNS_STATUS3_SIZE                           (1u)
#define CapSense_SNS_STATUS3_PARAM_ID                       (0x4300001Fu)

#define CapSense_SNS_STATUS4_VALUE                          (CapSense_dsRam.snsStatus[4u])
#define CapSense_SNS_STATUS4_OFFSET                         (32u)
#define CapSense_SNS_STATUS4_SIZE                           (1u)
#define CapSense_SNS_STATUS4_PARAM_ID                       (0x45000020u)

#define CapSense_SNS_STATUS5_VALUE                          (CapSense_dsRam.snsStatus[5u])
#define CapSense_SNS_STATUS5_OFFSET                         (33u)
#define CapSense_SNS_STATUS5_SIZE                           (1u)
#define CapSense_SNS_STATUS5_PARAM_ID                       (0x43000021u)

#define CapSense_CSD0_CONFIG_VALUE                          (CapSense_dsRam.csd0Config)
#define CapSense_CSD0_CONFIG_OFFSET                         (34u)
#define CapSense_CSD0_CONFIG_SIZE                           (2u)
#define CapSense_CSD0_CONFIG_PARAM_ID                       (0xA7800022u)

#define CapSense_MOD_CSX_CLK_VALUE                          (CapSense_dsRam.modCsxClk)
#define CapSense_MOD_CSX_CLK_OFFSET                         (36u)
#define CapSense_MOD_CSX_CLK_SIZE                           (1u)
#define CapSense_MOD_CSX_CLK_PARAM_ID                       (0x62800024u)

#define CapSense_GLB_CRC_VALUE                              (CapSense_dsRam.glbCrc)
#define CapSense_GLB_CRC_OFFSET                             (38u)
#define CapSense_GLB_CRC_SIZE                               (2u)
#define CapSense_GLB_CRC_PARAM_ID                           (0x80000026u)

#define CapSense_BUTTON0_CRC_VALUE                          (CapSense_dsRam.wdgtList.button0.crc)
#define CapSense_BUTTON0_CRC_OFFSET                         (40u)
#define CapSense_BUTTON0_CRC_SIZE                           (2u)
#define CapSense_BUTTON0_CRC_PARAM_ID                       (0x8F000028u)

#define CapSense_BUTTON0_RESOLUTION_VALUE                   (CapSense_dsRam.wdgtList.button0.resolution)
#define CapSense_BUTTON0_RESOLUTION_OFFSET                  (42u)
#define CapSense_BUTTON0_RESOLUTION_SIZE                    (2u)
#define CapSense_BUTTON0_RESOLUTION_PARAM_ID                (0xA580002Au)

#define CapSense_BUTTON0_FINGER_TH_VALUE                    (CapSense_dsRam.wdgtList.button0.fingerTh)
#define CapSense_BUTTON0_FINGER_TH_OFFSET                   (44u)
#define CapSense_BUTTON0_FINGER_TH_SIZE                     (2u)
#define CapSense_BUTTON0_FINGER_TH_PARAM_ID                 (0xA880002Cu)

#define CapSense_BUTTON0_NOISE_TH_VALUE                     (CapSense_dsRam.wdgtList.button0.noiseTh)
#define CapSense_BUTTON0_NOISE_TH_OFFSET                    (46u)
#define CapSense_BUTTON0_NOISE_TH_SIZE                      (1u)
#define CapSense_BUTTON0_NOISE_TH_PARAM_ID                  (0x6C80002Eu)

#define CapSense_BUTTON0_NNOISE_TH_VALUE                    (CapSense_dsRam.wdgtList.button0.nNoiseTh)
#define CapSense_BUTTON0_NNOISE_TH_OFFSET                   (47u)
#define CapSense_BUTTON0_NNOISE_TH_SIZE                     (1u)
#define CapSense_BUTTON0_NNOISE_TH_PARAM_ID                 (0x6A80002Fu)

#define CapSense_BUTTON0_HYSTERESIS_VALUE                   (CapSense_dsRam.wdgtList.button0.hysteresis)
#define CapSense_BUTTON0_HYSTERESIS_OFFSET                  (48u)
#define CapSense_BUTTON0_HYSTERESIS_SIZE                    (1u)
#define CapSense_BUTTON0_HYSTERESIS_PARAM_ID                (0x66800030u)

#define CapSense_BUTTON0_ON_DEBOUNCE_VALUE                  (CapSense_dsRam.wdgtList.button0.onDebounce)
#define CapSense_BUTTON0_ON_DEBOUNCE_OFFSET                 (49u)
#define CapSense_BUTTON0_ON_DEBOUNCE_SIZE                   (1u)
#define CapSense_BUTTON0_ON_DEBOUNCE_PARAM_ID               (0x60800031u)

#define CapSense_BUTTON0_LOW_BSLN_RST_VALUE                 (CapSense_dsRam.wdgtList.button0.lowBslnRst)
#define CapSense_BUTTON0_LOW_BSLN_RST_OFFSET                (50u)
#define CapSense_BUTTON0_LOW_BSLN_RST_SIZE                  (1u)
#define CapSense_BUTTON0_LOW_BSLN_RST_PARAM_ID              (0x6A800032u)

#define CapSense_BUTTON0_IDAC_GAIN_INDEX_VALUE              (CapSense_dsRam.wdgtList.button0.idacGainIndex)
#define CapSense_BUTTON0_IDAC_GAIN_INDEX_OFFSET             (51u)
#define CapSense_BUTTON0_IDAC_GAIN_INDEX_SIZE               (1u)
#define CapSense_BUTTON0_IDAC_GAIN_INDEX_PARAM_ID           (0x6C800033u)

#define CapSense_BUTTON0_SNS_CLK_VALUE                      (CapSense_dsRam.wdgtList.button0.snsClk)
#define CapSense_BUTTON0_SNS_CLK_OFFSET                     (52u)
#define CapSense_BUTTON0_SNS_CLK_SIZE                       (2u)
#define CapSense_BUTTON0_SNS_CLK_PARAM_ID                   (0xAF800034u)

#define CapSense_BUTTON0_SNS_CLK_SOURCE_VALUE               (CapSense_dsRam.wdgtList.button0.snsClkSource)
#define CapSense_BUTTON0_SNS_CLK_SOURCE_OFFSET              (54u)
#define CapSense_BUTTON0_SNS_CLK_SOURCE_SIZE                (1u)
#define CapSense_BUTTON0_SNS_CLK_SOURCE_PARAM_ID            (0x40800036u)

#define CapSense_BUTTON1_CRC_VALUE                          (CapSense_dsRam.wdgtList.button1.crc)
#define CapSense_BUTTON1_CRC_OFFSET                         (56u)
#define CapSense_BUTTON1_CRC_SIZE                           (2u)
#define CapSense_BUTTON1_CRC_PARAM_ID                       (0x89010038u)

#define CapSense_BUTTON1_RESOLUTION_VALUE                   (CapSense_dsRam.wdgtList.button1.resolution)
#define CapSense_BUTTON1_RESOLUTION_OFFSET                  (58u)
#define CapSense_BUTTON1_RESOLUTION_SIZE                    (2u)
#define CapSense_BUTTON1_RESOLUTION_PARAM_ID                (0xA381003Au)

#define CapSense_BUTTON1_FINGER_TH_VALUE                    (CapSense_dsRam.wdgtList.button1.fingerTh)
#define CapSense_BUTTON1_FINGER_TH_OFFSET                   (60u)
#define CapSense_BUTTON1_FINGER_TH_SIZE                     (2u)
#define CapSense_BUTTON1_FINGER_TH_PARAM_ID                 (0xAE81003Cu)

#define CapSense_BUTTON1_NOISE_TH_VALUE                     (CapSense_dsRam.wdgtList.button1.noiseTh)
#define CapSense_BUTTON1_NOISE_TH_OFFSET                    (62u)
#define CapSense_BUTTON1_NOISE_TH_SIZE                      (1u)
#define CapSense_BUTTON1_NOISE_TH_PARAM_ID                  (0x6A81003Eu)

#define CapSense_BUTTON1_NNOISE_TH_VALUE                    (CapSense_dsRam.wdgtList.button1.nNoiseTh)
#define CapSense_BUTTON1_NNOISE_TH_OFFSET                   (63u)
#define CapSense_BUTTON1_NNOISE_TH_SIZE                     (1u)
#define CapSense_BUTTON1_NNOISE_TH_PARAM_ID                 (0x6C81003Fu)

#define CapSense_BUTTON1_HYSTERESIS_VALUE                   (CapSense_dsRam.wdgtList.button1.hysteresis)
#define CapSense_BUTTON1_HYSTERESIS_OFFSET                  (64u)
#define CapSense_BUTTON1_HYSTERESIS_SIZE                    (1u)
#define CapSense_BUTTON1_HYSTERESIS_PARAM_ID                (0x67810040u)

#define CapSense_BUTTON1_ON_DEBOUNCE_VALUE                  (CapSense_dsRam.wdgtList.button1.onDebounce)
#define CapSense_BUTTON1_ON_DEBOUNCE_OFFSET                 (65u)
#define CapSense_BUTTON1_ON_DEBOUNCE_SIZE                   (1u)
#define CapSense_BUTTON1_ON_DEBOUNCE_PARAM_ID               (0x61810041u)

#define CapSense_BUTTON1_LOW_BSLN_RST_VALUE                 (CapSense_dsRam.wdgtList.button1.lowBslnRst)
#define CapSense_BUTTON1_LOW_BSLN_RST_OFFSET                (66u)
#define CapSense_BUTTON1_LOW_BSLN_RST_SIZE                  (1u)
#define CapSense_BUTTON1_LOW_BSLN_RST_PARAM_ID              (0x6B810042u)

#define CapSense_BUTTON1_IDAC_GAIN_INDEX_VALUE              (CapSense_dsRam.wdgtList.button1.idacGainIndex)
#define CapSense_BUTTON1_IDAC_GAIN_INDEX_OFFSET             (67u)
#define CapSense_BUTTON1_IDAC_GAIN_INDEX_SIZE               (1u)
#define CapSense_BUTTON1_IDAC_GAIN_INDEX_PARAM_ID           (0x6D810043u)

#define CapSense_BUTTON1_SNS_CLK_VALUE                      (CapSense_dsRam.wdgtList.button1.snsClk)
#define CapSense_BUTTON1_SNS_CLK_OFFSET                     (68u)
#define CapSense_BUTTON1_SNS_CLK_SIZE                       (2u)
#define CapSense_BUTTON1_SNS_CLK_PARAM_ID                   (0xAE810044u)

#define CapSense_BUTTON1_SNS_CLK_SOURCE_VALUE               (CapSense_dsRam.wdgtList.button1.snsClkSource)
#define CapSense_BUTTON1_SNS_CLK_SOURCE_OFFSET              (70u)
#define CapSense_BUTTON1_SNS_CLK_SOURCE_SIZE                (1u)
#define CapSense_BUTTON1_SNS_CLK_SOURCE_PARAM_ID            (0x41810046u)

#define CapSense_BUTTON2_CRC_VALUE                          (CapSense_dsRam.wdgtList.button2.crc)
#define CapSense_BUTTON2_CRC_OFFSET                         (72u)
#define CapSense_BUTTON2_CRC_SIZE                           (2u)
#define CapSense_BUTTON2_CRC_PARAM_ID                       (0x8E020048u)

#define CapSense_BUTTON2_RESOLUTION_VALUE                   (CapSense_dsRam.wdgtList.button2.resolution)
#define CapSense_BUTTON2_RESOLUTION_OFFSET                  (74u)
#define CapSense_BUTTON2_RESOLUTION_SIZE                    (2u)
#define CapSense_BUTTON2_RESOLUTION_PARAM_ID                (0xA482004Au)

#define CapSense_BUTTON2_FINGER_TH_VALUE                    (CapSense_dsRam.wdgtList.button2.fingerTh)
#define CapSense_BUTTON2_FINGER_TH_OFFSET                   (76u)
#define CapSense_BUTTON2_FINGER_TH_SIZE                     (2u)
#define CapSense_BUTTON2_FINGER_TH_PARAM_ID                 (0xA982004Cu)

#define CapSense_BUTTON2_NOISE_TH_VALUE                     (CapSense_dsRam.wdgtList.button2.noiseTh)
#define CapSense_BUTTON2_NOISE_TH_OFFSET                    (78u)
#define CapSense_BUTTON2_NOISE_TH_SIZE                      (1u)
#define CapSense_BUTTON2_NOISE_TH_PARAM_ID                  (0x6D82004Eu)

#define CapSense_BUTTON2_NNOISE_TH_VALUE                    (CapSense_dsRam.wdgtList.button2.nNoiseTh)
#define CapSense_BUTTON2_NNOISE_TH_OFFSET                   (79u)
#define CapSense_BUTTON2_NNOISE_TH_SIZE                     (1u)
#define CapSense_BUTTON2_NNOISE_TH_PARAM_ID                 (0x6B82004Fu)

#define CapSense_BUTTON2_HYSTERESIS_VALUE                   (CapSense_dsRam.wdgtList.button2.hysteresis)
#define CapSense_BUTTON2_HYSTERESIS_OFFSET                  (80u)
#define CapSense_BUTTON2_HYSTERESIS_SIZE                    (1u)
#define CapSense_BUTTON2_HYSTERESIS_PARAM_ID                (0x67820050u)

#define CapSense_BUTTON2_ON_DEBOUNCE_VALUE                  (CapSense_dsRam.wdgtList.button2.onDebounce)
#define CapSense_BUTTON2_ON_DEBOUNCE_OFFSET                 (81u)
#define CapSense_BUTTON2_ON_DEBOUNCE_SIZE                   (1u)
#define CapSense_BUTTON2_ON_DEBOUNCE_PARAM_ID               (0x61820051u)

#define CapSense_BUTTON2_LOW_BSLN_RST_VALUE                 (CapSense_dsRam.wdgtList.button2.lowBslnRst)
#define CapSense_BUTTON2_LOW_BSLN_RST_OFFSET                (82u)
#define CapSense_BUTTON2_LOW_BSLN_RST_SIZE                  (1u)
#define CapSense_BUTTON2_LOW_BSLN_RST_PARAM_ID              (0x6B820052u)

#define CapSense_BUTTON2_IDAC_GAIN_INDEX_VALUE              (CapSense_dsRam.wdgtList.button2.idacGainIndex)
#define CapSense_BUTTON2_IDAC_GAIN_INDEX_OFFSET             (83u)
#define CapSense_BUTTON2_IDAC_GAIN_INDEX_SIZE               (1u)
#define CapSense_BUTTON2_IDAC_GAIN_INDEX_PARAM_ID           (0x6D820053u)

#define CapSense_BUTTON2_SNS_CLK_VALUE                      (CapSense_dsRam.wdgtList.button2.snsClk)
#define CapSense_BUTTON2_SNS_CLK_OFFSET                     (84u)
#define CapSense_BUTTON2_SNS_CLK_SIZE                       (2u)
#define CapSense_BUTTON2_SNS_CLK_PARAM_ID                   (0xAE820054u)

#define CapSense_BUTTON2_SNS_CLK_SOURCE_VALUE               (CapSense_dsRam.wdgtList.button2.snsClkSource)
#define CapSense_BUTTON2_SNS_CLK_SOURCE_OFFSET              (86u)
#define CapSense_BUTTON2_SNS_CLK_SOURCE_SIZE                (1u)
#define CapSense_BUTTON2_SNS_CLK_SOURCE_PARAM_ID            (0x41820056u)

#define CapSense_BUTTON3_CRC_VALUE                          (CapSense_dsRam.wdgtList.button3.crc)
#define CapSense_BUTTON3_CRC_OFFSET                         (88u)
#define CapSense_BUTTON3_CRC_SIZE                           (2u)
#define CapSense_BUTTON3_CRC_PARAM_ID                       (0x88030058u)

#define CapSense_BUTTON3_RESOLUTION_VALUE                   (CapSense_dsRam.wdgtList.button3.resolution)
#define CapSense_BUTTON3_RESOLUTION_OFFSET                  (90u)
#define CapSense_BUTTON3_RESOLUTION_SIZE                    (2u)
#define CapSense_BUTTON3_RESOLUTION_PARAM_ID                (0xA283005Au)

#define CapSense_BUTTON3_FINGER_TH_VALUE                    (CapSense_dsRam.wdgtList.button3.fingerTh)
#define CapSense_BUTTON3_FINGER_TH_OFFSET                   (92u)
#define CapSense_BUTTON3_FINGER_TH_SIZE                     (2u)
#define CapSense_BUTTON3_FINGER_TH_PARAM_ID                 (0xAF83005Cu)

#define CapSense_BUTTON3_NOISE_TH_VALUE                     (CapSense_dsRam.wdgtList.button3.noiseTh)
#define CapSense_BUTTON3_NOISE_TH_OFFSET                    (94u)
#define CapSense_BUTTON3_NOISE_TH_SIZE                      (1u)
#define CapSense_BUTTON3_NOISE_TH_PARAM_ID                  (0x6B83005Eu)

#define CapSense_BUTTON3_NNOISE_TH_VALUE                    (CapSense_dsRam.wdgtList.button3.nNoiseTh)
#define CapSense_BUTTON3_NNOISE_TH_OFFSET                   (95u)
#define CapSense_BUTTON3_NNOISE_TH_SIZE                     (1u)
#define CapSense_BUTTON3_NNOISE_TH_PARAM_ID                 (0x6D83005Fu)

#define CapSense_BUTTON3_HYSTERESIS_VALUE                   (CapSense_dsRam.wdgtList.button3.hysteresis)
#define CapSense_BUTTON3_HYSTERESIS_OFFSET                  (96u)
#define CapSense_BUTTON3_HYSTERESIS_SIZE                    (1u)
#define CapSense_BUTTON3_HYSTERESIS_PARAM_ID                (0x6B830060u)

#define CapSense_BUTTON3_ON_DEBOUNCE_VALUE                  (CapSense_dsRam.wdgtList.button3.onDebounce)
#define CapSense_BUTTON3_ON_DEBOUNCE_OFFSET                 (97u)
#define CapSense_BUTTON3_ON_DEBOUNCE_SIZE                   (1u)
#define CapSense_BUTTON3_ON_DEBOUNCE_PARAM_ID               (0x6D830061u)

#define CapSense_BUTTON3_LOW_BSLN_RST_VALUE                 (CapSense_dsRam.wdgtList.button3.lowBslnRst)
#define CapSense_BUTTON3_LOW_BSLN_RST_OFFSET                (98u)
#define CapSense_BUTTON3_LOW_BSLN_RST_SIZE                  (1u)
#define CapSense_BUTTON3_LOW_BSLN_RST_PARAM_ID              (0x67830062u)

#define CapSense_BUTTON3_IDAC_GAIN_INDEX_VALUE              (CapSense_dsRam.wdgtList.button3.idacGainIndex)
#define CapSense_BUTTON3_IDAC_GAIN_INDEX_OFFSET             (99u)
#define CapSense_BUTTON3_IDAC_GAIN_INDEX_SIZE               (1u)
#define CapSense_BUTTON3_IDAC_GAIN_INDEX_PARAM_ID           (0x61830063u)

#define CapSense_BUTTON3_SNS_CLK_VALUE                      (CapSense_dsRam.wdgtList.button3.snsClk)
#define CapSense_BUTTON3_SNS_CLK_OFFSET                     (100u)
#define CapSense_BUTTON3_SNS_CLK_SIZE                       (2u)
#define CapSense_BUTTON3_SNS_CLK_PARAM_ID                   (0xA2830064u)

#define CapSense_BUTTON3_SNS_CLK_SOURCE_VALUE               (CapSense_dsRam.wdgtList.button3.snsClkSource)
#define CapSense_BUTTON3_SNS_CLK_SOURCE_OFFSET              (102u)
#define CapSense_BUTTON3_SNS_CLK_SOURCE_SIZE                (1u)
#define CapSense_BUTTON3_SNS_CLK_SOURCE_PARAM_ID            (0x4D830066u)

#define CapSense_BUTTON4_CRC_VALUE                          (CapSense_dsRam.wdgtList.button4.crc)
#define CapSense_BUTTON4_CRC_OFFSET                         (104u)
#define CapSense_BUTTON4_CRC_SIZE                           (2u)
#define CapSense_BUTTON4_CRC_PARAM_ID                       (0x8E040068u)

#define CapSense_BUTTON4_RESOLUTION_VALUE                   (CapSense_dsRam.wdgtList.button4.resolution)
#define CapSense_BUTTON4_RESOLUTION_OFFSET                  (106u)
#define CapSense_BUTTON4_RESOLUTION_SIZE                    (2u)
#define CapSense_BUTTON4_RESOLUTION_PARAM_ID                (0xA484006Au)

#define CapSense_BUTTON4_FINGER_TH_VALUE                    (CapSense_dsRam.wdgtList.button4.fingerTh)
#define CapSense_BUTTON4_FINGER_TH_OFFSET                   (108u)
#define CapSense_BUTTON4_FINGER_TH_SIZE                     (2u)
#define CapSense_BUTTON4_FINGER_TH_PARAM_ID                 (0xA984006Cu)

#define CapSense_BUTTON4_NOISE_TH_VALUE                     (CapSense_dsRam.wdgtList.button4.noiseTh)
#define CapSense_BUTTON4_NOISE_TH_OFFSET                    (110u)
#define CapSense_BUTTON4_NOISE_TH_SIZE                      (1u)
#define CapSense_BUTTON4_NOISE_TH_PARAM_ID                  (0x6D84006Eu)

#define CapSense_BUTTON4_NNOISE_TH_VALUE                    (CapSense_dsRam.wdgtList.button4.nNoiseTh)
#define CapSense_BUTTON4_NNOISE_TH_OFFSET                   (111u)
#define CapSense_BUTTON4_NNOISE_TH_SIZE                     (1u)
#define CapSense_BUTTON4_NNOISE_TH_PARAM_ID                 (0x6B84006Fu)

#define CapSense_BUTTON4_HYSTERESIS_VALUE                   (CapSense_dsRam.wdgtList.button4.hysteresis)
#define CapSense_BUTTON4_HYSTERESIS_OFFSET                  (112u)
#define CapSense_BUTTON4_HYSTERESIS_SIZE                    (1u)
#define CapSense_BUTTON4_HYSTERESIS_PARAM_ID                (0x67840070u)

#define CapSense_BUTTON4_ON_DEBOUNCE_VALUE                  (CapSense_dsRam.wdgtList.button4.onDebounce)
#define CapSense_BUTTON4_ON_DEBOUNCE_OFFSET                 (113u)
#define CapSense_BUTTON4_ON_DEBOUNCE_SIZE                   (1u)
#define CapSense_BUTTON4_ON_DEBOUNCE_PARAM_ID               (0x61840071u)

#define CapSense_BUTTON4_LOW_BSLN_RST_VALUE                 (CapSense_dsRam.wdgtList.button4.lowBslnRst)
#define CapSense_BUTTON4_LOW_BSLN_RST_OFFSET                (114u)
#define CapSense_BUTTON4_LOW_BSLN_RST_SIZE                  (1u)
#define CapSense_BUTTON4_LOW_BSLN_RST_PARAM_ID              (0x6B840072u)

#define CapSense_BUTTON4_IDAC_GAIN_INDEX_VALUE              (CapSense_dsRam.wdgtList.button4.idacGainIndex)
#define CapSense_BUTTON4_IDAC_GAIN_INDEX_OFFSET             (115u)
#define CapSense_BUTTON4_IDAC_GAIN_INDEX_SIZE               (1u)
#define CapSense_BUTTON4_IDAC_GAIN_INDEX_PARAM_ID           (0x6D840073u)

#define CapSense_BUTTON4_SNS_CLK_VALUE                      (CapSense_dsRam.wdgtList.button4.snsClk)
#define CapSense_BUTTON4_SNS_CLK_OFFSET                     (116u)
#define CapSense_BUTTON4_SNS_CLK_SIZE                       (2u)
#define CapSense_BUTTON4_SNS_CLK_PARAM_ID                   (0xAE840074u)

#define CapSense_BUTTON4_SNS_CLK_SOURCE_VALUE               (CapSense_dsRam.wdgtList.button4.snsClkSource)
#define CapSense_BUTTON4_SNS_CLK_SOURCE_OFFSET              (118u)
#define CapSense_BUTTON4_SNS_CLK_SOURCE_SIZE                (1u)
#define CapSense_BUTTON4_SNS_CLK_SOURCE_PARAM_ID            (0x41840076u)

#define CapSense_BUTTON5_CRC_VALUE                          (CapSense_dsRam.wdgtList.button5.crc)
#define CapSense_BUTTON5_CRC_OFFSET                         (120u)
#define CapSense_BUTTON5_CRC_SIZE                           (2u)
#define CapSense_BUTTON5_CRC_PARAM_ID                       (0x88050078u)

#define CapSense_BUTTON5_RESOLUTION_VALUE                   (CapSense_dsRam.wdgtList.button5.resolution)
#define CapSense_BUTTON5_RESOLUTION_OFFSET                  (122u)
#define CapSense_BUTTON5_RESOLUTION_SIZE                    (2u)
#define CapSense_BUTTON5_RESOLUTION_PARAM_ID                (0xA285007Au)

#define CapSense_BUTTON5_FINGER_TH_VALUE                    (CapSense_dsRam.wdgtList.button5.fingerTh)
#define CapSense_BUTTON5_FINGER_TH_OFFSET                   (124u)
#define CapSense_BUTTON5_FINGER_TH_SIZE                     (2u)
#define CapSense_BUTTON5_FINGER_TH_PARAM_ID                 (0xAF85007Cu)

#define CapSense_BUTTON5_NOISE_TH_VALUE                     (CapSense_dsRam.wdgtList.button5.noiseTh)
#define CapSense_BUTTON5_NOISE_TH_OFFSET                    (126u)
#define CapSense_BUTTON5_NOISE_TH_SIZE                      (1u)
#define CapSense_BUTTON5_NOISE_TH_PARAM_ID                  (0x6B85007Eu)

#define CapSense_BUTTON5_NNOISE_TH_VALUE                    (CapSense_dsRam.wdgtList.button5.nNoiseTh)
#define CapSense_BUTTON5_NNOISE_TH_OFFSET                   (127u)
#define CapSense_BUTTON5_NNOISE_TH_SIZE                     (1u)
#define CapSense_BUTTON5_NNOISE_TH_PARAM_ID                 (0x6D85007Fu)

#define CapSense_BUTTON5_HYSTERESIS_VALUE                   (CapSense_dsRam.wdgtList.button5.hysteresis)
#define CapSense_BUTTON5_HYSTERESIS_OFFSET                  (128u)
#define CapSense_BUTTON5_HYSTERESIS_SIZE                    (1u)
#define CapSense_BUTTON5_HYSTERESIS_PARAM_ID                (0x65850080u)

#define CapSense_BUTTON5_ON_DEBOUNCE_VALUE                  (CapSense_dsRam.wdgtList.button5.onDebounce)
#define CapSense_BUTTON5_ON_DEBOUNCE_OFFSET                 (129u)
#define CapSense_BUTTON5_ON_DEBOUNCE_SIZE                   (1u)
#define CapSense_BUTTON5_ON_DEBOUNCE_PARAM_ID               (0x63850081u)

#define CapSense_BUTTON5_LOW_BSLN_RST_VALUE                 (CapSense_dsRam.wdgtList.button5.lowBslnRst)
#define CapSense_BUTTON5_LOW_BSLN_RST_OFFSET                (130u)
#define CapSense_BUTTON5_LOW_BSLN_RST_SIZE                  (1u)
#define CapSense_BUTTON5_LOW_BSLN_RST_PARAM_ID              (0x69850082u)

#define CapSense_BUTTON5_IDAC_GAIN_INDEX_VALUE              (CapSense_dsRam.wdgtList.button5.idacGainIndex)
#define CapSense_BUTTON5_IDAC_GAIN_INDEX_OFFSET             (131u)
#define CapSense_BUTTON5_IDAC_GAIN_INDEX_SIZE               (1u)
#define CapSense_BUTTON5_IDAC_GAIN_INDEX_PARAM_ID           (0x6F850083u)

#define CapSense_BUTTON5_SNS_CLK_VALUE                      (CapSense_dsRam.wdgtList.button5.snsClk)
#define CapSense_BUTTON5_SNS_CLK_OFFSET                     (132u)
#define CapSense_BUTTON5_SNS_CLK_SIZE                       (2u)
#define CapSense_BUTTON5_SNS_CLK_PARAM_ID                   (0xAC850084u)

#define CapSense_BUTTON5_SNS_CLK_SOURCE_VALUE               (CapSense_dsRam.wdgtList.button5.snsClkSource)
#define CapSense_BUTTON5_SNS_CLK_SOURCE_OFFSET              (134u)
#define CapSense_BUTTON5_SNS_CLK_SOURCE_SIZE                (1u)
#define CapSense_BUTTON5_SNS_CLK_SOURCE_PARAM_ID            (0x43850086u)

#define CapSense_BUTTON0_RX0_RAW0_VALUE                     (CapSense_dsRam.snsList.button0[0u].raw[0u])
#define CapSense_BUTTON0_RX0_RAW0_OFFSET                    (136u)
#define CapSense_BUTTON0_RX0_RAW0_SIZE                      (2u)
#define CapSense_BUTTON0_RX0_RAW0_PARAM_ID                  (0x86000088u)

#define CapSense_BUTTON0_RX0_BSLN0_VALUE                    (CapSense_dsRam.snsList.button0[0u].bsln[0u])
#define CapSense_BUTTON0_RX0_BSLN0_OFFSET                   (138u)
#define CapSense_BUTTON0_RX0_BSLN0_SIZE                     (2u)
#define CapSense_BUTTON0_RX0_BSLN0_PARAM_ID                 (0x8A00008Au)

#define CapSense_BUTTON0_RX0_BSLN_INV0_VALUE                (CapSense_dsRam.snsList.button0[0u].bslnInv[0u])
#define CapSense_BUTTON0_RX0_BSLN_INV0_OFFSET               (140u)
#define CapSense_BUTTON0_RX0_BSLN_INV0_SIZE                 (2u)
#define CapSense_BUTTON0_RX0_BSLN_INV0_PARAM_ID             (0x8700008Cu)

#define CapSense_BUTTON0_RX0_BSLN_EXT0_VALUE                (CapSense_dsRam.snsList.button0[0u].bslnExt[0u])
#define CapSense_BUTTON0_RX0_BSLN_EXT0_OFFSET               (142u)
#define CapSense_BUTTON0_RX0_BSLN_EXT0_SIZE                 (1u)
#define CapSense_BUTTON0_RX0_BSLN_EXT0_PARAM_ID             (0x4300008Eu)

#define CapSense_BUTTON0_RX0_DIFF_VALUE                     (CapSense_dsRam.snsList.button0[0u].diff)
#define CapSense_BUTTON0_RX0_DIFF_OFFSET                    (144u)
#define CapSense_BUTTON0_RX0_DIFF_SIZE                      (2u)
#define CapSense_BUTTON0_RX0_DIFF_PARAM_ID                  (0x81000090u)

#define CapSense_BUTTON0_RX0_NEG_BSLN_RST_CNT0_VALUE        (CapSense_dsRam.snsList.button0[0u].negBslnRstCnt[0u])
#define CapSense_BUTTON0_RX0_NEG_BSLN_RST_CNT0_OFFSET       (146u)
#define CapSense_BUTTON0_RX0_NEG_BSLN_RST_CNT0_SIZE         (1u)
#define CapSense_BUTTON0_RX0_NEG_BSLN_RST_CNT0_PARAM_ID     (0x45000092u)

#define CapSense_BUTTON0_RX0_IDAC_COMP0_VALUE               (CapSense_dsRam.snsList.button0[0u].idacComp[0u])
#define CapSense_BUTTON0_RX0_IDAC_COMP0_OFFSET              (147u)
#define CapSense_BUTTON0_RX0_IDAC_COMP0_SIZE                (1u)
#define CapSense_BUTTON0_RX0_IDAC_COMP0_PARAM_ID            (0x43000093u)

#define CapSense_BUTTON0_RX1_RAW0_VALUE                     (CapSense_dsRam.snsList.button0[1u].raw[0u])
#define CapSense_BUTTON0_RX1_RAW0_OFFSET                    (148u)
#define CapSense_BUTTON0_RX1_RAW0_SIZE                      (2u)
#define CapSense_BUTTON0_RX1_RAW0_PARAM_ID                  (0x80000094u)

#define CapSense_BUTTON0_RX1_BSLN0_VALUE                    (CapSense_dsRam.snsList.button0[1u].bsln[0u])
#define CapSense_BUTTON0_RX1_BSLN0_OFFSET                   (150u)
#define CapSense_BUTTON0_RX1_BSLN0_SIZE                     (2u)
#define CapSense_BUTTON0_RX1_BSLN0_PARAM_ID                 (0x8C000096u)

#define CapSense_BUTTON0_RX1_BSLN_INV0_VALUE                (CapSense_dsRam.snsList.button0[1u].bslnInv[0u])
#define CapSense_BUTTON0_RX1_BSLN_INV0_OFFSET               (152u)
#define CapSense_BUTTON0_RX1_BSLN_INV0_SIZE                 (2u)
#define CapSense_BUTTON0_RX1_BSLN_INV0_PARAM_ID             (0x83000098u)

#define CapSense_BUTTON0_RX1_BSLN_EXT0_VALUE                (CapSense_dsRam.snsList.button0[1u].bslnExt[0u])
#define CapSense_BUTTON0_RX1_BSLN_EXT0_OFFSET               (154u)
#define CapSense_BUTTON0_RX1_BSLN_EXT0_SIZE                 (1u)
#define CapSense_BUTTON0_RX1_BSLN_EXT0_PARAM_ID             (0x4700009Au)

#define CapSense_BUTTON0_RX1_DIFF_VALUE                     (CapSense_dsRam.snsList.button0[1u].diff)
#define CapSense_BUTTON0_RX1_DIFF_OFFSET                    (156u)
#define CapSense_BUTTON0_RX1_DIFF_SIZE                      (2u)
#define CapSense_BUTTON0_RX1_DIFF_PARAM_ID                  (0x8200009Cu)

#define CapSense_BUTTON0_RX1_NEG_BSLN_RST_CNT0_VALUE        (CapSense_dsRam.snsList.button0[1u].negBslnRstCnt[0u])
#define CapSense_BUTTON0_RX1_NEG_BSLN_RST_CNT0_OFFSET       (158u)
#define CapSense_BUTTON0_RX1_NEG_BSLN_RST_CNT0_SIZE         (1u)
#define CapSense_BUTTON0_RX1_NEG_BSLN_RST_CNT0_PARAM_ID     (0x4600009Eu)

#define CapSense_BUTTON0_RX1_IDAC_COMP0_VALUE               (CapSense_dsRam.snsList.button0[1u].idacComp[0u])
#define CapSense_BUTTON0_RX1_IDAC_COMP0_OFFSET              (159u)
#define CapSense_BUTTON0_RX1_IDAC_COMP0_SIZE                (1u)
#define CapSense_BUTTON0_RX1_IDAC_COMP0_PARAM_ID            (0x4000009Fu)

#define CapSense_BUTTON1_RX0_RAW0_VALUE                     (CapSense_dsRam.snsList.button1[0u].raw[0u])
#define CapSense_BUTTON1_RX0_RAW0_OFFSET                    (160u)
#define CapSense_BUTTON1_RX0_RAW0_SIZE                      (2u)
#define CapSense_BUTTON1_RX0_RAW0_PARAM_ID                  (0x8E0000A0u)

#define CapSense_BUTTON1_RX0_BSLN0_VALUE                    (CapSense_dsRam.snsList.button1[0u].bsln[0u])
#define CapSense_BUTTON1_RX0_BSLN0_OFFSET                   (162u)
#define CapSense_BUTTON1_RX0_BSLN0_SIZE                     (2u)
#define CapSense_BUTTON1_RX0_BSLN0_PARAM_ID                 (0x820000A2u)

#define CapSense_BUTTON1_RX0_BSLN_INV0_VALUE                (CapSense_dsRam.snsList.button1[0u].bslnInv[0u])
#define CapSense_BUTTON1_RX0_BSLN_INV0_OFFSET               (164u)
#define CapSense_BUTTON1_RX0_BSLN_INV0_SIZE                 (2u)
#define CapSense_BUTTON1_RX0_BSLN_INV0_PARAM_ID             (0x8F0000A4u)

#define CapSense_BUTTON1_RX0_BSLN_EXT0_VALUE                (CapSense_dsRam.snsList.button1[0u].bslnExt[0u])
#define CapSense_BUTTON1_RX0_BSLN_EXT0_OFFSET               (166u)
#define CapSense_BUTTON1_RX0_BSLN_EXT0_SIZE                 (1u)
#define CapSense_BUTTON1_RX0_BSLN_EXT0_PARAM_ID             (0x4B0000A6u)

#define CapSense_BUTTON1_RX0_DIFF_VALUE                     (CapSense_dsRam.snsList.button1[0u].diff)
#define CapSense_BUTTON1_RX0_DIFF_OFFSET                    (168u)
#define CapSense_BUTTON1_RX0_DIFF_SIZE                      (2u)
#define CapSense_BUTTON1_RX0_DIFF_PARAM_ID                  (0x8C0000A8u)

#define CapSense_BUTTON1_RX0_NEG_BSLN_RST_CNT0_VALUE        (CapSense_dsRam.snsList.button1[0u].negBslnRstCnt[0u])
#define CapSense_BUTTON1_RX0_NEG_BSLN_RST_CNT0_OFFSET       (170u)
#define CapSense_BUTTON1_RX0_NEG_BSLN_RST_CNT0_SIZE         (1u)
#define CapSense_BUTTON1_RX0_NEG_BSLN_RST_CNT0_PARAM_ID     (0x480000AAu)

#define CapSense_BUTTON1_RX0_IDAC_COMP0_VALUE               (CapSense_dsRam.snsList.button1[0u].idacComp[0u])
#define CapSense_BUTTON1_RX0_IDAC_COMP0_OFFSET              (171u)
#define CapSense_BUTTON1_RX0_IDAC_COMP0_SIZE                (1u)
#define CapSense_BUTTON1_RX0_IDAC_COMP0_PARAM_ID            (0x4E0000ABu)

#define CapSense_BUTTON1_RX1_RAW0_VALUE                     (CapSense_dsRam.snsList.button1[1u].raw[0u])
#define CapSense_BUTTON1_RX1_RAW0_OFFSET                    (172u)
#define CapSense_BUTTON1_RX1_RAW0_SIZE                      (2u)
#define CapSense_BUTTON1_RX1_RAW0_PARAM_ID                  (0x8D0000ACu)

#define CapSense_BUTTON1_RX1_BSLN0_VALUE                    (CapSense_dsRam.snsList.button1[1u].bsln[0u])
#define CapSense_BUTTON1_RX1_BSLN0_OFFSET                   (174u)
#define CapSense_BUTTON1_RX1_BSLN0_SIZE                     (2u)
#define CapSense_BUTTON1_RX1_BSLN0_PARAM_ID                 (0x810000AEu)

#define CapSense_BUTTON1_RX1_BSLN_INV0_VALUE                (CapSense_dsRam.snsList.button1[1u].bslnInv[0u])
#define CapSense_BUTTON1_RX1_BSLN_INV0_OFFSET               (176u)
#define CapSense_BUTTON1_RX1_BSLN_INV0_SIZE                 (2u)
#define CapSense_BUTTON1_RX1_BSLN_INV0_PARAM_ID             (0x8B0000B0u)

#define CapSense_BUTTON1_RX1_BSLN_EXT0_VALUE                (CapSense_dsRam.snsList.button1[1u].bslnExt[0u])
#define CapSense_BUTTON1_RX1_BSLN_EXT0_OFFSET               (178u)
#define CapSense_BUTTON1_RX1_BSLN_EXT0_SIZE                 (1u)
#define CapSense_BUTTON1_RX1_BSLN_EXT0_PARAM_ID             (0x4F0000B2u)

#define CapSense_BUTTON1_RX1_DIFF_VALUE                     (CapSense_dsRam.snsList.button1[1u].diff)
#define CapSense_BUTTON1_RX1_DIFF_OFFSET                    (180u)
#define CapSense_BUTTON1_RX1_DIFF_SIZE                      (2u)
#define CapSense_BUTTON1_RX1_DIFF_PARAM_ID                  (0x8A0000B4u)

#define CapSense_BUTTON1_RX1_NEG_BSLN_RST_CNT0_VALUE        (CapSense_dsRam.snsList.button1[1u].negBslnRstCnt[0u])
#define CapSense_BUTTON1_RX1_NEG_BSLN_RST_CNT0_OFFSET       (182u)
#define CapSense_BUTTON1_RX1_NEG_BSLN_RST_CNT0_SIZE         (1u)
#define CapSense_BUTTON1_RX1_NEG_BSLN_RST_CNT0_PARAM_ID     (0x4E0000B6u)

#define CapSense_BUTTON1_RX1_IDAC_COMP0_VALUE               (CapSense_dsRam.snsList.button1[1u].idacComp[0u])
#define CapSense_BUTTON1_RX1_IDAC_COMP0_OFFSET              (183u)
#define CapSense_BUTTON1_RX1_IDAC_COMP0_SIZE                (1u)
#define CapSense_BUTTON1_RX1_IDAC_COMP0_PARAM_ID            (0x480000B7u)

#define CapSense_BUTTON2_RX0_RAW0_VALUE                     (CapSense_dsRam.snsList.button2[0u].raw[0u])
#define CapSense_BUTTON2_RX0_RAW0_OFFSET                    (184u)
#define CapSense_BUTTON2_RX0_RAW0_SIZE                      (2u)
#define CapSense_BUTTON2_RX0_RAW0_PARAM_ID                  (0x890000B8u)

#define CapSense_BUTTON2_RX0_BSLN0_VALUE                    (CapSense_dsRam.snsList.button2[0u].bsln[0u])
#define CapSense_BUTTON2_RX0_BSLN0_OFFSET                   (186u)
#define CapSense_BUTTON2_RX0_BSLN0_SIZE                     (2u)
#define CapSense_BUTTON2_RX0_BSLN0_PARAM_ID                 (0x850000BAu)

#define CapSense_BUTTON2_RX0_BSLN_INV0_VALUE                (CapSense_dsRam.snsList.button2[0u].bslnInv[0u])
#define CapSense_BUTTON2_RX0_BSLN_INV0_OFFSET               (188u)
#define CapSense_BUTTON2_RX0_BSLN_INV0_SIZE                 (2u)
#define CapSense_BUTTON2_RX0_BSLN_INV0_PARAM_ID             (0x880000BCu)

#define CapSense_BUTTON2_RX0_BSLN_EXT0_VALUE                (CapSense_dsRam.snsList.button2[0u].bslnExt[0u])
#define CapSense_BUTTON2_RX0_BSLN_EXT0_OFFSET               (190u)
#define CapSense_BUTTON2_RX0_BSLN_EXT0_SIZE                 (1u)
#define CapSense_BUTTON2_RX0_BSLN_EXT0_PARAM_ID             (0x4C0000BEu)

#define CapSense_BUTTON2_RX0_DIFF_VALUE                     (CapSense_dsRam.snsList.button2[0u].diff)
#define CapSense_BUTTON2_RX0_DIFF_OFFSET                    (192u)
#define CapSense_BUTTON2_RX0_DIFF_SIZE                      (2u)
#define CapSense_BUTTON2_RX0_DIFF_PARAM_ID                  (0x890000C0u)

#define CapSense_BUTTON2_RX0_NEG_BSLN_RST_CNT0_VALUE        (CapSense_dsRam.snsList.button2[0u].negBslnRstCnt[0u])
#define CapSense_BUTTON2_RX0_NEG_BSLN_RST_CNT0_OFFSET       (194u)
#define CapSense_BUTTON2_RX0_NEG_BSLN_RST_CNT0_SIZE         (1u)
#define CapSense_BUTTON2_RX0_NEG_BSLN_RST_CNT0_PARAM_ID     (0x4D0000C2u)

#define CapSense_BUTTON2_RX0_IDAC_COMP0_VALUE               (CapSense_dsRam.snsList.button2[0u].idacComp[0u])
#define CapSense_BUTTON2_RX0_IDAC_COMP0_OFFSET              (195u)
#define CapSense_BUTTON2_RX0_IDAC_COMP0_SIZE                (1u)
#define CapSense_BUTTON2_RX0_IDAC_COMP0_PARAM_ID            (0x4B0000C3u)

#define CapSense_BUTTON2_RX1_RAW0_VALUE                     (CapSense_dsRam.snsList.button2[1u].raw[0u])
#define CapSense_BUTTON2_RX1_RAW0_OFFSET                    (196u)
#define CapSense_BUTTON2_RX1_RAW0_SIZE                      (2u)
#define CapSense_BUTTON2_RX1_RAW0_PARAM_ID                  (0x880000C4u)

#define CapSense_BUTTON2_RX1_BSLN0_VALUE                    (CapSense_dsRam.snsList.button2[1u].bsln[0u])
#define CapSense_BUTTON2_RX1_BSLN0_OFFSET                   (198u)
#define CapSense_BUTTON2_RX1_BSLN0_SIZE                     (2u)
#define CapSense_BUTTON2_RX1_BSLN0_PARAM_ID                 (0x840000C6u)

#define CapSense_BUTTON2_RX1_BSLN_INV0_VALUE                (CapSense_dsRam.snsList.button2[1u].bslnInv[0u])
#define CapSense_BUTTON2_RX1_BSLN_INV0_OFFSET               (200u)
#define CapSense_BUTTON2_RX1_BSLN_INV0_SIZE                 (2u)
#define CapSense_BUTTON2_RX1_BSLN_INV0_PARAM_ID             (0x8B0000C8u)

#define CapSense_BUTTON2_RX1_BSLN_EXT0_VALUE                (CapSense_dsRam.snsList.button2[1u].bslnExt[0u])
#define CapSense_BUTTON2_RX1_BSLN_EXT0_OFFSET               (202u)
#define CapSense_BUTTON2_RX1_BSLN_EXT0_SIZE                 (1u)
#define CapSense_BUTTON2_RX1_BSLN_EXT0_PARAM_ID             (0x4F0000CAu)

#define CapSense_BUTTON2_RX1_DIFF_VALUE                     (CapSense_dsRam.snsList.button2[1u].diff)
#define CapSense_BUTTON2_RX1_DIFF_OFFSET                    (204u)
#define CapSense_BUTTON2_RX1_DIFF_SIZE                      (2u)
#define CapSense_BUTTON2_RX1_DIFF_PARAM_ID                  (0x8A0000CCu)

#define CapSense_BUTTON2_RX1_NEG_BSLN_RST_CNT0_VALUE        (CapSense_dsRam.snsList.button2[1u].negBslnRstCnt[0u])
#define CapSense_BUTTON2_RX1_NEG_BSLN_RST_CNT0_OFFSET       (206u)
#define CapSense_BUTTON2_RX1_NEG_BSLN_RST_CNT0_SIZE         (1u)
#define CapSense_BUTTON2_RX1_NEG_BSLN_RST_CNT0_PARAM_ID     (0x4E0000CEu)

#define CapSense_BUTTON2_RX1_IDAC_COMP0_VALUE               (CapSense_dsRam.snsList.button2[1u].idacComp[0u])
#define CapSense_BUTTON2_RX1_IDAC_COMP0_OFFSET              (207u)
#define CapSense_BUTTON2_RX1_IDAC_COMP0_SIZE                (1u)
#define CapSense_BUTTON2_RX1_IDAC_COMP0_PARAM_ID            (0x480000CFu)

#define CapSense_BUTTON3_RX0_RAW0_VALUE                     (CapSense_dsRam.snsList.button3[0u].raw[0u])
#define CapSense_BUTTON3_RX0_RAW0_OFFSET                    (208u)
#define CapSense_BUTTON3_RX0_RAW0_SIZE                      (2u)
#define CapSense_BUTTON3_RX0_RAW0_PARAM_ID                  (0x8C0000D0u)

#define CapSense_BUTTON3_RX0_BSLN0_VALUE                    (CapSense_dsRam.snsList.button3[0u].bsln[0u])
#define CapSense_BUTTON3_RX0_BSLN0_OFFSET                   (210u)
#define CapSense_BUTTON3_RX0_BSLN0_SIZE                     (2u)
#define CapSense_BUTTON3_RX0_BSLN0_PARAM_ID                 (0x800000D2u)

#define CapSense_BUTTON3_RX0_BSLN_INV0_VALUE                (CapSense_dsRam.snsList.button3[0u].bslnInv[0u])
#define CapSense_BUTTON3_RX0_BSLN_INV0_OFFSET               (212u)
#define CapSense_BUTTON3_RX0_BSLN_INV0_SIZE                 (2u)
#define CapSense_BUTTON3_RX0_BSLN_INV0_PARAM_ID             (0x8D0000D4u)

#define CapSense_BUTTON3_RX0_BSLN_EXT0_VALUE                (CapSense_dsRam.snsList.button3[0u].bslnExt[0u])
#define CapSense_BUTTON3_RX0_BSLN_EXT0_OFFSET               (214u)
#define CapSense_BUTTON3_RX0_BSLN_EXT0_SIZE                 (1u)
#define CapSense_BUTTON3_RX0_BSLN_EXT0_PARAM_ID             (0x490000D6u)

#define CapSense_BUTTON3_RX0_DIFF_VALUE                     (CapSense_dsRam.snsList.button3[0u].diff)
#define CapSense_BUTTON3_RX0_DIFF_OFFSET                    (216u)
#define CapSense_BUTTON3_RX0_DIFF_SIZE                      (2u)
#define CapSense_BUTTON3_RX0_DIFF_PARAM_ID                  (0x8E0000D8u)

#define CapSense_BUTTON3_RX0_NEG_BSLN_RST_CNT0_VALUE        (CapSense_dsRam.snsList.button3[0u].negBslnRstCnt[0u])
#define CapSense_BUTTON3_RX0_NEG_BSLN_RST_CNT0_OFFSET       (218u)
#define CapSense_BUTTON3_RX0_NEG_BSLN_RST_CNT0_SIZE         (1u)
#define CapSense_BUTTON3_RX0_NEG_BSLN_RST_CNT0_PARAM_ID     (0x4A0000DAu)

#define CapSense_BUTTON3_RX0_IDAC_COMP0_VALUE               (CapSense_dsRam.snsList.button3[0u].idacComp[0u])
#define CapSense_BUTTON3_RX0_IDAC_COMP0_OFFSET              (219u)
#define CapSense_BUTTON3_RX0_IDAC_COMP0_SIZE                (1u)
#define CapSense_BUTTON3_RX0_IDAC_COMP0_PARAM_ID            (0x4C0000DBu)

#define CapSense_BUTTON3_RX1_RAW0_VALUE                     (CapSense_dsRam.snsList.button3[1u].raw[0u])
#define CapSense_BUTTON3_RX1_RAW0_OFFSET                    (220u)
#define CapSense_BUTTON3_RX1_RAW0_SIZE                      (2u)
#define CapSense_BUTTON3_RX1_RAW0_PARAM_ID                  (0x8F0000DCu)

#define CapSense_BUTTON3_RX1_BSLN0_VALUE                    (CapSense_dsRam.snsList.button3[1u].bsln[0u])
#define CapSense_BUTTON3_RX1_BSLN0_OFFSET                   (222u)
#define CapSense_BUTTON3_RX1_BSLN0_SIZE                     (2u)
#define CapSense_BUTTON3_RX1_BSLN0_PARAM_ID                 (0x830000DEu)

#define CapSense_BUTTON3_RX1_BSLN_INV0_VALUE                (CapSense_dsRam.snsList.button3[1u].bslnInv[0u])
#define CapSense_BUTTON3_RX1_BSLN_INV0_OFFSET               (224u)
#define CapSense_BUTTON3_RX1_BSLN_INV0_SIZE                 (2u)
#define CapSense_BUTTON3_RX1_BSLN_INV0_PARAM_ID             (0x830000E0u)

#define CapSense_BUTTON3_RX1_BSLN_EXT0_VALUE                (CapSense_dsRam.snsList.button3[1u].bslnExt[0u])
#define CapSense_BUTTON3_RX1_BSLN_EXT0_OFFSET               (226u)
#define CapSense_BUTTON3_RX1_BSLN_EXT0_SIZE                 (1u)
#define CapSense_BUTTON3_RX1_BSLN_EXT0_PARAM_ID             (0x470000E2u)

#define CapSense_BUTTON3_RX1_DIFF_VALUE                     (CapSense_dsRam.snsList.button3[1u].diff)
#define CapSense_BUTTON3_RX1_DIFF_OFFSET                    (228u)
#define CapSense_BUTTON3_RX1_DIFF_SIZE                      (2u)
#define CapSense_BUTTON3_RX1_DIFF_PARAM_ID                  (0x820000E4u)

#define CapSense_BUTTON3_RX1_NEG_BSLN_RST_CNT0_VALUE        (CapSense_dsRam.snsList.button3[1u].negBslnRstCnt[0u])
#define CapSense_BUTTON3_RX1_NEG_BSLN_RST_CNT0_OFFSET       (230u)
#define CapSense_BUTTON3_RX1_NEG_BSLN_RST_CNT0_SIZE         (1u)
#define CapSense_BUTTON3_RX1_NEG_BSLN_RST_CNT0_PARAM_ID     (0x460000E6u)

#define CapSense_BUTTON3_RX1_IDAC_COMP0_VALUE               (CapSense_dsRam.snsList.button3[1u].idacComp[0u])
#define CapSense_BUTTON3_RX1_IDAC_COMP0_OFFSET              (231u)
#define CapSense_BUTTON3_RX1_IDAC_COMP0_SIZE                (1u)
#define CapSense_BUTTON3_RX1_IDAC_COMP0_PARAM_ID            (0x400000E7u)

#define CapSense_BUTTON4_RX0_RAW0_VALUE                     (CapSense_dsRam.snsList.button4[0u].raw[0u])
#define CapSense_BUTTON4_RX0_RAW0_OFFSET                    (232u)
#define CapSense_BUTTON4_RX0_RAW0_SIZE                      (2u)
#define CapSense_BUTTON4_RX0_RAW0_PARAM_ID                  (0x810000E8u)

#define CapSense_BUTTON4_RX0_BSLN0_VALUE                    (CapSense_dsRam.snsList.button4[0u].bsln[0u])
#define CapSense_BUTTON4_RX0_BSLN0_OFFSET                   (234u)
#define CapSense_BUTTON4_RX0_BSLN0_SIZE                     (2u)
#define CapSense_BUTTON4_RX0_BSLN0_PARAM_ID                 (0x8D0000EAu)

#define CapSense_BUTTON4_RX0_BSLN_INV0_VALUE                (CapSense_dsRam.snsList.button4[0u].bslnInv[0u])
#define CapSense_BUTTON4_RX0_BSLN_INV0_OFFSET               (236u)
#define CapSense_BUTTON4_RX0_BSLN_INV0_SIZE                 (2u)
#define CapSense_BUTTON4_RX0_BSLN_INV0_PARAM_ID             (0x800000ECu)

#define CapSense_BUTTON4_RX0_BSLN_EXT0_VALUE                (CapSense_dsRam.snsList.button4[0u].bslnExt[0u])
#define CapSense_BUTTON4_RX0_BSLN_EXT0_OFFSET               (238u)
#define CapSense_BUTTON4_RX0_BSLN_EXT0_SIZE                 (1u)
#define CapSense_BUTTON4_RX0_BSLN_EXT0_PARAM_ID             (0x440000EEu)

#define CapSense_BUTTON4_RX0_DIFF_VALUE                     (CapSense_dsRam.snsList.button4[0u].diff)
#define CapSense_BUTTON4_RX0_DIFF_OFFSET                    (240u)
#define CapSense_BUTTON4_RX0_DIFF_SIZE                      (2u)
#define CapSense_BUTTON4_RX0_DIFF_PARAM_ID                  (0x860000F0u)

#define CapSense_BUTTON4_RX0_NEG_BSLN_RST_CNT0_VALUE        (CapSense_dsRam.snsList.button4[0u].negBslnRstCnt[0u])
#define CapSense_BUTTON4_RX0_NEG_BSLN_RST_CNT0_OFFSET       (242u)
#define CapSense_BUTTON4_RX0_NEG_BSLN_RST_CNT0_SIZE         (1u)
#define CapSense_BUTTON4_RX0_NEG_BSLN_RST_CNT0_PARAM_ID     (0x420000F2u)

#define CapSense_BUTTON4_RX0_IDAC_COMP0_VALUE               (CapSense_dsRam.snsList.button4[0u].idacComp[0u])
#define CapSense_BUTTON4_RX0_IDAC_COMP0_OFFSET              (243u)
#define CapSense_BUTTON4_RX0_IDAC_COMP0_SIZE                (1u)
#define CapSense_BUTTON4_RX0_IDAC_COMP0_PARAM_ID            (0x440000F3u)

#define CapSense_BUTTON4_RX1_RAW0_VALUE                     (CapSense_dsRam.snsList.button4[1u].raw[0u])
#define CapSense_BUTTON4_RX1_RAW0_OFFSET                    (244u)
#define CapSense_BUTTON4_RX1_RAW0_SIZE                      (2u)
#define CapSense_BUTTON4_RX1_RAW0_PARAM_ID                  (0x870000F4u)

#define CapSense_BUTTON4_RX1_BSLN0_VALUE                    (CapSense_dsRam.snsList.button4[1u].bsln[0u])
#define CapSense_BUTTON4_RX1_BSLN0_OFFSET                   (246u)
#define CapSense_BUTTON4_RX1_BSLN0_SIZE                     (2u)
#define CapSense_BUTTON4_RX1_BSLN0_PARAM_ID                 (0x8B0000F6u)

#define CapSense_BUTTON4_RX1_BSLN_INV0_VALUE                (CapSense_dsRam.snsList.button4[1u].bslnInv[0u])
#define CapSense_BUTTON4_RX1_BSLN_INV0_OFFSET               (248u)
#define CapSense_BUTTON4_RX1_BSLN_INV0_SIZE                 (2u)
#define CapSense_BUTTON4_RX1_BSLN_INV0_PARAM_ID             (0x840000F8u)

#define CapSense_BUTTON4_RX1_BSLN_EXT0_VALUE                (CapSense_dsRam.snsList.button4[1u].bslnExt[0u])
#define CapSense_BUTTON4_RX1_BSLN_EXT0_OFFSET               (250u)
#define CapSense_BUTTON4_RX1_BSLN_EXT0_SIZE                 (1u)
#define CapSense_BUTTON4_RX1_BSLN_EXT0_PARAM_ID             (0x400000FAu)

#define CapSense_BUTTON4_RX1_DIFF_VALUE                     (CapSense_dsRam.snsList.button4[1u].diff)
#define CapSense_BUTTON4_RX1_DIFF_OFFSET                    (252u)
#define CapSense_BUTTON4_RX1_DIFF_SIZE                      (2u)
#define CapSense_BUTTON4_RX1_DIFF_PARAM_ID                  (0x850000FCu)

#define CapSense_BUTTON4_RX1_NEG_BSLN_RST_CNT0_VALUE        (CapSense_dsRam.snsList.button4[1u].negBslnRstCnt[0u])
#define CapSense_BUTTON4_RX1_NEG_BSLN_RST_CNT0_OFFSET       (254u)
#define CapSense_BUTTON4_RX1_NEG_BSLN_RST_CNT0_SIZE         (1u)
#define CapSense_BUTTON4_RX1_NEG_BSLN_RST_CNT0_PARAM_ID     (0x410000FEu)

#define CapSense_BUTTON4_RX1_IDAC_COMP0_VALUE               (CapSense_dsRam.snsList.button4[1u].idacComp[0u])
#define CapSense_BUTTON4_RX1_IDAC_COMP0_OFFSET              (255u)
#define CapSense_BUTTON4_RX1_IDAC_COMP0_SIZE                (1u)
#define CapSense_BUTTON4_RX1_IDAC_COMP0_PARAM_ID            (0x470000FFu)

#define CapSense_BUTTON5_RX0_RAW0_VALUE                     (CapSense_dsRam.snsList.button5[0u].raw[0u])
#define CapSense_BUTTON5_RX0_RAW0_OFFSET                    (256u)
#define CapSense_BUTTON5_RX0_RAW0_SIZE                      (2u)
#define CapSense_BUTTON5_RX0_RAW0_PARAM_ID                  (0x8C000100u)

#define CapSense_BUTTON5_RX0_BSLN0_VALUE                    (CapSense_dsRam.snsList.button5[0u].bsln[0u])
#define CapSense_BUTTON5_RX0_BSLN0_OFFSET                   (258u)
#define CapSense_BUTTON5_RX0_BSLN0_SIZE                     (2u)
#define CapSense_BUTTON5_RX0_BSLN0_PARAM_ID                 (0x80000102u)

#define CapSense_BUTTON5_RX0_BSLN_INV0_VALUE                (CapSense_dsRam.snsList.button5[0u].bslnInv[0u])
#define CapSense_BUTTON5_RX0_BSLN_INV0_OFFSET               (260u)
#define CapSense_BUTTON5_RX0_BSLN_INV0_SIZE                 (2u)
#define CapSense_BUTTON5_RX0_BSLN_INV0_PARAM_ID             (0x8D000104u)

#define CapSense_BUTTON5_RX0_BSLN_EXT0_VALUE                (CapSense_dsRam.snsList.button5[0u].bslnExt[0u])
#define CapSense_BUTTON5_RX0_BSLN_EXT0_OFFSET               (262u)
#define CapSense_BUTTON5_RX0_BSLN_EXT0_SIZE                 (1u)
#define CapSense_BUTTON5_RX0_BSLN_EXT0_PARAM_ID             (0x49000106u)

#define CapSense_BUTTON5_RX0_DIFF_VALUE                     (CapSense_dsRam.snsList.button5[0u].diff)
#define CapSense_BUTTON5_RX0_DIFF_OFFSET                    (264u)
#define CapSense_BUTTON5_RX0_DIFF_SIZE                      (2u)
#define CapSense_BUTTON5_RX0_DIFF_PARAM_ID                  (0x8E000108u)

#define CapSense_BUTTON5_RX0_NEG_BSLN_RST_CNT0_VALUE        (CapSense_dsRam.snsList.button5[0u].negBslnRstCnt[0u])
#define CapSense_BUTTON5_RX0_NEG_BSLN_RST_CNT0_OFFSET       (266u)
#define CapSense_BUTTON5_RX0_NEG_BSLN_RST_CNT0_SIZE         (1u)
#define CapSense_BUTTON5_RX0_NEG_BSLN_RST_CNT0_PARAM_ID     (0x4A00010Au)

#define CapSense_BUTTON5_RX0_IDAC_COMP0_VALUE               (CapSense_dsRam.snsList.button5[0u].idacComp[0u])
#define CapSense_BUTTON5_RX0_IDAC_COMP0_OFFSET              (267u)
#define CapSense_BUTTON5_RX0_IDAC_COMP0_SIZE                (1u)
#define CapSense_BUTTON5_RX0_IDAC_COMP0_PARAM_ID            (0x4C00010Bu)

#define CapSense_BUTTON5_RX1_RAW0_VALUE                     (CapSense_dsRam.snsList.button5[1u].raw[0u])
#define CapSense_BUTTON5_RX1_RAW0_OFFSET                    (268u)
#define CapSense_BUTTON5_RX1_RAW0_SIZE                      (2u)
#define CapSense_BUTTON5_RX1_RAW0_PARAM_ID                  (0x8F00010Cu)

#define CapSense_BUTTON5_RX1_BSLN0_VALUE                    (CapSense_dsRam.snsList.button5[1u].bsln[0u])
#define CapSense_BUTTON5_RX1_BSLN0_OFFSET                   (270u)
#define CapSense_BUTTON5_RX1_BSLN0_SIZE                     (2u)
#define CapSense_BUTTON5_RX1_BSLN0_PARAM_ID                 (0x8300010Eu)

#define CapSense_BUTTON5_RX1_BSLN_INV0_VALUE                (CapSense_dsRam.snsList.button5[1u].bslnInv[0u])
#define CapSense_BUTTON5_RX1_BSLN_INV0_OFFSET               (272u)
#define CapSense_BUTTON5_RX1_BSLN_INV0_SIZE                 (2u)
#define CapSense_BUTTON5_RX1_BSLN_INV0_PARAM_ID             (0x89000110u)

#define CapSense_BUTTON5_RX1_BSLN_EXT0_VALUE                (CapSense_dsRam.snsList.button5[1u].bslnExt[0u])
#define CapSense_BUTTON5_RX1_BSLN_EXT0_OFFSET               (274u)
#define CapSense_BUTTON5_RX1_BSLN_EXT0_SIZE                 (1u)
#define CapSense_BUTTON5_RX1_BSLN_EXT0_PARAM_ID             (0x4D000112u)

#define CapSense_BUTTON5_RX1_DIFF_VALUE                     (CapSense_dsRam.snsList.button5[1u].diff)
#define CapSense_BUTTON5_RX1_DIFF_OFFSET                    (276u)
#define CapSense_BUTTON5_RX1_DIFF_SIZE                      (2u)
#define CapSense_BUTTON5_RX1_DIFF_PARAM_ID                  (0x88000114u)

#define CapSense_BUTTON5_RX1_NEG_BSLN_RST_CNT0_VALUE        (CapSense_dsRam.snsList.button5[1u].negBslnRstCnt[0u])
#define CapSense_BUTTON5_RX1_NEG_BSLN_RST_CNT0_OFFSET       (278u)
#define CapSense_BUTTON5_RX1_NEG_BSLN_RST_CNT0_SIZE         (1u)
#define CapSense_BUTTON5_RX1_NEG_BSLN_RST_CNT0_PARAM_ID     (0x4C000116u)

#define CapSense_BUTTON5_RX1_IDAC_COMP0_VALUE               (CapSense_dsRam.snsList.button5[1u].idacComp[0u])
#define CapSense_BUTTON5_RX1_IDAC_COMP0_OFFSET              (279u)
#define CapSense_BUTTON5_RX1_IDAC_COMP0_SIZE                (1u)
#define CapSense_BUTTON5_RX1_IDAC_COMP0_PARAM_ID            (0x4A000117u)

#define CapSense_TEST_RESULT_MASK_VALUE                     (CapSense_dsRam.selfTest.testResultMask)
#define CapSense_TEST_RESULT_MASK_OFFSET                    (280u)
#define CapSense_TEST_RESULT_MASK_SIZE                      (4u)
#define CapSense_TEST_RESULT_MASK_PARAM_ID                  (0xEF000118u)

#define CapSense_EXT_CAP0_VALUE                             (CapSense_dsRam.selfTest.extCap[0u])
#define CapSense_EXT_CAP0_OFFSET                            (284u)
#define CapSense_EXT_CAP0_SIZE                              (2u)
#define CapSense_EXT_CAP0_PARAM_ID                          (0xA100011Cu)

#define CapSense_EXT_CAP1_VALUE                             (CapSense_dsRam.selfTest.extCap[1u])
#define CapSense_EXT_CAP1_OFFSET                            (286u)
#define CapSense_EXT_CAP1_SIZE                              (2u)
#define CapSense_EXT_CAP1_PARAM_ID                          (0xAD00011Eu)

#define CapSense_VDDA_VOLTAGE_VALUE                         (CapSense_dsRam.selfTest.vddaVoltage)
#define CapSense_VDDA_VOLTAGE_OFFSET                        (288u)
#define CapSense_VDDA_VOLTAGE_SIZE                          (2u)
#define CapSense_VDDA_VOLTAGE_PARAM_ID                      (0xAD000120u)

#define CapSense_GLB_CRC_CALC_VALUE                         (CapSense_dsRam.selfTest.glbCrcCalc)
#define CapSense_GLB_CRC_CALC_OFFSET                        (290u)
#define CapSense_GLB_CRC_CALC_SIZE                          (2u)
#define CapSense_GLB_CRC_CALC_PARAM_ID                      (0xA1000122u)

#define CapSense_WDGT_CRC_CALC_VALUE                        (CapSense_dsRam.selfTest.wdgtCrcCalc)
#define CapSense_WDGT_CRC_CALC_OFFSET                       (292u)
#define CapSense_WDGT_CRC_CALC_SIZE                         (2u)
#define CapSense_WDGT_CRC_CALC_PARAM_ID                     (0xAC000124u)

#define CapSense_WDGT_CRC_ID_VALUE                          (CapSense_dsRam.selfTest.wdgtCrcId)
#define CapSense_WDGT_CRC_ID_OFFSET                         (294u)
#define CapSense_WDGT_CRC_ID_SIZE                           (1u)
#define CapSense_WDGT_CRC_ID_PARAM_ID                       (0x68000126u)

#define CapSense_INV_BSLN_WDGT_ID_VALUE                     (CapSense_dsRam.selfTest.invBslnWdgtId)
#define CapSense_INV_BSLN_WDGT_ID_OFFSET                    (295u)
#define CapSense_INV_BSLN_WDGT_ID_SIZE                      (1u)
#define CapSense_INV_BSLN_WDGT_ID_PARAM_ID                  (0x6E000127u)

#define CapSense_INV_BSLN_SNS_ID_VALUE                      (CapSense_dsRam.selfTest.invBslnSnsId)
#define CapSense_INV_BSLN_SNS_ID_OFFSET                     (296u)
#define CapSense_INV_BSLN_SNS_ID_SIZE                       (1u)
#define CapSense_INV_BSLN_SNS_ID_PARAM_ID                   (0x67000128u)

#define CapSense_SHORTED_WDGT_ID_VALUE                      (CapSense_dsRam.selfTest.shortedWdgtId)
#define CapSense_SHORTED_WDGT_ID_OFFSET                     (297u)
#define CapSense_SHORTED_WDGT_ID_SIZE                       (1u)
#define CapSense_SHORTED_WDGT_ID_PARAM_ID                   (0x61000129u)

#define CapSense_SHORTED_SNS_ID_VALUE                       (CapSense_dsRam.selfTest.shortedSnsId)
#define CapSense_SHORTED_SNS_ID_OFFSET                      (298u)
#define CapSense_SHORTED_SNS_ID_SIZE                        (1u)
#define CapSense_SHORTED_SNS_ID_PARAM_ID                    (0x6B00012Au)

#define CapSense_IDAC_GAIN_INDEX_VALUE                      (CapSense_dsRam.selfTest.idacGainIndex)
#define CapSense_IDAC_GAIN_INDEX_OFFSET                     (299u)
#define CapSense_IDAC_GAIN_INDEX_SIZE                       (1u)
#define CapSense_IDAC_GAIN_INDEX_PARAM_ID                   (0x6D00012Bu)

#define CapSense_SNS_CLK_VALUE                              (CapSense_dsRam.selfTest.snsClk)
#define CapSense_SNS_CLK_OFFSET                             (300u)
#define CapSense_SNS_CLK_SIZE                               (2u)
#define CapSense_SNS_CLK_PARAM_ID                           (0xAE00012Cu)

#define CapSense_SNS_CAP_SNS_CLK_HZ_VALUE                   (CapSense_dsRam.selfTestConfig.snsCapSnsClkHz)
#define CapSense_SNS_CAP_SNS_CLK_HZ_OFFSET                  (304u)
#define CapSense_SNS_CAP_SNS_CLK_HZ_SIZE                    (4u)
#define CapSense_SNS_CAP_SNS_CLK_HZ_PARAM_ID                (0xE7000130u)

#define CapSense_SNS_CAP_SNS_CLK_DIVIDER_VALUE              (CapSense_dsRam.selfTestConfig.snsCapSnsClkDivider)
#define CapSense_SNS_CAP_SNS_CLK_DIVIDER_OFFSET             (308u)
#define CapSense_SNS_CAP_SNS_CLK_DIVIDER_SIZE               (2u)
#define CapSense_SNS_CAP_SNS_CLK_DIVIDER_PARAM_ID           (0xA9000134u)

#define CapSense_SNS_CAP_MOD_CLK_KHZ_VALUE                  (CapSense_dsRam.selfTestConfig.snsCapModClkKHz)
#define CapSense_SNS_CAP_MOD_CLK_KHZ_OFFSET                 (310u)
#define CapSense_SNS_CAP_MOD_CLK_KHZ_SIZE                   (2u)
#define CapSense_SNS_CAP_MOD_CLK_KHZ_PARAM_ID               (0xA5000136u)

#define CapSense_SNS_CAP_MOD_CLK_DIVIDER_VALUE              (CapSense_dsRam.selfTestConfig.snsCapModClkDivider)
#define CapSense_SNS_CAP_MOD_CLK_DIVIDER_OFFSET             (312u)
#define CapSense_SNS_CAP_MOD_CLK_DIVIDER_SIZE               (2u)
#define CapSense_SNS_CAP_MOD_CLK_DIVIDER_PARAM_ID           (0xAA000138u)

#define CapSense_SNS_CAP_VREF_VOLTAGE_VALUE                 (CapSense_dsRam.selfTestConfig.snsCapVrefVoltage)
#define CapSense_SNS_CAP_VREF_VOLTAGE_OFFSET                (314u)
#define CapSense_SNS_CAP_VREF_VOLTAGE_SIZE                  (2u)
#define CapSense_SNS_CAP_VREF_VOLTAGE_PARAM_ID              (0xA600013Au)

#define CapSense_VDDA_VREF_VOLTAGE_VALUE                    (CapSense_dsRam.selfTestConfig.vddaVrefVoltage)
#define CapSense_VDDA_VREF_VOLTAGE_OFFSET                   (316u)
#define CapSense_VDDA_VREF_VOLTAGE_SIZE                     (2u)
#define CapSense_VDDA_VREF_VOLTAGE_PARAM_ID                 (0xAB00013Cu)

#define CapSense_EXT_CAP_VREF_VOLTAGE_VALUE                 (CapSense_dsRam.selfTestConfig.extCapVrefVoltage)
#define CapSense_EXT_CAP_VREF_VOLTAGE_OFFSET                (318u)
#define CapSense_EXT_CAP_VREF_VOLTAGE_SIZE                  (2u)
#define CapSense_EXT_CAP_VREF_VOLTAGE_PARAM_ID              (0xA700013Eu)

#define CapSense_VDDA_MOD_CLK_DIVIDER_VALUE                 (CapSense_dsRam.selfTestConfig.vddaModClkDivider)
#define CapSense_VDDA_MOD_CLK_DIVIDER_OFFSET                (320u)
#define CapSense_VDDA_MOD_CLK_DIVIDER_SIZE                  (1u)
#define CapSense_VDDA_MOD_CLK_DIVIDER_PARAM_ID              (0x62000140u)

#define CapSense_VDDA_VREF_GAIN_VALUE                       (CapSense_dsRam.selfTestConfig.vddaVrefGain)
#define CapSense_VDDA_VREF_GAIN_OFFSET                      (321u)
#define CapSense_VDDA_VREF_GAIN_SIZE                        (1u)
#define CapSense_VDDA_VREF_GAIN_PARAM_ID                    (0x64000141u)

#define CapSense_VDDA_RESOLUTION_VALUE                      (CapSense_dsRam.selfTestConfig.vddaResolution)
#define CapSense_VDDA_RESOLUTION_OFFSET                     (322u)
#define CapSense_VDDA_RESOLUTION_SIZE                       (1u)
#define CapSense_VDDA_RESOLUTION_PARAM_ID                   (0x6E000142u)

#define CapSense_VDDA_IDAC_DEFAULT_VALUE                    (CapSense_dsRam.selfTestConfig.vddaIdacDefault)
#define CapSense_VDDA_IDAC_DEFAULT_OFFSET                   (323u)
#define CapSense_VDDA_IDAC_DEFAULT_SIZE                     (1u)
#define CapSense_VDDA_IDAC_DEFAULT_PARAM_ID                 (0x68000143u)

#define CapSense_SNS_CAP_RESOLUTION_VALUE                   (CapSense_dsRam.selfTestConfig.snsCapResolution)
#define CapSense_SNS_CAP_RESOLUTION_OFFSET                  (324u)
#define CapSense_SNS_CAP_RESOLUTION_SIZE                    (1u)
#define CapSense_SNS_CAP_RESOLUTION_PARAM_ID                (0x63000144u)

#define CapSense_SNS_CAP_VREF_GAIN_VALUE                    (CapSense_dsRam.selfTestConfig.snsCapVrefGain)
#define CapSense_SNS_CAP_VREF_GAIN_OFFSET                   (325u)
#define CapSense_SNS_CAP_VREF_GAIN_SIZE                     (1u)
#define CapSense_SNS_CAP_VREF_GAIN_PARAM_ID                 (0x65000145u)

#define CapSense_BIST_CSD_SNS_CAP_ISC_VALUE                 (CapSense_dsRam.selfTestConfig.bistCsdSnsCapISC)
#define CapSense_BIST_CSD_SNS_CAP_ISC_OFFSET                (326u)
#define CapSense_BIST_CSD_SNS_CAP_ISC_SIZE                  (1u)
#define CapSense_BIST_CSD_SNS_CAP_ISC_PARAM_ID              (0x6F000146u)

#define CapSense_BIST_CSX_SNS_CAP_ISC_VALUE                 (CapSense_dsRam.selfTestConfig.bistCsxSnsCapISC)
#define CapSense_BIST_CSX_SNS_CAP_ISC_OFFSET                (327u)
#define CapSense_BIST_CSX_SNS_CAP_ISC_SIZE                  (1u)
#define CapSense_BIST_CSX_SNS_CAP_ISC_PARAM_ID              (0x69000147u)

#define CapSense_BIST_CSD_SH_CAP_ISC_VALUE                  (CapSense_dsRam.selfTestConfig.bistCsdShCapISC)
#define CapSense_BIST_CSD_SH_CAP_ISC_OFFSET                 (328u)
#define CapSense_BIST_CSD_SH_CAP_ISC_SIZE                   (1u)
#define CapSense_BIST_CSD_SH_CAP_ISC_PARAM_ID               (0x60000148u)

#define CapSense_EXT_CAP_VREF_GAIN_VALUE                    (CapSense_dsRam.selfTestConfig.extCapVrefGain)
#define CapSense_EXT_CAP_VREF_GAIN_OFFSET                   (329u)
#define CapSense_EXT_CAP_VREF_GAIN_SIZE                     (1u)
#define CapSense_EXT_CAP_VREF_GAIN_PARAM_ID                 (0x66000149u)

#define CapSense_STARTUP_DELAY_VALUE                        (CapSense_dsRam.selfTestConfig.startupDelay)
#define CapSense_STARTUP_DELAY_OFFSET                       (330u)
#define CapSense_STARTUP_DELAY_SIZE                         (1u)
#define CapSense_STARTUP_DELAY_PARAM_ID                     (0x6C00014Au)

#define CapSense_FINE_INIT_TIME_VALUE                       (CapSense_dsRam.selfTestConfig.fineInitTime)
#define CapSense_FINE_INIT_TIME_OFFSET                      (331u)
#define CapSense_FINE_INIT_TIME_SIZE                        (1u)
#define CapSense_FINE_INIT_TIME_PARAM_ID                    (0x6A00014Bu)

#define CapSense_SNS_SHORT_TIME_VALUE                       (CapSense_dsRam.selfTestConfig.snsShortTimeUs)
#define CapSense_SNS_SHORT_TIME_OFFSET                      (332u)
#define CapSense_SNS_SHORT_TIME_SIZE                        (1u)
#define CapSense_SNS_SHORT_TIME_PARAM_ID                    (0x6100014Cu)

#define CapSense_BUTTON0_SNS_CP0_VALUE                      (CapSense_dsRam.snsCp.button0[0u])
#define CapSense_BUTTON0_SNS_CP0_OFFSET                     (336u)
#define CapSense_BUTTON0_SNS_CP0_SIZE                       (4u)
#define CapSense_BUTTON0_SNS_CP0_PARAM_ID                   (0xE0000150u)

#define CapSense_BUTTON0_SNS_CP1_VALUE                      (CapSense_dsRam.snsCp.button0[1u])
#define CapSense_BUTTON0_SNS_CP1_OFFSET                     (340u)
#define CapSense_BUTTON0_SNS_CP1_SIZE                       (4u)
#define CapSense_BUTTON0_SNS_CP1_PARAM_ID                   (0xE1000154u)

#define CapSense_BUTTON0_SNS_CP2_VALUE                      (CapSense_dsRam.snsCp.button0[2u])
#define CapSense_BUTTON0_SNS_CP2_OFFSET                     (344u)
#define CapSense_BUTTON0_SNS_CP2_SIZE                       (4u)
#define CapSense_BUTTON0_SNS_CP2_PARAM_ID                   (0xE2000158u)

#define CapSense_BUTTON1_SNS_CP0_VALUE                      (CapSense_dsRam.snsCp.button1[0u])
#define CapSense_BUTTON1_SNS_CP0_OFFSET                     (348u)
#define CapSense_BUTTON1_SNS_CP0_SIZE                       (4u)
#define CapSense_BUTTON1_SNS_CP0_PARAM_ID                   (0xE300015Cu)

#define CapSense_BUTTON1_SNS_CP1_VALUE                      (CapSense_dsRam.snsCp.button1[1u])
#define CapSense_BUTTON1_SNS_CP1_OFFSET                     (352u)
#define CapSense_BUTTON1_SNS_CP1_SIZE                       (4u)
#define CapSense_BUTTON1_SNS_CP1_PARAM_ID                   (0xEF000160u)

#define CapSense_BUTTON1_SNS_CP2_VALUE                      (CapSense_dsRam.snsCp.button1[2u])
#define CapSense_BUTTON1_SNS_CP2_OFFSET                     (356u)
#define CapSense_BUTTON1_SNS_CP2_SIZE                       (4u)
#define CapSense_BUTTON1_SNS_CP2_PARAM_ID                   (0xEE000164u)

#define CapSense_BUTTON2_SNS_CP0_VALUE                      (CapSense_dsRam.snsCp.button2[0u])
#define CapSense_BUTTON2_SNS_CP0_OFFSET                     (360u)
#define CapSense_BUTTON2_SNS_CP0_SIZE                       (4u)
#define CapSense_BUTTON2_SNS_CP0_PARAM_ID                   (0xED000168u)

#define CapSense_BUTTON2_SNS_CP1_VALUE                      (CapSense_dsRam.snsCp.button2[1u])
#define CapSense_BUTTON2_SNS_CP1_OFFSET                     (364u)
#define CapSense_BUTTON2_SNS_CP1_SIZE                       (4u)
#define CapSense_BUTTON2_SNS_CP1_PARAM_ID                   (0xEC00016Cu)

#define CapSense_BUTTON2_SNS_CP2_VALUE                      (CapSense_dsRam.snsCp.button2[2u])
#define CapSense_BUTTON2_SNS_CP2_OFFSET                     (368u)
#define CapSense_BUTTON2_SNS_CP2_SIZE                       (4u)
#define CapSense_BUTTON2_SNS_CP2_PARAM_ID                   (0xEA000170u)

#define CapSense_BUTTON3_SNS_CP0_VALUE                      (CapSense_dsRam.snsCp.button3[0u])
#define CapSense_BUTTON3_SNS_CP0_OFFSET                     (372u)
#define CapSense_BUTTON3_SNS_CP0_SIZE                       (4u)
#define CapSense_BUTTON3_SNS_CP0_PARAM_ID                   (0xEB000174u)

#define CapSense_BUTTON3_SNS_CP1_VALUE                      (CapSense_dsRam.snsCp.button3[1u])
#define CapSense_BUTTON3_SNS_CP1_OFFSET                     (376u)
#define CapSense_BUTTON3_SNS_CP1_SIZE                       (4u)
#define CapSense_BUTTON3_SNS_CP1_PARAM_ID                   (0xE8000178u)

#define CapSense_BUTTON3_SNS_CP2_VALUE                      (CapSense_dsRam.snsCp.button3[2u])
#define CapSense_BUTTON3_SNS_CP2_OFFSET                     (380u)
#define CapSense_BUTTON3_SNS_CP2_SIZE                       (4u)
#define CapSense_BUTTON3_SNS_CP2_PARAM_ID                   (0xE900017Cu)

#define CapSense_BUTTON4_SNS_CP0_VALUE                      (CapSense_dsRam.snsCp.button4[0u])
#define CapSense_BUTTON4_SNS_CP0_OFFSET                     (384u)
#define CapSense_BUTTON4_SNS_CP0_SIZE                       (4u)
#define CapSense_BUTTON4_SNS_CP0_PARAM_ID                   (0xEB000180u)

#define CapSense_BUTTON4_SNS_CP1_VALUE                      (CapSense_dsRam.snsCp.button4[1u])
#define CapSense_BUTTON4_SNS_CP1_OFFSET                     (388u)
#define CapSense_BUTTON4_SNS_CP1_SIZE                       (4u)
#define CapSense_BUTTON4_SNS_CP1_PARAM_ID                   (0xEA000184u)

#define CapSense_BUTTON4_SNS_CP2_VALUE                      (CapSense_dsRam.snsCp.button4[2u])
#define CapSense_BUTTON4_SNS_CP2_OFFSET                     (392u)
#define CapSense_BUTTON4_SNS_CP2_SIZE                       (4u)
#define CapSense_BUTTON4_SNS_CP2_PARAM_ID                   (0xE9000188u)

#define CapSense_BUTTON5_SNS_CP0_VALUE                      (CapSense_dsRam.snsCp.button5[0u])
#define CapSense_BUTTON5_SNS_CP0_OFFSET                     (396u)
#define CapSense_BUTTON5_SNS_CP0_SIZE                       (4u)
#define CapSense_BUTTON5_SNS_CP0_PARAM_ID                   (0xE800018Cu)

#define CapSense_BUTTON5_SNS_CP1_VALUE                      (CapSense_dsRam.snsCp.button5[1u])
#define CapSense_BUTTON5_SNS_CP1_OFFSET                     (400u)
#define CapSense_BUTTON5_SNS_CP1_SIZE                       (4u)
#define CapSense_BUTTON5_SNS_CP1_PARAM_ID                   (0xEE000190u)

#define CapSense_BUTTON5_SNS_CP2_VALUE                      (CapSense_dsRam.snsCp.button5[2u])
#define CapSense_BUTTON5_SNS_CP2_OFFSET                     (404u)
#define CapSense_BUTTON5_SNS_CP2_SIZE                       (4u)
#define CapSense_BUTTON5_SNS_CP2_PARAM_ID                   (0xEF000194u)

#define CapSense_SNR_TEST_WIDGET_ID_VALUE                   (CapSense_dsRam.snrTestWidgetId)
#define CapSense_SNR_TEST_WIDGET_ID_OFFSET                  (408u)
#define CapSense_SNR_TEST_WIDGET_ID_SIZE                    (1u)
#define CapSense_SNR_TEST_WIDGET_ID_PARAM_ID                (0x6B000198u)

#define CapSense_SNR_TEST_SENSOR_ID_VALUE                   (CapSense_dsRam.snrTestSensorId)
#define CapSense_SNR_TEST_SENSOR_ID_OFFSET                  (409u)
#define CapSense_SNR_TEST_SENSOR_ID_SIZE                    (1u)
#define CapSense_SNR_TEST_SENSOR_ID_PARAM_ID                (0x6D000199u)

#define CapSense_SNR_TEST_SCAN_COUNTER_VALUE                (CapSense_dsRam.snrTestScanCounter)
#define CapSense_SNR_TEST_SCAN_COUNTER_OFFSET               (410u)
#define CapSense_SNR_TEST_SCAN_COUNTER_SIZE                 (2u)
#define CapSense_SNR_TEST_SCAN_COUNTER_PARAM_ID             (0x8400019Au)

#define CapSense_SNR_TEST_RAW_COUNT0_VALUE                  (CapSense_dsRam.snrTestRawCount[0u])
#define CapSense_SNR_TEST_RAW_COUNT0_OFFSET                 (412u)
#define CapSense_SNR_TEST_RAW_COUNT0_SIZE                   (2u)
#define CapSense_SNR_TEST_RAW_COUNT0_PARAM_ID               (0x8900019Cu)

#define CapSense_SCAN_CSX_ISC_VALUE                         (CapSense_dsRam.scanCsxISC)
#define CapSense_SCAN_CSX_ISC_OFFSET                        (414u)
#define CapSense_SCAN_CSX_ISC_SIZE                          (1u)
#define CapSense_SCAN_CSX_ISC_PARAM_ID                      (0x4D00019Eu)

#define CapSense_SCAN_CURRENT_ISC_VALUE                     (CapSense_dsRam.scanCurrentISC)
#define CapSense_SCAN_CURRENT_ISC_OFFSET                    (415u)
#define CapSense_SCAN_CURRENT_ISC_SIZE                      (1u)
#define CapSense_SCAN_CURRENT_ISC_PARAM_ID                  (0x4B00019Fu)


/*****************************************************************************/
/* Flash Data structure register definitions                                 */
/*****************************************************************************/
#define CapSense_BUTTON0_PTR2SNS_FLASH_VALUE                (CapSense_dsFlash.wdgtArray[0].ptr2SnsFlash)
#define CapSense_BUTTON0_PTR2SNS_FLASH_OFFSET               (0u)
#define CapSense_BUTTON0_PTR2SNS_FLASH_SIZE                 (4u)
#define CapSense_BUTTON0_PTR2SNS_FLASH_PARAM_ID             (0xD1000000u)

#define CapSense_BUTTON0_PTR2WD_RAM_VALUE                   (CapSense_dsFlash.wdgtArray[0].ptr2WdgtRam)
#define CapSense_BUTTON0_PTR2WD_RAM_OFFSET                  (4u)
#define CapSense_BUTTON0_PTR2WD_RAM_SIZE                    (4u)
#define CapSense_BUTTON0_PTR2WD_RAM_PARAM_ID                (0xD0000004u)

#define CapSense_BUTTON0_PTR2SNS_RAM_VALUE                  (CapSense_dsFlash.wdgtArray[0].ptr2SnsRam)
#define CapSense_BUTTON0_PTR2SNS_RAM_OFFSET                 (8u)
#define CapSense_BUTTON0_PTR2SNS_RAM_SIZE                   (4u)
#define CapSense_BUTTON0_PTR2SNS_RAM_PARAM_ID               (0xD3000008u)

#define CapSense_BUTTON0_PTR2FLTR_HISTORY_VALUE             (CapSense_dsFlash.wdgtArray[0].ptr2FltrHistory)
#define CapSense_BUTTON0_PTR2FLTR_HISTORY_OFFSET            (12u)
#define CapSense_BUTTON0_PTR2FLTR_HISTORY_SIZE              (4u)
#define CapSense_BUTTON0_PTR2FLTR_HISTORY_PARAM_ID          (0xD200000Cu)

#define CapSense_BUTTON0_PTR2DEBOUNCE_VALUE                 (CapSense_dsFlash.wdgtArray[0].ptr2DebounceArr)
#define CapSense_BUTTON0_PTR2DEBOUNCE_OFFSET                (16u)
#define CapSense_BUTTON0_PTR2DEBOUNCE_SIZE                  (4u)
#define CapSense_BUTTON0_PTR2DEBOUNCE_PARAM_ID              (0xD4000010u)

#define CapSense_BUTTON0_STATIC_CONFIG_VALUE                (CapSense_dsFlash.wdgtArray[0].staticConfig)
#define CapSense_BUTTON0_STATIC_CONFIG_OFFSET               (20u)
#define CapSense_BUTTON0_STATIC_CONFIG_SIZE                 (4u)
#define CapSense_BUTTON0_STATIC_CONFIG_PARAM_ID             (0xD5000014u)

#define CapSense_BUTTON0_TOTAL_NUM_SNS_VALUE                (CapSense_dsFlash.wdgtArray[0].totalNumSns)
#define CapSense_BUTTON0_TOTAL_NUM_SNS_OFFSET               (24u)
#define CapSense_BUTTON0_TOTAL_NUM_SNS_SIZE                 (2u)
#define CapSense_BUTTON0_TOTAL_NUM_SNS_PARAM_ID             (0x99000018u)

#define CapSense_BUTTON0_TYPE_VALUE                         (CapSense_dsFlash.wdgtArray[0].wdgtType)
#define CapSense_BUTTON0_TYPE_OFFSET                        (26u)
#define CapSense_BUTTON0_TYPE_SIZE                          (1u)
#define CapSense_BUTTON0_TYPE_PARAM_ID                      (0x5D00001Au)

#define CapSense_BUTTON0_NUM_COLS_VALUE                     (CapSense_dsFlash.wdgtArray[0].numCols)
#define CapSense_BUTTON0_NUM_COLS_OFFSET                    (27u)
#define CapSense_BUTTON0_NUM_COLS_SIZE                      (1u)
#define CapSense_BUTTON0_NUM_COLS_PARAM_ID                  (0x5B00001Bu)

#define CapSense_BUTTON0_NUM_ROWS_VALUE                     (CapSense_dsFlash.wdgtArray[0].numRows)
#define CapSense_BUTTON0_NUM_ROWS_OFFSET                    (28u)
#define CapSense_BUTTON0_NUM_ROWS_SIZE                      (1u)
#define CapSense_BUTTON0_NUM_ROWS_PARAM_ID                  (0x5000001Cu)

#define CapSense_BUTTON0_PTR2SNS_CP_VALUE                   (CapSense_dsFlash.wdgtArray[0].ptr2SnsCpArr)
#define CapSense_BUTTON0_PTR2SNS_CP_OFFSET                  (32u)
#define CapSense_BUTTON0_PTR2SNS_CP_SIZE                    (4u)
#define CapSense_BUTTON0_PTR2SNS_CP_PARAM_ID                (0xDB000020u)

#define CapSense_BUTTON1_PTR2SNS_FLASH_VALUE                (CapSense_dsFlash.wdgtArray[1].ptr2SnsFlash)
#define CapSense_BUTTON1_PTR2SNS_FLASH_OFFSET               (36u)
#define CapSense_BUTTON1_PTR2SNS_FLASH_SIZE                 (4u)
#define CapSense_BUTTON1_PTR2SNS_FLASH_PARAM_ID             (0xD9010024u)

#define CapSense_BUTTON1_PTR2WD_RAM_VALUE                   (CapSense_dsFlash.wdgtArray[1].ptr2WdgtRam)
#define CapSense_BUTTON1_PTR2WD_RAM_OFFSET                  (40u)
#define CapSense_BUTTON1_PTR2WD_RAM_SIZE                    (4u)
#define CapSense_BUTTON1_PTR2WD_RAM_PARAM_ID                (0xDA010028u)

#define CapSense_BUTTON1_PTR2SNS_RAM_VALUE                  (CapSense_dsFlash.wdgtArray[1].ptr2SnsRam)
#define CapSense_BUTTON1_PTR2SNS_RAM_OFFSET                 (44u)
#define CapSense_BUTTON1_PTR2SNS_RAM_SIZE                   (4u)
#define CapSense_BUTTON1_PTR2SNS_RAM_PARAM_ID               (0xDB01002Cu)

#define CapSense_BUTTON1_PTR2FLTR_HISTORY_VALUE             (CapSense_dsFlash.wdgtArray[1].ptr2FltrHistory)
#define CapSense_BUTTON1_PTR2FLTR_HISTORY_OFFSET            (48u)
#define CapSense_BUTTON1_PTR2FLTR_HISTORY_SIZE              (4u)
#define CapSense_BUTTON1_PTR2FLTR_HISTORY_PARAM_ID          (0xDD010030u)

#define CapSense_BUTTON1_PTR2DEBOUNCE_VALUE                 (CapSense_dsFlash.wdgtArray[1].ptr2DebounceArr)
#define CapSense_BUTTON1_PTR2DEBOUNCE_OFFSET                (52u)
#define CapSense_BUTTON1_PTR2DEBOUNCE_SIZE                  (4u)
#define CapSense_BUTTON1_PTR2DEBOUNCE_PARAM_ID              (0xDC010034u)

#define CapSense_BUTTON1_STATIC_CONFIG_VALUE                (CapSense_dsFlash.wdgtArray[1].staticConfig)
#define CapSense_BUTTON1_STATIC_CONFIG_OFFSET               (56u)
#define CapSense_BUTTON1_STATIC_CONFIG_SIZE                 (4u)
#define CapSense_BUTTON1_STATIC_CONFIG_PARAM_ID             (0xDF010038u)

#define CapSense_BUTTON1_TOTAL_NUM_SNS_VALUE                (CapSense_dsFlash.wdgtArray[1].totalNumSns)
#define CapSense_BUTTON1_TOTAL_NUM_SNS_OFFSET               (60u)
#define CapSense_BUTTON1_TOTAL_NUM_SNS_SIZE                 (2u)
#define CapSense_BUTTON1_TOTAL_NUM_SNS_PARAM_ID             (0x9101003Cu)

#define CapSense_BUTTON1_TYPE_VALUE                         (CapSense_dsFlash.wdgtArray[1].wdgtType)
#define CapSense_BUTTON1_TYPE_OFFSET                        (62u)
#define CapSense_BUTTON1_TYPE_SIZE                          (1u)
#define CapSense_BUTTON1_TYPE_PARAM_ID                      (0x5501003Eu)

#define CapSense_BUTTON1_NUM_COLS_VALUE                     (CapSense_dsFlash.wdgtArray[1].numCols)
#define CapSense_BUTTON1_NUM_COLS_OFFSET                    (63u)
#define CapSense_BUTTON1_NUM_COLS_SIZE                      (1u)
#define CapSense_BUTTON1_NUM_COLS_PARAM_ID                  (0x5301003Fu)

#define CapSense_BUTTON1_NUM_ROWS_VALUE                     (CapSense_dsFlash.wdgtArray[1].numRows)
#define CapSense_BUTTON1_NUM_ROWS_OFFSET                    (64u)
#define CapSense_BUTTON1_NUM_ROWS_SIZE                      (1u)
#define CapSense_BUTTON1_NUM_ROWS_PARAM_ID                  (0x58010040u)

#define CapSense_BUTTON1_PTR2SNS_CP_VALUE                   (CapSense_dsFlash.wdgtArray[1].ptr2SnsCpArr)
#define CapSense_BUTTON1_PTR2SNS_CP_OFFSET                  (68u)
#define CapSense_BUTTON1_PTR2SNS_CP_SIZE                    (4u)
#define CapSense_BUTTON1_PTR2SNS_CP_PARAM_ID                (0xDE010044u)

#define CapSense_BUTTON2_PTR2SNS_FLASH_VALUE                (CapSense_dsFlash.wdgtArray[2].ptr2SnsFlash)
#define CapSense_BUTTON2_PTR2SNS_FLASH_OFFSET               (72u)
#define CapSense_BUTTON2_PTR2SNS_FLASH_SIZE                 (4u)
#define CapSense_BUTTON2_PTR2SNS_FLASH_PARAM_ID             (0xD8020048u)

#define CapSense_BUTTON2_PTR2WD_RAM_VALUE                   (CapSense_dsFlash.wdgtArray[2].ptr2WdgtRam)
#define CapSense_BUTTON2_PTR2WD_RAM_OFFSET                  (76u)
#define CapSense_BUTTON2_PTR2WD_RAM_SIZE                    (4u)
#define CapSense_BUTTON2_PTR2WD_RAM_PARAM_ID                (0xD902004Cu)

#define CapSense_BUTTON2_PTR2SNS_RAM_VALUE                  (CapSense_dsFlash.wdgtArray[2].ptr2SnsRam)
#define CapSense_BUTTON2_PTR2SNS_RAM_OFFSET                 (80u)
#define CapSense_BUTTON2_PTR2SNS_RAM_SIZE                   (4u)
#define CapSense_BUTTON2_PTR2SNS_RAM_PARAM_ID               (0xDF020050u)

#define CapSense_BUTTON2_PTR2FLTR_HISTORY_VALUE             (CapSense_dsFlash.wdgtArray[2].ptr2FltrHistory)
#define CapSense_BUTTON2_PTR2FLTR_HISTORY_OFFSET            (84u)
#define CapSense_BUTTON2_PTR2FLTR_HISTORY_SIZE              (4u)
#define CapSense_BUTTON2_PTR2FLTR_HISTORY_PARAM_ID          (0xDE020054u)

#define CapSense_BUTTON2_PTR2DEBOUNCE_VALUE                 (CapSense_dsFlash.wdgtArray[2].ptr2DebounceArr)
#define CapSense_BUTTON2_PTR2DEBOUNCE_OFFSET                (88u)
#define CapSense_BUTTON2_PTR2DEBOUNCE_SIZE                  (4u)
#define CapSense_BUTTON2_PTR2DEBOUNCE_PARAM_ID              (0xDD020058u)

#define CapSense_BUTTON2_STATIC_CONFIG_VALUE                (CapSense_dsFlash.wdgtArray[2].staticConfig)
#define CapSense_BUTTON2_STATIC_CONFIG_OFFSET               (92u)
#define CapSense_BUTTON2_STATIC_CONFIG_SIZE                 (4u)
#define CapSense_BUTTON2_STATIC_CONFIG_PARAM_ID             (0xDC02005Cu)

#define CapSense_BUTTON2_TOTAL_NUM_SNS_VALUE                (CapSense_dsFlash.wdgtArray[2].totalNumSns)
#define CapSense_BUTTON2_TOTAL_NUM_SNS_OFFSET               (96u)
#define CapSense_BUTTON2_TOTAL_NUM_SNS_SIZE                 (2u)
#define CapSense_BUTTON2_TOTAL_NUM_SNS_PARAM_ID             (0x9F020060u)

#define CapSense_BUTTON2_TYPE_VALUE                         (CapSense_dsFlash.wdgtArray[2].wdgtType)
#define CapSense_BUTTON2_TYPE_OFFSET                        (98u)
#define CapSense_BUTTON2_TYPE_SIZE                          (1u)
#define CapSense_BUTTON2_TYPE_PARAM_ID                      (0x5B020062u)

#define CapSense_BUTTON2_NUM_COLS_VALUE                     (CapSense_dsFlash.wdgtArray[2].numCols)
#define CapSense_BUTTON2_NUM_COLS_OFFSET                    (99u)
#define CapSense_BUTTON2_NUM_COLS_SIZE                      (1u)
#define CapSense_BUTTON2_NUM_COLS_PARAM_ID                  (0x5D020063u)

#define CapSense_BUTTON2_NUM_ROWS_VALUE                     (CapSense_dsFlash.wdgtArray[2].numRows)
#define CapSense_BUTTON2_NUM_ROWS_OFFSET                    (100u)
#define CapSense_BUTTON2_NUM_ROWS_SIZE                      (1u)
#define CapSense_BUTTON2_NUM_ROWS_PARAM_ID                  (0x56020064u)

#define CapSense_BUTTON2_PTR2SNS_CP_VALUE                   (CapSense_dsFlash.wdgtArray[2].ptr2SnsCpArr)
#define CapSense_BUTTON2_PTR2SNS_CP_OFFSET                  (104u)
#define CapSense_BUTTON2_PTR2SNS_CP_SIZE                    (4u)
#define CapSense_BUTTON2_PTR2SNS_CP_PARAM_ID                (0xD2020068u)

#define CapSense_BUTTON3_PTR2SNS_FLASH_VALUE                (CapSense_dsFlash.wdgtArray[3].ptr2SnsFlash)
#define CapSense_BUTTON3_PTR2SNS_FLASH_OFFSET               (108u)
#define CapSense_BUTTON3_PTR2SNS_FLASH_SIZE                 (4u)
#define CapSense_BUTTON3_PTR2SNS_FLASH_PARAM_ID             (0xD003006Cu)

#define CapSense_BUTTON3_PTR2WD_RAM_VALUE                   (CapSense_dsFlash.wdgtArray[3].ptr2WdgtRam)
#define CapSense_BUTTON3_PTR2WD_RAM_OFFSET                  (112u)
#define CapSense_BUTTON3_PTR2WD_RAM_SIZE                    (4u)
#define CapSense_BUTTON3_PTR2WD_RAM_PARAM_ID                (0xD6030070u)

#define CapSense_BUTTON3_PTR2SNS_RAM_VALUE                  (CapSense_dsFlash.wdgtArray[3].ptr2SnsRam)
#define CapSense_BUTTON3_PTR2SNS_RAM_OFFSET                 (116u)
#define CapSense_BUTTON3_PTR2SNS_RAM_SIZE                   (4u)
#define CapSense_BUTTON3_PTR2SNS_RAM_PARAM_ID               (0xD7030074u)

#define CapSense_BUTTON3_PTR2FLTR_HISTORY_VALUE             (CapSense_dsFlash.wdgtArray[3].ptr2FltrHistory)
#define CapSense_BUTTON3_PTR2FLTR_HISTORY_OFFSET            (120u)
#define CapSense_BUTTON3_PTR2FLTR_HISTORY_SIZE              (4u)
#define CapSense_BUTTON3_PTR2FLTR_HISTORY_PARAM_ID          (0xD4030078u)

#define CapSense_BUTTON3_PTR2DEBOUNCE_VALUE                 (CapSense_dsFlash.wdgtArray[3].ptr2DebounceArr)
#define CapSense_BUTTON3_PTR2DEBOUNCE_OFFSET                (124u)
#define CapSense_BUTTON3_PTR2DEBOUNCE_SIZE                  (4u)
#define CapSense_BUTTON3_PTR2DEBOUNCE_PARAM_ID              (0xD503007Cu)

#define CapSense_BUTTON3_STATIC_CONFIG_VALUE                (CapSense_dsFlash.wdgtArray[3].staticConfig)
#define CapSense_BUTTON3_STATIC_CONFIG_OFFSET               (128u)
#define CapSense_BUTTON3_STATIC_CONFIG_SIZE                 (4u)
#define CapSense_BUTTON3_STATIC_CONFIG_PARAM_ID             (0xD7030080u)

#define CapSense_BUTTON3_TOTAL_NUM_SNS_VALUE                (CapSense_dsFlash.wdgtArray[3].totalNumSns)
#define CapSense_BUTTON3_TOTAL_NUM_SNS_OFFSET               (132u)
#define CapSense_BUTTON3_TOTAL_NUM_SNS_SIZE                 (2u)
#define CapSense_BUTTON3_TOTAL_NUM_SNS_PARAM_ID             (0x99030084u)

#define CapSense_BUTTON3_TYPE_VALUE                         (CapSense_dsFlash.wdgtArray[3].wdgtType)
#define CapSense_BUTTON3_TYPE_OFFSET                        (134u)
#define CapSense_BUTTON3_TYPE_SIZE                          (1u)
#define CapSense_BUTTON3_TYPE_PARAM_ID                      (0x5D030086u)

#define CapSense_BUTTON3_NUM_COLS_VALUE                     (CapSense_dsFlash.wdgtArray[3].numCols)
#define CapSense_BUTTON3_NUM_COLS_OFFSET                    (135u)
#define CapSense_BUTTON3_NUM_COLS_SIZE                      (1u)
#define CapSense_BUTTON3_NUM_COLS_PARAM_ID                  (0x5B030087u)

#define CapSense_BUTTON3_NUM_ROWS_VALUE                     (CapSense_dsFlash.wdgtArray[3].numRows)
#define CapSense_BUTTON3_NUM_ROWS_OFFSET                    (136u)
#define CapSense_BUTTON3_NUM_ROWS_SIZE                      (1u)
#define CapSense_BUTTON3_NUM_ROWS_PARAM_ID                  (0x52030088u)

#define CapSense_BUTTON3_PTR2SNS_CP_VALUE                   (CapSense_dsFlash.wdgtArray[3].ptr2SnsCpArr)
#define CapSense_BUTTON3_PTR2SNS_CP_OFFSET                  (140u)
#define CapSense_BUTTON3_PTR2SNS_CP_SIZE                    (4u)
#define CapSense_BUTTON3_PTR2SNS_CP_PARAM_ID                (0xD403008Cu)

#define CapSense_BUTTON4_PTR2SNS_FLASH_VALUE                (CapSense_dsFlash.wdgtArray[4].ptr2SnsFlash)
#define CapSense_BUTTON4_PTR2SNS_FLASH_OFFSET               (144u)
#define CapSense_BUTTON4_PTR2SNS_FLASH_SIZE                 (4u)
#define CapSense_BUTTON4_PTR2SNS_FLASH_PARAM_ID             (0xDB040090u)

#define CapSense_BUTTON4_PTR2WD_RAM_VALUE                   (CapSense_dsFlash.wdgtArray[4].ptr2WdgtRam)
#define CapSense_BUTTON4_PTR2WD_RAM_OFFSET                  (148u)
#define CapSense_BUTTON4_PTR2WD_RAM_SIZE                    (4u)
#define CapSense_BUTTON4_PTR2WD_RAM_PARAM_ID                (0xDA040094u)

#define CapSense_BUTTON4_PTR2SNS_RAM_VALUE                  (CapSense_dsFlash.wdgtArray[4].ptr2SnsRam)
#define CapSense_BUTTON4_PTR2SNS_RAM_OFFSET                 (152u)
#define CapSense_BUTTON4_PTR2SNS_RAM_SIZE                   (4u)
#define CapSense_BUTTON4_PTR2SNS_RAM_PARAM_ID               (0xD9040098u)

#define CapSense_BUTTON4_PTR2FLTR_HISTORY_VALUE             (CapSense_dsFlash.wdgtArray[4].ptr2FltrHistory)
#define CapSense_BUTTON4_PTR2FLTR_HISTORY_OFFSET            (156u)
#define CapSense_BUTTON4_PTR2FLTR_HISTORY_SIZE              (4u)
#define CapSense_BUTTON4_PTR2FLTR_HISTORY_PARAM_ID          (0xD804009Cu)

#define CapSense_BUTTON4_PTR2DEBOUNCE_VALUE                 (CapSense_dsFlash.wdgtArray[4].ptr2DebounceArr)
#define CapSense_BUTTON4_PTR2DEBOUNCE_OFFSET                (160u)
#define CapSense_BUTTON4_PTR2DEBOUNCE_SIZE                  (4u)
#define CapSense_BUTTON4_PTR2DEBOUNCE_PARAM_ID              (0xD40400A0u)

#define CapSense_BUTTON4_STATIC_CONFIG_VALUE                (CapSense_dsFlash.wdgtArray[4].staticConfig)
#define CapSense_BUTTON4_STATIC_CONFIG_OFFSET               (164u)
#define CapSense_BUTTON4_STATIC_CONFIG_SIZE                 (4u)
#define CapSense_BUTTON4_STATIC_CONFIG_PARAM_ID             (0xD50400A4u)

#define CapSense_BUTTON4_TOTAL_NUM_SNS_VALUE                (CapSense_dsFlash.wdgtArray[4].totalNumSns)
#define CapSense_BUTTON4_TOTAL_NUM_SNS_OFFSET               (168u)
#define CapSense_BUTTON4_TOTAL_NUM_SNS_SIZE                 (2u)
#define CapSense_BUTTON4_TOTAL_NUM_SNS_PARAM_ID             (0x990400A8u)

#define CapSense_BUTTON4_TYPE_VALUE                         (CapSense_dsFlash.wdgtArray[4].wdgtType)
#define CapSense_BUTTON4_TYPE_OFFSET                        (170u)
#define CapSense_BUTTON4_TYPE_SIZE                          (1u)
#define CapSense_BUTTON4_TYPE_PARAM_ID                      (0x5D0400AAu)

#define CapSense_BUTTON4_NUM_COLS_VALUE                     (CapSense_dsFlash.wdgtArray[4].numCols)
#define CapSense_BUTTON4_NUM_COLS_OFFSET                    (171u)
#define CapSense_BUTTON4_NUM_COLS_SIZE                      (1u)
#define CapSense_BUTTON4_NUM_COLS_PARAM_ID                  (0x5B0400ABu)

#define CapSense_BUTTON4_NUM_ROWS_VALUE                     (CapSense_dsFlash.wdgtArray[4].numRows)
#define CapSense_BUTTON4_NUM_ROWS_OFFSET                    (172u)
#define CapSense_BUTTON4_NUM_ROWS_SIZE                      (1u)
#define CapSense_BUTTON4_NUM_ROWS_PARAM_ID                  (0x500400ACu)

#define CapSense_BUTTON4_PTR2SNS_CP_VALUE                   (CapSense_dsFlash.wdgtArray[4].ptr2SnsCpArr)
#define CapSense_BUTTON4_PTR2SNS_CP_OFFSET                  (176u)
#define CapSense_BUTTON4_PTR2SNS_CP_SIZE                    (4u)
#define CapSense_BUTTON4_PTR2SNS_CP_PARAM_ID                (0xD10400B0u)

#define CapSense_BUTTON5_PTR2SNS_FLASH_VALUE                (CapSense_dsFlash.wdgtArray[5].ptr2SnsFlash)
#define CapSense_BUTTON5_PTR2SNS_FLASH_OFFSET               (180u)
#define CapSense_BUTTON5_PTR2SNS_FLASH_SIZE                 (4u)
#define CapSense_BUTTON5_PTR2SNS_FLASH_PARAM_ID             (0xD30500B4u)

#define CapSense_BUTTON5_PTR2WD_RAM_VALUE                   (CapSense_dsFlash.wdgtArray[5].ptr2WdgtRam)
#define CapSense_BUTTON5_PTR2WD_RAM_OFFSET                  (184u)
#define CapSense_BUTTON5_PTR2WD_RAM_SIZE                    (4u)
#define CapSense_BUTTON5_PTR2WD_RAM_PARAM_ID                (0xD00500B8u)

#define CapSense_BUTTON5_PTR2SNS_RAM_VALUE                  (CapSense_dsFlash.wdgtArray[5].ptr2SnsRam)
#define CapSense_BUTTON5_PTR2SNS_RAM_OFFSET                 (188u)
#define CapSense_BUTTON5_PTR2SNS_RAM_SIZE                   (4u)
#define CapSense_BUTTON5_PTR2SNS_RAM_PARAM_ID               (0xD10500BCu)

#define CapSense_BUTTON5_PTR2FLTR_HISTORY_VALUE             (CapSense_dsFlash.wdgtArray[5].ptr2FltrHistory)
#define CapSense_BUTTON5_PTR2FLTR_HISTORY_OFFSET            (192u)
#define CapSense_BUTTON5_PTR2FLTR_HISTORY_SIZE              (4u)
#define CapSense_BUTTON5_PTR2FLTR_HISTORY_PARAM_ID          (0xD00500C0u)

#define CapSense_BUTTON5_PTR2DEBOUNCE_VALUE                 (CapSense_dsFlash.wdgtArray[5].ptr2DebounceArr)
#define CapSense_BUTTON5_PTR2DEBOUNCE_OFFSET                (196u)
#define CapSense_BUTTON5_PTR2DEBOUNCE_SIZE                  (4u)
#define CapSense_BUTTON5_PTR2DEBOUNCE_PARAM_ID              (0xD10500C4u)

#define CapSense_BUTTON5_STATIC_CONFIG_VALUE                (CapSense_dsFlash.wdgtArray[5].staticConfig)
#define CapSense_BUTTON5_STATIC_CONFIG_OFFSET               (200u)
#define CapSense_BUTTON5_STATIC_CONFIG_SIZE                 (4u)
#define CapSense_BUTTON5_STATIC_CONFIG_PARAM_ID             (0xD20500C8u)

#define CapSense_BUTTON5_TOTAL_NUM_SNS_VALUE                (CapSense_dsFlash.wdgtArray[5].totalNumSns)
#define CapSense_BUTTON5_TOTAL_NUM_SNS_OFFSET               (204u)
#define CapSense_BUTTON5_TOTAL_NUM_SNS_SIZE                 (2u)
#define CapSense_BUTTON5_TOTAL_NUM_SNS_PARAM_ID             (0x9C0500CCu)

#define CapSense_BUTTON5_TYPE_VALUE                         (CapSense_dsFlash.wdgtArray[5].wdgtType)
#define CapSense_BUTTON5_TYPE_OFFSET                        (206u)
#define CapSense_BUTTON5_TYPE_SIZE                          (1u)
#define CapSense_BUTTON5_TYPE_PARAM_ID                      (0x580500CEu)

#define CapSense_BUTTON5_NUM_COLS_VALUE                     (CapSense_dsFlash.wdgtArray[5].numCols)
#define CapSense_BUTTON5_NUM_COLS_OFFSET                    (207u)
#define CapSense_BUTTON5_NUM_COLS_SIZE                      (1u)
#define CapSense_BUTTON5_NUM_COLS_PARAM_ID                  (0x5E0500CFu)

#define CapSense_BUTTON5_NUM_ROWS_VALUE                     (CapSense_dsFlash.wdgtArray[5].numRows)
#define CapSense_BUTTON5_NUM_ROWS_OFFSET                    (208u)
#define CapSense_BUTTON5_NUM_ROWS_SIZE                      (1u)
#define CapSense_BUTTON5_NUM_ROWS_PARAM_ID                  (0x520500D0u)

#define CapSense_BUTTON5_PTR2SNS_CP_VALUE                   (CapSense_dsFlash.wdgtArray[5].ptr2SnsCpArr)
#define CapSense_BUTTON5_PTR2SNS_CP_OFFSET                  (212u)
#define CapSense_BUTTON5_PTR2SNS_CP_SIZE                    (4u)
#define CapSense_BUTTON5_PTR2SNS_CP_PARAM_ID                (0xD40500D4u)


#endif /* End CY_SENSE_CapSense_REGISTER_MAP_H */


/* [] END OF FILE */
