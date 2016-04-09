#pragma once

#include <QMainWindow>
#include <QStackedWidget>

#include "WelcomeWidget.h"
#include "MapWidget.h"
#include "PlayerConfigWidget.h"
#include "BoardWidget.h"
#include "Observer.h"

class MainFrame : public QMainWindow,  public Observer {
	Q_OBJECT
public:
	MainFrame(string title);
	~MainFrame();
	void Update() override;
private:
	QStackedWidget *centerStackWidget;
	WelcomeWidget *welcomeWidget;
	MapWidget *mapWidget;
	PlayerConfigWidget *playerConfigWidget;
	BoardWidget *boardWidget;
	void paintEvent(QPaintEvent *) override;
	int welcomeWidgetIndex, mapWidgetIndex, playerConfigWidgetIndex, boardWidgetIndex;
private slots:
	void onNewGame();
	void onMapBack();
	void onMapNext();
	void onPlayerConfigBack();
	void onPlayerConfigNext();
};
