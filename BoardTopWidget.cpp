#include "BoardTopWidget.h"

// BoardTopWidget class

BoardTopWidget::BoardTopWidget() {

	// Init components
	gridLayout = new QGridLayout();

	// Set layout
	setLayout(gridLayout);

	// Set id
	setObjectName("boardTopWidget");

	
}

BoardTopWidget::~BoardTopWidget() {
	delete gridLayout;
}

// GameTurnWidget class

GameTurnWidget::GameTurnWidget() {
	
	// Init components
	gridLayout = new QGridLayout();

	// Set layout
	setLayout(gridLayout);


}

GameTurnWidget::~GameTurnWidget() {
	delete gridLayout;
}