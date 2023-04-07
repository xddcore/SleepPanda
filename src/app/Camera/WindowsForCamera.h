/*
 * @Author: Chengsen Dong 1034029664@qq.com
 * @Date: 2023-04-07 13:30:41
 * @LastEditors: Chengsen Dong 1034029664@qq.com
 * @LastEditTime: 2023-04-07 14:07:34
 * @FilePath: /SleepPanda/src/app/Camera/WindowsForCamera.h
 * @Description: 
 * Copyright (c) 2023 by ${git_name_email}(www.github.com/xddcore), All Rights Reserved. 
 */
#ifndef _WINDOWFORCAMERA_H_
#define _WINDOWFORCAMERA_H_

#include <qwt/qwt_thermo.h>

#include <QBoxLayout>
#include <QPushButton>
#include <QLabel>

#include "Camera.h"

// class definition 'Window'
class Window : public QWidget
{
	// must include the Q_OBJECT macro for for the Qt signals/slots framework to work with this class
	Q_OBJECT

public:
	Window();
	~Window();

    
	void updateImage(const cv::Mat &mat);

	QwtThermo *thermo;
	QHBoxLayout  *hLayout;  // horizontal layout
	QLabel *image;

	struct MyCallback : Camera::SceneCallback {
		Window* window = nullptr;
		virtual void nextScene(const cv::Mat &mat) {
			if (nullptr != window) {
				window->updateImage(mat);
			}
		}
	};

	MyCallback myCallback;
};

#endif // _WINDOWFORCAMERA_H_