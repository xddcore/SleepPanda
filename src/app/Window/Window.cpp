#include "Window.h"

#include <cmath>  // for sine stuff


Window::Window()
{
	/*************控件************/
	//测试图片
    Test_Image = new QImage;
	Test_Image->load("/home/pi/xddcore/SleepPanda/img/Panda.png");
    //摄像头
    Camera_Display_Label = new QLabel;
	Camera_Display_Label->setPixmap(QPixmap::fromImage(*Test_Image));
    Camera_Text_Label = new QLabel("Sleep State:");
    Camera_SleepStatus_Label = new QLabel("Sleeping On The Front / Side");
    Camera_Log_TextEdit = new QTextEdit("Sleep State Log");
    //血氧控件
    BloodOxygen_Label = new QLabel("Blood Oxygen(%):");
    BloodOxygen_Lcdnumber = new QLCDNumber;
    BloodOxygen_Plot = new QwtPlot;
	BloodOxygen_Curve = new QwtPlotCurve;
    //心率控件
    HeartRate_Label = new QLabel("Heart Rate(bpm):");
    HeartRate_Lcdnumber = new QLCDNumber;
    HeartRate_Plot = new QwtPlot;
	HeartRate_Curve = new QwtPlotCurve;
    //呼吸暂停综合症
    ApneaSyndrome_Label = new QLabel("ApneaSyndrome Detector Power: ");
    ApneaSyndrome_PowerStatus_Label = new QLabel;
	ApneaSyndrome_PowerStatus_Label->setText("Turn On / Turn Off");
    ApneaSyndrome_TurnOn_Button = new QPushButton("Turn On"); 
    ApneaSyndrome_TurnOff_Button = new QPushButton("Turn Off");
    ApneaSyndrome__Log_TextEdit = new QTextEdit("ApneaSyndrome Detector Log");
    //报警设置
    AlarmSettings_Label = new QLabel("Alarm Settings");
	AlarmSettings_BloodOxygen_Label = new QLabel("Blood Oxygen Threshold: ");
	AlarmSettings_BloodOxygen_MinValue_QLineEdit = new QLineEdit("99.0");
	AlarmSettings_BloodOxygen_Connection_Label = new QLabel("--");
    AlarmSettings_BloodOxygen_MaxValue_QLineEdit = new QLineEdit("100.0");
    AlarmSettings_HeartRate_Label = new QLabel("Heart Rate Threshold: ");
	AlarmSettings_HeartRate_MinValue_QLineEdit = new QLineEdit("60");
	AlarmSettings_HeartRate_Connection_Label = new QLabel("--");
    AlarmSettings_HeartRate_MaxValue_QLineEdit = new QLineEdit("90");
    AlarmSettings_ApneaSyndrome_Label = new QLabel("Apnea Syndrome: ");
	AlarmSettings_ApneaSyndrome_TurnOnOff_Label = new QLabel("Turn On / Turn Off");
    AlarmSettings_ApneaSyndrome_TurnOn_Button = new QPushButton("Turn On");
    AlarmSettings_ApneaSyndrome_TurnOff_Button = new QPushButton("Turn Off");
	AlarmSettings_Update_Settings_Button = new QPushButton("Update Settings");
    //播放助眠音乐
    Music_Label = new QLabel("Music Player");
	Music_Play_Button = new QPushButton("Play");
    //MQTT
    MQTT_Label = new QLabel("MQTT Info");
	MQTT_Log_TextEdit = new QTextEdit("MQTT Log");

    /*************Box************/
    // = new QVBoxLayout();
    // = new QHBoxLayout();
    /*************Box************/
    //主Box
    Main_vLayout = new QVBoxLayout();  // vertical layout
	Main_vLayout->setAlignment(Qt::AlignLeft | Qt::AlignTop);
    //主box中第1行中的Camera
    Camera_hLayout = new QHBoxLayout();  // horizontal layout
    Camera_vLayout = new QVBoxLayout();  // vertical layout
	Camera_hLayout_2= new QHBoxLayout();  // horizontal layout
	Camera_hLayout_2->setAlignment(Qt::AlignLeft | Qt::AlignTop);
    //主box中第2行中的血氧和心率
    BloodOxygen_HeartRate_hLayout = new QHBoxLayout();
    BloodOxygen_vLayout = new QVBoxLayout();  // vertical layout
	BloodOxygen_hLayout= new QHBoxLayout();  // horizontal layout
	BloodOxygen_hLayout->setAlignment(Qt::AlignLeft | Qt::AlignTop);
    HeartRate_vLayout = new QVBoxLayout();  // vertical layout
	HeartRate_hLayout= new QHBoxLayout();  // horizontal layout
	HeartRate_hLayout->setAlignment(Qt::AlignLeft | Qt::AlignTop);

    //主box中第3行中的呼吸暂停综合症和报警设置
    ApneaSyndrome_AlarmSettings_hLayout = new QHBoxLayout();
    ApneaSyndrome_vLayout = new QVBoxLayout();  // vertical layout
	ApneaSyndrome_hLayout_1 = new QHBoxLayout;  // horizontal layout
	ApneaSyndrome_hLayout_1->setAlignment(Qt::AlignLeft | Qt::AlignTop);
    ApneaSyndrome_hLayout_2 = new QHBoxLayout;  // horizontal layout
	ApneaSyndrome_hLayout_2->setAlignment(Qt::AlignLeft | Qt::AlignTop);
    AlarmSettings_vLayout = new QVBoxLayout();  // vertical layout
	AlarmSettings_hLayout_1 = new QHBoxLayout;  // horizontal layout
	AlarmSettings_hLayout_1->setAlignment(Qt::AlignLeft | Qt::AlignTop);
    AlarmSettings_hLayout_2 = new QHBoxLayout;  // horizontal layout
	AlarmSettings_hLayout_2->setAlignment(Qt::AlignLeft | Qt::AlignTop);
    AlarmSettings_hLayout_3 = new QHBoxLayout;  // horizontal layout
	AlarmSettings_hLayout_3->setAlignment(Qt::AlignLeft | Qt::AlignTop);

    //主box中第4行中的播放音乐和MQTT
    Music_MQTT_hLayout = new QHBoxLayout();
    Music_vLayout = new QVBoxLayout();  // vertical layout
	Music_vLayout->setAlignment(Qt::AlignLeft | Qt::AlignTop);
    MQTT_vLayout = new QVBoxLayout();  // vertical layout
	MQTT_vLayout->setAlignment(Qt::AlignLeft | Qt::AlignTop);

	//为box添加控件
	//第一行
	Camera_hLayout_2->addWidget(Camera_Text_Label);
	Camera_hLayout_2->addWidget(Camera_SleepStatus_Label);
	Camera_vLayout->addLayout(Camera_hLayout_2);
	Camera_vLayout->addWidget(Camera_Log_TextEdit);
	Camera_hLayout->addWidget(Camera_Display_Label);
	Camera_hLayout->addLayout(Camera_vLayout);
	//第二行-血氧&心率
	BloodOxygen_hLayout->addWidget(BloodOxygen_Label);
	BloodOxygen_hLayout->addWidget(BloodOxygen_Lcdnumber);
	BloodOxygen_vLayout->addLayout(BloodOxygen_hLayout);
	BloodOxygen_vLayout->addWidget(BloodOxygen_Plot);
	HeartRate_hLayout->addWidget(HeartRate_Label);
	HeartRate_hLayout->addWidget(HeartRate_Lcdnumber);
	HeartRate_vLayout->addLayout(HeartRate_hLayout);
	HeartRate_vLayout->addWidget(HeartRate_Plot);
	BloodOxygen_HeartRate_hLayout->addLayout(BloodOxygen_vLayout);
	BloodOxygen_HeartRate_hLayout->addLayout(HeartRate_vLayout);
	//第三行-呼吸暂停&报警设置
	ApneaSyndrome_hLayout_1->addWidget(ApneaSyndrome_Label);
	ApneaSyndrome_hLayout_1->addWidget(ApneaSyndrome_PowerStatus_Label);
	ApneaSyndrome_hLayout_2->addWidget(ApneaSyndrome_TurnOn_Button);
	ApneaSyndrome_hLayout_2->addWidget(ApneaSyndrome_TurnOff_Button);
	ApneaSyndrome_vLayout->addLayout(ApneaSyndrome_hLayout_1);
	ApneaSyndrome_vLayout->addLayout(ApneaSyndrome_hLayout_2);
	ApneaSyndrome_vLayout->addWidget(ApneaSyndrome__Log_TextEdit);
	AlarmSettings_vLayout->addWidget(AlarmSettings_Label);
	AlarmSettings_hLayout_1->addWidget(AlarmSettings_BloodOxygen_Label);
	AlarmSettings_hLayout_1->addWidget(AlarmSettings_BloodOxygen_MinValue_QLineEdit);
	AlarmSettings_hLayout_1->addWidget(AlarmSettings_BloodOxygen_Connection_Label);
	AlarmSettings_hLayout_1->addWidget(AlarmSettings_BloodOxygen_MaxValue_QLineEdit);
	AlarmSettings_hLayout_2->addWidget(AlarmSettings_HeartRate_Label);
	AlarmSettings_hLayout_2->addWidget(AlarmSettings_HeartRate_MinValue_QLineEdit);
	AlarmSettings_hLayout_2->addWidget(AlarmSettings_HeartRate_Connection_Label);
	AlarmSettings_hLayout_2->addWidget(AlarmSettings_HeartRate_MaxValue_QLineEdit);
	AlarmSettings_hLayout_3->addWidget(AlarmSettings_ApneaSyndrome_Label);
	AlarmSettings_hLayout_3->addWidget(AlarmSettings_ApneaSyndrome_TurnOnOff_Label);
	AlarmSettings_hLayout_3->addWidget(AlarmSettings_ApneaSyndrome_TurnOn_Button);
	AlarmSettings_hLayout_3->addWidget(AlarmSettings_ApneaSyndrome_TurnOff_Button);
	AlarmSettings_vLayout->addLayout(AlarmSettings_hLayout_1);
	AlarmSettings_vLayout->addLayout(AlarmSettings_hLayout_2);
	AlarmSettings_vLayout->addLayout(AlarmSettings_hLayout_3);
	AlarmSettings_vLayout->addWidget(AlarmSettings_Update_Settings_Button);
	ApneaSyndrome_AlarmSettings_hLayout->addLayout(ApneaSyndrome_vLayout);
	ApneaSyndrome_AlarmSettings_hLayout->addLayout(AlarmSettings_vLayout);
	//第四行-音乐&MQTT
	Music_vLayout->addWidget(Music_Label);
	Music_vLayout->addWidget(Music_Play_Button);
	MQTT_vLayout->addWidget(MQTT_Label);
	MQTT_vLayout->addWidget(MQTT_Log_TextEdit);
	Music_MQTT_hLayout->addLayout(Music_vLayout);
	Music_MQTT_hLayout->addLayout(MQTT_vLayout);

	//主box
	Main_vLayout->addLayout(Camera_hLayout);
	Main_vLayout->addLayout(BloodOxygen_HeartRate_hLayout);
	Main_vLayout->addLayout(ApneaSyndrome_AlarmSettings_hLayout);
	Main_vLayout->addLayout(Music_MQTT_hLayout);
	setLayout(Main_vLayout);





	/*
	// set up the thermometer
	thermo = new QwtThermo; 
	thermo->setFillBrush( QBrush(Qt::red) );
	thermo->setScale(0, 10);
	thermo->show();


	// set up the initial plot data
	for( int index=0; index<plotDataSize; ++index )
	{
		xData[index] = index;
		yData[index] = 0;
	}

	curve = new QwtPlotCurve;
	plot = new QwtPlot;
	// make a plot curve from the data and attach it to the plot
	curve->setSamples(xData, yData, plotDataSize);
	curve->attach(plot);

	plot->replot();
	plot->show();

	button = new QPushButton("Reset");
	// see https://doc.qt.io/qt-5/signalsandslots-syntaxes.html
	connect(button,&QPushButton::clicked,this,&Window::reset);

	// set up the layout - button above thermometer
	vLayout = new QVBoxLayout();
	vLayout->addWidget(button);
	vLayout->addWidget(thermo);

	// plot to the left of button and thermometer
	hLayout = new QHBoxLayout();
	hLayout->addLayout(vLayout);
	hLayout->addWidget(plot);

	setLayout(hLayout);
	*/
}

void Window::reset() {
	// set up the initial plot data
	for( int index=0; index<plotDataSize; ++index )
	{
		xData[index] = index;
		yData[index] = 0;
	}
}


void Window::timerEvent( QTimerEvent * )
{
	/*
	double inVal = gain * sin( M_PI * count/50.0 );
	++count;

	// add the new input to the plot
	std::move( yData, yData + plotDataSize - 1, yData+1 );
	yData[0] = inVal;
	curve->setSamples(xData, yData, plotDataSize);
	plot->replot();

	// set the thermometer value
	thermo->setValue( fabs(inVal) );
	*/
}