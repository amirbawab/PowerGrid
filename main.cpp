#include "Player.h"
#include "Resource.h"
#include "PowerPlantCard.h"

#include <iostream>
#include <string>
#include <memory>

#include "GameStatus.h"
#include "Config.h"
#include "Game.h"

int main(int argc, char *argv[]) {

	// Create game
	Game game;

	// Play
	game.PlayGame();

	/*
	USER DRIVEN MAP CREATION
	Map m;
	std::shared_ptr<City> city = std::make_shared<City>("Montreal");
	city->SetRegion(nullptr);
	std::shared_ptr<City> city2 = std::make_shared<City>("Toronto");
	city2->SetRegion(nullptr);

	m.AddCity(city);
	m.AddCity(city2);

	m.AddConnection(city, city2, 2);

	m.DisplayMap();*/

	return 0;
}