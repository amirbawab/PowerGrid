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
#include "Config.h"
#include "GameStatus.h"

using std::shared_ptr;
using std::cout;
using std::endl;
using std::cin;

class Game {
private:

	// General attributes
	int fullTurn;  // the number of turns since the beginning
	std::vector<std::shared_ptr<Player>> players;
	std::shared_ptr<ResourceMarket> rMarket;
	CardStack cardStack;
	std::vector<std::shared_ptr<Card>> allCards;
	std::shared_ptr<Map> map;
	std::vector<std::shared_ptr<Player>> playerOrder;  // players placed in the correct turn order
	int playStep;  // current step of the turn
	std::shared_ptr<Player> currentPlayer;  // pointer to the current player
	int phase;  // phase of the game (1, 2 or 3)
	bool gameOver = false;
	int currentBid;
	std::shared_ptr<Player> highestBidder;

public:
	Game();
	~Game();

	void Setup();
	void PlayGame();

	// Getters
	int GetFullTurn() const { return fullTurn; }
	std::vector<std::shared_ptr<Player>>& GetPlayers() { return players; }
	std::shared_ptr<ResourceMarket>& GetResourceMarket() { return rMarket; }
	CardStack& GetCardStack() { return cardStack; }
	std::vector<std::shared_ptr<Card>>& GetAllCards() { return allCards; }
	std::shared_ptr<Map>& GetMap() { return map; }
	std::vector<std::shared_ptr<Player>>& GetPlayerOrder() { return playerOrder; }
	int GetPhase() { return phase; }

	// Setters
	void SetMap(std::shared_ptr<Map>& map) { this->map = map; }
	void SetFullTurn(int fullTurn) { this->fullTurn = fullTurn; }
	void SetPhase(int phase) { this->phase = phase; }

	void UpdatePlayOrder(bool); // step 1
	void AuctionPlants();  // step 2
	void BuyRawMaterials(); // step 3
	void BuyCities(); // step 4
	void Bureaucracy();  // step 5
};

