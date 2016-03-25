#include "PowerPlantModeWidget.h"

PowerPlantModeWidget::PowerPlantModeWidget() {

	// Init components
	gridLayout = new QGridLayout();

	// Set layout
	setLayout(gridLayout);

	// Dummy cards
	for (int i = 0; i < 8; i++) {
		PowerPlantCard *card = new PowerPlantCard();
		card->setObjectName("powerPlantCardButton");
		card->setIcon(QIcon(":/PowerGrid/Resources/powerplants/dummy.png"));
		card->setIconSize(QSize(200,200));
		cards.push_back(card);
	}

	// Add components
	for (int i = 0; i < cards.size(); i++) {
		gridLayout->addWidget(cards[i], i/per_row, i%per_row, Qt::AlignCenter);
	}
}


PowerPlantModeWidget::~PowerPlantModeWidget() {
}
