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
	// TODO Use cin for those data
	std::string cinMap = "USA";
	int cinNumberOfPlayers = 2;
	bool cinNewGame = true;
	std::shared_ptr<Player> p1 = std::make_shared<Player>("Joe", std::make_shared<HouseColor>("Red", ""), initElektro);
	std::shared_ptr<Player> p2 = std::make_shared<Player>("Jane", std::make_shared<HouseColor>("Blue", ""), initElektro);

	
	if (cinNewGame) {

		// Initialize game
		GameStatus::GetInstance().Init(this, cinMap, ":/test/Resources/config/Config.xml");

		// Prepate card stack
		cardStack.Prepare(cinNumberOfPlayers);

	} else {
		// If loading saved game
		GameStatus::GetInstance().LoadFile(this, "Resources/saved games/PG_2016-03-2/Game.xml",
			"Resources/saved games/PG_2016-03-2/Players.xml");
	}

	// Initialize components
	fullTurn = 1;

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

/// Finds the index of the player in the vector
int GetPlayerIndex(shared_ptr<Player> player, vector<shared_ptr<Player>>& playerVec) {
	int index = 0;
	for (shared_ptr<Player> p : playerVec) {
		if (p.get() == player.get()) {
			return index;
		}
		index++;
	}
	return -1;
}

/// Plays out step 2, auctioning power plants
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
				currentPlayer->BuyPowerPlant(cardStack, plantIndex, currentBid);
				canBuy[currentPlayer.get()] = false;
				cout << currentPlayer->GetName() << " won this auction for " << currentBid << endl;
				break;
			}
			else {
				// For subsequent bids, need to enter the amount
				cout << currentPlayer->GetName() << ", enter your bid amount: (Enter 0 to pass)" << endl;
				cin >> bid;
			}

			// Subsequent bids are not the initial one anymore
			initialBid = false;

			// Updates the current bid and highest bidder
			if (bid > currentBid && currentPlayer->HasElektro(bid)) {
				currentBid = bid;
				highestBidder = currentPlayer;
				cout << "The highest bid is now " << currentBid << endl;
			}
			else {
				// Can't participate in current bidding round if you pass
				canBid[currentPlayer.get()] = false;
				cout << currentPlayer->GetName() << " passed." << endl;
			}

			// Get the next player to bid following a 'clockwise' fashion
			bool found = false;
			for (int i = GetPlayerIndex(currentPlayer, players) + 1; i < players.size(); i++) {
				if (canBid[players[i].get()]) {
					found = true;
					currentPlayer = players[i];
				}
			}

			if (!found) {
				for (int i = 0; i < GetPlayerIndex(currentPlayer, players); i++) {
					if (canBid[players[i].get()]) {
						currentPlayer = players[i];
					}
				}
			}
		}
	}

	}
}
	

/// Plays out step 3, buying raw materials
void Game::BuyRawMaterials() {
	// Update play order so worst starts
	UpdatePlayOrder(true);

	// Each player gets to buy resources
	for (shared_ptr<Player> p : playerOrder) {
		currentPlayer = p;
		cout << currentPlayer->GetName() << ", it is your turn to buy resources." << endl;

		int amount;
		int allowed;
		// Loop over every power plant the player owns
		for (shared_ptr<PowerPlantCard> plant : currentPlayer->GetPowerPlants()) {
			cout << "Buying resources for this power plant: " << endl;
			cout << plant << endl << endl;

			// Loop over every resource the power plant accepts
			for (Resource resource : plant->GetActiveResources()) {
				bool allowed;
				do {
					cout << "How much " << GetResourceName(resource) << " would you like to buy? (Enter 0 to pass)" << endl;
					cin >> amount;
					allowed = currentPlayer->BuyResources(rMarket, plant, resource, amount);

					if (!allowed)
						cout << "You cannot buy that amount. Try again." << endl;
				} while (!allowed);
			}
		}
	}
}

/// Step 4, buying cities
void Game::BuyCities() {
	// Update play order so worst starts
	UpdatePlayOrder(true);

	// Each player gets to buy cities
	for (shared_ptr<Player> p : playerOrder) {
		currentPlayer = p;
		cout << *currentPlayer << ", it is your turn to buy cities." << endl;

		string temp;
		while (true) {
			cout << "Would you like to buy a city? (Enter \"N\" if not, anything else if yes)" << endl;
			cin >> temp;
			
			if (temp == "N") {
				cout << *currentPlayer << " has finished buying cities." << endl;
				break;
			}
			else {
				cout << "Enter the name of the city: " << endl;
				cin >> temp;
				shared_ptr<City> city = map->GetCityByName(temp);
				
				// Check if name is legal
				if (city == nullptr) {
					cout << "Invalid city name." << endl;
					continue;
				}
				else {
					// Find the cost of connecting to that city
					int cost;
					if (currentPlayer->GetHouses().empty())
						cost = city->GetHousePrice();
					else 
						cost = city->GetHousePrice() + map->GetShortestPath(currentPlayer, temp);
				}
				// Buy the city
				shared_ptr<House> newHouse = std::make_shared<House>(city);
				if (currentPlayer->BuyHouse(newHouse))
					cout << *currentPlayer << " has bought " << temp << endl;
				else
					cout << "Could not buy " << temp << endl;

			}
		}

	}
}

/// Step 5, bureaucracy
void Game::Bureaucracy() {



	// Refresh the stack at the end
	cardStack.PlaceHighestVisibleAtEndOfStack();
	cardStack.DrawPlant();
}

/// Runs the main game loop
void Game::PlayGame() {

	while (!gameOver) {
		cout << "Turn " << fullTurn << endl;

		for (std::shared_ptr<Player> p : players) {
			p->DisplayStatus();
		}
		cout << "-------" << endl << endl;

		// Step 1
		cout << endl << "Step 1. Determining turn order\n" << endl;
		UpdatePlayOrder(false);
		cout << "The order is:" << endl;
		for (std::shared_ptr<Player> p : playerOrder) {
			cout << *p << " ";
		}
		cout << endl;

		// Step 2
		cout << endl << "Step 2. Power plant auction\n" << endl;
		AuctionPlants();

		for (std::shared_ptr<Player> p : players) {
			p->DisplayStatus();
		}

		// Step 3
		cout << endl << "Step 3. Buy raw materials\n" << endl;
		BuyRawMaterials();

		// Step 4
		cout << endl << "Step 4. Buy cities\n" << endl;

		// Step 5
		cout << endl << "Step 3. Bureaucracy\n" << endl;

		break; // testing for now
	};
}