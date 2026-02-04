/*
  Copyright (C) CNflysky.
  U2HTS stands for "USB to HID TouchScreen".
  This file is licensed under GPL V3.
  All rights reserved.
*/

#include <stdbool.h>

#include "main.h"
#include "u2hts_core.h"
#include "usb_device.h"
#include "usbd_customhid.h"
#include "gpio_i2c.h"

extern USBD_HandleTypeDef hUsbDeviceFS;

inline static bool getscl() {
  return HAL_GPIO_ReadPin(TP_SCL_GPIO_Port, TP_SCL_Pin);
}

inline static bool getsda() {
  return HAL_GPIO_ReadPin(TP_SDA_GPIO_Port, TP_SDA_Pin);
}

inline static void setsda(bool state) {
  HAL_GPIO_WritePin(TP_SDA_GPIO_Port, TP_SDA_Pin, state);
}

inline static void setscl(bool state) {
  HAL_GPIO_WritePin(TP_SCL_GPIO_Port, TP_SCL_Pin, state);
}

inline static void setsdamode(bool mode) {
#if 0
  GPIO_InitTypeDef gpio = {.Mode = mode ? GPIO_MODE_OUTPUT_OD :
  GPIO_MODE_INPUT,
                           .Pull = mode ? GPIO_PULLUP : GPIO_NOPULL,
                           .Speed = GPIO_SPEED_FREQ_HIGH,
                           .Pin = TP_SDA_Pin};
  HAL_GPIO_Init(TP_SDA_GPIO_Port, &gpio);
#endif
}

static gpio_i2c i2c = {.delay = 5,
                .delay_us = u2hts_delay_us,
                .getscl = getscl,
                .getsda = getsda,
                .setscl = setscl,
                .setsda = setsda,
                .setsdamode = setsdamode};

inline bool u2hts_i2c_write(uint8_t slave_addr, void* buf, size_t len, bool stop) {
  return gpio_i2c_write(&i2c, slave_addr, buf, len, stop);
}

inline bool u2hts_i2c_read(uint8_t slave_addr, void* buf, size_t len) {
  return gpio_i2c_read(&i2c, slave_addr, buf, len);
}

inline bool u2hts_i2c_detect_slave(uint8_t addr) {
  return gpio_i2c_detect_slave(&i2c, addr);
}

// gpio bitbanging i2c
inline void u2hts_i2c_init(uint32_t bus_speed) { UNUSED(bus_speed); }
inline void u2hts_i2c_set_speed(uint32_t speed_hz) { UNUSED(speed_hz); }
// not implemented
inline void u2hts_spi_init(bool cpol, bool cpha, uint32_t speed) {}
inline bool u2hts_spi_transfer(void* buf, size_t len) { return false; }

inline void u2hts_delay_ms(uint32_t ms) { HAL_Delay(ms); }
inline void u2hts_delay_us(uint32_t us) {
  __IO uint32_t currentTicks = SysTick->VAL;
  /* Number of ticks per millisecond */
  const uint32_t tickPerMs = SysTick->LOAD + 1;
  /* Number of ticks to count */
  const uint32_t nbTicks = ((us - ((us > 0) ? 1 : 0)) * tickPerMs) / 1000;
  /* Number of elapsed ticks */
  uint32_t elapsedTicks = 0;
  __IO uint32_t oldTicks = currentTicks;
  do {
    currentTicks = SysTick->VAL;
    elapsedTicks += (oldTicks < currentTicks)
                        ? tickPerMs + oldTicks - currentTicks
                        : oldTicks - currentTicks;
    oldTicks = currentTicks;
  } while (nbTicks > elapsedTicks);
}

inline void u2hts_tpint_set(bool value) {
  HAL_GPIO_WritePin(TP_INT_GPIO_Port, TP_INT_Pin, value);
}

inline void u2hts_tprst_set(bool value) {
  HAL_GPIO_WritePin(TP_RST_GPIO_Port, TP_RST_Pin, value);
}

inline bool u2hts_usb_init() {
  MX_USB_DEVICE_Init();
  return true;
}
inline uint16_t u2hts_get_timestamp() { return (uint16_t)HAL_GetTick(); }

