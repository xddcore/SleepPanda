/*
 * @Author: Chengsen Dong 1034029664@qq.com
 * @Date: 2023-02-17 19:28:49
 * @LastEditors: Chengsen Dong 1034029664@qq.com
 * @LastEditTime: 2023-03-02 09:46:28
 * @FilePath: /SleepPanda/src/app/MAX30101/test/MAX30101_Test.cpp
 * @Description: 
 * Copyright (c) 2023 by ${git_name_email}(www.github.com/xddcore), All Rights Reserved. 
 */
#include "../MAX30101.h"

#include "gtest/gtest.h"
//The following code is used for the unit test of the MAX30101.
//What you will see: 
//Place your fingertip on the MAX30101 sensor, wait 15 seconds, and press the Enter key on your keyboard. Unit testing is complete.

//BUG记录:
/*
1.通过示波器观测(示波器脚单独接MAX30101的INT脚): 当iic read清除ISR寄存器时，INT脚的电平能够发生改变
2.通过示波器观测(示波器脚接MAX30101的INT脚+接树莓派4脚):当iic read清除ISR寄存器时，INT脚的电平一直为低，导致无法进入下降沿中断函数，进而无法处理数据
3.树莓派的4脚已经设置为输入模式，上拉，gpioREAD得到0.
4.树莓派4脚单独接5v后，gpioREAD得到1.

怀疑:树莓派gpio有问题，导致树莓派gpio4一直拉低，同时导致INT引脚也被一直拉低。

解决方案:通过`raspi-gpio get`查询发现，原计划使用的Interrupt Pin -4被其他程序占用，导致输入电平始终为0，无法实现外部中断功能。
所以将使用GPIO6连接MAX30101的引脚。
*/
class Rpi_MAX30101 : public MAX30101 {
    using MAX30101::MAX30101;
    public:
        bool ISR_CheckPoint = false;
        int32_t heart_rate_test;
        double spo2_test;
	    virtual void MAX30101_DataReadyEvent_Handle(int32_t heart_rate, double spo2) {
            heart_rate_test = heart_rate;
            spo2_test = spo2;
		    printf("Rpi_MAX30101 Class DEBUG: MAX30101_DataReadyEvent_Handle() was triggered.|heart_rate:%d, spo2: %lf\r\n", heart_rate,spo2);
            ISR_CheckPoint = true;
	    }
};


TEST(MAX30101_Test, Check_MAX30101_ISR_HeartRate_SPO2)
{
    MAX30101_Settings My_MAX30101_Settings;
    Rpi_MAX30101 My_Rpi_MAX30101(My_MAX30101_Settings);
    My_Rpi_MAX30101.Start();
    getchar();
    My_Rpi_MAX30101.Stop();
    printf("Rpi_MAX30101.ISR_CheckPoint:%d\r\n",My_Rpi_MAX30101.ISR_CheckPoint);
    ASSERT_NE(My_Rpi_MAX30101.heart_rate_test,-999);//heart_rate !=-999
    ASSERT_GT((int32_t)My_Rpi_MAX30101.spo2_test,98);//spo2 > 98%
    ASSERT_TRUE(My_Rpi_MAX30101.ISR_CheckPoint);//pass gtest
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    printf("\r\nPlace your fingertip on the MAX30101 sensor, wait 15 seconds, and press the Enter key on your keyboard.\r\n");
    return RUN_ALL_TESTS();
}
