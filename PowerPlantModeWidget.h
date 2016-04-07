#pragma once

#include <QWidget>
#include <QPushButton>
#include <vector>
#include <QGridLayout>
#include <memory>
#include "DataStore.h"
#include <string>

class PowerPlantModeWidget : public QWidget {
public:
	PowerPlantModeWidget();
	~PowerPlantModeWidget();
	void Refresh();
    const int NONE = -1;
    QPushButton *selectedCard = nullptr;
private:
	int per_row = 4;
	std::vector<QPushButton*> cards;
	QGridLayout *gridLayout;
};