inline void u2hts_led_set(bool on) {
  // low level is on
  HAL_GPIO_WritePin(USR_LED_GPIO_Port, USR_LED_Pin, !on);
}

#define U2HTS_CONFIG_STORAGE_OFFSET FLASH_BANK1_END - FLASH_PAGE_SIZE + 1 // 0x08007C00UL

inline void u2hts_write_config(uint16_t cfg) {
  HAL_FLASH_Unlock();
  uint32_t e = 0;
  FLASH_EraseInitTypeDef erase = {.NbPages = 1,
                                  .PageAddress = U2HTS_CONFIG_STORAGE_OFFSET,
                                  .TypeErase = FLASH_TYPEERASE_PAGES};
  HAL_FLASHEx_Erase(&erase, &e);
  HAL_FLASH_Program(FLASH_TYPEPROGRAM_HALFWORD, U2HTS_CONFIG_STORAGE_OFFSET,
                    cfg);
  HAL_FLASH_Lock();
}

inline uint16_t u2hts_read_config() {
  return *(uint16_t*)U2HTS_CONFIG_STORAGE_OFFSET;
}

inline void u2hts_ts_irq_set(bool enable) {
  enable ? HAL_NVIC_EnableIRQ(TP_INT_EXTI_IRQn)
         : HAL_NVIC_DisableIRQ(TP_INT_EXTI_IRQn);
}

inline bool u2hts_usrkey_get() {
  // default low
  return HAL_GPIO_ReadPin(USR_KEY_GPIO_Port, USR_KEY_Pin);
}

inline void u2hts_ts_irq_init(U2HTS_IRQ_TYPES irq_flag) {
  HAL_GPIO_DeInit(TP_INT_GPIO_Port, TP_INT_Pin);
  uint32_t real_irq_flag = 0x00;
  switch (irq_flag) {
    case IRQ_TYPE_EDGE_RISING:
    case IRQ_TYPE_LEVEL_HIGH:
      real_irq_flag = GPIO_MODE_IT_RISING;
      break;
    case IRQ_TYPE_EDGE_FALLING:
    case IRQ_TYPE_LEVEL_LOW:
    default:
      real_irq_flag = GPIO_MODE_IT_FALLING;
      break;
  }
  GPIO_InitTypeDef gpio = {.Mode = real_irq_flag,
                           .Pin = TP_INT_Pin,
                           .Pull = (real_irq_flag == GPIO_MODE_IT_RISING)
                                       ? GPIO_PULLDOWN
                                       : GPIO_PULLUP};
  HAL_GPIO_Init(TP_INT_GPIO_Port, &gpio);
}

inline void u2hts_usb_report(uint8_t report_id, const u2hts_hid_report* report) {
  static uint8_t report_buf[sizeof(u2hts_hid_report) + 1];
  report_buf[0] = report_id;
  memcpy(report_buf + 1, report, sizeof(u2hts_hid_report));
  USBD_CUSTOM_HID_SendReport(&hUsbDeviceFS, report_buf, sizeof(report_buf));
}

inline bool u2hts_get_usb_status() {
  return (((USBD_CUSTOM_HID_HandleTypeDef*)hUsbDeviceFS.pClassData)->state ==
          CUSTOM_HID_IDLE);
}

inline void u2hts_tpint_set_mode(bool mode, bool pull) {
  GPIO_InitTypeDef gpio = {.Pin = TP_INT_Pin,
                           .Mode = mode ? GPIO_MODE_OUTPUT_PP : GPIO_MODE_INPUT,
                           .Pull = pull ? GPIO_PULLUP : GPIO_PULLDOWN,
                           .Speed = GPIO_SPEED_FREQ_MEDIUM};
  HAL_GPIO_Init(TP_INT_GPIO_Port, &gpio);
}

inline bool u2hts_tpint_get() {
  return HAL_GPIO_ReadPin(TP_INT_GPIO_Port, TP_INT_Pin);
}

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin) {
  u2hts_ts_irq_status_set((GPIO_Pin == TP_INT_Pin));
}