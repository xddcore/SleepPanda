/*
 * @Author: Chengsen Dong 1034029664@qq.com
 * @Date: 2023-02-19 15:55:20
 * @LastEditors: Chengsen Dong 1034029664@qq.com
 * @LastEditTime: 2023-02-19 17:06:45
 * @FilePath: /SleepPanda/src/app/MAX30101/MAX30101.cpp
 * @Description: 
 * Copyright (c) 2023 by ${git_name_email}(www.github.com/xddcore), All Rights Reserved. 
 */
#include "MAX30101.h"

//MAX30101构造函数
MAX30101::MAX30101(MAX30101_Settings max30101_settings) {
#if(RPI_DEBUG==1)
    if (max30101_settings.initPIGPIO) {
        int cfg = gpioCfgGetInternals();
        cfg |= PI_CFG_NOSIGHANDLER;
        gpioCfgSetInternals(cfg);
        int err;
        My_MAX30101_Settings = max30101_settings;
        err = gpioInitialise();    
        if (err < 0)
        {
            printf("RPI DEBUG: MAX30101 pigpio initialisation failed.\r\n");
            // pigpio initialisation failed.
        }
        else
        {
            printf("RPI DEBUG: MAX30101 pigpio initialised okay.\r\n");
            // pigpio initialised okay.
	        // set gpio4(pin7) mode to input
            gpioSetMode(My_MAX30101_Settings.MAX30101_Interrupt_GPIO_Pin,PI_INPUT);
            gpioSetISRFuncEx(My_MAX30101_Settings.MAX30101_Interrupt_GPIO_Pin,My_MAX30101_Settings.Trigger_Method,My_MAX30101_Settings.ISR_TIMEOUT,gpioISR,(void*)this);
        }
    }
    //此处添加MAX30101的初始化代码
#else
    printf("LOGIC DEBUG: MAX30101 Init.\r\n");
#endif
}

//MAX30101析构函数
MAX30101::~MAX30101() {
#if(RPI_DEBUG==1)
    //此处添加MAX30101 Interrupt GPIO资源释放代码(程序执行结束后自动释放)
    gpioSetISRFuncEx(My_MAX30101_Settings.MAX30101_Interrupt_GPIO_Pin,RISING_EDGE,-1,NULL,(void*)this);
    gpioTerminate();
    printf("RPI DEBUG: MAX30101 Delete.\r\n");
#else
    printf("LOGIC DEBUG: MAX30101 Delete.\r\n");
#endif
}

//MAX30101_DataReady事件函数
void MAX30101::DataReady_Event() {
#if(RPI_DEBUG==1)
    MAX30101_DataReadyEvent_Handle();
    printf("RPI DEBUG: MAX30101 DataReady was triggered.\r\n");
#else
    printf("LOGIC DEBUG: MAX30101 DataReady was triggered.\r\n");
#endif
}