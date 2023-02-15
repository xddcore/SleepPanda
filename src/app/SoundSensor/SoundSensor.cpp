/*
 * @Author: Yihan Wang yihanwangg@163.com
 * @Date: 2023-02-13 10:03:05
 * @LastEditors: Yihan Wang yihanwangg@163.com
 * @LastEditTime: 2023-02-15 20:58:37
 * @FilePath: /SleepPanda/src/app/SoundSensor/SoundSensor.cpp
 * @Description: 
 * Copyright (c) 2023 by ${git_name_email}(www.github.com/xddcore), All Rights Reserved. 
 */
#include <iostream>
#include "stdio.h"

#include "SoundSensor.h"
#include "../Config/Config.h"
#include <pigpio.h>

#define UNIT_TEST 1

//声音传感器构造函数
SoundSensor::SoundSensor(SoundSensor_Settings soundsensor_settings) {
#if(RPI_DEBUG==1)
    if (soundsensor_settings.initPIGPIO) {
        int cfg = gpioCfgGetInternals();
        cfg |= PI_CFG_NOSIGHANDLER;
        gpioCfgSetInternals(cfg);
        int err;
        My_SoundSensor_Settings = soundsensor_settings;
        err = gpioInitialise();    
        if (err < 0)
        {
            printf("RPI DEBUG: SoundSensor pigpio initialisation failed.\r\n");
            // pigpio initialisation failed.
        }
        else
        {
            printf("RPI DEBUG: SoundSensor pigpio initialised okay.\r\n");
            // pigpio initialised okay.
	        // set gpio5 mode to input
            gpioSetMode(My_SoundSensor_Settings.SoundSensor_GPIO_Pin,PI_INPUT);
            gpioSetISRFuncEx(My_SoundSensor_Settings.SoundSensor_GPIO_Pin,My_SoundSensor_Settings.Trigger_Method,My_SoundSensor_Settings.ISR_TIMEOUT,gpioISR,(void*)this);
        }
    }
    //此处添加声音传感器的初始化代码
#else
    printf("LOGIC DEBUG: SoundSensor Init.\r\n");
#endif
}

//声音传感器析构函数
SoundSensor::~SoundSensor() {
#if(RPI_DEBUG==1)
    //此处添加声音传感器GPIO资源释放代码(程序执行结束后自动释放)
    gpioSetISRFuncEx(My_SoundSensor_Settings.SoundSensor_GPIO_Pin,RISING_EDGE,-1,NULL,(void*)this);
    gpioTerminate();
    printf("RPI DEBUG: SoundSensor Delete.\r\n");
#else
    printf("LOGIC DEBUG: SoundSensor Delete.\r\n");
#endif
}

//声音传感器_声音事件函数
void SoundSensor::Sound_Event() {
#if(RPI_DEBUG==1)
    //此处添加声音传感器GPIO资源释放代码(程序执行结束后自动释放)
    SoundSensor_SoundEvent_Handle();
    printf("RPI DEBUG: SoundSensor SoundEvent was triggered.\r\n");
#else
    printf("LOGIC DEBUG: SoundSensor SoundEvent was triggered.\r\n");
#endif
}

#if(UNIT_TEST==1)

#include <unistd.h>
//The following code is used for the unit test of the buzzer.
//What you will see: The buzzer turns on for one second and then turns off.

class Rpi_SoundSensor : public SoundSensor {
    using SoundSensor::SoundSensor;
	virtual void SoundSensor_SoundEvent_Handle() {
		printf("Rpi_SoundSensor Class DEBUG: SoundSensor_SoundEvent_Handle() was triggered.\r\n");
	}
};

int main() {
    SoundSensor_Settings My_SoundSensor_Settings;
    My_SoundSensor_Settings.Trigger_Method = RISING_EDGE; //when sound appears
    Rpi_SoundSensor My_Rpi_SoundSensor(My_SoundSensor_Settings);
    getchar();
    return 0;
}

#endif
