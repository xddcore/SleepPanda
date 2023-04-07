/*
* Note:
*		1.原来的呼吸暂停综合症报警开关从功能上被改为全局报警功能开关，但是没有相应的修改变量名(XXXXXX_Alarm_ApneaSyndrome)
*
*
*/

#include "Window.h"

#include <cmath>  // for sine stuff

#include <ctime> //log time

Camera_Settings My_Camera_Settings;
Camera MyCamera(My_Camera_Settings);

/**********************Sensor Class*****************************/
//MAX30101血氧心率传感器Callback函数
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
//声音传感器SoundSensor Callback函数
class Rpi_SoundSensor : public SoundSensor {
    using SoundSensor::SoundSensor;
    public:
        bool ISR_CheckPoint = false;
		unsigned int Event_Flag;
	    virtual void SoundSensor_SoundEvent_Handle() {
		    printf("Rpi_SoundSensor Class DEBUG: SoundSensor_SoundEvent_Handle() was triggered.\r\n");
			Event_Flag=1;
            ISR_CheckPoint = true;
	    }
};

//MAX30101
MAX30101_Settings My_MAX30101_Settings;
Rpi_MAX30101 My_Rpi_MAX30101(My_MAX30101_Settings);

//SoundSensor
SoundSensor_Settings My_SoundSensor_Settings;
Rpi_SoundSensor My_Rpi_SoundSensor(My_SoundSensor_Settings);

//Camera
unsigned int Sleep_On_frontOrSide=0;//front = 0, side = 1

//Buzzer
Buzzer_Settings My_Buzzer_Settings;
Buzzer MyBuzzer(My_Buzzer_Settings);

//呼吸暂停综合症蜂鸣器报警开关
unsigned int TurnOnOff_Alarm_ApneaSyndrome=0;//0:off 1:on

//报警阈值(定义了正常值范围，不在正常值范围内则报警)
double BloodOxygen_MinValue=99.0;
double BloodOxygen_MAXValue=100.0;
int32_t HeartRate_MinValue=60;
int32_t HeartRate_MaxValue=90;


