#pragma once

#include <QWidget>
#include <QGridLayout>
#include <vector>
#include "Player.h"
#include "DataStore.h"
#include <memory>
#include <QLabel>

class GameTurnWidget : public QWidget {
public:
	GameTurnWidget();
	~GameTurnWidget();
	void UpdatePlayersTurn();
private:
	QGridLayout *gridLayout;
	std::vector<std::shared_ptr<Player>> playersTurn;
	std::vector<QLabel*> playersTurnLabels;
};

class BoardTopWidget : public QWidget {
public:
	std::vector<std::shared_ptr<Player>> players;
	BoardTopWidget();
	~BoardTopWidget();
	void Refresh();
	
private:
	QGridLayout *gridLayout;
	GameTurnWidget *gameTurnWidget;
};
