#include "ResourceMarketModeWidget.h"

ResourceMarketModeWidget::ResourceMarketModeWidget() {
	
	// Init components
	gridLayout = new QGridLayout();

	// Set layout
	setLayout(gridLayout);

	// Dummy data
	for (int i = 0; i < 8; i++) {
		ResourceMarketLevelWidget *dummy = new ResourceMarketLevelWidget();
		dummy->createFullLevelMarket();
		resourceMarketLevelwidgets.push_back(dummy);
	}

	// Dummy data
	for (int i = 0; i < 4; i++) {
		ResourceMarketLevelWidget *dummy = new ResourceMarketLevelWidget();
		dummy->createUraniumLevelMarket();
		resourceMarketLevelwidgets.push_back(dummy);
	}
	
	// Add widgets
	int max_row = resourceMarketLevelwidgets.size() / 2;
	for (int i = 0; i < resourceMarketLevelwidgets.size(); i++)
		gridLayout->addWidget(resourceMarketLevelwidgets[i], i/max_row, i%max_row, Qt::AlignCenter);
}

ResourceMarketModeWidget::~ResourceMarketModeWidget() {
	for (int i = 0; i < resourceMarketLevelwidgets.size(); i++)
		delete resourceMarketLevelwidgets[i];
}
