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
	bool cinNewGame = true;

	if (cinNewGame) {
		// Initialize game
		GameStatus::GetInstance().Init(this, cinMap, ":/test/Resources/config/Config.xml");

	} else {
		// If loading saved game
		GameStatus::GetInstance().LoadFile(this, "Resources/saved games/PG_2016-03-2/Game.xml",
			"Resources/saved games/PG_2016-03-2/Players.xml");
	}

	// Initialize components
	int numTurns = 1;

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
	// Add bidding-related info
	std::map<Player*, bool> canBid;
	std::map<Player*, bool> canBuy;

	for (shared_ptr<Player> p : players) {
		canBid.insert(std::make_pair(p.get(), true));
		canBuy.insert(std::make_pair(p.get(), true));
	}

	bool stepTwoDone;
	while (true) {
		// Check if there are still players left who can buy
		stepTwoDone = true;
		for (shared_ptr<Player> p : players) {
			if (canBid[p.get()]) {
				stepTwoDone = false;
				break;
			}
		}
		if (stepTwoDone) {
			cout << "Step 2 is over." << endl;
			break;
		}

		// Reset bidding attribute to true for all playeres who can still buy
		for (shared_ptr<Player> p : players) {
			if (canBuy[p.get()])
				canBid[p.get()] = true;
			else
				canBid[p.get()] = false;
		}
		// Also reset bids
		currentBid = 0;
		highestBidder = nullptr;

		// Find who starts the bidding war (best begins)
		for (shared_ptr<Player> p : playerOrder) {
			if (canBuy[p.get()]) {
				currentPlayer = p;
				break;
			}
		}
		cout << currentPlayer->GetName() << " starts this auctioning round and can pick a power plant." << endl;

		// Place a bid or pass
		cout << "Place a bid? (Type \"N\" if not, anything else for yes)" << endl;
		string answer;
		cin >> answer;

		if (answer == "N") {
			canBuy[currentPlayer.get()] = false;
			continue;
		}
		else {
			// Pick a power plant
			int plantIndex;
			bool enoughMoney;
			do {
				cout << currentPlayer->GetName() << ", pick a power plant: (Enter the index)" << endl;
				cin >> plantIndex;
				enoughMoney = currentPlayer->HasElektro(cardStack.GetPlant(plantIndex)->GetPrice());

				if (!enoughMoney) {
					cout << "Not enough money for that plant: You have " << currentPlayer->GetElektro()
						<< " and the plant costs " << cardStack.GetPlant(plantIndex)->GetPrice() << endl;
				}
			} while (!enoughMoney);
			
			// Start bidding war
			bool initialBid = true;
			int bid;

			while (true) {
				// Check if the current player won the bidding round
				if (!initialBid && currentPlayer.get() == highestBidder.get()) {
					
				}
			}
		}
	}
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
		std::cout << "Turn " << fullTurn << std::endl;

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