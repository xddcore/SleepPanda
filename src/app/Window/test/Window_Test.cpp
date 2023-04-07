/*
 * @Author: Chengsen Dong 1034029664@qq.com
 * @Date: 2023-03-15 11:02:57
 * @LastEditors: Chengsen Dong 1034029664@qq.com
 * @LastEditTime: 2023-04-07 17:59:10
 * @FilePath: /SleepPanda/src/app/Window/test/Window_Test.cpp
 * @Description: 
 * Copyright (c) 2023 by ${git_name_email}(www.github.com/xddcore), All Rights Reserved. 
 */
#include "Window.h"

#include <QApplication>


#include "gtest/gtest.h"

//The following code is used for the unit test of the Windows.
//When the unit test program runs:
//Display the camera image in real time, use the red circle to mark the front face and eyes, 
//and use the blue circle to mark the side face and eyes.

TEST(Window_Test, Window_Display_Test)
{
	//QApplication app(argc, argv);
    // create the window
	//Window window;
	//window.show();

	// call the window.timerEvent function every 40 ms
	//window.startTimer(40);
	//return app.exec();
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    //return RUN_ALL_TESTS();
	QApplication app(argc, argv);
    // create the window(这里自己建立的窗口window继承自QWidget，其实就是个自定义大控件)
	Window window;
	window.show();

	// call the window.timerEvent function every 40 ms
	window.startTimer(40);
	return app.exec();
}