/**********************Windows Class*****************************/
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
	BloodOxygen_Curve->setSamples(BloodOxygen_xData, BloodOxygen_yData, plotDataSize);
	BloodOxygen_Curve->attach(BloodOxygen_Plot);
	BloodOxygen_Plot->replot();
	BloodOxygen_Plot->show();
	// set up the initial plot data
	for( int index=0; index<plotDataSize; ++index )
	{
		BloodOxygen_xData[index] = index;
		BloodOxygen_yData[index] = 0;
	}
    //心率控件
    HeartRate_Label = new QLabel("Heart Rate(bpm):");
    HeartRate_Lcdnumber = new QLCDNumber;
    HeartRate_Plot = new QwtPlot;
	HeartRate_Curve = new QwtPlotCurve;
	HeartRate_Curve->setSamples(HeartRate_xData, HeartRate_yData, plotDataSize);
	HeartRate_Curve->attach(HeartRate_Plot);
	HeartRate_Plot->replot();
	HeartRate_Plot->show();
	// set up the initial plot data
	for( int index=0; index<plotDataSize; ++index )
	{
		HeartRate_xData[index] = index;
		HeartRate_yData[index] = 0;
	}
    //呼吸暂停综合症
    ApneaSyndrome_Label = new QLabel("ApneaSyndrome Detector Power: ");
    ApneaSyndrome_PowerStatus_Label = new QLabel;
	ApneaSyndrome_PowerStatus_Label->setText("Turn On / Turn Off");
    ApneaSyndrome_TurnOn_Button = new QPushButton("Turn On"); 
	connect(ApneaSyndrome_TurnOn_Button,&QPushButton::clicked,this,&Window::TurnOn_SoundSensor);
    ApneaSyndrome_TurnOff_Button = new QPushButton("Turn Off");
	connect(ApneaSyndrome_TurnOff_Button,&QPushButton::clicked,this,&Window::TurnOff_SoundSensor);
    ApneaSyndrome__Log_TextEdit = new QTextEdit("ApneaSyndrome Detector Log");
    //报警设置
	IntValidator = new QIntValidator(0, 200, this);//设置整数检测器，防止用户输入无效值
	DoubleValidator = new QDoubleValidator(0.0, 100.0, 2,this);//设置double检测器，防止用户输入无效值
    AlarmSettings_Label = new QLabel("Alarm Settings");
	AlarmSettings_BloodOxygen_Label = new QLabel("Blood Oxygen Threshold: ");
	AlarmSettings_BloodOxygen_MinValue_QLineEdit = new QLineEdit("99.0");
	AlarmSettings_BloodOxygen_MinValue_QLineEdit->setValidator(DoubleValidator);//应用double检测器
	AlarmSettings_BloodOxygen_Connection_Label = new QLabel("--");
    AlarmSettings_BloodOxygen_MaxValue_QLineEdit = new QLineEdit("100.0");
	AlarmSettings_BloodOxygen_MaxValue_QLineEdit->setValidator(DoubleValidator);//应用double检测器
    AlarmSettings_HeartRate_Label = new QLabel("Heart Rate Threshold: ");
	AlarmSettings_HeartRate_MinValue_QLineEdit = new QLineEdit("60");
	AlarmSettings_HeartRate_MinValue_QLineEdit->setValidator(IntValidator);//应用整数检测器
	AlarmSettings_HeartRate_Connection_Label = new QLabel("--");
    AlarmSettings_HeartRate_MaxValue_QLineEdit = new QLineEdit("90");
	AlarmSettings_HeartRate_MaxValue_QLineEdit->setValidator(IntValidator);//应用整数检测器
    AlarmSettings_ApneaSyndrome_Label = new QLabel("Alarm Switch: ");
	AlarmSettings_ApneaSyndrome_TurnOnOff_Label = new QLabel("Turn On / Turn Off");
    AlarmSettings_ApneaSyndrome_TurnOn_Button = new QPushButton("Turn On");
	connect(AlarmSettings_ApneaSyndrome_TurnOn_Button,&QPushButton::clicked,this,&Window::TurnOn_Alarm_ApneaSyndrome);
    AlarmSettings_ApneaSyndrome_TurnOff_Button = new QPushButton("Turn Off");
	connect(AlarmSettings_ApneaSyndrome_TurnOff_Button,&QPushButton::clicked,this,&Window::TurnOff_Alarm_ApneaSyndrome);
	AlarmSettings_Update_Settings_Button = new QPushButton("Update Settings");
	connect(AlarmSettings_Update_Settings_Button,&QPushButton::clicked,this,&Window::Update_Alarm_Threshold);
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
	//取消第四行的功能(音乐播放器和MQTT服务器)
	//Main_vLayout->addLayout(Music_MQTT_hLayout);
	//设置Window(QWidget)的Layout
	setLayout(Main_vLayout);

	/*****************************第一行:摄像头*******************************************************/
	myCallback.window = this;
	MyCamera.registerSceneCallback(&myCallback);
	MyCamera.Start();
	/*****************************第二行:心率和血氧*******************************************************/
	My_Rpi_MAX30101.Start();




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

//打开声音传感器，以监测呼吸暂停综合症
void Window::TurnOn_SoundSensor() {
	ApneaSyndrome_PowerStatus_Label->setText("Turn On");
	My_Rpi_SoundSensor.Start();
}
//关闭声音传感器，以暂停监测呼吸暂停综合症
void Window::TurnOff_SoundSensor() {
	ApneaSyndrome_PowerStatus_Label->setText("Turn Off");
	My_Rpi_SoundSensor.Stop();
}
void Window::Update_Alarm_Threshold() {
	//设置血氧的报警阈值
	QString AlarmSettings_BloodOxygen_MinValue_QString = AlarmSettings_BloodOxygen_MinValue_QLineEdit->text();
	QString AlarmSettings_BloodOxygen_MaxValue_QString = AlarmSettings_BloodOxygen_MaxValue_QLineEdit->text();
	BloodOxygen_MinValue = AlarmSettings_BloodOxygen_MinValue_QString.toDouble();
	BloodOxygen_MAXValue = AlarmSettings_BloodOxygen_MaxValue_QString.toDouble(); 
	//设置心率的报警阈值
	QString AlarmSettings_HeartRate_MinValue_QString = AlarmSettings_HeartRate_MinValue_QLineEdit->text();
	QString AlarmSettings_HeartRate_MaxValue_QString = AlarmSettings_HeartRate_MaxValue_QLineEdit->text();
	HeartRate_MinValue = AlarmSettings_HeartRate_MinValue_QString.toInt();
	HeartRate_MaxValue = AlarmSettings_HeartRate_MaxValue_QString.toInt();
}

