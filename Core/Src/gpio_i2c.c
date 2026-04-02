#include "gpio_i2c.h"
static void gpio_i2c_start(gpio_i2c* i2c) {
  i2c->setsda(1);
  i2c->setscl(1);
  i2c->delay_us(i2c->delay / 2);
  i2c->setsda(0);
  i2c->delay_us(i2c->delay / 2);
  i2c->setscl(0);
}

static void gpio_i2c_stop(gpio_i2c* i2c) {
  i2c->setsda(0);
  i2c->delay_us(i2c->delay / 2);
  i2c->setscl(1);
  i2c->delay_us(i2c->delay / 2);
  i2c->setsda(1);
  i2c->delay_us(i2c->delay);
}

static bool gpio_i2c_scl_high_wait(gpio_i2c* i2c) {
  uint16_t timeout = 0xFFFF;
  while (!i2c->getscl())
    if (--timeout == 0) return false;
  return true;
}

// true: ack false: nack
static void gpio_i2c_ack(gpio_i2c* i2c, bool ack) {
  i2c->setscl(0);
  i2c->setsda(!ack);
  i2c->delay_us(i2c->delay);
  i2c->setscl(1);
  gpio_i2c_scl_high_wait(i2c);
  i2c->delay_us(i2c->delay / 2);
  i2c->setscl(0);
  i2c->setsda(1);
}

static bool gpio_i2c_wait_ack(gpio_i2c* i2c) {
  i2c->setsda(1);
  i2c->delay_us(i2c->delay / 2);
  i2c->setscl(1);
  i2c->delay_us(i2c->delay / 2);
  uint8_t timeout = 0;
  i2c->setsdamode(false);
  while (i2c->getsda()) {
    timeout++;
    if (timeout > 254) {
      gpio_i2c_stop(i2c);
      return false;
    }
    i2c->delay_us(1);
  }
  i2c->setsdamode(true);
  i2c->delay_us(i2c->delay);
  i2c->setscl(0);
  return true;
}

static bool gpio_i2c_write_byte(gpio_i2c* i2c, uint8_t byte) {
  for (uint8_t i = 0; i < 8; i++) {
    i2c->setsda((byte & 0x80) ? 1 : 0);
    byte <<= 1;
    i2c->delay_us(i2c->delay / 2);
    i2c->setscl(1);
    i2c->delay_us(i2c->delay / 2);
    if (!gpio_i2c_scl_high_wait(i2c)) return false;
    i2c->delay_us(i2c->delay);
    i2c->setscl(0);
    i2c->delay_us(i2c->delay / 2);
  }
  i2c->delay_us(i2c->delay);
  return true;
}

static bool gpio_i2c_read_byte(gpio_i2c* i2c, uint8_t* byte, bool ack) {
  i2c->setsdamode(false);
  for (uint8_t i = 0; i < 8; i++) {
    i2c->setscl(0);
    i2c->delay_us(i2c->delay / 2);
    i2c->setscl(1);
    if (!gpio_i2c_scl_high_wait(i2c)) return false;
    i2c->delay_us(i2c->delay / 2);
    *byte <<= 1;
    if (i2c->getsda()) (*byte)++;
    i2c->delay_us(i2c->delay / 2);
    i2c->setscl(0);
  }
  i2c->setsdamode(true);
  gpio_i2c_ack(i2c, ack);
  return true;
}

bool gpio_i2c_write(gpio_i2c* i2c, uint8_t slave_addr, void* buf, size_t len,
                    bool stop) {
  uint8_t* buf_ptr = buf;
  gpio_i2c_start(i2c);
  if (!gpio_i2c_write_byte(i2c, slave_addr << 1 | 0)) goto error;
  if (!gpio_i2c_wait_ack(i2c)) goto error;

  for (uint32_t i = 0; i < len; i++) {
    if (!gpio_i2c_write_byte(i2c, buf_ptr[i])) goto error;
    if (!gpio_i2c_wait_ack(i2c)) goto error;
  }
  if (stop) gpio_i2c_stop(i2c);
  return true;
error:
  gpio_i2c_stop(i2c);
  return false;
}

bool gpio_i2c_read(gpio_i2c* i2c, uint8_t slave_addr, void* buf, size_t len) {
  uint8_t* buf_ptr = buf;
  gpio_i2c_start(i2c);
  if (!gpio_i2c_write_byte(i2c, slave_addr << 1 | 1)) goto error;
  if (!gpio_i2c_wait_ack(i2c)) goto error;
  for (uint32_t i = 0; i < len; i++)
    if (!gpio_i2c_read_byte(i2c, buf_ptr + i, (i != len - 1))) goto error;
  gpio_i2c_stop(i2c);
  return true;
error:
  gpio_i2c_stop(i2c);
  return false;
}

void gpio_i2c_reset_bus(gpio_i2c* i2c) {
  i2c->setsda(1);
  i2c->setscl(1);
  i2c->delay_us(i2c->delay);
  for (uint8_t i = 0; i < 9; i++) {
    i2c->setscl(0);
    i2c->delay_us(i2c->delay);
    i2c->setscl(1);
    i2c->delay_us(i2c->delay);
  }
  gpio_i2c_stop(i2c);
}

bool gpio_i2c_detect_slave(gpio_i2c* i2c, uint8_t addr) {
  gpio_i2c_start(i2c);
  if (!gpio_i2c_write_byte(i2c, addr << 1)) goto error;

  i2c->setsda(true);
  i2c->delay_us(i2c->delay / 2);

  if (gpio_i2c_scl_high_wait(i2c)) goto error;

  i2c->delay_us(i2c->delay / 2);
  i2c->setsdamode(false);
  bool ack = !i2c->getsda();
  i2c->setsdamode(true);
  i2c->delay_us(i2c->delay);
  i2c->setscl(false);
  gpio_i2c_stop(i2c);
  return ack;
error:
  gpio_i2c_stop(i2c);
  return false;
}