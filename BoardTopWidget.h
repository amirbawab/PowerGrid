#pragma once

#include <QWidget>
#include <QGridLayout>
#include <vector>
#include "Player.h"

class GameTurnWidget : public QWidget {
public:
	GameTurnWidget();
	~GameTurnWidget();
private:
	QGridLayout *gridLayout;
};

class BoardTopWidget : public QWidget {
public:
	std::vector<Player*> players;
	BoardTopWidget();
	~BoardTopWidget();
	
private:
	QGridLayout *gridLayout;
};
