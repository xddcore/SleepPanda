/*
 * @Author: Chengsen Dong 1034029664@qq.com
 * @Date: 2023-03-04 10:37:06
 * @LastEditors: Chengsen Dong 1034029664@qq.com
 * @LastEditTime: 2023-04-07 13:41:09
 * @FilePath: /SleepPanda/src/app/Camera/Camera.h
 * @Description: 
 * Copyright (c) 2023 by ${git_name_email}(www.github.com/xddcore), All Rights Reserved. 
 */
#ifndef _Camera_H
#define _Camera_H

#include <stdint.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <pigpio.h>
#include <assert.h>

#include "../Config/Config.h"

#include "opencv2/objdetect.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/videoio.hpp"
#include <iostream>
#include <thread>

using namespace std;
using namespace cv;

struct Camera_Settings {
    unsigned int Camera_Device = 0; //摄像头编号
    String Upperbody_Cascade_Path = "/home/pi/xddcore/SleepPanda/src/app/Camera/haarcascade_upperbody.xml"; //上半身XML文件
    String Frontal_Face_Cascade_Path = "/home/pi/xddcore/SleepPanda/src/app/Camera/haarcascade_frontalface_alt.xml"; //正脸XML文件
    String Profile_Face_Cascade_Path = "/home/pi/xddcore/SleepPanda/src/app/Camera/haarcascade_profileface.xml"; //侧脸XML文件
    String Eye_Cascade_Path = "/home/pi/xddcore/SleepPanda/src/app/Camera/haarcascade_eye_tree_eyeglasses.xml"; //眼睛XML文件
};



class Camera{
    public:
    std::vector<Rect> Frontal_Face_Position;
    std::vector<Rect> Frontal_Face_Eye_Position;
    std::vector<Rect> Profile_Face_Position;
    std::vector<Rect> Profile_Face_Eye_Position;

    Camera(Camera_Settings My_Camera_Settings);
    ~Camera();
    Mat Detection(Mat frame,bool show_result);//检测函数
    void Start();
    void Stop();

	/**
	 * Callback which needs to be implemented by the client
	 **/
	struct SceneCallback {
		virtual void nextScene(const cv::Mat &mat) = 0;
	};
	/**
	 * Registers the callback which receives the
	 * frames.
	 **/
	void registerSceneCallback(SceneCallback* sc) {
		sceneCallback = sc;
	}
    
    private:
    Camera_Settings My_Camera_Settings;
    VideoCapture capture;
    CascadeClassifier Upperbody_Cascade;
    CascadeClassifier Frontal_Face_Cascade;
    CascadeClassifier Profile_Face_Cascade;
    CascadeClassifier Eye_Cascade;

    void postFrame();
    void threadLoop();
    std::thread cameraThread;
    bool isOn = false;
	SceneCallback* sceneCallback = nullptr;
};

#endif //_Camera_H