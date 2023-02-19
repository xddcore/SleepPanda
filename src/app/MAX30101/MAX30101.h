/*
 * @Author: Chengsen Dong 1034029664@qq.com
 * @Date: 2023-02-19 15:55:46
 * @LastEditors: Chengsen Dong 1034029664@qq.com
 * @LastEditTime: 2023-02-19 17:08:09
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

#define MAX30101_IIC_Address 0x57

struct MAX30101_Settings {
    unsigned int MAX30101_Interrupt_GPIO_Pin = 4; //GPIO4(PIN7) will be used to connect with MAX30101 INT Pin
    unsigned int Trigger_Method;
    unsigned int ISR_TIMEOUT = -1; //一直等待声音传感器的触发信号(<0)

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
    virtual void MAX30101_DataReadyEvent_Handle()=0; //纯虚函数(规范回调接口)

    private:
    MAX30101_Settings My_MAX30101_Settings;
    void DataReady_Event();
	static void gpioISR(int, int, uint32_t, void* userdata) {
		((MAX30101*)userdata)->DataReady_Event();
	}
};

#endif //_MAX30101_H