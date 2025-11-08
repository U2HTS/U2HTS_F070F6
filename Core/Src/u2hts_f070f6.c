/*
  Copyright (C) CNflysky.
  U2HTS stands for "USB to HID TouchScreen".
  This file is licensed under GPL V3.
  All rights reserved.
*/

#include "u2hts_f070f6.h"

#include "u2hts_core.h"

extern USBD_HandleTypeDef hUsbDeviceFS;

#define I2C_SDA(x) HAL_GPIO_WritePin(TP_SDA_GPIO_Port, TP_SDA_Pin, x)
#define I2C_SCL(x) HAL_GPIO_WritePin(TP_SCL_NSS_GPIO_Port, TP_SCL_NSS_Pin, x)

#define I2C_SDA_R HAL_GPIO_ReadPin(TP_SDA_GPIO_Port, TP_SDA_Pin)
#define I2C_SCL_R HAL_GPIO_ReadPin(TP_SCL_NSS_GPIO_Port, TP_SCL_NSS_Pin)

#define I2C_SDA_I                                                 \
  GPIO_InitTypeDef sda_gpio_in = {.Pin = TP_SDA_Pin,              \
                                  .Mode = GPIO_MODE_INPUT,        \
                                  .Speed = GPIO_SPEED_FREQ_HIGH}; \
  HAL_GPIO_Init(TP_SDA_GPIO_Port, &sda_gpio_in);

#define I2C_SDA_O                                                  \
  GPIO_InitTypeDef sda_gpio_out = {.Pin = TP_SDA_Pin,              \
                                   .Mode = GPIO_MODE_OUTPUT_OD,    \
                                   .Pull = GPIO_NOPULL,            \
                                   .Speed = GPIO_SPEED_FREQ_HIGH}; \
  HAL_GPIO_Init(TP_SDA_GPIO_Port, &sda_gpio_out);

#define I2C_DELAY 5

static void i2c_start() {
  I2C_SDA(1);
  I2C_SCL(1);
  u2hts_delay_us(I2C_DELAY / 2);
  I2C_SDA(0);
  u2hts_delay_us(I2C_DELAY / 2);
  I2C_SCL(0);
}

void i2c_stop() {
  I2C_SDA(0);
  u2hts_delay_us(I2C_DELAY / 2);
  I2C_SCL(1);
  u2hts_delay_us(I2C_DELAY / 2);
  I2C_SDA(1);
  u2hts_delay_us(I2C_DELAY);
}

static void i2c_scl_high_wait(void) {
  I2C_SCL(1);
  u2hts_delay_us(I2C_DELAY / 2);
  uint16_t timeout = 0xFFFF;
  while (!I2C_SCL_R)
    if (--timeout == 0) break;
}

// true: ack false: nack
static void i2c_ack(bool ack) {
  I2C_SCL(0);
  I2C_SDA(!ack);
  u2hts_delay_us(I2C_DELAY);
  i2c_scl_high_wait();
  I2C_SCL(0);
  I2C_SDA(1);
}

static bool i2c_wait_ack() {
  I2C_SDA(1);
  u2hts_delay_us(I2C_DELAY / 2);
  I2C_SCL(1);
  u2hts_delay_us(I2C_DELAY / 2);
  uint8_t timeout = 0;
  while (I2C_SDA_R) {
    timeout++;
    if (timeout > 254) {
      i2c_stop();
      return false;
    }
    u2hts_delay_us(1);
  }
  u2hts_delay_us(I2C_DELAY);
  I2C_SCL(0);
  return true;
}

static void i2c_write_byte(uint8_t byte) {
  for (uint8_t i = 0; i < 8; i++) {
    I2C_SCL(0);
    I2C_SDA((byte & 0x80) ? 1 : 0);
    byte <<= 1;
    u2hts_delay_us(I2C_DELAY);
    i2c_scl_high_wait();
    u2hts_delay_us(I2C_DELAY);
    I2C_SCL(0);
  }
  u2hts_delay_us(I2C_DELAY);
}

static uint8_t i2c_read_byte(bool ack) {
  uint8_t buf = 0x00;
  I2C_SDA_I;
  for (uint8_t i = 0; i < 8; i++) {
    I2C_SCL(0);
    u2hts_delay_us(I2C_DELAY);
    i2c_scl_high_wait();
    buf <<= 1;
    if (I2C_SDA_R) buf++;
    u2hts_delay_us(I2C_DELAY);
    I2C_SCL(0);
  }
  I2C_SDA_O;
  i2c_ack(ack);
  return buf;
}

bool u2hts_i2c_write(uint8_t slave_addr, void* buf, size_t len, bool stop) {
  uint8_t* buf_ptr = buf;
  bool ret = false;
  i2c_start();
  i2c_write_byte(slave_addr << 1 | 0);
  ret = i2c_wait_ack();
  if (!ret) return ret;
  for (uint32_t i = 0; i < len; i++) {
    i2c_write_byte(buf_ptr[i]);
    ret = i2c_wait_ack();
    if (!ret) return ret;
  }
  if (stop) i2c_stop();
  return ret;
}

bool u2hts_i2c_read(uint8_t slave_addr, void* buf, size_t len) {
  uint8_t* buf_ptr = buf;
  bool ret = false;
  i2c_start();
  i2c_write_byte((slave_addr << 1) | 1);
  ret = i2c_wait_ack();
  if (!ret) return ret;
  for (uint32_t i = 0; i < len; i++) buf_ptr[i] = i2c_read_byte((i != len - 1));

  i2c_stop();
  return ret;
}

static void i2c_reset(void) {
  I2C_SDA(1);
  I2C_SCL(1);
  u2hts_delay_us(I2C_DELAY);
  for (uint8_t i = 0; i < 9; i++) {
    I2C_SCL(0);
    u2hts_delay_us(I2C_DELAY);
    I2C_SCL(1);
    u2hts_delay_us(I2C_DELAY);
  }

  i2c_stop();
}

bool u2hts_i2c_detect_slave(uint8_t addr) {
  i2c_stop();  // 确保总线空闲
  u2hts_delay_us(200);

  i2c_reset();

  i2c_start();
  i2c_write_byte(addr << 1);

  I2C_SDA(1);
  u2hts_delay_us(I2C_DELAY);
  I2C_SCL(1);
  u2hts_delay_us(I2C_DELAY);

  bool ack = !I2C_SDA_R;

  u2hts_delay_us(I2C_DELAY);
  I2C_SCL(0);
  i2c_stop();
  u2hts_delay_us(200);

  return ack;
}

inline void u2hts_ts_irq_setup(uint8_t irq_flag) {
  HAL_GPIO_DeInit(TP_INT_GPIO_Port, TP_INT_Pin);
  uint32_t real_irq_flag = 0x00;
  switch (irq_flag) {
    case U2HTS_IRQ_TYPE_HIGH:
    case U2HTS_IRQ_TYPE_RISING:
      real_irq_flag = GPIO_MODE_IT_RISING;
      break;
    case U2HTS_IRQ_TYPE_FALLING:
    case U2HTS_IRQ_TYPE_LOW:
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

inline void u2hts_usb_report(void* report, uint8_t report_id) {
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

#ifndef U2HTS_POLLING
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin) {
  u2hts_ts_irq_status_set((GPIO_Pin == TP_INT_Pin));
}
#endif