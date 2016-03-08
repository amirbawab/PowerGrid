#include "CardStack.h"
#include <algorithm>

CardStack::~CardStack() {
}

/// Draws a power plant from the deck and adds it to the plants on the board
bool CardStack::DrawPlant() {

	// Cast
	std::shared_ptr<PowerPlantCard> powerPlantCard = nullptr;
	std::shared_ptr<StepCard> stepCard = nullptr;

	// If can draw
	while (visiblePlants.size() < VISIBILE_CARDS && cards.size() > 0 && powerPlantCard == nullptr) {
		
		// Cache top
		std::shared_ptr<Card> top = cards[0];
		
		// Place card in the visible market
		visiblePlants.push_back(top);

		// Remove from stack
		cards.erase(cards.begin());
		
		// If Power plant card
		if (powerPlantCard = std::dynamic_pointer_cast<PowerPlantCard>(top)) {
			std::sort(visiblePlants.begin(), visiblePlants.end(), [](std::shared_ptr<Card> pp1, std::shared_ptr<Card> pp2) { return std::dynamic_pointer_cast<PowerPlantCard> (pp1)->GetPrice() < std::dynamic_pointer_cast<PowerPlantCard> (pp2)->GetPrice(); });
			return true;

		// If step card
		} else if(stepCard = std::dynamic_pointer_cast<StepCard>(top)) {
			lastStepDrawn = stepCard->GetStep();
			return DrawPlant();
		}
	}
	return false;
}

/// Print power plant market info
void CardStack::PrintInfo() {
	std::cout << std::endl;
	std::cout << "Current plants: " << std::endl;
	for (int i = 0; i < FUTURE_MARKET_INDEX; i++)
		std::cout << "[" << i << "] " << *GetPlant(i) << std::endl;

	std::cout << std::endl;

	std::cout << "Future plants: " << std::endl;
	for (int i = FUTURE_MARKET_INDEX; i < VISIBILE_CARDS; i++)
		std::cout << "[" << i << "] " << *GetPlant(i) << std::endl;
	std::cout << std:: endl;
}

/// Place highest visible card at the end of the stack
void CardStack::PlaceHighestVisibleAtEndOfStack() {
	std::shared_ptr<Card> lastCard = visiblePlants.back();
	visiblePlants.pop_back();
	cards.push_back(lastCard);
}

void CardStack::Prepare(int removeCard) {
	
	// Sort cards
	std::sort(cards.begin(), cards.end(), [](std::shared_ptr<Card> lhs, std::shared_ptr<Card> rhs) {
		
		// Cast
		std::shared_ptr<PowerPlantCard> powerPlantCardLHS = nullptr;
		std::shared_ptr<StepCard> stepCardLHS = nullptr;

		std::shared_ptr<PowerPlantCard> powerPlantCardRHS = nullptr;
		std::shared_ptr<StepCard> stepCardRHS = nullptr;

		// If LHS is Power plant card
		if (powerPlantCardLHS = std::dynamic_pointer_cast<PowerPlantCard>(lhs)) {
			
			// If RHS is Power plant card
			if (powerPlantCardRHS = std::dynamic_pointer_cast<PowerPlantCard>(rhs)) {
				return powerPlantCardLHS->GetPrice() < powerPlantCardRHS->GetPrice();
			
			// If RHS is Step card
			} else if (stepCardRHS = std::dynamic_pointer_cast<StepCard>(rhs)) {
				return true;
			}

		// If LHS is Step card
		} else if (stepCardLHS = std::dynamic_pointer_cast<StepCard>(lhs)) {

			// If RHS is Power plant card
			if (powerPlantCardRHS = std::dynamic_pointer_cast<PowerPlantCard>(rhs)) {
				return false;

			// If RHS is Step card
			} else if (stepCardRHS = std::dynamic_pointer_cast<StepCard>(rhs)) {
				return stepCardLHS->GetStep() <stepCardRHS->GetStep();
			}
		}

		// Default
		return true;
	});

	// Place the cards in the visible
	for (int i = 0; i < VISIBILE_CARDS && i < cards.size(); i++) {
		visiblePlants.push_back(cards[0]);
		cards.erase(cards.begin());
	}

	// Shuffle
	std::random_shuffle(cards.begin(), cards.end());
	
	// Remove N cards
	int right = cards.size()-1;
	while (removeCard > 0 && right >= 0) {

		// If can be removed
		if (cards[right]->GetIgnoreFirst()) {
			cards.erase(cards.begin() + right);
			removeCard--;
		}

		// Move left
		right--;
	}

	// Position cards correctly
	int size = cards.size();
	for (int i = 0; i < size; ++i) {
		
		if(cards[i]->GetPosition() == TOP) {
			std::iter_swap(cards.begin(), cards.begin() + i);
		
		} else if (cards[i]->GetPosition() == BOTTOM) {
			std::iter_swap(cards.begin() + size - 1, cards.begin() + i);
			i--; // Re-evaluate the index again
			size--; // Decrease the size
		}
	}

	// Sort step cards
	std::sort(cards.begin() + size, cards.end(), [](std::shared_ptr<Card> lhs, std::shared_ptr<Card> rhs) {

		// Cast
		std::shared_ptr<StepCard> stepCardLHS = std::dynamic_pointer_cast<StepCard>(lhs);
		std::shared_ptr<StepCard> stepCardRHS = std::dynamic_pointer_cast<StepCard>(rhs);
		return stepCardLHS->GetStep() <stepCardRHS->GetStep();
	});
}

std::shared_ptr<PowerPlantCard> const CardStack::GetPlant(int index){ 
	if (index < 0 || index >= visiblePlants.size()) return nullptr;
	return std::dynamic_pointer_cast<PowerPlantCard>(visiblePlants[index]); 
}

void CardStack::RemoveLowestVisible() {
	if (visiblePlants.size() > 0)
		visiblePlants.erase(visiblePlants.begin());
}