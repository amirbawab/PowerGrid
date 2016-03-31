#pragma once

#include <vector>
#include "Player.h"
#include "HouseColor.h"
#include <memory>

class DataStore {
private:

	// Constructor
	DataStore() {

		// Step and phase number
		step = 1;
		phase = 1;

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

		// Current player
		currentPlayer = players[0];
	}

	DataStore(DataStore const&) = delete;
	void operator=(DataStore const&) = delete;

public:
	std::vector<std::shared_ptr<HouseColor>> houseColors;
	std::vector<std::shared_ptr<Player>> players;
	std::vector<std::shared_ptr<Player>> playersTurn;
	std::shared_ptr<Player> currentPlayer;
	int step;
	int phase;

	static DataStore& getInstance() {
		static DataStore instance;
		return instance;
	}
};