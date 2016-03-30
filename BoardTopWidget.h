#pragma once

#include <QWidget>
#include <QGridLayout>
#include <vector>
#include "Player.h"
#include "DataStore.h"
#include <memory>

class GameTurnWidget : public QWidget {
public:
	GameTurnWidget();
	~GameTurnWidget();
	void SetPlayers(std::vector<std::shared_ptr<Player>> &players) { this->players = players; }
private:
	QGridLayout *gridLayout;
	std::vector<std::shared_ptr<Player>> players;
};

class BoardTopWidget : public QWidget {
public:
	std::vector<std::shared_ptr<Player>> players;
	void SetPlayers(std::vector<std::shared_ptr<Player>> &players);
	BoardTopWidget();
	~BoardTopWidget();
	
private:
	QGridLayout *gridLayout;
	GameTurnWidget *gameTurnWidget;
};
