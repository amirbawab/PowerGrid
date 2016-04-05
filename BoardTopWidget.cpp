#include "BoardTopWidget.h"

// BoardTopWidget class

BoardTopWidget::BoardTopWidget() {

	// Init components
	gridLayout = new QGridLayout();
	gameTurnWidget = new GameTurnWidget();
	stepPhaseWidget = new StepPhaseWidget();
	scoreWidget = new ScoreWidget();

	// Set layout
	setLayout(gridLayout);

	// Set id
	setObjectName("boardTopWidget");

	// Add components
	gridLayout->addWidget(gameTurnWidget, 0, 0, Qt::AlignLeft);
	gridLayout->setColumnStretch(0, 2);
	gridLayout->setColumnStretch(1, 1);
	gridLayout->setColumnStretch(2, 2);
	gridLayout->addWidget(stepPhaseWidget, 0, 1, Qt::AlignCenter);
	gridLayout->addWidget(scoreWidget, 0, 2, Qt::AlignRight);
}

BoardTopWidget::~BoardTopWidget() {
	delete gameTurnWidget;
	delete stepPhaseWidget;
	delete gridLayout;
}

void BoardTopWidget::Refresh() {
	
	// Refresh widgets
	gameTurnWidget->Refresh();
	stepPhaseWidget->Refresh();
	scoreWidget->Refresh();
}

// GameTurnWidget class

GameTurnWidget::GameTurnWidget() {
	
	// Init components
	gridLayout = new QGridLayout();

	// Set layout
	setLayout(gridLayout);

	// Set players
	playersTurn = DataStore::getInstance().playersTurn;
}

void GameTurnWidget::Refresh() {
	
	// Clear old components
	for (int i = 0; i < playersTurnLabels.size(); i++) {
		gridLayout->removeWidget(playersTurnLabels[i]);
		delete playersTurnLabels[i];
		playersTurnLabels.erase(playersTurnLabels.begin() + i);
	}

	// Draw houses
	for (int i = 0; i < playersTurn.size(); i++) {
		QLabel *playerLabel = new QLabel();
		playerLabel->setPixmap(QPixmap(playersTurn[i]->GetColor()->getImage().c_str()));
		gridLayout->addWidget(playerLabel, 0, i*2, Qt::AlignCenter);

		QLabel *playerNameLabel = new QLabel(playersTurn[i]->GetInitials().c_str());
		playerNameLabel->setObjectName("turn_label");
		gridLayout->addWidget(playerNameLabel, 0, i*2+1, Qt::AlignLeft);

		playersTurnLabels.push_back(playerLabel);
		playersTurnLabels.push_back(playerNameLabel);
	}
}

GameTurnWidget::~GameTurnWidget() {
	delete gridLayout;
}

// StepPhaseWidget class

StepPhaseWidget::StepPhaseWidget() {

	// Init components
	gridLayout = new QGridLayout();
	stepNumberLabel = new QLabel();
	phaseNumberLabel = new QLabel();
	QLabel *stepLabel = new QLabel("Step");
	QLabel *phaseLabel = new QLabel("Phase");

	// Set ID
	stepNumberLabel->setObjectName("step_phase_label");
	phaseNumberLabel->setObjectName("step_phase_label");
	stepLabel->setObjectName("step_phase_label");
	phaseLabel->setObjectName("step_phase_label");

	// Set layout
	setLayout(gridLayout);

	// Add components
	gridLayout->addWidget(stepLabel, 0, 0, Qt::AlignLeft);
	gridLayout->addWidget(stepNumberLabel, 0, 1, Qt::AlignLeft);
	gridLayout->addWidget(phaseLabel, 1, 0, Qt::AlignLeft); 
	gridLayout->addWidget(phaseNumberLabel, 1, 1, Qt::AlignLeft);
}

StepPhaseWidget::~StepPhaseWidget() {
	delete phaseNumberLabel;
	delete stepNumberLabel;
	delete gridLayout;
}

void StepPhaseWidget::Refresh() {
	stepNumberLabel->setText(QString::fromStdString(std::to_string(DataStore::getInstance().step)));
	phaseNumberLabel->setText(QString::fromStdString(std::to_string(DataStore::getInstance().phase)));
}

// ScoreWidget class

ScoreWidget::ScoreWidget() {
	
	// Init components
	gridLayout = new QGridLayout();
	
	// Set layout
	setLayout(gridLayout);

	// Set players
	players = DataStore::getInstance().players;
}

ScoreWidget::~ScoreWidget() {
	delete gridLayout;
}

void ScoreWidget::Refresh() {

	// Clear old components
	for (int i = 0; i < playersScoreLabels.size(); i++) {
		gridLayout->removeWidget(playersScoreLabels[i]);
		delete playersScoreLabels[i];
		playersScoreLabels.erase(playersScoreLabels.begin() + i);
	}

	// Draw houses
	for (int i = 0; i < players.size(); i++) {
		QLabel *playerLabel = new QLabel();
		playerLabel->setPixmap(QPixmap(players[i]->GetColor()->getImage().c_str()));
		gridLayout->addWidget(playerLabel, 0, i * 2, Qt::AlignCenter);

		QLabel *playerNameLabel = new QLabel((players[i]->GetInitials() + ":" + std::to_string(players[i]->GetHouses().size())).c_str());
		playerNameLabel->setObjectName("turn_label");
		gridLayout->addWidget(playerNameLabel, 0, i * 2 + 1, Qt::AlignLeft);

		playersScoreLabels.push_back(playerLabel);
		playersScoreLabels.push_back(playerNameLabel);
	}
}