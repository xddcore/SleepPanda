/*
 * @Author: Chengsen Dong 1034029664@qq.com
 * @Date: 2023-02-07 12:52:10
 * @LastEditors: Chengsen Dong 1034029664@qq.com
 * @LastEditTime: 2023-02-12 14:22:50
 * @FilePath: /SleepPanda/src/main/Buzzer.cpp
 * @Description: 
 * Copyright (c) 2023 by ${git_name_email}(www.github.com/xddcore), All Rights Reserved. 
 */
#include <iostream>
#include "Buzzer.h"
#include "config.h"
#include "stdio.h"

#define CLASS_DEBUG 1

class Buzzer{
    public:
    Buzzer();
    ~Buzzer();
    int Buzzer_Contorl(unsigned int Buzzer_OnOff);
};

//蜂鸣器构造函数
Buzzer::Buzzer() {
#if(DEBUG==0)
    //此处添加蜂鸣器Buzzer_GPIO_Pin 6 的初始化代码
#else
    printf("DEBUG: Buzzer Init.\r\n");
#endif
}

//蜂鸣器析构函数
Buzzer::~Buzzer() {
#if(DEBUG==0)
    //此处添加蜂鸣器GPIO资源释放代码(程序执行结束后自动释放)
#else
    printf("DEBUG: Buzzer Delete.\r\n");
#endif
}

//蜂鸣器开关状态控制函数
int Buzzer::Buzzer_Contorl(unsigned int Buzzer_OnOff) {
#if(DEBUG==0)
    if(Buzzer_OnOff==Buzzer_On){
        //此处添加蜂鸣器触发代码Buzzer_On_Level
    }
    else if(Buzzer_OnOff==Buzzer_Off){
        //此处添加蜂鸣器关闭代码Buzzer_Off_Level
    }
    return 0;
#else
    if(Buzzer_OnOff==Buzzer_On){
        printf("DEBUG: Buzzer On.\r\n");
    }
    else if(Buzzer_OnOff==Buzzer_Off){
        printf("DEBUG: Buzzer Off.\r\n");
    }
    return 0;
#endif
}

#if(CLASS_DEBUG==1)
//以下代码用于基本逻辑测试
int main() {
    Buzzer MyBuzzer;
    MyBuzzer.Buzzer_Contorl(Buzzer_On);
    MyBuzzer.Buzzer_Contorl(Buzzer_Off);
    return 0;
}
#endif



