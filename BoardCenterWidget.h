#pragma once

#include <QWidget>
#include <QGridLayout>
#include <QStackedWidget>
#include <QPushButton>

class BoardCenterWidget : public QWidget {
public:
	BoardCenterWidget();
	~BoardCenterWidget();
private:
	QGridLayout *gridLayout;
	QStackedWidget *centerStackedWidget;
	QWidget *modeWidget;
	QPushButton *powerPlantsModeButton, *mapModeButton, *resourceModeButton, *overviewModeButton;
	QVBoxLayout *vBoxLayout;
};

