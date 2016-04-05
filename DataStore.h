#pragma once

#include <vector>
#include "Player.h"
#include "HouseColor.h"
#include <memory>
#include "Map.h"
#include "Game.h"

class DataStore {
private:

	// Constructor
	DataStore() {

		// Step and phase number
		step = 1;
		phase = 1;
		messageText = "Question goes here";

		// Houses
		std::shared_ptr<HouseColor> house1 = std::make_shared<HouseColor>("black", ":/PowerGrid/Resources/houses/black.png");
		std::shared_ptr<HouseColor> house2 = std::make_shared<HouseColor>("purple", ":/PowerGrid/Resources/houses/purple.png");
		std::shared_ptr<HouseColor> house3 = std::make_shared<HouseColor>("green", ":/PowerGrid/Resources/houses/green.png");
		std::shared_ptr<HouseColor> house4 = std::make_shared<HouseColor>("blue", ":/PowerGrid/Resources/houses/blue.png");
		std::shared_ptr<HouseColor> house5 = std::make_shared<HouseColor>("yellow", ":/PowerGrid/Resources/houses/yellow.png");
		std::shared_ptr<HouseColor> house6 = std::make_shared<HouseColor>("red", ":/PowerGrid/Resources/houses/red.png");

		// Players
		std::shared_ptr<Player> player1 = std::make_shared<Player>("Player one", house1, 50);
		std::shared_ptr<Player> player2 = std::make_shared<Player>("Player two", house2, 50);
		std::shared_ptr<Player> player3 = std::make_shared<Player>("Player three", house3, 50);
		std::shared_ptr<Player> player4 = std::make_shared<Player>("Player four", house4, 50);
		std::shared_ptr<Player> player5 = std::make_shared<Player>("Player five", house5, 50);
		std::shared_ptr<Player> player6 = std::make_shared<Player>("Player six", house6, 50);

		// Power plants
		std::shared_ptr<PowerPlantCard> card1 = std::make_shared<PowerPlantCard>(2, 0, 6);
		card1->SetImagePath(":/PowerGrid/Resources/powerplants/dummy.png");
		std::shared_ptr<PowerPlantCard> card2 = std::make_shared<PowerPlantCard>(2, 0, 6);
		card2->SetImagePath(":/PowerGrid/Resources/powerplants/dummy.png");
		std::shared_ptr<PowerPlantCard> card3 = std::make_shared<PowerPlantCard>(2, 0, 6);
		card3->SetImagePath(":/PowerGrid/Resources/powerplants/dummy.png");
		std::shared_ptr<PowerPlantCard> card4 = std::make_shared<PowerPlantCard>(2, 0, 6);
		card4->SetImagePath(":/PowerGrid/Resources/powerplants/dummy.png");
		std::shared_ptr<PowerPlantCard> card5 = std::make_shared<PowerPlantCard>(2, 0, 6);
		card5->SetImagePath(":/PowerGrid/Resources/powerplants/dummy.png");
		std::shared_ptr<PowerPlantCard> card6 = std::make_shared<PowerPlantCard>(2, 0, 6);
		card6->SetImagePath(":/PowerGrid/Resources/powerplants/dummy.png");
		std::shared_ptr<PowerPlantCard> card7 = std::make_shared<PowerPlantCard>(2, 0, 6);
		card7->SetImagePath(":/PowerGrid/Resources/powerplants/dummy.png");
		std::shared_ptr<PowerPlantCard> card8 = std::make_shared<PowerPlantCard>(2, 0, 6);
		card8->SetImagePath(":/PowerGrid/Resources/powerplants/dummy.png");

		// Resource market levels
		resourceMarket = std::make_shared<ResourceMarket>();

		// Store house colors
		houseColors.push_back(house1);
		houseColors.push_back(house2);
		houseColors.push_back(house3);
		houseColors.push_back(house4);
		houseColors.push_back(house5);
		houseColors.push_back(house6);

		// Store players
		players.push_back(player1);
		players.push_back(player2);
		players.push_back(player3);
		players.push_back(player4);
		players.push_back(player5);
		players.push_back(player6);

		// Store order
		playersTurn.push_back(player1);
		playersTurn.push_back(player2);
		playersTurn.push_back(player3);
		playersTurn.push_back(player4);
		playersTurn.push_back(player5);
		playersTurn.push_back(player6);

		// Store power plants
		marketPowerPlantCards.push_back(card1);
		marketPowerPlantCards.push_back(card2);
		marketPowerPlantCards.push_back(card3);
		marketPowerPlantCards.push_back(card4);
		marketPowerPlantCards.push_back(card5);
		marketPowerPlantCards.push_back(card6);
		marketPowerPlantCards.push_back(card7);
		marketPowerPlantCards.push_back(card8);

		// Current player
		currentPlayer = players[0];

		// Set power plants for current player
		currentPlayer->AddPowerPlant(card1);
		currentPlayer->AddPowerPlant(card2);
		currentPlayer->AddPowerPlant(card3);

		// Add resources to current player power plant
		card1->AddActiveResource(COAL);
		card1->AddActiveResource(OIL);
		card1->AddActiveResource(GARBAGE);
		card1->AddActiveResource(URANIUM);
		card1->PlaceResource(COAL, 2);
		card1->PlaceResource(OIL, 2);
		card1->PlaceResource(GARBAGE, 1);
		card1->PlaceResource(URANIUM, 1);

		// Add resources to resource market
		resourceMarket->AddResource(COAL, 3);
	}

	DataStore(DataStore const&) = delete;
	void operator=(DataStore const&) = delete;

public:
	std::vector<std::shared_ptr<HouseColor>> houseColors;
	std::vector<std::shared_ptr<Player>> players;
	std::vector<std::shared_ptr<Player>> playersTurn;
	std::shared_ptr<Player> currentPlayer;
	std::vector<std::shared_ptr<Card>> marketPowerPlantCards;
	std::shared_ptr<ResourceMarket> resourceMarket;
    std::shared_ptr<Map> map;
	std::string messageText;
	int step;
	int phase;
    Game *game;

	static DataStore& getInstance() {
		static DataStore instance;
		return instance;
	}
};