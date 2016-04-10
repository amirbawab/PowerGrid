#pragma once

#include <vector>
#include <memory>
#include "Card.h"
#include "PowerPlantCard.h"

class CardStack {
private:
    // Constants
    const string TOP = "top";
    const string BOTTOM = "bottom";

    int justDrewStep = 0;  // equal to 3 if we just drew the step 3 card
    std::vector<std::shared_ptr<Card>> cards;
    std::vector<std::shared_ptr<Card>> visiblePlants;  // indices 0-3 are current plants, 4-7 are future plants

public:
    int visibleCards = 8;
    int futureMarketIndex = 4;

    CardStack() {}
    ~CardStack();

    // Reset methods
    void ResetCards() { cards = std::vector<std::shared_ptr<Card>>(); }
    void ResetVisiblePlants() { visiblePlants = std::vector<std::shared_ptr<Card>>(); }

    void SetCards(std::vector<std::shared_ptr<Card>> cards) { this->cards = cards; };
    std::vector<std::shared_ptr<Card>>& GetCards() { return cards; }
    std::vector<std::shared_ptr<Card>>& GetVisibleCards() { return visiblePlants; }


    std::shared_ptr<PowerPlantCard> const GetPlant(int index);
    void RemovePlant(int index) { visiblePlants.erase(visiblePlants.begin() + index); };
    bool DrawPlant();
    void Prepare(int removeCard);
    void ShuffleStack() { std::random_shuffle(cards.begin(), cards.end()); };


    void PlaceHighestVisibleAtEndOfStack();
    void RemoveLowestVisible();
    
    int GetJustDrewStep() { return justDrewStep; };
    void resetJustDrewStep() { justDrewStep = 0; };
    void AdjustForStep3();
    void SetVisibleCards(int num) { visibleCards = num; };


    void PrintInfo();
};
