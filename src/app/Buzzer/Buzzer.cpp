/*
 * @Author: Yihan Wang yihanwangg@163.com
 * @Date: 2023-02-12 20:04:33
 * @LastEditors: Yihan Wang yihanwangg@163.com
 * @LastEditTime: 2023-02-18 11:48:51
 * @FilePath: /SleepPanda/src/app/Buzzer/Buzzer.cpp
 * @Description: 
 * Copyright (c) 2023 by ${git_name_email}(www.github.com/xddcore), All Rights Reserved. 
 */
/*
 * @Author: Chengsen Dong 1034029664@qq.com
 * @Date: 2023-02-07 12:52:10
 * @LastEditors: Yihan Wang yihanwangg@163.com
 * @LastEditTime: 2023-02-18 11:48:27
 * @FilePath: /SleepPanda/src/app/Buzzer/Buzzer.cpp
 * @Description: 
 * Copyright (c) 2023 by ${git_name_email}(www.github.com/xddcore), All Rights Reserved. 
 */
#include "Buzzer.h"

//蜂鸣器构造函数
Buzzer::Buzzer(Buzzer_Settings buzzer_settings) {
#if(RPI_DEBUG==1)
    if (buzzer_settings.initPIGPIO) {
        int cfg = gpioCfgGetInternals();
        cfg |= PI_CFG_NOSIGHANDLER;
        gpioCfgSetInternals(cfg);
        int err;
        My_Buzzer_Settings = buzzer_settings;
        err = gpioInitialise();    
        if (err < 0)
        {
            printf("RPI DEBUG: Buzzer pigpio initialisation failed.\r\n");
            // pigpio initialisation failed.
        }
        else
        {
            printf("RPI DEBUG: Buzzer pigpio initialised okay.\r\n");
            // pigpio initialised okay.
	        // set gpio6 mode to output
	        gpioSetMode(My_Buzzer_Settings.Buzzer_GPIO_Pin, PI_OUTPUT); // Set GPIO6 as output.
        }
    }
    //此处添加蜂鸣器Buzzer_GPIO_Pin 6 的初始化代码
#else
    printf("LOGIC DEBUG: Buzzer Init.\r\n");
#endif
}

//蜂鸣器析构函数
Buzzer::~Buzzer() {
#if(RPI_DEBUG==1)
    //此处添加蜂鸣器GPIO资源释放代码(程序执行结束后自动释放)
    gpioTerminate();
    printf("RPI DEBUG: Buzzer Delete.\r\n");
#else
    printf("LOGIC DEBUG: Buzzer Delete.\r\n");
#endif
}

//蜂鸣器开关状态控制函数
int Buzzer::Buzzer_Contorl(unsigned int Buzzer_OnOff) {
#if(RPI_DEBUG==1)
    if(Buzzer_OnOff==Buzzer_On){
        //此处添加蜂鸣器触发代码Buzzer_On_Level
	gpioWrite(My_Buzzer_Settings.Buzzer_GPIO_Pin, My_Buzzer_Settings.Buzzer_On_Level); // Set GPIO6 low.
	printf("RPI DEBUG: Buzzer On.\r\n");
    }
    else if(Buzzer_OnOff==Buzzer_Off){
        //此处添加蜂鸣器关闭代码Buzzer_Off_Level
	gpioWrite(My_Buzzer_Settings.Buzzer_GPIO_Pin, My_Buzzer_Settings.Buzzer_Off_Level); // Set GPIO6 high.
	printf("RPI DEBUG: Buzzer Off.\r\n");
    }
    return 0;
#else
    if(Buzzer_OnOff==Buzzer_On){
        printf("LOGIC DEBUG: Buzzer On.\r\n");
    }
    else if(Buzzer_OnOff==Buzzer_Off){
        printf("LOGIC DEBUG: Buzzer Off.\r\n");
    }
    return 0;
#endif
}


