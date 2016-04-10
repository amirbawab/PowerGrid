#include "ResourceMarketModeWidget.h"
#include "Game.h"

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

	// Initialize market
	for (int i = 0; i < 8; i++) {
		resourceMarketLevelwidgets[i]->CreateFullLevelMarket();
	}

	for (int i = 0; i < 4; i++) {
		resourceMarketLevelwidgets[i + 8]->CreateUraniumLevelMarket();
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
	std::vector<std::shared_ptr<ResourceMarketLevel>> levels = Game::getInstance().GetResourceMarket()->GetLevels();

	// Refresh market
	for (int i = 0; i < resourceMarketLevelwidgets.size(); i++) {
		resourceMarketLevelwidgets[i]->SetResourceMarketLevel(levels[i]);
		resourceMarketLevelwidgets[i]->Refresh();
	}
}

void ResourceMarketModeWidget::ActivateResource(Resource resource, int amount) {
    int levelIndex = 0;
    std::vector<std::shared_ptr<ResourceMarketLevel>> levels = Game::getInstance().GetResourceMarket()->GetLevels();
    while (levelIndex < levels.size()) {
        int total = levels[levelIndex]->GetCounter(resource);
        int value = 0;
        if (amount >= total)
            value = total;
        else
            value = std::max(0, amount);
        resourceMarketLevelwidgets[levelIndex++]->ActivateResource(resource, value);
        amount -= total;
    }
}