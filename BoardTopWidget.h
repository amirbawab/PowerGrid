#pragma once

#include <QWidget>
#include <QGridLayout>
#include <vector>
#include "Player.h"

class GameTurnWidget : public QWidget {
public:
	GameTurnWidget();
	~GameTurnWidget();
	void SetPlayer(std::vector<Player*> &players) { this->players = players; }
private:
	QGridLayout *gridLayout;
	std::vector<Player*> players;
};

class BoardTopWidget : public QWidget {
public:
	std::vector<Player*> players;
	BoardTopWidget();
	~BoardTopWidget();
	
private:
	QGridLayout *gridLayout;
};
