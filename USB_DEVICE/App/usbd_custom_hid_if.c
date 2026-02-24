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
#define U2HTS_HID_TP    \
    0x09 ,0x22,         \
    0xA1 ,0x02,         \
    0x09 ,0x42,         \
    0x25 ,0x01,         \
    0x75 ,0x01,         \
    0x95 ,0x01,         \
    0x81 ,0x02,         \
    0x09 ,0x51,         \
    0x75 ,0x07,         \
    0x81 ,0x02,         \
    0x05 ,0x01,         \
    0x26 ,0xFF, 0x0F,   \
    0x75 ,0x0C,         \
    0x09 ,0x30,         \
    0x46 ,0xFF, 0x0F,   \
    0x81 ,0x02,         \
    0x46 ,0xFF, 0x0F,   \
    0x09 ,0x31,         \
    0x81 ,0x02,         \
    0x05 ,0x0D,         \
    0xC0
#else
#define U2HTS_HID_TP  \
    0x09, 0x22,       \
    0xA1, 0x02,       \
    0x09, 0x42,       \
    0x25, 0x01,       \
    0x75, 0x01,       \
    0x95, 0x01,       \
    0x81, 0x02,       \
    0x09, 0x51,       \
    0x75, 0x07,       \
    0x81, 0x02,       \
    0x05, 0x01,       \
    0x26, 0x00, 0x10, \
    0x75, 0x10,       \
    0x09, 0x30,       \
    0x46, 0x00, 0x10, \
    0x81, 0x02,       \
    0x46, 0x00, 0x10, \
    0x09, 0x31,       \
    0x81, 0x02,       \
    0x05, 0x0D,       \
    0x26, 0xFF, 0x00, \
    0x46, 0xFF, 0x00, \
    0x75, 0x08,       \
    0x95, 0x03,       \
    0x09, 0x48,       \
    0x09, 0x49,       \
    0x09, 0x30,       \
    0x81, 0x02,       \
    0xC0
#endif
__ALIGN_BEGIN static const uint8_t CUSTOM_HID_ReportDesc_FS[] __ALIGN_END = 
    {
      0x05, 0x0D,                    // (GLOBAL) USAGE_PAGE         
      0x09, 0x04,                    // (LOCAL)  USAGE              
      0xA1, 0x01,                    // (MAIN)   COLLECTION         
      0x85, U2HTS_HID_REPORT_TP_ID,  // (GLOBAL) REPORT_ID          
      0x09, 0x22,                    // (LOCAL)  USAGE              
      0x34,                    // (GLOBAL) PHYSICAL_MINIMUM   
      0x14,                    // (GLOBAL) LOGICAL_MINIMUM    
      0x55, 0x0E,                    // (GLOBAL) UNIT_EXPONENT      
      0x65, 0x11,                    // (GLOBAL) UNIT               
      U2HTS_HID_TP,
      U2HTS_HID_TP,
      U2HTS_HID_TP,
      U2HTS_HID_TP,
      U2HTS_HID_TP,
      U2HTS_HID_TP,
      U2HTS_HID_TP,
      U2HTS_HID_TP,
      U2HTS_HID_TP,
      U2HTS_HID_TP,
      0x27, 0xFF, 0xFF, 0x00, 0x00,  // (GLOBAL) LOGICAL_MAXIMUM    
      0x75, 0x10,                    // (GLOBAL) REPORT_SIZE        
      0x55, 0x0C,                    // (GLOBAL) UNIT_EXPONENT      
      0x66, 0x01, 0x10,              // (GLOBAL) UNIT               
      0x95, 0x01,                    // (GLOBAL) REPORT_COUNT       
      0x09, 0x56,                    // (LOCAL)  USAGE              
      0x81, 0x02,                    // (MAIN)   INPUT              
      0x09, 0x54,                    // (LOCAL)  USAGE              
      0x25, 0x0A,                    // (GLOBAL) LOGICAL_MAXIMUM    
      0x75, 0x08,                    // (GLOBAL) REPORT_SIZE        
      0x81, 0x02,                    // (MAIN)   INPUT              
      0x85, U2HTS_HID_REPORT_TP_MAX_COUNT_ID, // (GLOBAL) REPORT_ID          
      0x09, 0x55,                    // (LOCAL)  USAGE              
      0xB1, 0x02,                    // (MAIN)   FEATURE            
      0x85, U2HTS_HID_REPORT_TP_MS_THQA_CERT_ID, //   (GLOBAL) REPORT_ID
      0x06, 0x00, 0xFF,              //   (GLOBAL) USAGE_PAGE     
      0x09, 0xC5,                    //   (LOCAL)  USAGE          
      0x26, 0xFF, 0x00,              //   (GLOBAL) LOGICAL_MAXIMUM
      0x96, 0x00, 0x01,              //   (GLOBAL) REPORT_COUNT   
      0xB1, 0x02,                    //   (MAIN)   FEATURE
      0xC0,                          // (MAIN)   END_COLLECTION
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

