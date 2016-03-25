#include "BoardBottomWidget.h"

BoardBottomWidget::BoardBottomWidget() {

	// Init components
	gridLayout = new QGridLayout();

	// Set layout
	setLayout(gridLayout);

	// Set id
	setObjectName("boardBottomWidget");
}


BoardBottomWidget::~BoardBottomWidget() {
	delete gridLayout;
}
