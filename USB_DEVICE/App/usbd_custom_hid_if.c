/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : usbd_custom_hid_if.c
  * @version        : v2.0_Cube
  * @brief          : USB Device Custom HID interface file.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2026 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "usbd_custom_hid_if.h"
#include "u2hts_core.h"
/* USER CODE BEGIN INCLUDE */

/* USER CODE END INCLUDE */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/

/* USER CODE BEGIN PV */
/* Private variables ---------------------------------------------------------*/

/* USER CODE END PV */

/** @addtogroup STM32_USB_OTG_DEVICE_LIBRARY
  * @brief Usb device.
  * @{
  */

/** @addtogroup USBD_CUSTOM_HID
  * @{
  */

/** @defgroup USBD_CUSTOM_HID_Private_TypesDefinitions USBD_CUSTOM_HID_Private_TypesDefinitions
  * @brief Private types.
  * @{
  */

/* USER CODE BEGIN PRIVATE_TYPES */

/* USER CODE END PRIVATE_TYPES */

/**
  * @}
  */

/** @defgroup USBD_CUSTOM_HID_Private_Defines USBD_CUSTOM_HID_Private_Defines
  * @brief Private defines.
  * @{
  */

/* USER CODE BEGIN PRIVATE_DEFINES */

/* USER CODE END PRIVATE_DEFINES */

/**
  * @}
  */

/** @defgroup USBD_CUSTOM_HID_Private_Macros USBD_CUSTOM_HID_Private_Macros
  * @brief Private macros.
  * @{
  */

/* USER CODE BEGIN PRIVATE_MACRO */

/* USER CODE END PRIVATE_MACRO */

/**
  * @}
  */

/** @defgroup USBD_CUSTOM_HID_Private_Variables USBD_CUSTOM_HID_Private_Variables
  * @brief Private variables.
  * @{
  */

/** Usb HID report descriptor. */
#ifdef U2HTS_ENABLE_COMPACT_REPORT
#define U2HTS_HID_TP                                                           \
      0x09, 0x22, /* (LOCAL)  USAGE              Touch Point */            \
      0xA1, 0x02, /* (MAIN)   COLLECTION         Logical */                    \
      0x09, 0x42, /* (LOCAL)  USAGE              Tip Switch */                 \
      0x25, 0x01, /* (GLOBAL) LOGICAL_MAXIMUM    1 */                          \
      0x75, 0x01, /* (GLOBAL) REPORT_SIZE        1 bit */                      \
      0x95, 0x01, /* (GLOBAL) REPORT_COUNT       1 */                          \
      0x81, 0x02, /* (MAIN)   INPUT              (Data, Variable, Absolute) */ \
      0x09, 0x51, /* (LOCAL)  USAGE              Contact Identifier */         \
      0x75, 0x07, /* (GLOBAL) REPORT_SIZE        7 bits */                     \
      0x81, 0x02, /* (MAIN)   INPUT              (Data, Variable, Absolute) */ \
      0x05, 0x01, /* (GLOBAL) USAGE_PAGE         Generic Desktop */            \
      0x26, 0xFF, 0x0F, /* (GLOBAL) LOGICAL_MAXIMUM    4095 */                 \
      0x75, 0x0C,       /* (GLOBAL) REPORT_SIZE        12 bits */              \
      0x09, 0x30,       /* (LOCAL)  USAGE              X */                    \
      0x46, 0xFF, 0x0F, /* (GLOBAL) PHYSICAL_MAXIMUM   4095 */                 \
      0x81, 0x02, /* (MAIN)   INPUT              (Data, Variable, Absolute) */ \
      0x46, 0xFF, 0x0F, /* (GLOBAL) PHYSICAL_MAXIMUM   4095 */                 \
      0x09, 0x31,       /* (LOCAL)  USAGE              Y */                    \
      0x81, 0x02, /* (MAIN)   INPUT              (Data, Variable, Absolute) */ \
      0x05, 0x0D, /* (GLOBAL) USAGE_PAGE         Digitizers */                 \
      0xC0        /* (MAIN)   END_COLLECTION */