void Window::TurnOn_Alarm_ApneaSyndrome(){
	TurnOnOff_Alarm_ApneaSyndrome = 1;
	AlarmSettings_ApneaSyndrome_TurnOnOff_Label->setText("Turn On");
}
void Window::TurnOff_Alarm_ApneaSyndrome(){
	TurnOnOff_Alarm_ApneaSyndrome = 0;
	MyBuzzer.Buzzer_Contorl(Buzzer_Off);
	AlarmSettings_ApneaSyndrome_TurnOnOff_Label->setText("Turn Off");
}

Window::~Window()
{
	MyCamera.Stop();
	My_Rpi_SoundSensor.Stop();
	My_Rpi_MAX30101.Stop();
	MyBuzzer.Buzzer_Contorl(Buzzer_Off);
}

//摄像头更新图像Callback
void Window::updateImage(const cv::Mat &mat) {
	const QImage frame(mat.data, mat.cols, mat.rows, mat.step,
			   QImage::Format_RGB888);
	Camera_Display_Label->setPixmap(QPixmap::fromImage(frame));
	//只要检测到侧脸或侧眼睛，用户都是侧睡
	if(MyCamera.Profile_Face_Position.size()>0 || MyCamera.Profile_Face_Eye_Position.size()>0)
	{
		Camera_SleepStatus_Label->setText("Sleeping On The Side.");//Sleeping On The Front / Side
		if(Sleep_On_frontOrSide==0)//刚才的状态是侧睡，状态改变，打印Log
		{	
			//log时间
			
			char Log_char8[1000];
			time_t rawtime;
    		struct tm *ptminfo;
 			time(&rawtime);
    		ptminfo = localtime(&rawtime);
    		sprintf(Log_char8,"Sleeping On The Side.｜Time: %02d-%02d-%02d %02d:%02d:%02d\n",
            ptminfo->tm_year + 1900, ptminfo->tm_mon + 1, ptminfo->tm_mday,
            ptminfo->tm_hour, ptminfo->tm_min, ptminfo->tm_sec);
			
			string Log_String = Log_char8;
			Camera_Log_TextEdit->append(QString::fromStdString(Log_String));
		}
		Sleep_On_frontOrSide=1;
		MyCamera.Frontal_Face_Position.clear();
   		MyCamera.Frontal_Face_Eye_Position.clear();
    	MyCamera.Profile_Face_Position.clear();
    	MyCamera.Profile_Face_Eye_Position.clear();
	}
	else//其他情况用户都是正睡
	{
		Camera_SleepStatus_Label->setText("Sleeping On The Front.");//Sleeping On The Front / Side
		if(Sleep_On_frontOrSide==1)//刚才的状态是侧睡，状态改变，打印Log
		{	
			//log时间
			
			char Log_char8[1000];
			time_t rawtime;
    		struct tm *ptminfo;
 			time(&rawtime);
    		ptminfo = localtime(&rawtime);
    		sprintf(Log_char8,"Sleeping On The Front.｜Time: %02d-%02d-%02d %02d:%02d:%02d\n",
            ptminfo->tm_year + 1900, ptminfo->tm_mon + 1, ptminfo->tm_mday,
            ptminfo->tm_hour, ptminfo->tm_min, ptminfo->tm_sec);
			
			string Log_String = Log_char8;
			Camera_Log_TextEdit->append(QString::fromStdString(Log_String));
		}
		Sleep_On_frontOrSide=0;
		MyCamera.Frontal_Face_Position.clear();
   		MyCamera.Frontal_Face_Eye_Position.clear();
    	MyCamera.Profile_Face_Position.clear();
    	MyCamera.Profile_Face_Eye_Position.clear();
	}
	//const int h = frame.height();
	//const int w = frame.width();
	//const QColor c = frame.pixelColor(w/2, h/2);
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
	
	/*****************************第二行:心率和血氧*******************************************************/
	HeartRate_Lcdnumber->display(My_Rpi_MAX30101.heart_rate_test);//心率数值更新
	// add the new input to the plot
	std::move( HeartRate_yData, HeartRate_yData + plotDataSize - 1, HeartRate_yData+1 );
	HeartRate_yData[0] = My_Rpi_MAX30101.heart_rate_test;
	HeartRate_Curve->setSamples(HeartRate_xData, HeartRate_yData, plotDataSize);
	HeartRate_Plot->replot();

	BloodOxygen_Lcdnumber->display(My_Rpi_MAX30101.spo2_test);//血氧数值更新
	// add the new input to the plot
	std::move( BloodOxygen_yData, BloodOxygen_yData + plotDataSize - 1, BloodOxygen_yData+1 );
	BloodOxygen_yData[0] = My_Rpi_MAX30101.spo2_test;
	BloodOxygen_Curve->setSamples(BloodOxygen_xData, BloodOxygen_yData, plotDataSize);
	BloodOxygen_Plot->replot();
	/*****************************第三行左栏:声音传感器*******************************************************/
	if(My_Rpi_SoundSensor.Event_Flag){//用户出现呼吸暂停综合症。
		char Log_char8[1000];
		time_t rawtime;
    	struct tm *ptminfo;
 		time(&rawtime);
    	ptminfo = localtime(&rawtime);
    	sprintf(Log_char8,"User suffers from apnea syndrome.｜Time: %02d-%02d-%02d %02d:%02d:%02d\n",
        ptminfo->tm_year + 1900, ptminfo->tm_mon + 1, ptminfo->tm_mday,
        ptminfo->tm_hour, ptminfo->tm_min, ptminfo->tm_sec);
			
		string Log_String = Log_char8;
		ApneaSyndrome__Log_TextEdit->append(QString::fromStdString(Log_String));
		My_Rpi_SoundSensor.Event_Flag=0;
	}
	else{//没有出现呼吸暂停综合症
		//心率，血氧，呼吸暂停综合症都没问题才关蜂鸣器报警
		if((My_Rpi_MAX30101.heart_rate_test>=HeartRate_MinValue && My_Rpi_MAX30101.heart_rate_test<= HeartRate_MaxValue) &&\
			(My_Rpi_MAX30101.spo2_test>=BloodOxygen_MinValue && My_Rpi_MAX30101.spo2_test<= BloodOxygen_MAXValue) )
			MyBuzzer.Buzzer_Contorl(Buzzer_Off);
	}
	/*****************************第四行右栏:蜂鸣器报警+报警阈值设置*******************************************************/
	//心率报警
	if(My_Rpi_MAX30101.heart_rate_test>=HeartRate_MinValue && My_Rpi_MAX30101.heart_rate_test<= HeartRate_MaxValue){
		//在正常范围内不报警
		//MyBuzzer.Buzzer_Contorl(Buzzer_Off);
	}
	else{//不在正常范围内，报警
		if(TurnOnOff_Alarm_ApneaSyndrome==1){//报警开关打开
		MyBuzzer.Buzzer_Contorl(Buzzer_On);
		}
	}
	//血氧报警
	if(My_Rpi_MAX30101.spo2_test>=BloodOxygen_MinValue && My_Rpi_MAX30101.spo2_test<= BloodOxygen_MAXValue){
		//在正常范围内不报警
		//MyBuzzer.Buzzer_Contorl(Buzzer_Off);
	}
	else{//不在正常范围内，报警
		if(TurnOnOff_Alarm_ApneaSyndrome==1){//报警开关打开
		MyBuzzer.Buzzer_Contorl(Buzzer_On);
		}
	}
}