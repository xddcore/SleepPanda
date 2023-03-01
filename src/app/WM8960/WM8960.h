/*
 * @Author: Yihan Wang yihanwangg@163.com
 * @Date: 2023-03-01 13:11:38
 * @LastEditors: Yihan Wang yihanwangg@163.com
 * @LastEditTime: 2023-03-01 14:27:08
 * @FilePath: /SleepPanda/src/app/WM8960/WM8960.h
 * @Description: 
 * Copyright (c) 2023 by ${git_name_email}(www.github.com/xddcore), All Rights Reserved. 
 */
#ifndef _WM8960_H
#define _WM8960_H

#include <stdint.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <pigpio.h>
#include <assert.h>
#include <string>

#include "../Config/Config.h"

struct WM8960_Settings {
    unsigned int Card_Numer = 3;
    unsigned int Device_Number = 0;
};

class WM8960{
    public:
    WM8960();
    ~WM8960();
    int Record(WM8960_Settings WM8960_settings,unsigned int time,char* wav_file);
    
    private:
    WM8960_Settings My_WM8960_Settings;
};

#endif //_BUZZER_H