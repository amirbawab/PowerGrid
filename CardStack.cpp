#include "CardStack.h"

CardStack::~CardStack() {
}

/// Draws a power plant from the deck and adds it to the plants on the board
void CardStack::DrawPlant() {

	// Cast
	std::shared_ptr<PowerPlantCard> powerPlantCard = NULL;
	std::shared_ptr<StepCard> stepCard = NULL;

	// If can draw
	while (visiblePlants.size() < VISIBILE_CARDS && cards.size() > 0 && powerPlantCard == NULL) {
		
		// Cache top
		std::shared_ptr<Card> top = cards[0];
		
		// Place card in the visible market
		visiblePlants.push_back(top);

		// Remove from stack
		cards.erase(cards.begin());
		
		// If Power plant card
		if (powerPlantCard = std::dynamic_pointer_cast<PowerPlantCard>(top)) {
			std::sort(visiblePlants.begin(), visiblePlants.end(), [](std::shared_ptr<Card> pp1, std::shared_ptr<Card> pp2) { return std::dynamic_pointer_cast<PowerPlantCard> (pp1)->GetPrice() < std::dynamic_pointer_cast<PowerPlantCard> (pp2)->GetPrice(); });
		
		} else if(stepCard = std::dynamic_pointer_cast<StepCard>(top)) {
			// TODO Check card 3
		}
	}
}

/// Print power plant market info
void CardStack::PrintInfo() {
	std::cout << "Current plants: " << std::endl;
	for (int i = 0; i < FUTURE_MARKET_INDEX; i++)
		std::cout << visiblePlants[i] << std::endl;

	std::cout << "Future plants: " << std::endl;
	for (int i = FUTURE_MARKET_INDEX; i < VISIBILE_CARDS; i++)
		std::cout << visiblePlants[i] << std::endl;
}

void CardStack::DiscardCards(int i) {
	while (i-- > 0) {
		// TODO Remove power plant cards, not step card
	}
}