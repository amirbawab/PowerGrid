#include <qfile.h>
#include <qstring.h>

#include "mainframe.h"
#include "application.h"
#include <memory>

#include "Game.h"

int main(int argc, char *argv[]) {

	// Create game
	Game game;

	// Application
	Application application(argc, argv);

	// Load css
	QFile File(":/PowerGrid/Resources/stylesheet.qss");
	File.open(QFile::ReadOnly);
	QString StyleSheet = QLatin1String(File.readAll());

	// Main frame
	MainFrame mainframe("Power Grid");

	// Attach observer
	game.Attach(&mainframe);

	// Apply css
	application.setStyleSheet(StyleSheet);

	// Return
	return application.exec();
}