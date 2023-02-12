/*
 * @Author: Chengsen Dong 1034029664@qq.com
 * @Date: 2023-02-12 12:50:52
 * @LastEditors: Chengsen Dong 1034029664@qq.com
 * @LastEditTime: 2023-02-12 19:57:25
 * @FilePath: /SleepPanda/src/app/Buzzer/Buzzer.h
 * @Description: 
 * Copyright (c) 2023 by ${git_name_email}(www.github.com/xddcore), All Rights Reserved. 
 */

#ifndef SRC_BUZZER_H
#define SRC_BUZZER_H


struct Buzzer_Settings {
    unsigned int Buzzer_GPIO_Pin = 6;
    unsigned int Buzzer_On_Level = 0; //蜂鸣器低电平触发
    unsigned int Buzzer_Off_Level = 1; //蜂鸣器高电平不触发
};

#define Buzzer_On 1
#define Buzzer_Off 0

class Buzzer{
    public:
    Buzzer();
    ~Buzzer();
    int Buzzer_Contorl(unsigned int Buzzer_OnOff);
};

#endif //SRC_BUZZER_H
