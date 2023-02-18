/*
 * @Author: Chengsen Dong 1034029664@qq.com
 * @Date: 2023-02-12 12:50:52
 * @LastEditors: Chengsen Dong 1034029664@qq.com
 * @LastEditTime: 2023-02-13 10:11:35
 * @FilePath: /SleepPanda/src/app/Buzzer/Buzzer.h
 * @Description: 
 * Copyright (c) 2023 by ${git_name_email}(www.github.com/xddcore), All Rights Reserved. 
 */

#ifndef _BUZZER_H
#define _BUZZER_H

#include <stdint.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <pigpio.h>
#include <assert.h>

#include "../Config/Config.h"

struct Buzzer_Settings {
    unsigned int Buzzer_GPIO_Pin = 6; //GPIO6(PIN31) will be used
    unsigned int Buzzer_On_Level = 0; //蜂鸣器低电平触发
    unsigned int Buzzer_Off_Level = 1; //蜂鸣器高电平不触发

    /**
	 * If set to true the pigpio will be initialised
	 **/
	bool initPIGPIO = true;
};

#define Buzzer_On 1
#define Buzzer_Off 0

class Buzzer{
    public:
    Buzzer(Buzzer_Settings My_Buzzer_Settings);
    ~Buzzer();
    int Buzzer_Contorl(unsigned int Buzzer_OnOff);
    
    private:
    Buzzer_Settings My_Buzzer_Settings;
};

#endif //_BUZZER_H
