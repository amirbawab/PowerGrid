#include "BoardTopWidget.h"

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
