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
	void Refresh();
private:
	QGridLayout *gridLayout;
	std::vector<std::shared_ptr<Player>> playersTurn;
	std::vector<QLabel*> playersTurnLabels;
};

class StepPhaseWidget : public QWidget {
public:
	StepPhaseWidget();
	~StepPhaseWidget();
	void Refresh();
private:
	QGridLayout *gridLayout;
	QLabel *stepNumberLabel;
	QLabel *phaseNumberLabel;
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
	StepPhaseWidget *stepPhaseWidget;
};
