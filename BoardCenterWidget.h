#pragma once

#include <QWidget>
#include <QGridLayout>
#include <QStackedWidget>
#include <QPushButton>
#include "PowerPlantModeWidget.h"
#include "ResourceMarketModeWidget.h"
#include "OverviewModeWidget.h"
#include <QStyleOption>
#include <QPainter>
#include "MapModeWidget.h"

class BoardCenterWidget : public QWidget {
	Q_OBJECT;
public:
	BoardCenterWidget();
	~BoardCenterWidget();
	void Refresh();
private:
	QGridLayout *gridLayout;
	QStackedWidget *centerStackedWidget;
	QWidget *modeWidget;
	QPushButton *powerPlantsModeButton, *mapModeButton, *resourceMarketModeButton, *overviewModeButton;
	QVBoxLayout *vBoxLayout;
	PowerPlantModeWidget *powerPlantModeWidget;
	ResourceMarketModeWidget* resourceMarketModeWidget;
	OverviewModeWidget *overviewModeWidget;
	MapModeWidget* mapModeWidget;
	int powerPlantModeWidgetIndex, resourceMarketModeWidgetIndex, overviewModeWidgetIndex, mapModeWidgetIndex;
	void paintEvent(QPaintEvent *pe);
private slots:
	void onMapMode();
	void onPowerPlantMode();
	void onResourceMarketMode();
	void onOverviewMode();
};

