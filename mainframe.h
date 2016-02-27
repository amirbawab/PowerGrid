#ifndef MAINFRAME_H
#define MAINFRAME_H

#include <QMainWindow>
#include <QMenuBar>
#include <QStyle>
#include <Qt>
#include <QApplication>
#include <QDesktopWidget>
#include <QToolBar>
#include <QTextEdit>
#include <QStyleOption>
#include <QPainter>
#include <QStackedWidget>

#include "Application.h"
#include "WelcomeWidget.h"
#include "MapWidget.h"
#include "PlayerConfigWidget.h"

class MainFrame : public QMainWindow {
	Q_OBJECT
public:
	MainFrame(char*, Application*);
	~MainFrame();
private:
	QStackedWidget *centerStackWidget;
	WelcomeWidget *welcomeWidget;
	MapWidget *mapWidget;
	PlayerConfigWidget *playerConfigWidget;
	void paintEvent(QPaintEvent *);
	int welcomeWidgetIndex, mapWidgetIndex, playerConfigWidgetIndex;
private slots:
	void onNewGame();
	void onMapBack();
	void onMapNext();
	void onPlayerConfigBack();
};

#endif // MAINFRAME_H