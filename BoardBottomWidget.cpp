#include "BoardBottomWidget.h"

// BoardBottomWidget class

BoardBottomWidget::BoardBottomWidget() {

	// Init components
	gridLayout = new QGridLayout();
	boardProfileWidget = new BoardProfileWidget();

	// Set layout
	setLayout(gridLayout);

	// Set id
	setObjectName("boardBottomWidget");

	// Add components
	gridLayout->addWidget(boardProfileWidget, 0, 0, Qt::AlignCenter);
}

BoardBottomWidget::~BoardBottomWidget() {
	delete boardProfileWidget;
	delete gridLayout;
}

void BoardBottomWidget::Refresh() {
	boardProfileWidget->Refresh();
}

// BoardProfileWidget class

BoardProfileWidget::BoardProfileWidget() {

	// Init components
	gridLayout = new QGridLayout();
	playerNameLabel = new QLabel();
	playerPictureLabel = new QLabel();

	// Set id
	playerNameLabel->setObjectName("board_bottom_subtitle");

	// Set layout
	setLayout(gridLayout);

	// Add components
	gridLayout->addWidget(playerPictureLabel, 0, 0, Qt::AlignBottom | Qt::AlignCenter);
	gridLayout->addWidget(playerNameLabel, 1, 0, Qt::AlignTop | Qt::AlignCenter);
}

BoardProfileWidget::~BoardProfileWidget() {
	delete playerNameLabel;
	delete playerPictureLabel;
	delete gridLayout;
}

void BoardProfileWidget::Refresh() {
	playerNameLabel->setText(DataStore::getInstance().currentPlayer->GetName().c_str());
	playerPictureLabel->setPixmap(QPixmap(DataStore::getInstance().currentPlayer->GetColor()->getImage().c_str()));
}