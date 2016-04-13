#pragma once

#include <vector>
#include <memory>
#include <iostream>
#include <map>
#include "Player.h"
#include "Card.h"
#include "ResourceMarket.h"
#include "PowerPlantCard.h"
#include "CardStack.h"
#include "Map.h"
#include "Overview.h"
#include "Subject.h"

using std::vector;
using std::shared_ptr;
using std::cout;
using std::endl;
using std::cin;

class Game : public Subject {
private:
    Game();

    // General attributes
    int fullTurn;  // the number of turns since the beginning
    vector<shared_ptr<Player>> players;
    shared_ptr<ResourceMarket> rMarket;
    CardStack cardStack;
    vector<shared_ptr<HouseColor>> houseColor;
    vector<shared_ptr<Card>> allCards;
    shared_ptr<Map> map;
    vector<shared_ptr<Player>> playerOrder;  // players placed in the correct turn order
    shared_ptr<Player> winner;
    int playStep = 0;  // current step of the turn
    shared_ptr<Player> currentPlayer;  // pointer to the current player
    int phase;  // phase of the game (1, 2 or 3)
    bool gameOver = false;
    string errorMessageText;
    string errorMessageTextTitle;
    string infoMessageText;
    string infoMessageTextTitle;

    Overview overview;
    int initElektro;

    // Singleton
    Game(Game const&) = delete;
    void operator=(Game const&) = delete;


public:
    string messageText;
    ~Game() {};

    // Singleton
    static Game& getInstance() {
        static Game instance;
        return instance;
    }

    void LoadGame(string fileName);

    // Reset everything
    void Reset();

    // Getters
    int GetFullTurn() const { return fullTurn; }
    vector<shared_ptr<Player>>& GetPlayers() { return players; }
    shared_ptr<ResourceMarket> GetResourceMarket() const { return rMarket; }
    CardStack& GetCardStack() { return cardStack; }
    vector<shared_ptr<Card>>& GetAllCards() { return allCards; }
    shared_ptr<Map>& GetMap() { return map; }
    vector<shared_ptr<Player>>& GetPlayerOrder() { return playerOrder; }
    vector<shared_ptr<HouseColor>>& GetHouseColor() { return houseColor; }
    int GetPhase() const { return phase; }
    Overview& GetOverview() { return overview; }
    int GetInitElektro() const { return initElektro; }
    shared_ptr<Player> &GetCurrentPlayer() { return currentPlayer; }
    string GetMessageText() const { return messageText; }
    int GetStep() const { return playStep; }
    string GetErrorMessageText() const { return errorMessageText; }
    string GetErrorMessageTextTitle() const { return errorMessageTextTitle; }
    string GetInfoMessageText() const { return infoMessageText; }
    string GetInfoMessageTextTitle() const { return infoMessageTextTitle; }
    bool GetNowBidding() const { return nowBidding; }
    int GetPowerPlantIndex() const { return powerPlantIndex; } // setp 3
    int GetPlantIndex() const { return plantIndex; }
    bool step5SelectResource = false;

    // Setters
    void SetMap(shared_ptr<Map>& map) { this->map = map; }
    void SetFullTurn(int fullTurn) { this->fullTurn = fullTurn; }
    void SetPhase(int phase) { this->phase = phase; }
    void SetStep(int playStep) { this->playStep = playStep; }
    void SetInitElektro(int initElektro) { this->initElektro = initElektro; }
    void SetErrorMessageTextBox(string errorMessageTextTitle, string errorMessageText) { this->errorMessageTextTitle = errorMessageTextTitle; this->errorMessageText = errorMessageText; }
    void SetInfoMessageTextBox(string infoMessageTextTitle, string infoMessageText) { this->infoMessageTextTitle = infoMessageTextTitle; this->infoMessageText = infoMessageText; }

    void UpdatePlayOrder(bool); // step 1

    void PrintScore() const;
    void PrintRemainingResources();

    // Phase 0 Removing regions
    int regionsToRemove = 0;
    shared_ptr<Region> pickedRegion;
    void Phase0Start();
    void Phase0RemoveRegions1();
    void Phase0RemoveRegions2();

    // Step 1 Ordering players
    void Step1Start();

    // Step 2 Auctioning plants
    int GetNextPlayerIndex();
    void Step2Start();
    void Step2PickPlant1();
    void Step2PickPlant2(bool skip, int plantIndex=-1, int price=-1);
    void Step2Bid1();
    void Step2Bid2(int bid=-1);
    void Step2ReplacePlant1();
    void Step2ReplacePlant2(int plantToReplaceIndex);
    void Step2BidEnd();
    void Step2End();

    bool nowBidding;
    int biddingPlayer;
    bool initialBid;
    int currentBid;
    int plantIndex;
    std::map<Player*, bool> canBid;
    std::map<Player*, bool> canBuy;
    shared_ptr<Player> highestBidder;

    // Step 3 Buying Resources
    void Step3Start();
    void Step3BuyingResources1();
    void Step3BuyingResources2(int amount);
    void Step3End();

    int resourceIndex;
    Resource resourceIdentity;
    int powerPlantIndex;
    int resourceAmount = 0;

    // Step 4 Buying Cities
    shared_ptr<City> pickedCity = nullptr;
    void Step4Start();
    void Step4BuyingCities1();
    void Step4BuyingCities2();
    void Step4End();

    // Step 5 Bureaucracy
    void Step5Start();
    void Step5UsingPlants1();
    void Step5UsingPlants2(std::shared_ptr<PowerPlantCard> pickedPlant);
    void Step5ChoosingResource1();
    void Step5ChoosingResource2(vector<int> resourceAmounts);
    void Step5End();
    int getUnusedResources(Resource);

    shared_ptr<PowerPlantCard> pickedPlant;
    vector<shared_ptr<Player>> winners;
    int numPoweredCities;

    void GameEnd();
    
    // Integration
    void NewGame();
    void ConfigNewGame(string map, int numberOfPlayers, bool customMap = false);
    void StartGame();
};
