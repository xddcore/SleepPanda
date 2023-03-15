#ifndef _WINDOW_H
#define _WINDOW_H

//Qwt控件
#include <qwt/qwt_thermo.h>
#include <qwt/qwt_plot.h>
#include <qwt/qwt_plot_curve.h>

//#include <QtWidgets> //包含所有QT5控件，这将会增加编译时间

#include <QBoxLayout>
#include <QPushButton>

#include <QLCDNumber>
#include <QLabel>


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
    //主box中第1行
    QVBoxLayout  *V1_vLayout;  // vertical layout
    //主box中第2行
    QVBoxLayout  *V2_vLayout;  // vertical layout
    //主box中第3行
    QVBoxLayout  *V3_vLayout;  // vertical layout

    //主box中第1行中的Camera
    QHBoxLayout  *Camera_hLayout;  // horizontal layout
    QVBoxLayout  *Camera_vLayout;  // vertical layout

    //主box中第2行中的血氧和心率
    QHBoxLayout  *BloodOxygen_HeartRate_hLayout;
    QVBoxLayout  *BloodOxygen_vLayout;  // vertical layout
    QVBoxLayout  *HeartRate_vLayout;  // vertical layout

    //主box中第3行中的呼吸暂停综合症和报警设置
    QHBoxLayout  *ApneaSyndrome_AlarmSettings_hLayout;
    QVBoxLayout  *ApneaSyndrome_vLayout;  // vertical layout
    QVBoxLayout  *AlarmSettings_vLayout;  // vertical layout



    /*************控件************/
    //血氧控件
    QLabel *Blood_Oxygen_Label;
    QLCDNumber *Blood_Oxygen_Lcdnumber;
    QwtPlot      *Blood_Oxygen_Plot;
	QwtPlotCurve *Blood_Oxygen_Curve;

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