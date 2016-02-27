#include "Application.h"

Application::Application(int argc, char *argv[]) : QApplication(argc, argv) {
	
	// Load font
	QFontDatabase::addApplicationFont(":/PowerGrid/Resources/fonts/Hemmet.ttf");
}