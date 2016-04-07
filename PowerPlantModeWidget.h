#pragma once

#include <QWidget>
#include <QPushButton>
#include <vector>
#include <QGridLayout>
#include <memory>
#include "DataStore.h"
#include <string>

class PowerPlantModeWidget : public QWidget {
    Q_OBJECT
public:
	PowerPlantModeWidget();
	~PowerPlantModeWidget();
	void Refresh();
    QPushButton *GetSelectedCard() { return selectedCard; }
private:
    QPushButton *selectedCard = nullptr;
    int per_row = 4;
	std::vector<QPushButton*> cards;
	QGridLayout *gridLayout;
signals:
    void CardSelected(int index);
};

