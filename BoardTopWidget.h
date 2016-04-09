#pragma once

#include <QWidget>
#include <QGridLayout>
#include <vector>
#include <QLabel>

class GameTurnWidget : public QWidget {
public:
	GameTurnWidget();
	~GameTurnWidget();
	void Refresh();
private:
	QGridLayout *gridLayout;
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

class ScoreWidget : public QWidget {
public:
	ScoreWidget();
	~ScoreWidget();
	void Refresh();
private:
	QGridLayout *gridLayout;
	std::vector<QLabel*> playersScoreLabels;
};

class BoardTopWidget : public QWidget {
public:
	BoardTopWidget();
	~BoardTopWidget();
	void Refresh() const;
	
private:
	QGridLayout *gridLayout;
	GameTurnWidget *gameTurnWidget;
	StepPhaseWidget *stepPhaseWidget;
	ScoreWidget *scoreWidget;
};