#else
#define U2HTS_HID_TP                                                           \
      0x09, 0x22, /* (LOCAL)  USAGE              Touch Point */                \
      0xA1, 0x02, /* (MAIN)   COLLECTION         Logical */                    \
      0x09, 0x42, /* (LOCAL)  USAGE              Tip Switch */                 \
      0x25, 0x01, /* (GLOBAL) LOGICAL_MAXIMUM    1 */                          \
      0x75, 0x01, /* (GLOBAL) REPORT_SIZE        1 bit */                      \
      0x95, 0x01, /* (GLOBAL) REPORT_COUNT       1 */                          \
      0x81, 0x02, /* (MAIN)   INPUT              (Data, Variable, Absolute) */ \
      0x09, 0x51, /* (LOCAL)  USAGE              Contact Identifier */         \
      0x75, 0x07, /* (GLOBAL) REPORT_SIZE        7 bits */                     \
      0x81, 0x02, /* (MAIN)   INPUT              (Data, Variable, Absolute) */ \
      0x05, 0x01, /* (GLOBAL) USAGE_PAGE         Generic Desktop */            \
      0x26, 0xFF, 0x0F, /* (GLOBAL) LOGICAL_MAXIMUM    4095 */                 \
      0x75, 0x10,       /* (GLOBAL) REPORT_SIZE        16 bits */              \
      0x09, 0x30,       /* (LOCAL)  USAGE              X */                    \
      0x46, 0xFF, 0x0F, /* (GLOBAL) PHYSICAL_MAXIMUM   4095 */                 \
      0x81, 0x02, /* (MAIN)   INPUT              (Data, Variable, Absolute) */ \
      0x46, 0xFF, 0x0F, /* (GLOBAL) PHYSICAL_MAXIMUM   4095 */                 \
      0x09, 0x31,       /* (LOCAL)  USAGE              Y */                    \
      0x81, 0x02, /* (MAIN)   INPUT              (Data, Variable, Absolute) */ \
      0x05, 0x0D, /* (GLOBAL) USAGE_PAGE         Digitizers */                 \
      0x26, 0xFF, 0x00, /* (GLOBAL) LOGICAL_MAXIMUM    255 */                  \
      0x46, 0xFF, 0x00, /* (GLOBAL) PHYSICAL_MAXIMUM   255 */                  \
      0x75, 0x08,       /* (GLOBAL) REPORT_SIZE        8 bits */               \
      0x95, 0x03,       /* (GLOBAL) REPORT_COUNT       3 */                    \
      0x09, 0x48,       /* (LOCAL)  USAGE              Width */                \
      0x09, 0x49,       /* (LOCAL)  USAGE              Height */               \
      0x09, 0x30,       /* (LOCAL)  USAGE              Tip Pressure */         \
      0x81, 0x02, /* (MAIN)   INPUT              (Data, Variable, Absolute) */ \
      0xC0        /* (MAIN)   END_COLLECTION */
#endif
__ALIGN_BEGIN static const uint8_t CUSTOM_HID_ReportDesc_FS[] __ALIGN_END = 
  {
    0x05, 0x0D,                    // (GLOBAL) USAGE_PAGE         Digitizers
    0x09, 0x04,                    // (LOCAL)  USAGE              Touch Screen
    0xA1, 0x01,                    // (MAIN)   COLLECTION         Application
    0x85, U2HTS_HID_REPORT_TP_ID,  // (GLOBAL) REPORT_ID
    0x09, 0x22,                    // (LOCAL)  USAGE              Touch Point
    0x34,                          // (GLOBAL) PHYSICAL_MINIMUM   0
    0x14,                          // (GLOBAL) LOGICAL_MINIMUM    0
    0x55, 0x0E,                    // (GLOBAL) UNIT_EXPONENT      -2
    0x65, 0x11,                    // (GLOBAL) UNIT               SI Linear (Centimeter)
    U2HTS_HID_TP,                  // Touch Point 1
    U2HTS_HID_TP,                  // Touch Point 2
    U2HTS_HID_TP,                  // Touch Point 3
    U2HTS_HID_TP,                  // Touch Point 4
    U2HTS_HID_TP,                  // Touch Point 5
    U2HTS_HID_TP,                  // Touch Point 6
    U2HTS_HID_TP,                  // Touch Point 7
    U2HTS_HID_TP,                  // Touch Point 8
    U2HTS_HID_TP,                  // Touch Point 9
    U2HTS_HID_TP,                  // Touch Point 10
    0x27, 0xFF, 0xFF, 0x00, 0x00,  // (GLOBAL) LOGICAL_MAXIMUM    65535
    0x75, 0x10,                    // (GLOBAL) REPORT_SIZE        16 bits
    0x55, 0x0C,                    // (GLOBAL) UNIT_EXPONENT      -4
    0x66, 0x01, 0x10,              // (GLOBAL) UNIT               SI Linear (Seconds)
    0x95, 0x01,                    // (GLOBAL) REPORT_COUNT       1
    0x09, 0x56,                    // (LOCAL)  USAGE              Scan Time
    0x81, 0x02,                    // (MAIN)   INPUT              (Data, Variable, Absolute)
    0x09, 0x54,                    // (LOCAL)  USAGE              Contact Count
    0x25, U2HTS_MAX_TPS,           // (GLOBAL) LOGICAL_MAXIMUM
    0x75, 0x08,                    // (GLOBAL) REPORT_SIZE        8 bits
    0x81, 0x02,                    // (MAIN)   INPUT              (Data, Variable, Absolute)
    0x85, U2HTS_HID_REPORT_TP_MAX_COUNT_ID, // (GLOBAL) REPORT_ID
    0x09, 0x55,                    // (LOCAL)  USAGE              Contact Count Maximum
    0xB1, 0x02,                    // (MAIN)   FEATURE            (Data, Variable, Absolute)
    0x85, U2HTS_HID_REPORT_TP_MS_THQA_CERT_ID, // (GLOBAL) REPORT_ID
    0x06, 0x00, 0xFF,              // (GLOBAL) USAGE_PAGE         Vendor Defined
    0x09, 0xC5,                    // (LOCAL)  USAGE              Vendor Usage 0xC5 (THQA Cert)
    0x26, 0xFF, 0x00,              // (GLOBAL) LOGICAL_MAXIMUM    255
    0x96, 0x00, 0x01,              // (GLOBAL) REPORT_COUNT       256
    0xB1, 0x02,                    // (MAIN)   FEATURE            (Data, Variable, Absolute)
    0xC0                           // (MAIN)   END_COLLECTION
};

