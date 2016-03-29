#pragma once

#include "TemplateNextBackWidget.h"
#include "PlayerConfigRowWidget.h"
#include "Player.h"
#include <vector>

class PlayerConfigWidget : public TemplateNextBackWidget {
	Q_OBJECT
private:
	void paintEvent(QPaintEvent *pe);
	std::vector<PlayerConfigRowWidget*> rows;
	std::vector<Player*> players;
	std::vector<HouseColor*> houseColors;
	QWidget *rowsWidget;
	QLabel *titleLabel;
	QVBoxLayout *vBoxLayout;
public:
	PlayerConfigWidget();
	~PlayerConfigWidget();
	void SetPlayers(std::vector<Player*> &players) { this->players = players; }
	void SetHouseColrs(std::vector<HouseColor*> houseColors) { this->houseColors = houseColors; }
	void SetNumberOfPlayers(int num);
};

