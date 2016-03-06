#pragma once

#include <vector>
#include <memory>
#include "Card.h"
#include "StepCard.h"
#include "PowerPlantCard.h"

class CardStack {
private:
	// Constants
	const string TOP = "top";
	const string BOTTOM = "bottom";

	const int VISIBILE_CARDS = 8;
	const int FUTURE_MARKET_INDEX = 4;
	int lastStepDrawn = 0;
	std::vector<std::shared_ptr<Card>> cards;
	std::vector<std::shared_ptr<Card>> visiblePlants;  // indices 0-3 are current plants, 4-7 are future plants

public:
	CardStack() {}
	~CardStack();

	void SetCards(std::vector<std::shared_ptr<Card>> cards) { this->cards = cards; };
    std::vector<std::shared_ptr<Card>>& GetCards() { return cards; }

	std::shared_ptr<PowerPlantCard> const GetPlant(int index) { return std::dynamic_pointer_cast<PowerPlantCard>(visiblePlants[index]); };
	void RemovePlant(int index) { visiblePlants.erase(visiblePlants.begin() + index); };
	bool DrawPlant();
	void Prepare(int numPlayers);

	int GetLastStepDrawn() { return lastStepDrawn; }
	
	void PrintInfo();
};


