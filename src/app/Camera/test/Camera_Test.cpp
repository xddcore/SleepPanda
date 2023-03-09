/*
 * @Author: Chengsen Dong 1034029664@qq.com
 * @Date: 2023-03-04 13:47:31
 * @LastEditors: Chengsen Dong 1034029664@qq.com
 * @LastEditTime: 2023-03-05 15:57:34
 * @FilePath: /SleepPanda/src/app/Camera/test/Camera_Test.cpp
 * @Description: 
 * Copyright (c) 2023 by ${git_name_email}(www.github.com/xddcore), All Rights Reserved. 
 */
#include "Camera.h"

#include "gtest/gtest.h"

//The following code is used for the unit test of the camera.
//When the unit test program runs:
//Display the camera image in real time, use the red circle to mark the front face and eyes, 
//and use the blue circle to mark the side face and eyes.

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
        int c = waitKey(1);
        if( (char)c == 'c' ) { break; } //press c to exit
    }
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}