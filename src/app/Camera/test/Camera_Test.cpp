/*
 * @Author: Chengsen Dong 1034029664@qq.com
 * @Date: 2023-03-04 13:47:31
 * @LastEditors: Chengsen Dong 1034029664@qq.com
 * @LastEditTime: 2023-03-04 18:38:45
 * @FilePath: /SleepPanda/src/app/Camera/test/Camera_Test.cpp
 * @Description: 
 * Copyright (c) 2023 by ${git_name_email}(www.github.com/xddcore), All Rights Reserved. 
 */
#include "Camera.h"

#include "gtest/gtest.h"

//The following code is used for the unit test of the buzzer.
//When the unit test program runs:
//If you hear the buzzer, please use the keyboard to type "Y" and press ENTER.

TEST(Camera_Test, Check_Cemera_Detection)
{
    Camera_Settings My_Camera_Settings;
    Mat frame;
    Camera MyCamera(My_Camera_Settings);
    while(1)
    {
    frame = MyCamera.Detection(MyCamera.Start(),true);
    for ( size_t i = 0; i < MyCamera.Frontal_Face_Position.size(); i++ )
    {
        printf("[%d]Frontal_Face_Position | x: %d, y: %d\r\n",i,MyCamera.Frontal_Face_Position[i].x,MyCamera.Frontal_Face_Position[i].y);
    }
    for ( size_t i = 0; i < MyCamera.Frontal_Face_Eye_Position.size(); i++ )
    {
        printf("[%d]Frontal_Face_Eye_Position | x: %d, y: %d\r\n",i,MyCamera.Frontal_Face_Eye_Position[i].x,MyCamera.Frontal_Face_Eye_Position[i].y);
    }
    imshow( "Capture - SleepPanda", frame );
    int c = waitKey(10);
    if( (char)c == 'c' ) { break; } //press c to exit
    }
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    printf("\r\nPlease confirm whether the buzzer sounds (Y/N):\r\n");
    return RUN_ALL_TESTS();
}