#include "PowerPlantModeWidget.h"

PowerPlantModeWidget::PowerPlantModeWidget() {

	// Init components
	gridLayout = new QGridLayout();

	// Set layout
	setLayout(gridLayout);
}


PowerPlantModeWidget::~PowerPlantModeWidget() {
	delete gridLayout;
}

void PowerPlantModeWidget::Refresh() {

	// Load power plant cards
	std::vector<std::shared_ptr<PowerPlantCard>> marketCards = DataStore::getInstance().marketPowerPlantCards;

	// Clear old components
	for (int i = 0; i < cards.size(); i++) {
		gridLayout->removeWidget(cards[i]);
		delete cards[i];
		cards.erase(cards.begin() + i);
	}

	// Dummy cards
	for (int i = 0; i < marketCards.size(); i++) {
		QPushButton *card = new QPushButton();
		card->setObjectName("powerPlantCardButton");
		card->setIcon(QIcon(marketCards[i]->GetImagePath().c_str()));
		card->setIconSize(QSize(200, 200));
		cards.push_back(card);
	}

	// Add components
	for (int i = 0; i < cards.size(); i++) {
		gridLayout->addWidget(cards[i], i / per_row, i%per_row, Qt::AlignCenter);
	}
}
