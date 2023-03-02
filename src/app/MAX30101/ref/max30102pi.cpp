/***********************************************************************
 * \file max30102pi.cpp                                                *
 *                                                                     *
 *                                                                     *
 * Project:     MAXREFDES117#pi                                        *
 * Filename:    max30102pi.cpp                                         *
 * References:    Code adapted/stolen from:                            *
 *     Maxim:                                                          *
 *         https://github.com/MaximIntegratedRefDesTeam/RD117_ARDUINO  *
 *     Robert Fraczkiewicz                                             *
 *         https://github.com/aromring/MAX30102_by_RF/blob/master/     *
 *     pigpio c library ADXL345 example code at:                       *
 *         http://abyz.me.uk/rpi/pigpio/code/adxl345_c.zip             *
 *                                                                     *
 * Description: Raspberry Pi header file for MAX30102 "driver".        *
 *                                                                     *
 * Revision History:                                                   *
 *   01-18-2016 Rev 01.00 GL Initial release.                          *
 *   12-22-2017 Rev 02.00 Greatly mod'd by Robert Fraczkiewicz (RF)    *
 *                          to use Wire lib vs. MAXIM's SoftI2C lib.   *
 *   06/11/2020 Rev 03.00 Ported to Raspberry Pi using "pigpio" lib.   *
 ***********************************************************************/

#include "max30102pi.h"
//#include "algorithm_by_RF.h"

int max30102pi_initDevice()
/**
* \brief        Initialize the MAX30102
* \par          Details
*               This function initializes the MAX30102
*
* \param        None
*
* \retval       pigpio i2c Device Handle on success; -1 on failure.
*/
   /* initialise MAX30102 */
{
  int i2cDevHandle;
  
  i2cSwitchCombined(I2CBUS);  // This sets the I2C (i2c-bcm2708) module
                              // "use combined transactions" parameter on or off.
                              // Setting: 0 -> parameter off, non-zero -> on.
                              // NOTE: when the flag is on a write followed by
                              //       a read to the same slave address will use
                              //       a repeated start (rather than a stop/start).
  
  if ((i2cDevHandle = i2cOpen(I2CBUS, MAX30102PI_I2C_ADDR, 0)) < 0)
  {
    printf("Failure to i2cOpen MAX30102!  Handle = %d\n", i2cDevHandle);
    return -1;
  }
  else
  {
    printf("i2cOpen Succeeded!  fd = %d\n", i2cDevHandle);
  }

  i2cWriteByteData(i2cDevHandle, REG_MODE_CONFIG,0x40);    // Reset MAX30102.
  sleep(1);                                                // Delay 1 sec.
  
  i2cWriteByteData(i2cDevHandle, REG_INTR_ENABLE_1, 0xC0); // INTR setting 1-of-2
  i2cWriteByteData(i2cDevHandle, REG_INTR_ENABLE_2, 0x00); // INTR setting 2-of-2
  i2cWriteByteData(i2cDevHandle, REG_FIFO_WR_PTR,   0x00); // FIFO_WR_PTR[4:0]
  i2cWriteByteData(i2cDevHandle, REG_OVF_COUNTER,   0x00); // OVF_COUNTER[4:0]
  i2cWriteByteData(i2cDevHandle, REG_FIFO_RD_PTR,   0x00); // FIFO_RD_PTR[4:0]
  i2cWriteByteData(i2cDevHandle, REG_FIFO_CONFIG,   0x4F); // sample avg = 4, fifo rollover=false, 
  i2cWriteByteData(i2cDevHandle, REG_MODE_CONFIG,   0x03); // 0x02 for Red only, 0x03 for SpO2 mode,
  i2cWriteByteData(i2cDevHandle, REG_SPO2_CONFIG,   0x27); // SPO2_ADC range = 4096nA,
                                                                     //     SPO2 sample rate (100 Hz),
                                                                     //     LED pulseWidth (411uS).
  
  i2cWriteByteData(i2cDevHandle, REG_LED1_PA,       0x24); // Choose value for ~ 7mA for LED1
  i2cWriteByteData(i2cDevHandle, REG_LED2_PA,       0x24); // Choose value for ~ 7mA for LED2
  i2cWriteByteData(i2cDevHandle, REG_PILOT_PA,      0x7f); // Choose value for ~ 25mA for Pilot LED

  return i2cDevHandle;
}  // end of int max30102pi_initDevice(int fd)


