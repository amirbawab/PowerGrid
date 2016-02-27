#ifndef APPLICATION_H
#define APPLICATION_H

#include <QApplication>
#include <QFontDatabase>

class Application : public QApplication {
public:
	Application(int, char*[]);
};

#endif // APPLICATION_H