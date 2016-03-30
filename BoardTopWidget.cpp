#include "BoardTopWidget.h"

// BoardTopWidget class

BoardTopWidget::BoardTopWidget() {

	// Init components
	gridLayout = new QGridLayout();
	gameTurnWidget = new GameTurnWidget();

	// Set layout
	setLayout(gridLayout);

	// Set id
	setObjectName("boardTopWidget");

	// Dummy data
	gameTurnWidget->SetPlayers(DataStore::getInstance().players);

	// Add components
	gridLayout->addWidget(gameTurnWidget,0,0,Qt::AlignLeft);
}

BoardTopWidget::~BoardTopWidget() {
	delete gameTurnWidget;
	delete gridLayout;
}

void BoardTopWidget::SetPlayers(std::vector<std::shared_ptr<Player>> &players) {
	gameTurnWidget->SetPlayers(players);
}

// GameTurnWidget class

GameTurnWidget::GameTurnWidget() {
	
	// Init components
	gridLayout = new QGridLayout();

	// Set layout
	setLayout(gridLayout);

	// Add players

}

GameTurnWidget::~GameTurnWidget() {
	delete gridLayout;
}