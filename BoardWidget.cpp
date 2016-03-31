#include "BoardWidget.h"

BoardWidget::BoardWidget() {

	// Init components
	gridLayout = new QGridLayout();
	boardTopWidget = new BoardTopWidget();
	boardBottomWidget = new BoardBottomWidget();
	boardCenterWidget = new BoardCenterWidget();

	// Set layout
	setLayout(gridLayout);

	// Set id
	setObjectName("templateBoardWidget");

	// Set margin
	gridLayout->setContentsMargins(0, 0, 0, 0);
	
	// Add components
	gridLayout->addWidget(boardTopWidget, 0, 0, Qt::AlignTop);
	gridLayout->addWidget(boardCenterWidget, 1, 0);
	gridLayout->addWidget(boardBottomWidget, 2, 0, Qt::AlignBottom);
}

BoardWidget::~BoardWidget() {
	delete boardTopWidget;
	delete boardBottomWidget;
	delete gridLayout;
}

void BoardWidget::Refresh() {
	
	// Refresh all widgets
	boardTopWidget->Refresh();

}