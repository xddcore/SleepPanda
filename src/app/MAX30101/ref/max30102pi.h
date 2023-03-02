/***********************************************************************
 * \file max30102pi.cpp                                                *
 *                                                                     *
 *                                                                     *
 * Project:     MAXREFDES117#pi                                        *
 * Filename:    max30102pi.h                                           *
 * References:    Code adapted/stolen from:                            *
 *     Maxim:                                                          *
 *         https://github.com/MaximIntegratedRefDesTeam/RD117_ARDUINO  *
 *     Robert Fraczkiewicz                                             *
 *         https://github.com/aromring/MAX30102_by_RF/blob/master/ )   *
 *     pigpio c library ADXL345 example code at:                       *
 *         http://abyz.me.uk/rpi/pigpio/code/adxl345_c.zip             *
 *                                                                     *
 * Description: Raspberry Pi code file for MAX30102 "driver".          *
 *                                                                     *
 * Revision History:                                                   *
 *   01-18-2016 Rev 01.00 GL Initial release.                          *
 *   12-22-2017 Rev 02.00 Greatly mod'd by Robert Fraczkiewicz (RF)    *
 *                          to use Wire lib vs. MAXIM's SoftI2C lib.   *
 *   06/11/2020 Rev 03.00 Ported to Raspberry Pi using "pigpio" lib.   *
 ***********************************************************************/

#ifndef MAX30102PI_H_
#define MAX30102PI_H_

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <linux/i2c-dev.h>
#include <thread>
#include <chrono>

#define MAX30102PI_I2C_ADDR 0x57

//register addresses
#define REG_INTR_STATUS_1 0x00
#define REG_INTR_STATUS_2 0x01
#define REG_INTR_ENABLE_1 0x02
#define REG_INTR_ENABLE_2 0x03
#define REG_FIFO_WR_PTR 0x04
#define REG_OVF_COUNTER 0x05
#define REG_FIFO_RD_PTR 0x06
#define REG_FIFO_DATA 0x07
#define REG_FIFO_CONFIG 0x08
#define REG_MODE_CONFIG 0x09
#define REG_SPO2_CONFIG 0x0A
#define REG_LED1_PA 0x0C
#define REG_LED2_PA 0x0D
#define REG_PILOT_PA 0x10
#define REG_MULTI_LED_CTRL1 0x11
#define REG_MULTI_LED_CTRL2 0x12
#define REG_TEMP_INTR 0x1F
#define REG_TEMP_FRAC 0x20
#define REG_TEMP_CONFIG 0x21
#define REG_PROX_INT_THRESH 0x30
#define REG_REV_ID 0xFE
#define REG_PART_ID 0xFF

#define I2CBUS 1
#define INTPIN 4

int max30102pi_initDevice(int fd);
int max30102pi_readFromFIFO(uint32_t *pun_red_led, uint32_t *pun_ir_led);
int max30102pi_dumpRegs(int fd, unsigned char *byteArray);

#endif /*  MAX30102PI_H_ */
