#include "Window.h"

#include <cmath>  // for sine stuff


Window::Window()
{
    /*************Box************/
    // = new QVBoxLayout();
    // = new QHBoxLayout();
    //主Box
    Main_vLayout = new QVBoxLayout();
    Main_vLayout->addLayout(V1_vLayout);
    Main_vLayout->addLayout(V2_vLayout);
    Main_vLayout->addLayout(V3_vLayout);
    //Test
    Main_vLayout->addLayout(hLayout);
    //主box中第1行
    V1_vLayout = new QVBoxLayout();  // vertical layout
    V1_vLayout->addLayout(Camera_hLayout);
    //主box中第2行
    V2_vLayout = new QVBoxLayout();  // vertical layout
    V2_vLayout->addLayout(BloodOxygen_HeartRate_hLayout);
    //主box中第3行
    V3_vLayout = new QVBoxLayout();  // vertical layout
    V3_vLayout->addLayout(ApneaSyndrome_AlarmSettings_hLayout);
    //主box中第1行中的Camera
    Camera_hLayout = new QHBoxLayout();;  // horizontal layout
    Camera_vLayout = new QVBoxLayout();  // vertical layout

    //主box中第2行中的血氧和心率
    BloodOxygen_HeartRate_hLayout = new QHBoxLayout();;
    BloodOxygen_vLayout = new QVBoxLayout();  // vertical layout
    HeartRate_vLayout = new QVBoxLayout();  // vertical layout

    //主box中第3行中的呼吸暂停综合症和报警设置
    ApneaSyndrome_AlarmSettings_hLayout = new QHBoxLayout();;
    ApneaSyndrome_vLayout = new QVBoxLayout();  // vertical layout
    AlarmSettings_vLayout = new QVBoxLayout();  // vertical layout


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
	double inVal = gain * sin( M_PI * count/50.0 );
	++count;

	// add the new input to the plot
	std::move( yData, yData + plotDataSize - 1, yData+1 );
	yData[0] = inVal;
	curve->setSamples(xData, yData, plotDataSize);
	plot->replot();

	// set the thermometer value
	thermo->setValue( fabs(inVal) );
}