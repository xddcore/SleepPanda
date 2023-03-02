/*
 * @Author: Chengsen Dong 1034029664@qq.com
 * @Date: 2023-02-19 15:55:20
 * @LastEditors: Chengsen Dong 1034029664@qq.com
 * @LastEditTime: 2023-03-02 09:21:28
 * @FilePath: /SleepPanda/src/app/MAX30101/MAX30101.cpp
 * @Description: 
 * Copyright (c) 2023 by ${git_name_email}(www.github.com/xddcore), All Rights Reserved. 
 */
#include "MAX30101.h"

//MAX30101构造函数
MAX30101::MAX30101(MAX30101_Settings max30101_settings) {
#if(RPI_DEBUG==1)
    if (max30101_settings.initPIGPIO) {
        int cfg = gpioCfgGetInternals();
        cfg |= PI_CFG_NOSIGHANDLER;
        gpioCfgSetInternals(cfg);
        int err;
        My_MAX30101_Settings = max30101_settings;
        err = gpioInitialise();    
        if (err < 0)
        {
            printf("RPI DEBUG: MAX30101 pigpio initialisation failed.\r\n");
            // pigpio initialisation failed.
        }
        else
        {
            printf("RPI DEBUG: MAX30101 pigpio initialised okay.\r\n");
            // pigpio initialised okay.
	        // set gpio4(pin7) mode to input
            gpioSetMode(My_MAX30101_Settings.MAX30101_Interrupt_GPIO_Pin,PI_INPUT);
            //Must connect pull-up to INT Pin
            gpioSetPullUpDown(My_MAX30101_Settings.MAX30101_Interrupt_GPIO_Pin, PI_PUD_UP);
            //GPIO Interrupt
            //gpioSetISRFuncEx(My_MAX30101_Settings.MAX30101_Interrupt_GPIO_Pin,My_MAX30101_Settings.Trigger_Method,My_MAX30101_Settings.ISR_TIMEOUT,gpioISR,(void*)this);
            //MAX30101 Init below
            i2cHndl = Device_Init(My_MAX30101_Settings);
            if (i2cHndl == -1)
            {
                printf("RPI DEBUG: MAX30101 Device Init failed.\r\n");
                exit(1);
            }
            else
            {
                printf("RPI DEBUG: MAX30101 Device Init ok.\r\n");
            }
            DumpRegs(i2cHndl, regDumpArray);
            sleep(1);
        }
    }
    //此处添加MAX30101的初始化代码
#else
    printf("LOGIC DEBUG: MAX30101 Init.\r\n");
#endif
}

//MAX30101析构函数
MAX30101::~MAX30101() {
#if(RPI_DEBUG==1)
    //此处添加MAX30101 Interrupt GPIO资源释放代码(程序执行结束后自动释放)
    DumpRegs(i2cHndl, regDumpArray);
    i2cWriteByteData(i2cHndl, REG_MODE_CONFIG,0x40);    // Reset MAX30102.
    sleep(1);                                           // Delay 1 sec.
    i2cClose(i2cHndl);
    gpioSetISRFuncEx(My_MAX30101_Settings.MAX30101_Interrupt_GPIO_Pin,RISING_EDGE,-1,NULL,(void*)this);
    gpioTerminate();
    printf("RPI DEBUG: MAX30101 Delete.\r\n");
#else
    printf("LOGIC DEBUG: MAX30101 Delete.\r\n");
#endif
}

