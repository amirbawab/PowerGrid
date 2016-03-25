#include "BoardWidget.h"

BoardWidget::BoardWidget() {

	// Init components
	gridLayout = new QGridLayout();

	// Set layout
	setLayout(gridLayout);


}


BoardWidget::~BoardWidget() {

	delete gridLayout;
}