/* USER CODE BEGIN PRIVATE_VARIABLES */

/* USER CODE END PRIVATE_VARIABLES */

/**
  * @}
  */

/** @defgroup USBD_CUSTOM_HID_Exported_Variables USBD_CUSTOM_HID_Exported_Variables
  * @brief Public variables.
  * @{
  */
extern USBD_HandleTypeDef hUsbDeviceFS;

/* USER CODE BEGIN EXPORTED_VARIABLES */

/* USER CODE END EXPORTED_VARIABLES */
/**
  * @}
  */

/** @defgroup USBD_CUSTOM_HID_Private_FunctionPrototypes USBD_CUSTOM_HID_Private_FunctionPrototypes
  * @brief Private functions declaration.
  * @{
  */

static int8_t CUSTOM_HID_Init_FS(void);
static int8_t CUSTOM_HID_DeInit_FS(void);
static int8_t CUSTOM_HID_OutEvent_FS(uint8_t event_idx, uint8_t state);

/**
  * @}
  */

USBD_CUSTOM_HID_ItfTypeDef USBD_CustomHID_fops_FS =
{
  (uint8_t *)CUSTOM_HID_ReportDesc_FS,
  CUSTOM_HID_Init_FS,
  CUSTOM_HID_DeInit_FS,
  CUSTOM_HID_OutEvent_FS
};

/** @defgroup USBD_CUSTOM_HID_Private_Functions USBD_CUSTOM_HID_Private_Functions
  * @brief Private functions.
  * @{
  */

/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Initializes the CUSTOM HID media low layer
  * @retval USBD_OK if all operations are OK else USBD_FAIL
  */
static int8_t CUSTOM_HID_Init_FS(void)
{
  /* USER CODE BEGIN 4 */
  return (USBD_OK);
  /* USER CODE END 4 */
}

/**
  * @brief  DeInitializes the CUSTOM HID media low layer
  * @retval USBD_OK if all operations are OK else USBD_FAIL
  */
static int8_t CUSTOM_HID_DeInit_FS(void)
{
  /* USER CODE BEGIN 5 */
  return (USBD_OK);
  /* USER CODE END 5 */
}

/**
  * @brief  Manage the CUSTOM HID class events
  * @param  event_idx: Event index
  * @param  state: Event state
  * @retval USBD_OK if all operations are OK else USBD_FAIL
  */
static int8_t CUSTOM_HID_OutEvent_FS(uint8_t event_idx, uint8_t state)
{
  /* USER CODE BEGIN 6 */
  return (USBD_OK);
  /* USER CODE END 6 */
}

/* USER CODE BEGIN 7 */
/**
  * @brief  Send the report to the Host
  * @param  report: The report to be sent
  * @param  len: The report length
  * @retval USBD_OK if all operations are OK else USBD_FAIL
  */
/*
static int8_t USBD_CUSTOM_HID_SendReport_FS(uint8_t *report, uint16_t len)
{
  return USBD_CUSTOM_HID_SendReport(&hUsbDeviceFS, report, len);
}
*/
/* USER CODE END 7 */

/* USER CODE BEGIN PRIVATE_FUNCTIONS_IMPLEMENTATION */

/* USER CODE END PRIVATE_FUNCTIONS_IMPLEMENTATION */
/**
  * @}
  */

/**
  * @}
  */

/**
  * @}
  */

