#ifndef _GPIO_I2C_H_
#define _GPIO_I2C_H_
#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>

typedef struct {
  uint8_t delay;
  void (*delay_us)(uint32_t us);
  void (*setsda)(bool state);
  void (*setscl)(bool state);
  bool (*getsda)(void);
  bool (*getscl)(void);
  // true = out, false = in
  void (*setsdamode)(bool mode);
} gpio_i2c;

bool gpio_i2c_write(gpio_i2c* i2c, uint8_t slave_addr, void* buf, size_t len,
                    bool stop);
bool gpio_i2c_read(gpio_i2c* i2c, uint8_t slave_addr, void* buf, size_t len);
bool gpio_i2c_detect_slave(gpio_i2c* i2c, uint8_t addr);
#endif