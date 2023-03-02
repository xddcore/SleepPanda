/*
 * @Author: Yihan Wang yihanwangg@163.com
 * @Date: 2023-03-01 13:11:28
 * @LastEditors: Yihan Wang yihanwangg@163.com
 * @LastEditTime: 2023-03-01 21:08:50
 * @FilePath: /SleepPanda/src/app/WM8960/WM8960.cpp
 * @Description: 
 * Copyright (c) 2023 by ${git_name_email}(www.github.com/xddcore), All Rights Reserved. 
 */
#include "WM8960.h"

//WM8960构造函数
WM8960::WM8960() {
#if(RPI_DEBUG==1)
    printf("RPI DEBUG: WM8960 initialisation success.\r\n");
    //此处添加WM8960的初始化代码
#else
    printf("LOGIC DEBUG: WM8960 Init.\r\n");
#endif
}

//WM8960析构函数
WM8960::~WM8960() {
#if(RPI_DEBUG==1)
    //此处添加WM8960资源释放代码(程序执行结束后自动释放)
    printf("RPI DEBUG: WM8960 Delete.\r\n");
#else
    printf("LOGIC DEBUG: WM8960 Delete.\r\n");
#endif
}

//WM8960录音函数
int WM8960::Record(WM8960_Settings WM8960_settings,unsigned int time,const char* wav_file){
#if(RPI_DEBUG==1)
    //std::string cmd;
    char cmd[1000];
    //arecord -D "plughw:3,0" -d 5 -f cd -c 2 -v -t wav test.wav
    sprintf(cmd,"arecord -D \"plughw:%d,%d\" -d %d -f cd -c 2 -v -t wav %s",WM8960_settings.Card_Numer,WM8960_settings.Device_Number,time,wav_file);
    printf("RPI DEBUG: WM8960 Recording(time = %d, Wav File = %s).\r\n",time,wav_file);
    return system(cmd);
#else
    printf("LOGIC DEBUG: WM8960 WM8960_Record().\r\n");
#endif
}