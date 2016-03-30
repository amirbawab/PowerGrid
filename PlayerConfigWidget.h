#pragma once

#include "TemplateNextBackWidget.h"
#include "PlayerConfigRowWidget.h"
#include "Player.h"
#include <vector>
#include "DataStore.h"
#include <memory>

class PlayerConfigWidget : public TemplateNextBackWidget {
	Q_OBJECT
private:
	void paintEvent(QPaintEvent *pe);
	std::vector<PlayerConfigRowWidget*> rows;
	std::vector<std::shared_ptr<Player>> players;
	std::vector<std::shared_ptr<HouseColor>> houseColors;
	QWidget *rowsWidget;
	QLabel *titleLabel;
	QVBoxLayout *vBoxLayout;
public:
	PlayerConfigWidget();
	~PlayerConfigWidget();
	void SetPlayers(std::vector<std::shared_ptr<Player>> &players) { this->players = players; }
	void SetHouseColors(std::vector<std::shared_ptr<HouseColor>> houseColors) { this->houseColors = houseColors; }
	void SetNumberOfPlayers(int num);
	bool HasError();
	void UpdatePlayersInfo();
};

