/*
 * @Author: Chengsen Dong 1034029664@qq.com
 * @Date: 2023-02-17 19:28:49
 * @LastEditors: Chengsen Dong 1034029664@qq.com
 * @LastEditTime: 2023-02-21 18:33:37
 * @FilePath: /SleepPanda/src/app/MAX30101/test/MAX30101_Test.cpp
 * @Description: 
 * Copyright (c) 2023 by ${git_name_email}(www.github.com/xddcore), All Rights Reserved. 
 */
#include "../MAX30101.h"

#include "gtest/gtest.h"
//The following code is used for the unit test of the MAX30101.
//What you will see: 

class Rpi_MAX30101 : public MAX30101 {
    using MAX30101::MAX30101;
    public:
        bool ISR_CheckPoint = false;
	    virtual void MAX30101_DataReadyEvent_Handle(int32_t heart_rate, double spo2) {
		    printf("Rpi_MAX30101 Class DEBUG: MAX30101_DataReadyEvent_Handle() was triggered.|heart_rate:%d, spo2: %lf\r\n", heart_rate,spo2);
            ISR_CheckPoint = true;
	    }
};


TEST(MAX30101_Test, Check_MAX30101_Event_ISR)
{
    MAX30101_Settings My_MAX30101_Settings;
    Rpi_MAX30101 My_Rpi_MAX30101(My_MAX30101_Settings);
    My_Rpi_MAX30101.Start();
    getchar();
    printf("Rpi_MAX30101.ISR_CheckPoint:%d\r\n",My_Rpi_MAX30101.ISR_CheckPoint);
    ASSERT_TRUE(My_Rpi_MAX30101.ISR_CheckPoint);//pass gtest
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    printf("\r\nMAX30101 SENSOR:\r\n");
    return RUN_ALL_TESTS();
}
