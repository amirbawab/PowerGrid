#pragma once

#include <QWidget>
#include <QPushButton>
#include <vector>
#include "PowerPlantCard.h"
#include <QGridLayout>

class PowerPlantModeWidget : public QWidget {
public:
	PowerPlantModeWidget();
	~PowerPlantModeWidget();
private:
	int per_row = 4;
	std::vector<PowerPlantCard*> cards;
	QGridLayout *gridLayout;
};

