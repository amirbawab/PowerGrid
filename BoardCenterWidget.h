#pragma once

#include <QWidget>
#include <QGridLayout>
#include <QStackedWidget>
#include <QPushButton>
#include "PowerPlantModeWidget.h"

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
	PowerPlantModeWidget *powerPlantModeWidget;
	int powerPlantModeWidgetIndex;
};

