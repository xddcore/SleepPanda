#ifndef _WINDOW_H
#define _WINDOW_H

//Qwt控件
#include <qwt/qwt_thermo.h>
#include <qwt/qwt_plot.h>
#include <qwt/qwt_plot_curve.h>


//Qt Weights控件
//#include <QtWidgets> //包含所有QT5控件，这将会增加编译时间
#include <QBoxLayout>
#include <QPushButton>
#include <QLCDNumber>
#include <QLabel>
#include <QTextEdit>
#include <QLineEdit>


// class definition 'Window'
class Window : public QWidget
{
	// must include the Q_OBJECT macro for for the Qt signals/slots framework to work with this class
	Q_OBJECT

public:
	Window(); // default constructor - called when a Window is declared without arguments

	void timerEvent( QTimerEvent * );

// internal variables for the window class
private:
	static constexpr int plotDataSize = 100;
	static constexpr double gain = 7.5;

	QPushButton  *button;
	QwtThermo    *thermo;
	QwtPlot      *plot;
	QwtPlotCurve *curve;

    /*************Box************/
    //主Box
    QVBoxLayout  *Main_vLayout;  // vertical layout

    //主box中第1行中的Camera
    QHBoxLayout  *Camera_hLayout;  // horizontal layout
    QVBoxLayout  *Camera_vLayout;  // vertical layout
    QHBoxLayout  *Camera_hLayout_2;  // horizontal layout
    //主box中第2行中的血氧和心率
    QHBoxLayout  *BloodOxygen_HeartRate_hLayout;
    QVBoxLayout  *BloodOxygen_vLayout;  // vertical layout
    QHBoxLayout  *BloodOxygen_hLayout;  // horizontal layout
    QVBoxLayout  *HeartRate_vLayout;  // vertical layout
    QHBoxLayout  *HeartRate_hLayout;  // horizontal layout

    //主box中第3行中的呼吸暂停综合症和报警设置
    QHBoxLayout  *ApneaSyndrome_AlarmSettings_hLayout;
    QVBoxLayout  *ApneaSyndrome_vLayout;  // vertical layout
    QHBoxLayout  *ApneaSyndrome_hLayout_1;  // horizontal layout
    QHBoxLayout  *ApneaSyndrome_hLayout_2;  // horizontal layout
    QVBoxLayout  *AlarmSettings_vLayout;  // vertical layout
    QHBoxLayout  *AlarmSettings_hLayout_1;  // horizontal layout
    QHBoxLayout  *AlarmSettings_hLayout_2;  // horizontal layout
    QHBoxLayout  *AlarmSettings_hLayout_3;  // horizontal layout

    //主box中第4行中的播放音乐和MQTT
    QHBoxLayout  *Music_MQTT_hLayout;
    QVBoxLayout  *Music_vLayout;  // vertical layout
    QVBoxLayout  *MQTT_vLayout;  // vertical layout


    /*************控件************/
    //测试图片
    QImage *Test_Image;
    //摄像头
    QLabel *Camera_Display_Label;
    QLabel *Camera_Text_Label;
    QLabel *Camera_SleepStatus_Label;
    QTextEdit *Camera_Log_TextEdit;
    //血氧控件
    QLabel *BloodOxygen_Label;
    QLCDNumber *BloodOxygen_Lcdnumber;
    QwtPlot      *BloodOxygen_Plot;
	QwtPlotCurve *BloodOxygen_Curve;
    //心率控件
    QLabel *HeartRate_Label;
    QLCDNumber *HeartRate_Lcdnumber;
    QwtPlot      *HeartRate_Plot;
	QwtPlotCurve *HeartRate_Curve;
    //呼吸暂停综合症
    QLabel *ApneaSyndrome_Label;
    QLabel *ApneaSyndrome_PowerStatus_Label;
    QPushButton  *ApneaSyndrome_TurnOn_Button;
    QPushButton  *ApneaSyndrome_TurnOff_Button;
    QTextEdit *ApneaSyndrome__Log_TextEdit;
    //报警设置
    QLabel *AlarmSettings_Label;
    QLabel *AlarmSettings_BloodOxygen_Label;
    QLineEdit *AlarmSettings_BloodOxygen_MinValue_QLineEdit;
    QLabel *AlarmSettings_BloodOxygen_Connection_Label;
    QLineEdit *AlarmSettings_BloodOxygen_MaxValue_QLineEdit;
    QLabel *AlarmSettings_HeartRate_Label;
    QLineEdit *AlarmSettings_HeartRate_MinValue_QLineEdit;
    QLabel *AlarmSettings_HeartRate_Connection_Label;
    QLineEdit *AlarmSettings_HeartRate_MaxValue_QLineEdit;
    QLabel *AlarmSettings_ApneaSyndrome_Label;
    QLabel *AlarmSettings_ApneaSyndrome_TurnOnOff_Label;
    QPushButton  *AlarmSettings_ApneaSyndrome_TurnOn_Button;
    QPushButton  *AlarmSettings_ApneaSyndrome_TurnOff_Button;
    QPushButton  *AlarmSettings_Update_Settings_Button;
    //播放助眠音乐
    QLabel *Music_Label;
    QPushButton  *Music_Play_Button;
    //MQTT
    QLabel *MQTT_Label;
    QTextEdit *MQTT_Log_TextEdit;

	// layout elements from Qt itself http://qt-project.org/doc/qt-4.8/classes.html
	QVBoxLayout  *vLayout;  // vertical layout
	QHBoxLayout  *hLayout;  // horizontal layout

	// data arrays for the plot
	double xData[plotDataSize];
	double yData[plotDataSize];

	long count = 0;

	void reset();
};

#endif // WINDOW_H