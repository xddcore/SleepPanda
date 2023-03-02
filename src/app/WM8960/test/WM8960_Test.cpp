/*
 * @Author: Yihan Wang yihanwangg@163.com
 * @Date: 2023-03-01 14:08:17
 * @LastEditors: Yihan Wang yihanwangg@163.com
 * @LastEditTime: 2023-03-01 20:53:05
 * @FilePath: /SleepPanda/src/app/WM8960/test/WM8960_Test.cpp
 * @Description: 
 * Copyright (c) 2023 by ${git_name_email}(www.github.com/xddcore), All Rights Reserved. 
 */
#include "WM8960.h"

#include "gtest/gtest.h"

//The following code is used for the unit test of the buzzer.
//When the unit test program runs:
//Will use WM8960 to record for 3 seconds and create a Record_Wav_File_Test.wav file.

inline bool Check_Exists_File (const std::string& name) {
    struct stat buffer;   
    return (stat (name.c_str(), &buffer) == 0); 
}


TEST(WM8960_Test, Record_Wav_File_Test)
{
    int cmd_result;
    WM8960_Settings My_WM8960_Settings;
    WM8960 MyWM8960;
    cmd_result = MyWM8960.Record(My_WM8960_Settings,3,"Record_Wav_File_Test.wav");
    ASSERT_FALSE(cmd_result);
}
TEST(WM8960_Test, Check_Wav_File_Exists)
{
    ASSERT_TRUE(Check_Exists_File("Record_Wav_File_Test.wav"));
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}