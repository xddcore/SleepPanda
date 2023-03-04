/*
 * @Author: Chengsen Dong 1034029664@qq.com
 * @Date: 2023-03-04 10:36:54
 * @LastEditors: Chengsen Dong 1034029664@qq.com
 * @LastEditTime: 2023-03-04 17:44:37
 * @FilePath: /SleepPanda/src/app/Camera/Camera.cpp
 * @Description: 
 * Copyright (c) 2023 by ${git_name_email}(www.github.com/xddcore), All Rights Reserved. 
 */
#include "Camera.h"


//Camera构造函数
Camera::Camera(Camera_Settings camera_settings){
    My_Camera_Settings = camera_settings;

    if( !Upperbody_Cascade.load(My_Camera_Settings.Upperbody_Cascade_Path))
    {
        printf("RPI DEBUG: Camera Error loading Upperbody_Cascade.\r\n");
    };
    if( !Frontal_Face_Cascade.load(My_Camera_Settings.Frontal_Face_Cascade_Path))
    {
        printf("RPI DEBUG: Camera Error loading Frontal_Face_Cascade.\r\n");
    };
    if( !Profile_Face_Cascade.load(My_Camera_Settings.Profile_Face_Cascade_Path))
    {
        printf("RPI DEBUG: Camera Error loading Profile_Face_Cascade.\r\n");
    };
    if( !Eye_Cascade.load(My_Camera_Settings.Eye_Cascade_Path))
    {
        printf("RPI DEBUG: Camera Error loading Eye_Cascade.\r\n");
    };

    capture.open(My_Camera_Settings.Camera_Device);
    if (!capture.isOpened())
    {
        printf("RPI DEBUG: Camera Error opening video capture.\r\n");
    }
}

//Camera析构函数
Camera::~Camera(){
    capture.release();
}

//Camera Detection函数
Mat Camera::Detection(Mat frame){
    Mat frame_gray;
    cvtColor( frame, frame_gray, COLOR_BGR2GRAY );
    equalizeHist( frame_gray, frame_gray );

    /*
    //-- Detect Upperbody
    std::vector<Rect> Upperbody;
    Upperbody_Cascade.detectMultiScale( frame_gray, Upperbody );

    for ( size_t i = 0; i < Upperbody.size(); i++ )
    {
        Point center( Upperbody[i].x + Upperbody[i].width/2, Upperbody[i].y + Upperbody[i].height/2 );
        ellipse( frame, center, Size( Upperbody[i].width/2, Upperbody[i].height/2 ), 0, 0, 360, Scalar( 255, 0, 255 ), 4 );

        Mat UpperbodyROI = frame_gray( Upperbody[i] );

        //-- In each Upperbody, detect Frontal_Face
        std::vector<Rect> FrontalFace;
        Frontal_Face_Cascade.detectMultiScale( UpperbodyROI, FrontalFace );

        for ( size_t j = 0; j < FrontalFace.size(); j++ )
        {
            Point FrontalFace_Center( Upperbody[i].x + FrontalFace[j].x + FrontalFace[j].width/2, Upperbody[i].y + FrontalFace[j].y + FrontalFace[j].height/2 );
            int radius = cvRound( (FrontalFace[j].width + FrontalFace[j].height)*0.25 );
            circle( frame, FrontalFace_Center, radius, Scalar( 255, 0, 0 ), 4 );
        }
    }
    */
    
    //Opencv 正脸检测+人眼检测
    std::vector<Rect> faces;
    Frontal_Face_Cascade.detectMultiScale( frame_gray, faces );
    //Color Blue Green Red
    for ( size_t i = 0; i < faces.size(); i++ )
    {
        Point center( faces[i].x + faces[i].width/2, faces[i].y + faces[i].height/2 );
        ellipse( frame, center, Size( faces[i].width/2, faces[i].height/2 ), 0, 0, 360, Scalar( 0, 0, 255 ), 4 );

        Mat faceROI = frame_gray( faces[i] );

        //-- In each face, detect eyes
        std::vector<Rect> eyes;
        Eye_Cascade.detectMultiScale( faceROI, eyes );

        for ( size_t j = 0; j < eyes.size(); j++ )
        {
            Point eye_center( faces[i].x + eyes[j].x + eyes[j].width/2, faces[i].y + eyes[j].y + eyes[j].height/2 );
            int radius = cvRound( (eyes[j].width + eyes[j].height)*0.25 );
            circle( frame, eye_center, radius, Scalar( 0, 0, 255 ), 4 );
        }
    }
    //Opencv 侧脸检测+人眼检测
    std::vector<Rect> faces2;
    Profile_Face_Cascade.detectMultiScale( frame_gray, faces2 );
    for ( size_t i = 0; i < faces2.size(); i++ )
    {
        Point center( faces2[i].x + faces2[i].width/2, faces2[i].y + faces2[i].height/2 );
        ellipse( frame, center, Size( faces2[i].width/2, faces2[i].height/2 ), 0, 0, 360, Scalar( 255, 0, 0 ), 4 );

        Mat faceROI = frame_gray( faces2[i] );

        //-- In each face, detect eyes
        std::vector<Rect> eyes;
        Eye_Cascade.detectMultiScale( faceROI, eyes );

        for ( size_t j = 0; j < eyes.size(); j++ )
        {
            Point eye_center( faces2[i].x + eyes[j].x + eyes[j].width/2, faces2[i].y + eyes[j].y + eyes[j].height/2 );
            int radius = cvRound( (eyes[j].width + eyes[j].height)*0.25 );
            circle( frame, eye_center, radius, Scalar( 255, 0, 0 ), 4 );
        }
    }
    
    return frame;
}

//Camera 开始捕获摄像头帧函数
Mat Camera::Start(){
    Mat frame;
    capture.read(frame);
    if( frame.empty() )
    {
        printf("RPI DEBUG: Camera Error No captured frame.\r\n");
    }
    return frame;
}