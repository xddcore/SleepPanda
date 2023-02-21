/*
 * @Author: Chengsen Dong 1034029664@qq.com
 * @Date: 2023-02-19 15:55:46
 * @LastEditors: Chengsen Dong 1034029664@qq.com
 * @LastEditTime: 2023-02-21 20:38:11
 * @FilePath: /SleepPanda/src/app/MAX30101/MAX30101.h
 * @Description: 
 * Copyright (c) 2023 by ${git_name_email}(www.github.com/xddcore), All Rights Reserved. 
 */

#ifndef _MAX30101_H
#define _MAX30101_H

#include <stdint.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <pigpio.h>
#include <assert.h>

#include "../Config/Config.h"

#include "algorithm2.h"
#include "algorithm2_by_RF.h"

#define DUMPREGSSIZE 20

// IIC Address(7bit)
#define MAX30101_IIC_Address 0x57

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

struct MAX30101_Settings {
    unsigned int MAX30101_Interrupt_GPIO_Pin = 4; //GPIO4(PIN7) will be used to connect with MAX30101 INT Pin
    unsigned int Trigger_Method = FALLING_EDGE; //Interrupt is low level
    unsigned int ISR_TIMEOUT = -1; //一直等待MAX30101的INT触发信号(<0)

    /**
	 * I2C bus used (99% always set to one)
	 **/
	int i2c_bus = 1;

	/**
	 * I2C address of the MAX30101
	 **/
	uint8_t Address = MAX30101_IIC_Address;

    /**
	 * If set to true the pigpio will be initialised
	 **/
	bool initPIGPIO = true;
};

class MAX30101{
    public:
    MAX30101(MAX30101_Settings My_MAX30101_Settings);
    ~MAX30101();
    virtual void MAX30101_DataReadyEvent_Handle(int32_t heart_rate, double spo2)=0; //纯虚函数(规范回调接口)
	void Start();
	void Stop();
	//DEBUG
	void ClearINT();
	int WriteRegsToLog();

    private:
	int i2cHndl;
	int regDumpArray[DUMPREGSSIZE];
	int BUFFER_SIZE_i = 0;
	uint32_t aun_ir_buffer[100];  // infrared LED sensor data, FS=25 BUFFER_SIZE = FS*4 = 100
	uint32_t aun_red_buffer[100];  // red LED sensor data, FS=25 BUFFER_SIZE = FS*4 = 100
	//------Sensor Data
	double n_spo2,ratio,correl;  //SPO2 value
	int8_t ch_spo2_valid;        //indicator to show if the SPO2 calculation is valid
	int32_t n_heart_rate;        //heart rate value
	int8_t  ch_hr_valid;         //indicator to show if the heart rate calculation is valid
	int32_t i;
	char hr_str[10];

	double n_spo2_maxim;         //SPO2 value
	int8_t ch_spo2_valid_maxim;  //indicator to show if the SPO2 calculation is valid
	int32_t n_heart_rate_maxim;  //heart rate value
	int8_t  ch_hr_valid_maxim;   //indicator to show if the heart rate calculation is valid
	//------Sensor Data
    MAX30101_Settings My_MAX30101_Settings;
	int Device_Init(MAX30101_Settings My_MAX30101_Settings);
	int DumpRegs(int fd, int *dumpArray);
	int Read_FIFO(int fd, uint32_t *pun_red_led, uint32_t *pun_ir_led);
    void DataReady_Event();
	static void gpioISR(int, int, uint32_t, void* userdata) {
		((MAX30101*)userdata)->DataReady_Event();
	}
};

#endif //_MAX30101_H