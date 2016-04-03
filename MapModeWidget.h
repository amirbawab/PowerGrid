#pragma once

#include <QWidget>
#include "MapGraphicsView.h"
#include <QGridLayout>

class MapModeWidget : public QWidget {
public:
	MapModeWidget();
	~MapModeWidget();
private:
	MapGraphicsView *mapGraphicsView;
	QGridLayout *gridLayout;
};