//MAX30101_DataReady事件函数
void MAX30101::DataReady_Event() {
#if(RPI_DEBUG==1)
    //printf("RPI DEBUG: MAX30101 DataReady was triggered.\r\n");
    if(BUFFER_SIZE_i<BUFFER_SIZE){
        i2cReadByteData(i2cHndl, REG_INTR_STATUS_1);    // Clear ISR1.
        Read_FIFO(i2cHndl, (aun_red_buffer+BUFFER_SIZE_i), (aun_ir_buffer+BUFFER_SIZE_i));  // Read from MAX30102 FIFO.
        BUFFER_SIZE_i++;
        //printf("RPI DEBUG: MAX30101 Read_FIFO was runed.|BUFFER_SIZE_i=%d\r\n",BUFFER_SIZE_i);
    }
    else
    {
        BUFFER_SIZE_i = 0;
        rf_heart_rate_and_oxygen_saturation(aun_ir_buffer, BUFFER_SIZE, aun_red_buffer,
                                            &n_spo2, &ch_spo2_valid, &n_heart_rate,
                                            &ch_hr_valid, &ratio, &correl); 
        /*
        printf("RF: SpO2: %lf\n", n_spo2);
        printf("    HR:   %d\n", n_heart_rate);
        printf("---------------\n");
        */
        maxim_heart_rate_and_oxygen_saturation(aun_ir_buffer, BUFFER_SIZE, aun_red_buffer,
                                               &n_spo2_maxim, &ch_spo2_valid_maxim,
					                           &n_heart_rate_maxim, &ch_hr_valid_maxim);
        /*
        printf("MX: SpO2: %lf\n", n_spo2_maxim);
        printf("    HR:   %d\n", n_heart_rate_maxim);
        printf("---------------\n");
        */
        //call back function
        MAX30101_DataReadyEvent_Handle(n_heart_rate,n_spo2);
        i2cReadByteData(i2cHndl, REG_INTR_STATUS_1);    // Clear ISR1.
    }
#else
    printf("LOGIC DEBUG: MAX30101 DataReady was triggered.\r\n");
#endif
}

//MAX30101器件初始化
int MAX30101::Device_Init(MAX30101_Settings My_MAX30101_Settings){
  int i2cDevHandle;
  
  i2cSwitchCombined(My_MAX30101_Settings.i2c_bus);  // This sets the I2C (i2c-bcm2708) module
                              // "use combined transactions" parameter on or off.
                              // Setting: 0 -> parameter off, non-zero -> on.
                              // NOTE: when the flag is on a write followed by
                              //       a read to the same slave address will use
                              //       a repeated start (rather than a stop/start).
  
  if ((i2cDevHandle = i2cOpen(My_MAX30101_Settings.i2c_bus, My_MAX30101_Settings.Address, 0)) < 0)
  {
    printf("Device_Init: Failure to i2cOpen MAX30101!  Handle = %d\n", i2cDevHandle);
    return -1;
  }
  else
  {
    printf("Device_Init: MAX30101 i2cOpen Succeeded!  fd = %d\n", i2cDevHandle);
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

  i2cReadByteData(i2cDevHandle, REG_INTR_STATUS_1);  // clear ISR1.
  i2cReadByteData(i2cDevHandle, REG_INTR_STATUS_2);  // clear ISR2.

  return i2cDevHandle;
}

//读取MAX30101的20个bytes寄存器
int MAX30101::DumpRegs(int fd, int *dumpArray){
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

int MAX30101::Read_FIFO(int fd, uint32_t *pun_red_led, uint32_t *pun_ir_led)
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

  return 0;
}  

//MAX30101开始读取数据
void MAX30101::Start(){
    gpioSetISRFuncEx(My_MAX30101_Settings.MAX30101_Interrupt_GPIO_Pin,My_MAX30101_Settings.Trigger_Method,My_MAX30101_Settings.ISR_TIMEOUT,gpioISR,(void*)this);
    i2cReadByteData(i2cHndl, REG_INTR_STATUS_1);  // Clear ISR1.
}

//MAX30101停止读取数据
void MAX30101::Stop(){
    gpioSetISRFuncEx(My_MAX30101_Settings.MAX30101_Interrupt_GPIO_Pin,RISING_EDGE,-1,NULL,(void*)this);
}

/*--------------------------以下为DEBUG方法--------------------------------*/
//读取MAX30101的20个寄存器
int MAX30101::WriteRegsToLog(){
  FILE *ofpR;
  ofpR = fopen("oxiRegs.dat", "w");
  fprintf(ofpR, "----Begin MAX3010x Regs------------\n");
  for(int i = 0; i < DUMPREGSSIZE; i++)
  {
    fprintf(ofpR, "%02X\n", regDumpArray[i]);
  }
  fprintf(ofpR, "----End   MAX3010x Regs------------\n");
  fclose(ofpR);
  return 0;
}
//清除MAX30101的INT flag
void MAX30101::ClearINT(){
    i2cReadByteData(i2cHndl, REG_INTR_STATUS_1);                           // Clear ISR1.
    i2cReadByteData(i2cHndl, REG_INTR_STATUS_2);                           // Clear ISR2.
}