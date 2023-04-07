#include "./Window/Window.h"

#include <QApplication>

int main(int argc, char **argv) {
	QApplication app(argc, argv);
    // create the window
	Window window;
	window.show();

	// call the window.timerEvent function every 40 ms
	window.startTimer(40);
	return app.exec();
}