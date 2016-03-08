#include "Game.h"

Game::Game() {

	// Log
	std::cout << "Game start!" << std::endl;

	// Setup
	Setup();
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

	// If new game
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
	phase = 1;

	// Add players to players vector
	players.push_back(p1);
	players.push_back(p2);

	// Configure players
	for (auto player : players) {

		// Adjust money value
		player->SetElektro(initElektro);
	}

	// Add then shuffle
	for (auto player : players) playerOrder.push_back(player);
	std::random_shuffle(playerOrder.begin(), playerOrder.end());
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

/// Step 1, sets the players in the proper turn order
void Game::UpdatePlayOrder(bool reverse) {
	if (!reverse)
		std::sort(playerOrder.begin(), playerOrder.end(), comparePlayerPriority);
	else
		std::sort(playerOrder.begin(), playerOrder.end(), [](std::shared_ptr<Player> p1, std::shared_ptr<Player> p2) { return !comparePlayerPriority(p1, p2); });
}

/// Plays out step 2, auctioning power plants
void Game::AuctionPlants() {
	
	// Add bidding-related info
	std::map<Player*, bool> canBid;
	std::map<Player*, bool> canBuy;

	// Anyone can bid and buy at the beginning
	for (shared_ptr<Player> p : players) {
		canBid.insert(std::make_pair(p.get(), true));
		canBuy.insert(std::make_pair(p.get(), true));
	}

	// Flag for step two
	bool stepTwoDone;

	// The auction process
	while (true) {
		
		// Check if there are still players left who can buy
		stepTwoDone = true;
		for (shared_ptr<Player> p : players) {
			if (canBuy[p.get()]) {
				stepTwoDone = false;
				break;
			}
		}

		// Step 2 done
		if (stepTwoDone) {
			cout << "Step 2 is over." << endl;
			break;
		}

		// Reset bidding attribute to true for all players who can still buy
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
		cout << *currentPlayer << " starts this auctioning round and can pick a power plant.\n" << endl;

		// Prepare answer
		string answer;

		// If not first turn
		if (fullTurn > 1) {
			// Place a bid or pass
			cout << "Place a bid? (Type \"N\" if not, anything else for yes)" << endl;
			cin >> answer;
		}

		// If pass or not enough money for cheapest plant
		if (ToLower(answer) == "n" || !currentPlayer->HasElektro(cardStack.GetPlant(0)->GetPrice())) {
			canBuy[currentPlayer.get()] = false;
			continue;
		
		// If buy
		} else {

			// Print cards
			cardStack.PrintInfo();

			// Pick a power plant
			int plantIndex;
			std::shared_ptr<PowerPlantCard> selectedPlant;
			bool enoughMoney;
			do {
				cout << *currentPlayer << ", pick a power plant: (Enter the index)" << endl;
				cin >> plantIndex;

				// If invalid character
				if (!cin.good()) {
					Error("Invalid input.");
					cin.clear();
					cin.ignore(INT_MAX, '\n');

				// If player selected a card from the future market
				} else if (plantIndex >= CardStack::FUTURE_MARKET_INDEX && plantIndex < CardStack::VISIBILE_CARDS) {
					Error("You can't buy from future market plants.");
				
				// If player selected a valid index
				} else {
					// Get power plant or nullptr
					selectedPlant = cardStack.GetPlant(plantIndex);

					// If power plant found
					if (selectedPlant) {
						enoughMoney = currentPlayer->HasElektro(selectedPlant->GetPrice());
						if (!enoughMoney) {
							cout << "Not enough money for that plant: You have " << currentPlayer->GetElektro()
								<< " and the plant costs " << cardStack.GetPlant(plantIndex)->GetPrice() << endl;
							selectedPlant = nullptr;
						}
					}
					else {
						Error("Power plant not found!");
					}
				}

			} while (!selectedPlant);

			// Start bidding war
			bool initialBid = true;
			int bid;

			// Print selected plant
			cout << "Selected card: " << *cardStack.GetPlant(plantIndex) << endl;

			// Bidding on the chosen plant
			while (true) {
				// Check if the current player won the bidding round
				if (!initialBid && currentPlayer.get() == highestBidder.get()) {
					currentPlayer->BuyPowerPlant(cardStack, plantIndex, currentBid);
					canBuy[currentPlayer.get()] = false;
					cout << *currentPlayer << " won this auction for " << currentBid << endl;
					break;
				} 
				
				// If initial bid
				if (initialBid) {
					bid = cardStack.GetPlant(plantIndex)->GetPrice();
					cout << *currentPlayer << " makes the initial bid." << endl;
				}
				else {
					// For subsequent bids, need to enter the amount
					cout << *currentPlayer << ", enter your bid amount: (Enter 0 to pass)" << endl;
					cin >> bid;

					// If invalid input, pass
					if (!cin.good()) {
						bid = 0;
						cin.clear();
						cin.ignore(INT_MAX, '\n');
					}
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
					cout << *currentPlayer << " passed." << endl;
				}

				// Get current player index
				int nextIndex = (std::distance(players.begin(), std::find(players.begin(), players.end(), currentPlayer)) + 1) % players.size();
				
				// Loop on players till full turn
				while (players[nextIndex] != currentPlayer) {
					
					// If can bid
					if (canBid[players[nextIndex].get()]) {
						currentPlayer = players[nextIndex];
						break;
					}

					// Next player
					nextIndex = (nextIndex + 1) % players.size();
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
		cout << *currentPlayer << ", it is your turn to buy resources." << endl;

		int amount;
		int allowed;
		// Loop over every power plant the player owns
		for (shared_ptr<PowerPlantCard> plant : currentPlayer->GetPowerPlants()) {
			
			// If does not take any resources
			if (plant->GetActiveResources().size() == 0) {
				cout << "Skipping the following power plant because it doesn't take any resources:" << endl;
				plant->PrintDetails();
				continue;
			}

			cout << "\nBuying resources for this power plant: " << endl;
			plant->PrintDetails();

			// Loop over every resource the power plant accepts
			for (Resource resource : plant->GetActiveResources()) {
				bool allowed;
				do {
					cout << "How much " << GetResourceName(resource) << " would you like to buy? (Enter 0 to pass)" << endl;
					cin >> amount;

					// Check invalid input
					if (!cin.good()) {
						cin.clear();
						cin.ignore(INT_MAX, '\n');
						amount = PG::INVALID;
					}

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
			cout << "\nWould you like to buy a city? (Enter \"N\" if not, anything else if yes)" << endl;
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
				if (!city) {
					cout << "Invalid city name." << endl;
					continue;
				}
				else if (city->GetNumberOfHouses() == phase) {
					cout << "Cannot buy a house. " << city->GetName() << " is already saturated for this phase.";
					continue;
				}

				// Find the cost of connecting to that city
				int cost;
				if (currentPlayer->GetHouses().empty())
					cost = city->GetHousePrice();
				else
					cost = city->GetHousePrice() + map->GetShortestPath(currentPlayer, temp);
				
				// Not enough money
				if (!currentPlayer->HasElektro(cost)) {
					cout << "Not enough money to buy " << city->GetName() << " for a total cost of " << std::to_string(cost) << " Elektro." << endl;
				
				// Can buy
				} else {
					// Buy the city
					shared_ptr<House> newHouse = std::make_shared<House>(city);
					currentPlayer->BuyHouse(newHouse);
					cout << *currentPlayer << " has bought a house at " << temp << " for a total cost of " << std::to_string(cost) << " Elektro." << endl;
				}
			}
		}
	}
}

/// Step 5, bureaucracy
void Game::Bureaucracy() {

	// Update play order to best starts
	UpdatePlayOrder(false);
	
	// End of game cities
	int citiesEndOfGame = overview.GetRuleByNumOfPlayers(players.size()).citiesEndOfGame;

	// Winners
	vector<std::shared_ptr<Player>> winners;

	// Check for end of game
	/*
	The player should power all the 17 cities to end the game.
	for (int i = 0; i < players.size() && !gameOver; i++) {
		
		// Get player
		std::shared_ptr<Player> p = players[i];

		// If player has the number of cities to end the game
		if (p->GetHouses().size() >= citiesEndOfGame) {
			gameOver = true;
			cout << "The game is ending this turn, power as many cities as you can!" << endl;
			vector<int> numPowered;
			numPowered.resize(players.size(), 0);
		}
	}*/

	// Count number of cities that can be powered and get money
	// TODO let players move resources around
	for (shared_ptr<Player> p : playerOrder) {
		int numPoweredCities = 0;
		currentPlayer = p;
		cout << *currentPlayer << ", it is your turn to use your power plants." << endl;

		string input;
		for (shared_ptr<PowerPlantCard> plant : currentPlayer->GetPowerPlants()) {
			cout << *plant << endl;
			if (plant->GetTotalPlacedResources() >= plant->GetCapacity()) {
				cout << "Do you want to power this plant? (Enter \"N\" for no, anything else for yes)" << endl;
				cin >> input;

				// If don't want to power houses
				if (input == "N") {
					continue;
				}
				else {

					// Update total number of powered houses
					numPoweredCities += plant->GetPower();

					// If one type of resources
					if (plant->GetActiveResources().size() == 1) {
						cout << "Consumed " << plant->GetCapacity() << " " << GetResourceName(*plant->GetActiveResources().begin()) << " from this power plant." << endl;
						plant->ConsumeResources(*plant->GetActiveResources().begin(), plant->GetCapacity());

					// If the plant accepts more than one type of resource
					} else if (plant->GetActiveResources().size() > 1) {
						
						// Required
						int required = plant->GetCapacity();

						// While require is not satisfied
						while (required > 0) {
							
							// Loop on active resources
							for (auto resource : plant->GetActiveResources()) {

								// If required satisfied
								if (required == 0) break;

								// Usable placed resources
								int usablePlaced = std::min(plant->GetPlacedResource(resource), plant->GetCapacity());
								cout << "How many " << GetResourceName(resource) << " out of " << usablePlaced << " usuable (might have more) would you like to power ?" << endl;

								// Take input from user
								int consume;
								do {
									cin >> consume;

									// If invalid input
									if (!cin.good()) {
										consume = 0;
										cin.clear();
										cin.ignore(INT_MAX, '\n');
									}

									// If invalid number
									if (consume < 0 || consume > usablePlaced)
										cout << "Cannot consume this amount. Try again." << endl;

									// If consumed more than required
									else if (required - consume < 0)
										cout << "You cannot consume more than required. Try again.";
										
								} while (consume < 0 || consume > usablePlaced || required - consume < 0);

								// Consume
								cout << "Consumed " << consume << " " << GetResourceName(resource) << " from this power plant." << endl;
								required -= consume;
								plant->ConsumeResources(resource, consume);
							}
						}
					}
				}
			}
			else {
				cout << "Not enough resources to power this plant\n" << plant << endl;
			}

			// Take min between number of houses that can be powered and the actual number of houses
			int playerHouses = currentPlayer->GetHouses().size();
			numPoweredCities = std::min(numPoweredCities, playerHouses);

			// Get money
			currentPlayer->SetElektro(currentPlayer->GetElektro() + overview.GetPayment(numPoweredCities));

			// If game over
			if (citiesEndOfGame <= numPoweredCities) {
				gameOver = true;
				winners.push_back(currentPlayer);
			}
		}
	}

	// If game is over
	if (gameOver) {

		// If more than one winner
		if (winners.size() == 1)
			winner = winners[0];
		else {
			
			// TODO Based on money
		}

		// End of game
		return;
	}


	// Change the visible power plants
	if (phase == 1 || phase == 2) {
		// Place highest plant under stack 
		cardStack.PlaceHighestVisibleAtEndOfStack();
		cardStack.DrawPlant();
	}
	else {
		// Remove lowest plant from the game
	}

	// Restock raw materials





}

/// Runs the main game loop
void Game::PlayGame() {

	while (!gameOver) {

		// Display turn number
		cout << "Turn " << fullTurn << endl;

		// Display players info
		for (std::shared_ptr<Player> p : players) p->DisplayStatus();

		// Reset step counter
		playStep = 0;

		// Step 1
		cout << endl << *overview.GetSteps()[playStep++] << endl;

		// Update players order ASC order
		UpdatePlayOrder(false);

		cout << "The order is:" << endl;
		for (std::shared_ptr<Player> p : playerOrder)  cout << *p << " ";
		cout << endl;

		// Step 2
		cout << endl << *overview.GetSteps()[playStep++] << endl;

		// Auction
		AuctionPlants();

		// Display players info
		for (std::shared_ptr<Player> p : players) p->DisplayStatus();

		// Step 3
		cout << endl << *overview.GetSteps()[playStep++] << endl;
		
		// Buy resources
		BuyRawMaterials();

		// Step 4
		cout << endl << *overview.GetSteps()[playStep++] << endl;
		
		// Buy houses
		BuyCities();

		// Step 5
		cout << endl << *overview.GetSteps()[playStep++] << endl;

		break; // testing for now
	};
}