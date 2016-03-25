#include "BoardWidget.h"

BoardWidget::BoardWidget() {

	// Init components
	gridLayout = new QGridLayout();
	boardTopWidget = new BoardTopWidget();
	boardBottomWidget = new BoardBottomWidget();

	// Set layout
	setLayout(gridLayout);

	// Set id
	setObjectName("templateBoardWidget");

	// Add components
	gridLayout->addWidget(boardTopWidget, 0, 0, Qt::AlignTop);
	gridLayout->addWidget(boardBottomWidget, 2, 0, Qt::AlignBottom);
}


BoardWidget::~BoardWidget() {
	delete boardTopWidget;
	delete boardBottomWidget;
	delete gridLayout;
}
