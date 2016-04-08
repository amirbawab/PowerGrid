#include "PowerPlantModeWidget.h"
#include <QStyle>

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

    // Reset selected card
    selectedCard = nullptr;

	// Load power plant cards
	std::vector<std::shared_ptr<Card>> marketCards = Game::getInstance().GetCardStack().GetVisibleCards();

	// Clear old components
	while(cards.size() > 0) {
		gridLayout->removeWidget(cards[0]);
		delete cards[0];
		cards.erase(cards.begin());
	}

	// Dummy cards
	for (int i = 0; i < marketCards.size(); i++) {
		QPushButton *card = new QPushButton();
		card->setObjectName("powerPlantCardButton");
		card->setIcon(QIcon(marketCards[i]->GetImagePath().c_str()));
		card->setIconSize(QSize(200, 200));
		cards.push_back(card);

        // Only first row
        if (i < Game::getInstance().GetCardStack().futureMarketIndex) {
        
			// If step 2
			if (Game::getInstance().GetStep() == 2 && !Game::getInstance().GetNowBidding()) {

				// Connect
				connect(cards[i], &QPushButton::clicked, [=]() {
					qDebug(string("Card " + std::to_string(i) + " clicked!").c_str());

					if (selectedCard) {
						selectedCard->setObjectName("powerPlantCardButton");
						style()->unpolish(selectedCard);
						style()->polish(selectedCard);
						selectedCard->repaint();
					}

					selectedCard = cards[i];
					card->setObjectName("powerPlantCardButton_highlight");
					style()->unpolish(selectedCard);
					style()->polish(selectedCard);
					selectedCard->repaint();

					emit CardSelected(i);
				});
			}
        }
	}

	// Add components
	for (int i = 0; i < cards.size(); i++) {
		gridLayout->addWidget(cards[i], i / per_row, i%per_row, Qt::AlignCenter);
	}
}

int PowerPlantModeWidget::GetSelectedCardIndex() {
    if (selectedCard) {
        for (int i = 0; i < cards.size(); i++) {
            if (selectedCard == cards[i])
                return i;
        }
    }
    return -1;
}