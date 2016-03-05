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
	
	// Load information from config
	int initElektro = Config::GetInstance().GetElektro();
	cardStack.SetCards(Config::GetInstance().GetCards());

	auto maps = Config::GetInstance().GetMaps();

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


/// Used for determining the turn order
bool comparePlayerPriority(std::shared_ptr<Player> p1, std::shared_ptr<Player> p2) {
	
	// Priority: 1 - House
	if (p1->GetHouses().size() > p2->GetHouses().size())
		return true;

	if (p1->GetHouses().size() < p2->GetHouses().size())
		return false;

	// Priority: 2 - Highest power plant
	if (p1->GetHighestPowerPlant() > p2->GetHighestPowerPlant())
		return true;

	return false;
}

/// Reverse comparison
bool comparePlayerPriorityReverse(std::shared_ptr<Player> p1, std::shared_ptr<Player> p2) {
	return !comparePlayerPriority(p1, p2);
}

/// Step 1, sets the players in the proper turn order
void Game::UpdatePlayOrder(bool reverse) {
	if (!reverse)
		std::sort(playerOrder.begin(), playerOrder.end(), comparePlayerPriority);
	else
		std::sort(playerOrder.begin(), playerOrder.end(), comparePlayerPriorityReverse);
}

// Plays out step 2, auctioning power plants
void Game::AuctionPlants() {

}

/// Plays out step 3, buying raw materials
void Game::BuyRawMaterials() {

}

/// Step 4, buying cities
void Game::BuyCities() {

}

/// Step 5, bureaucracy
void Game::Bureaucracy() {

}

/// Runs the main game loop
void Game::PlayGame() {

	while (!gameOver) {
		std::cout << "Turn " << numTurns << std::endl;

		for (std::shared_ptr<Player> p : players) {
			std::cout << p << std::endl;
		}
		std::cout << "-------" << std::endl << std::endl;

		// Step 1
		std::cout << std::endl << "Step 1. Determining turn order" << std::endl << std::endl;
		UpdatePlayOrder(false);
		std::cout << "The order is:" << std::endl;
		for (std::shared_ptr<Player> p : playerOrder) {
			std::cout << p->GetName() << " ";
		}
		std::cout << std::endl;

		// Step 2
		std::cout << std::endl << "Step 2. Power plant auction" << std::endl << std::endl;
		AuctionPlants();

		for (std::shared_ptr<Player> p : players) {
			std::cout << p << std::endl;
		}
		std::cout << "-------" << std::endl << std::endl;

		// Step 3
		std::cout << std::endl << "Step 3. Buy raw materials\n\n";
		BuyRawMaterials();

		// Step 4


		// Step 5

		break; // testing for now
	};
}