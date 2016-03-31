#include "ResourceMarketModeWidget.h"

ResourceMarketModeWidget::ResourceMarketModeWidget() {
	
	// Init components
	gridLayout = new QGridLayout();

	// Set layout
	setLayout(gridLayout);
	
	// Create widgets
	for (int i = 0; i < 12; i++) {
		ResourceMarketLevelWidget *widget = new ResourceMarketLevelWidget();
		resourceMarketLevelwidgets.push_back(widget);
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

void ResourceMarketModeWidget::Refresh() {

	// Load market
	std::vector<std::shared_ptr<ResourceMarketLevel>> levels = DataStore::getInstance().resourceMarket->GetLevels();

	// Dummy data
	for (int i = 0; i < 8; i++) {
		resourceMarketLevelwidgets[i]->SetResourceMarketLevel(levels[i]);
		resourceMarketLevelwidgets[i]->RefreshFullLevelMarket();
	}

	// Dummy data
	for (int i = 0; i < 4; i++) {
		resourceMarketLevelwidgets[i+8]->SetResourceMarketLevel(levels[i+8]);
		resourceMarketLevelwidgets[i+8]->RefreshUraniumLevelMarket();
	}
}