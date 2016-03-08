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

	int lastStepDrawn = 0;
	std::vector<std::shared_ptr<Card>> cards;
	std::vector<std::shared_ptr<Card>> visiblePlants;  // indices 0-3 are current plants, 4-7 are future plants

public:
	
	static const int VISIBILE_CARDS = 8;
	static const int FUTURE_MARKET_INDEX = 4;

	CardStack() {}
	~CardStack();

	void SetCards(std::vector<std::shared_ptr<Card>> cards) { this->cards = cards; };
    std::vector<std::shared_ptr<Card>>& GetCards() { return cards; }

	std::shared_ptr<PowerPlantCard> const GetPlant(int index);
	void RemovePlant(int index) { visiblePlants.erase(visiblePlants.begin() + index); };
	bool DrawPlant();
	void Prepare(int removeCard);
	void ShuffleStack() { std::random_shuffle(cards.begin(), cards.end()); };

	int GetLastStepDrawn() { return lastStepDrawn; }
	void PlaceHighestVisibleAtEndOfStack();
	void RemoveLowestVisible();
	
	void PrintInfo();
};


