/*
 * @Author: Yihan Wang yihanwangg@163.com
 * @Date: 2023-02-13 10:03:21
 * @LastEditors: Yihan Wang yihanwangg@163.com
 * @LastEditTime: 2023-02-15 12:25:53
 * @FilePath: /SleepPanda/src/app/SoundSensor/SoundSensor.h
 * @Description: 
 * Copyright (c) 2023 by ${git_name_email}(www.github.com/xddcore), All Rights Reserved. 
 */

#ifndef _SoundSensor_H
#define _SoundSensor_H

struct SoundSensor_Settings {
    unsigned int SoundSensor_GPIO_Pin = 5; //GPIO5(PIN29) will be used
    unsigned int Trigger_Method;
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
    virtual void SoundSensor_SoundEvent_Handle()=0; //纯虚函数(规范回调接口)

    private:
    SoundSensor_Settings My_SoundSensor_Settings;
    void Sound_Event();
	static void gpioISR(int, int, uint32_t, void* userdata) {
		((SoundSensor*)userdata)->Sound_Event();
	}
};

#endif //_SoundSensor_H