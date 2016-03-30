#pragma once

#include <vector>
#include "Player.h"
#include "HouseColor.h"
#include <memory>

class DummyData {
private:

	// Constructor
	DummyData() {

		// Houses
		std::shared_ptr<HouseColor> house1 = std::make_shared<HouseColor>("black", ":/PowerGrid/Resources/houses/black.png");
		std::shared_ptr<HouseColor> house2 = std::make_shared<HouseColor>("purple", ":/PowerGrid/Resources/houses/purple.png");
		std::shared_ptr<HouseColor> house3 = std::make_shared<HouseColor>("green", ":/PowerGrid/Resources/houses/green.png");
		std::shared_ptr<HouseColor> house4 = std::make_shared<HouseColor>("blue", ":/PowerGrid/Resources/houses/blue.png");
		std::shared_ptr<HouseColor> house5 = std::make_shared<HouseColor>("yellow", ":/PowerGrid/Resources/houses/yellow.png");
		std::shared_ptr<HouseColor> house6 = std::make_shared<HouseColor>("red", ":/PowerGrid/Resources/houses/red.png");

		// Store house colors
		houseColors.push_back(house1.get());
		houseColors.push_back(house2.get());
		houseColors.push_back(house3.get());
		houseColors.push_back(house4.get());
		houseColors.push_back(house5.get());
		houseColors.push_back(house6.get());

		// Players
		players.push_back(new Player("Player one", house1, 50));
		players.push_back(new Player("Player two", house2, 50));
		players.push_back(new Player("Player three", house3, 50));
		players.push_back(new Player("Player four", house4, 50));
		players.push_back(new Player("Player five", house5, 50));
		players.push_back(new Player("Player six", house6, 50));
	}

	DummyData(DummyData const&) = delete;
	void operator=(DummyData const&) = delete;

public:
	std::vector<HouseColor*> houseColors;
	std::vector<Player*> players;

	static DummyData& getInstance() {
		static DummyData instance;
		return instance;
	}
};