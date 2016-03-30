#pragma once

#include "HouseColor.h"
#include "Player.h"
#include <qwidget.h>
#include <QLineEdit>
#include <QHBoxLayout>
#include <string>
#include <QPushButton>
#include <vector>
#include <QPaintEvent>
#include <QStyleOption>
#include <QPainter>
#include <memory>

using std::string;

class PlayerConfigRowWidget : public QWidget {
	Q_OBJECT
private:
	QLineEdit *playerName;
	QHBoxLayout *hBoxLayout;
	QPushButton *houseButton;
	std::vector<std::shared_ptr<HouseColor>> houseColors;
	std::shared_ptr<Player> player;
	int houseColorIndex;
	void paintEvent(QPaintEvent *pe);
	
public:
	PlayerConfigRowWidget(string name);
	~PlayerConfigRowWidget();
	void SetPlayerName(string name);
	string GetPlayerName() const { return playerName->text().toStdString(); }
	void SetHouseColors(std::vector<std::shared_ptr<HouseColor>> &houseColors) { this->houseColors = houseColors; }
	void SetHouseColorIndex(int houseColorIndex);
	void SetPlayer(std::shared_ptr<Player> player) { this->player = player; }
	std::shared_ptr<Player> GetPlayer() { return player; }
	std::shared_ptr<HouseColor> GetHouseColor() { return houseColors[houseColorIndex]; }
	void UpdatePlayer();

private slots:
	void OnHouseClick();
};

