#include "Game.h"

Game::Game() {

	// Log
	std::cout << "Game start!" << std::endl;

	// Setup
	Setup();
}

Game::~Game() {
}

/// Sets up the game at the beginning
void Game::Setup() {

	// Data from the user
	std::string cinMap = "USA";

	// Load information from 
	int initElektro = Config::GetInstance().GetElektro();
	cardStack.SetCards(Config::GetInstance().GetCards());

	// Select map
	for (auto tmpMap : Config::GetInstance().GetMaps()) {
		if (tmpMap->GetName() == cinMap) {
			map = tmpMap;
			break;
		}
	}

	// Initialize components
	int numTurns = 0;
	rMarket = std::make_shared<ResourceMarket>();

	// Test players
	std::shared_ptr<Player> p1 = std::make_shared<Player>("Joe", std::make_shared<HouseColor>("Red", ""), initElektro);
	std::shared_ptr<Player> p2 = std::make_shared<Player>("Jane", std::make_shared<HouseColor>("Blue", ""), initElektro);

	// Add players
	players.push_back(p1);
	players.push_back(p2);
	playerOrder.push_back(p1);
	playerOrder.push_back(p2);
}