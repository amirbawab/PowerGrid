#include "BoardTopWidget.h"

// BoardTopWidget class

BoardTopWidget::BoardTopWidget() {

	// Init components
	gridLayout = new QGridLayout();
	gameTurnWidget = new GameTurnWidget();
	stepPhaseWidget = new StepPhaseWidget();

	// Set layout
	setLayout(gridLayout);

	// Set id
	setObjectName("boardTopWidget");

	// Add components
	gridLayout->addWidget(gameTurnWidget, 0, 0, Qt::AlignLeft);
	gridLayout->addWidget(stepPhaseWidget, 0, 1, Qt::AlignCenter);
}

BoardTopWidget::~BoardTopWidget() {
	delete gameTurnWidget;
	delete stepPhaseWidget;
	delete gridLayout;
}

void BoardTopWidget::Refresh() {
	
	// Refresh widgets
	gameTurnWidget->UpdatePlayersTurn();
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

void GameTurnWidget::UpdatePlayersTurn() {
	
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
	stepNumberLabel = new QLabel("1");
	phaseNumberLabel = new QLabel("2");
	QLabel *stepLabel = new QLabel("Step");
	QLabel *phaseLabel = new QLabel("Phase");

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