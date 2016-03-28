#pragma once

#include <QWidget>
#include <QGridLayout>
#include <QStackedWidget>
#include <QPushButton>
#include "PowerPlantModeWidget.h"
#include "ResourceMarketModeWidget.h"
#include <QStyleOption>
#include <QPainter>

class BoardCenterWidget : public QWidget {
	Q_OBJECT;
public:
	BoardCenterWidget();
	~BoardCenterWidget();
private:
	QGridLayout *gridLayout;
	QStackedWidget *centerStackedWidget;
	QWidget *modeWidget;
	QPushButton *powerPlantsModeButton, *mapModeButton, *resourceMarketModeButton, *overviewModeButton;
	QVBoxLayout *vBoxLayout;
	PowerPlantModeWidget *powerPlantModeWidget;
	ResourceMarketModeWidget* resourceMarketModeWidget;
	int powerPlantModeWidgetIndex, resourceMarketModeWidgetIndex;
	void paintEvent(QPaintEvent *pe);
private slots:
	void onMapMode();
	void onPowerPlantMode();
	void onResourceMarketMode();
	void onOverviewMode();
};