int max30102pi_dumpRegs(int fd, int *dumpArray)
{
  // dumpArray must be 20 bytes.
  int i = 0;

  dumpArray[i++] = i2cReadByteData(fd, REG_INTR_STATUS_1);
  dumpArray[i++] = i2cReadByteData(fd, REG_INTR_STATUS_2);
  dumpArray[i++] = i2cReadByteData(fd, REG_INTR_ENABLE_1);
  dumpArray[i++] = i2cReadByteData(fd, REG_INTR_ENABLE_2);
  dumpArray[i++] = i2cReadByteData(fd, REG_FIFO_WR_PTR);
  dumpArray[i++] = i2cReadByteData(fd, REG_OVF_COUNTER);
  // we don't read fifo data for a reg dump.
  dumpArray[i++] = i2cReadByteData(fd, REG_FIFO_RD_PTR);
  dumpArray[i++] = i2cReadByteData(fd, REG_FIFO_CONFIG);
  dumpArray[i++] = i2cReadByteData(fd, REG_MODE_CONFIG);
  dumpArray[i++] = i2cReadByteData(fd, REG_SPO2_CONFIG);
  dumpArray[i++] = i2cReadByteData(fd, REG_LED1_PA);
  dumpArray[i++] = i2cReadByteData(fd, REG_LED2_PA);
  dumpArray[i++] = i2cReadByteData(fd, REG_PILOT_PA);
  dumpArray[i++] = i2cReadByteData(fd, REG_MULTI_LED_CTRL1);
  dumpArray[i++] = i2cReadByteData(fd, REG_MULTI_LED_CTRL2);
  dumpArray[i++] = i2cReadByteData(fd, REG_TEMP_INTR);
  dumpArray[i++] = i2cReadByteData(fd, REG_TEMP_FRAC);
  dumpArray[i++] = i2cReadByteData(fd, REG_PROX_INT_THRESH);
  dumpArray[i++] = i2cReadByteData(fd, REG_REV_ID);
  dumpArray[i++] = i2cReadByteData(fd, REG_PART_ID);

  return 0;
}


int max30102pi_read_fifo(int fd, uint32_t *pun_red_led, uint32_t *pun_ir_led)
/**
 * \brief        Read a set of samples from the MAX30102 FIFO register
 * \par          Details
 *               This function reads a set of samples from the MAX30102 FIFO register
 *
 * \param[out]   *pun_red_led   - pointer that stores the red LED reading data
 * \param[out]   *pun_ir_led    - pointer that stores the IR LED reading data
 *
 * \retval       true on success
 */
{
  uint32_t un_temp;
  char buf[8];
  *pun_ir_led=0;
  *pun_red_led=0;
  i2cReadByteData(fd, REG_INTR_STATUS_1);
  i2cReadByteData(fd, REG_INTR_STATUS_2);

  
  // i2cReadBlockData((unsigned) fd, REG_FIFO_DATA, buf);
  //   Above: i2cReadBlockData: SMBUS command not supported by driver
  // i2cWriteByte((unsigned) fd, REG_FIFO_DATA);
  // i2cReadI2CBlockData((unsigned) fd, REG_FIFO_DATA, buf, 6);
  i2cReadI2CBlockData((unsigned) fd, REG_FIFO_DATA, buf, 6);

  un_temp=buf[0];
  un_temp<<=16;
  *pun_red_led+=un_temp;
  un_temp=buf[1];
  un_temp<<=8;
  *pun_red_led+=un_temp;
  un_temp=buf[2];
  *pun_red_led+=un_temp;

  un_temp=buf[3];
  un_temp<<=16;
  *pun_ir_led+=un_temp;
  un_temp=buf[4];
  un_temp<<=8;
  *pun_ir_led+=un_temp;
  un_temp=buf[5];
  *pun_ir_led+=un_temp;

  *pun_red_led&=0x0003FFFF;  //Mask off MSB [31:18], keep [17:0]
  *pun_ir_led&=0x0003FFFF;   //Mask off MSB [31:18], keep [17:0]

  /*
  buf[0] = REG_FIFO_DATA;

  if ((write(fd, buf, 1)) != 1)
  {
    // Send the register to read from
    fprintf(stderr, "Error writing to MAX30102\n");
  }
 
  if (read(fd, buf, 6) != 6)
  {
    fprintf(stderr, "Error reading from MAX30102\n");
  }
  else
  {
    un_temp=buf[0];
    un_temp<<=16;
    *pun_red_led+=un_temp;
    un_temp=buf[1];
    un_temp<<=8;
    *pun_red_led+=un_temp;
    un_temp=buf[2];
    *pun_red_led+=un_temp;

    un_temp=buf[3];
    un_temp<<=16;
    *pun_ir_led+=un_temp;
    un_temp=buf[4];
    un_temp<<=8;
    *pun_ir_led+=un_temp;
    un_temp=buf[5];
    *pun_ir_led+=un_temp;

    *pun_red_led&=0x0003FFFF;  //Mask off MSB [31:18], keep [17:0]
    *pun_ir_led&=0x0003FFFF;   //Mask off MSB [31:18], keep [17:0]
  }
  */
  
  return 0;
}  // end of int max30102pi_read_fifo(uint32_t *pun_red_led, uint32_t *pun_ir_led, int *tckVal)

