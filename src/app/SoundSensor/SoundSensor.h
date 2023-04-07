/*
 * @Author: Yihan Wang yihanwangg@163.com
 * @Date: 2023-02-13 10:03:21
 * @LastEditors: Chengsen Dong 1034029664@qq.com
 * @LastEditTime: 2023-04-07 19:28:56
 * @FilePath: /SleepPanda/src/app/SoundSensor/SoundSensor.h
 * @Description: 
 * Copyright (c) 2023 by ${git_name_email}(www.github.com/xddcore), All Rights Reserved. 
 */

#ifndef _SoundSensor_H
#define _SoundSensor_H

#include <stdint.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <pigpio.h>
#include <assert.h>

#include "../Config/Config.h"

struct SoundSensor_Settings {
    unsigned int SoundSensor_GPIO_Pin = 5; //GPIO5(PIN29) will be used
    unsigned int Trigger_Method = RISING_EDGE; //when sound appears
    unsigned int ISR_TIMEOUT = -1; //一直等待声音传感器的触发信号(<0)

    /**
	 * If set to true the pigpio will be initialised
	 **/
	bool initPIGPIO = true;
};

class SoundSensor{
    public:
    SoundSensor(SoundSensor_Settings My_SoundSensor_Settings);
    ~SoundSensor();
    void Start();
    void Stop();
    virtual void SoundSensor_SoundEvent_Handle()=0; //纯虚函数(规范回调接口)

    private:
    SoundSensor_Settings My_SoundSensor_Settings;
    void Sound_Event();
	static void gpioISR(int, int, uint32_t, void* userdata) {
		((SoundSensor*)userdata)->Sound_Event();
	}
};

#endif //_SoundSensor_H