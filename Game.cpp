#include "Game.h"
#include "GameStatus.h"

Game::Game() {

    // Log
    cout << "Game start!" << endl;

    // TODO Remove this ?
    // Save map
    //if (map->SaveFile("SavedMap.xml"))
    //	cout << "Map saved." << endl;
}

void Game::NewGame() {
    
    // New game message
    cout << "New game selected" << endl;

    // Initialize components
    fullTurn = 1;
    phase = 1; // TODO Change this to 0
    playStep = 1;

    // Resource market
    rMarket = std::make_shared<ResourceMarket>();
}

void Game::ConfigNewGame(string map, int numberOfPlayers) {

    // Log
    cout << "Map selected: " << map << ". Number of players selected: " << numberOfPlayers << endl;

    // Initialize game
    GameStatus::GetInstance().Init(this, map, ":/PowerGrid/Resources/config/Config.xml");

    // Reset vector of players
    players.clear();
    playerOrder.clear();

    // Add players to players vector
    while (numberOfPlayers-- > 0)
        players.push_back(std::make_shared<Player>("", nullptr, initElektro));
    
    // Configure players
    for (auto player : players) {

        // Adjust money value
        player->SetElektro(initElektro);
    }

    // Add then shuffle
    for (auto player : players) playerOrder.push_back(player);
    random_shuffle(playerOrder.begin(), playerOrder.end());

    // Set current player
    currentPlayer = playerOrder[0];
}

void Game::StartGame() {
    // Prepate card stack
    cardStack.Prepare(overview.GetRuleByNumOfPlayers(players.size()).randomeRemove);
}

// Load game
void Game::LoadGame() {

        // If loading saved game
        GameStatus::GetInstance().LoadFile(this, "Resources/saved games/PG_2016-03-2/Game.xml",
            "Resources/saved games/PG_2016-03-2/Players.xml");
}


/// Used for determining the turn order


// ISSUE Investigate why this is crashing when:
//      - Player number -> 2
//      - Next button
//      - Back button
//      - Next button
//      - Next button
//      - Finish step 1


bool comparePlayerPriority(shared_ptr<Player> p1, shared_ptr<Player> p2) {
    
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
        sort(playerOrder.begin(), playerOrder.end(), comparePlayerPriority);
    else
        std::sort(playerOrder.begin(), playerOrder.end(), [](shared_ptr<Player> p1, shared_ptr<Player> p2) { return !comparePlayerPriority(p1, p2); });
}

void Game::DisplayRemoveRegions() {
    messageText = "Click on the region(s) you want to remove then press `OK`";
    Notify();
}

void Game::Step1Start() {

    messageText = "Player order updated. Press `OK` to continue ...";
    playStep = 1;

    // Notify GUI
    Notify();
}

void Game::Step2Start() {
    // GUI Message: "Step 2"
    playStep = 2;

    // Don't sort in the first turn
    if(fullTurn > 1)
        UpdatePlayOrder(true);

    // nowBidding is false when picking a plant and true when bidding on a plant
    nowBidding = false;

    // Find who starts the bidding war (best begins)
    currentPlayer = playerOrder[0];

    // Reset can buy for all players
    for (shared_ptr<Player> p : players) {
        canBuy[p.get()] = true;
    }

    Step2PickPlant1();
}

void Game::Step2PickPlant1() {
    if(fullTurn == 1) {
        messageText = "Pick a power plant card to buy, then press `OK`";
    } else {
        messageText = "Pick a power plant card to buy, then press `OK`, or press `SKIP`";
    }
    Notify();
}

void Game::Step2PickPlant2(bool skip, int plantIndex, int price) {
    
    // Set power plant index
    this->plantIndex = plantIndex;

    if (skip) {

        // If first turn, don't allow skip
        if (fullTurn == 1) {
            SetErrorMessageTextBox("Skipping Error", "You are not allowed to skip in the first round");
            return Step2PickPlant1();

        } else {
            canBuy[currentPlayer.get()] = false;
            // Go to next player
            currentPlayer = playerOrder[GetNextPlayerIndex()];
            return Step2PickPlant1(); // Next player picks a plant
        }
    }

    // Check that player can buy that plant
    if (price > currentPlayer->GetElektro()) {
        SetErrorMessageTextBox("Not Enough Money", "You don't have enough money to buy this power plant.");
        return Step2PickPlant1();  // Pick another plant
    }

    // Reset bidding attribute to true for all players who can still buy
    for (shared_ptr<Player> p : players) {
        if (canBuy[p.get()])
            canBid[p.get()] = true;
        else
            canBid[p.get()] = false;
    }

    // Make initial bid
    currentBid = price;
    highestBidder = currentPlayer;
    cout << *currentPlayer << " makes the initial bid." << endl;

    initialBid = true;
    nowBidding = true;

    Step2Bid2();
}

void Game::Step2Bid1() {
    messageText = "The highest bid is now " + std::to_string(currentBid) + ", enter your bid:";
    Notify();
}

void Game::Step2Bid2(int bid) {
    if (!initialBid) {
        
        // Updates the current bid and highest bidder
        if (bid > currentBid) {
            
            // If has enough money, proceed
            if (currentPlayer->HasElektro(bid)) {
                currentBid = bid;
                highestBidder = currentPlayer;
                cout << "The highest bid is now " << currentBid << endl;
            }
            else {
                SetErrorMessageTextBox("Not Enough Money", "You don't enough money to bid this amount");
                return Step2Bid1();
            }
        }

        // If skip
        else if (bid < 0) {
            // Can't participate in current bidding round if you pass
            canBid[currentPlayer.get()] = false;
            cout << *currentPlayer << " passed." << endl;
        } 

        // If between 0 and current bid inclusive
        else {
            SetErrorMessageTextBox("Bidding Error", "You cannot bid less than the current amount");
            return Step2Bid1();
        }
    }

    // No longer initial bid
    initialBid = false;

    // Updates currentPlayer to be the next person to play
    // Get current player index
    int nextIndex = (distance(players.begin(), find(players.begin(), players.end(), currentPlayer)) + 1) % players.size();
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

    // Check if the current player won the bidding round
    if (currentPlayer.get() == highestBidder.get()) {
        currentPlayer->BuyPowerPlant(cardStack, plantIndex, currentBid);
        canBuy[currentPlayer.get()] = false;
        cout << *currentPlayer << " won this auction for " << currentBid << endl;

        // Check if we drew the Step 3 card
        if (cardStack.GetJustDrewStep() == 3) {
            cout << "The step 3 card was drawn." << endl;

            // Treat the step 3 card as though it was the highest plant in the market (only 7 plants are now visible)
            cardStack.SetVisibleCards(7);
            cardStack.ShuffleStack();
        }

        // End the bidding phase
        return Step2BidEnd();
    }

    // Bidding continues
    return Step2Bid1();
}

void Game::Step2BidEnd() {

    // Update info message in GUI
    SetInfoMessageTextBox("Winner", highestBidder->GetName() + " won the bid for this round");

    // Figure out what the next part of the game is
    // Find who starts the next bidding war (best begins)
    currentPlayer = nullptr;
    for (shared_ptr<Player> p : playerOrder) {
        if (canBuy[p.get()]) {
            currentPlayer = p;
            break;
        }
    }

    // If no one can buy anymore, end step 2
    if (currentPlayer == nullptr) {
        return Step2End();  // Go to step 3
    }

    // Reset now bidding
    nowBidding = false;

    // Else we go to the next player to pick a plant for auction
    return Step2PickPlant1();
}

void Game::Step2End() {

    // If we drew the step 3 card during any of the auction, only now do we adjust the size of the market and phase (at the end of all auctions)
    if (cardStack.GetJustDrewStep() == 3) {
        cardStack.AdjustForStep3();
        phase = 3;
    }

    // Start step 3
    Step3Start();
}

void Game::Step3Start() {
    // GUI Message : "Step 3"
    playStep = 3;

    // Reorder players (worst starts)
    UpdatePlayOrder(true);

    // Reset attributes and start buying resources
    currentPlayer = playerOrder[0];
    resourceIndex = 0;
    powerPlantIndex = 0;

    std::set<Resource> rSet = currentPlayer->GetPowerPlants()[powerPlantIndex]->GetActiveResources();
    vector<Resource> temp(rSet.size());
    copy(rSet.begin(), rSet.end(), back_inserter(temp));
    resourceIdentity = temp[resourceIndex];

    Step3BuyingResources1();
}

void Game::Step3BuyingResources1() {
    messageText = "How many " + GetResourceName(resourceIdentity) + " would you like to buy for the selected power plant?";
    Notify();
}

void Game::Step3BuyingResources2() {
    int amount = 0;  // GUI get: the amount of resources the player wants

    // Buy resources and check if allowed to do so
    bool allowed = currentPlayer->BuyResources(*rMarket, currentPlayer->GetPowerPlants()[powerPlantIndex], resourceIdentity, amount);
    if (!allowed) {
        // GUI Error: "Invalid amount of resources"
        return Step3BuyingResources1();
    }

    // Find next playing step
    // Check if there are other resources to iterate through
    vector<Resource> temp;
    for (Resource r : currentPlayer->GetPowerPlants()[powerPlantIndex]->GetActiveResources()) {
        temp.push_back(r);
    }
    resourceIndex++;
    
    if (resourceIndex > temp.size()) {
        // If no more resources, go to next power plant
        powerPlantIndex++;
        resourceIndex = 0;

        if (powerPlantIndex > currentPlayer->GetPowerPlants().size()) {
            // If no more power plants, go to next player
            currentPlayer = playerOrder[GetNextPlayerIndex()];
            powerPlantIndex = 0;

            if (currentPlayer.get() == playerOrder[0].get()) {
                // If we returned to the first player, then go to step 4
                return Step4Start();
            }

        }
    }

    // Get next resource to use
    temp.clear();
    for (Resource r : currentPlayer->GetPowerPlants()[powerPlantIndex]->GetActiveResources()) {
        temp.push_back(r);
    }
    resourceIdentity = temp[resourceIndex];

    return Step3BuyingResources1();
}

void Game::Step3End() {

}


void Game::Step4Start() {
    // GUI Message: "Step 4" 
    playStep = 4;

    // Reorder players (worst starts)
    UpdatePlayOrder(true);
    currentPlayer = playerOrder[0];

    Step4BuyingCities1();
}

void Game::Step4BuyingCities1() {
    // GUI Message: "Player, pick a city to buy:"

}

void Game::Step4BuyingCities2() {
    shared_ptr<City> pickedCity = nullptr;  // GUI get: the pointer to the picked city

    if (pickedCity == nullptr) {
        // Player skipped, go to next player
        currentPlayer = playerOrder[GetNextPlayerIndex()];

        if (currentPlayer.get() == playerOrder[0].get()) {
            Step4End();
        }

        return Step4BuyingCities1();
    }

    // Check if city is full
    if (pickedCity->GetNumberOfHouses() == phase) {
        cout << "Cannot buy a house. " << pickedCity->GetName() << " is already saturated for this phase.";
        return Step4BuyingCities1();
    }

    // Find the cost of connecting to that city
    int cost;
    if (currentPlayer->GetHouses().empty())
        cost = pickedCity->GetHousePrice();
    else
        cost = pickedCity->GetHousePrice() + map->GetShortestPath(currentPlayer, pickedCity->GetName());

    // Check if you have enough money
    if (!currentPlayer->HasElektro(cost)) {
        cout << "Not enough money to buy " << pickedCity->GetName() << " for a total cost of " << std::to_string(cost) << " Elektro." << endl;
        return Step4BuyingCities1();
    }

    // Buy the city
    shared_ptr<House> newHouse = std::make_shared<House>(pickedCity);
    currentPlayer->BuyHouse(newHouse);
    cout << *currentPlayer << " has bought a house at " << pickedCity << " for a total cost of " << std::to_string(cost) << " Elektro." << endl;

    // Go back to buy another one
    return Step4BuyingCities1();
}


void Game::Step4End() {

    // Replace power plants in the market if they have a price less or equal to the highest number of cities owned by a player
    int maxHouses = 0;
    for (shared_ptr<Player> p : players) {
        if (p->GetHouses().size() > maxHouses)
            maxHouses = p->GetHouses().size();
    }
    while (cardStack.GetPlant(0)->GetPrice() <= maxHouses) {
        cardStack.RemoveLowestVisible();
        cardStack.DrawPlant();
    }
    // Check if we drew the step 3 card
    if (cardStack.GetJustDrewStep() == 3) {
        cardStack.AdjustForStep3();
        cardStack.ShuffleStack();
        phase = 3;

        cout << "Entering phase 3." << endl;
    }

    Step5Start();
}




void Game::Step5Start() {
    // GUI Message: "Step 5"
    playStep = 5;

    // Check if we enter phase 2
    if (phase == 1) {
        int step2Cities = overview.GetRuleByNumOfPlayers(players.size()).step2Cities;

        // Check that a player has reached the number of cities for step 2
        for (shared_ptr<Player> p : players) {
            if (p->GetHouses().size() >= step2Cities) {
                // Go to phase 2
                phase = 2;

                // Replace lowest power plant from the market
                cardStack.RemoveLowestVisible();
                cardStack.DrawPlant();

                cout << "Entering phase 2." << endl;
                break;
            }
        }
    }

    // Update play order to best starts
    UpdatePlayOrder(false);

    // Reset attributes
    currentPlayer = playerOrder[0];
    powerPlantIndex = 0;
    resourceIndex = 0;
    numPoweredCities = 0;
    winners.clear();
    
    Step5UsingPlants1();
}

void Game::Step5UsingPlants1() {
    // GUI Message: "Player, which power plants do you want to use?"

}

void Game::Step5UsingPlants2() {
    pickedPlant = nullptr;  // GUI get: player clicks on the plant they want to power

    // If skip, get paid and go to next player
    if (pickedPlant = nullptr) {

        // Take min between number of houses that can be powered and the actual number of houses
        int playerHouses = currentPlayer->GetHouses().size();
        numPoweredCities = std::min(numPoweredCities, playerHouses);

        // Get money
        cout << *currentPlayer << " powered " << std::to_string(numPoweredCities) << " house(s), earning " << std::to_string(overview.GetPayment(numPoweredCities)) << " Elektro." << endl;
        currentPlayer->SetElektro(currentPlayer->GetElektro() + overview.GetPayment(numPoweredCities));

        // If game over
        if (overview.GetRuleByNumOfPlayers(players.size()).citiesEndOfGame <= numPoweredCities) {
            gameOver = true;
            winners.push_back(currentPlayer);
        }

        // Go to next player
        currentPlayer = playerOrder[GetNextPlayerIndex()];
        if (currentPlayer.get() == playerOrder[0].get()) {
            // If no more players, go the end of step 5
            return Step5End();
        }
        return Step5UsingPlants1();
    }

    // If there is only one resource
    if (pickedPlant->GetActiveResources().size() == 1) {
        pickedPlant->ConsumeResources(*pickedPlant->GetActiveResources().begin(), pickedPlant->GetCapacity());
        
    }
    else {
        // If there is more than one resource, need to specify quantity of each
        return Step5ChoosingResource1();
    }

    // Pick another plant to use
    return Step5UsingPlants1();
}

void Game::Step5ChoosingResource1() {
    // GUI Message: "Enter the number of each resource to use" 
}

void Game::Step5ChoosingResource2() {
    vector<int> resourceAmounts = {0, 0, 0, 0};  // GUI get: the amount of each resource to consume

    int sum = 0;
    for (int i : resourceAmounts)
        sum += i;

    // Check if not enough resources to be consumed
    if (sum < pickedPlant->GetCapacity()) {
        // GUI Error: "Not enough resources to use the plant" 
        return Step5UsingPlants1();  // Go use other plants
    }

    // Consume resources
    for (int i = 0; i < resourceAmounts.size(); ++i) {
        pickedPlant->ConsumeResources(static_cast<Resource>(i), resourceAmounts[i]);
    }

    // Increase number of cities powered
    numPoweredCities += pickedPlant->GetPower();

    // Go back to picking other plants to use
    Step5UsingPlants1();
    
}

void Game::Step5End() {
    // Check if game is over
    if (gameOver) {
        // If single winner
        if (winners.size() == 1)
            winner = winners[0];

        // If more than one winner
        else if (winners.size() > 1) {

            // Get max
            int maxMoney = winners[0]->GetElektro();
            for (int i = 1; i < winners.size(); i++) {
                if (winners[i]->GetElektro() > maxMoney)
                    maxMoney = winners[i]->GetElektro();
            }

            // Take the max only
            for (int i = 0; i < winners.size(); i++)
                if (winners[i]->GetElektro() != maxMoney)
                    winners.erase(winners.begin() + i);

            // If only one winner
            if (winners.size() == 1) {
                winner = winners[0];

            // If more than one winner
            }
            else if (winners.size() > 1) {

                int maxCitiesIndex = 0;
                for (int i = 1; i < winners.size(); i++) {
                    if (winners[i]->GetHouses().size() > winners[maxCitiesIndex]->GetHouses().size())
                        maxCitiesIndex = i;
                }
                winner = winners[maxCitiesIndex];
            }
            
            // End the game
            GameEnd();
        }
    }
    // If not, reset gameOver
    gameOver = false;

    // Change the visible power plants
    if (phase == 1 || phase == 2) {
        // Place highest plant under stack 
        cardStack.PlaceHighestVisibleAtEndOfStack();
        cardStack.DrawPlant();
    }
    else if (phase == 3) {
        // Remove lowest plant from the game
        cardStack.RemoveLowestVisible();
        cardStack.DrawPlant();
    }

    // Check if we drew the step 3 card
    if (cardStack.GetJustDrewStep() == 3) {
        cardStack.AdjustForStep3();
        cardStack.ShuffleStack();
        phase = 3;

        cout << "Entering phase 3." << endl;
    }

    // Restock raw materials
    for (int i = 0; i < res::total; i++) {
        Resource resource = static_cast<Resource>(COAL + i);
        rMarket->AddResource(resource, overview.GetRuleByNumOfPlayers(players.size()).GetResourceAt(phase, resource));
    }
}

void Game::GameEnd() {
    // GUI Message: "Game is over! The winner is " + winner 
}

int Game::GetNextPlayerIndex() {
    return (distance(playerOrder.begin(), find(playerOrder.begin(), playerOrder.end(), currentPlayer)) + 1) % playerOrder.size();
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
            cout << "Place a bid? (Y/N): ";
            cin >> answer;

            // Keep trying
            while (ToLower(answer) != "n" && ToLower(answer) != "y") {
                cout << "Please enter \"Y\" for yes and \"N\" for no: ";
                cin >> answer;
            }
        }

        // If pass or not enough money for cheapest plant
        if (ToLower(answer) == "n" || !currentPlayer->HasElektro(cardStack.GetPlant(0)->GetPrice())) {
            canBuy[currentPlayer.get()] = false;
            continue;

            // If buy
        }
        else {

            // Print cards
            cardStack.PrintInfo();

            // Pick a power plant
            int plantIndex;
            shared_ptr<PowerPlantCard> selectedPlant;
            bool enoughMoney;
            do {
                cout << *currentPlayer << ", pick a power plant (Enter the index): ";
                cin >> plantIndex;

                // If invalid character
                if (!cin.good()) {
                    Error("Invalid input.");
                    cin.clear();
                    cin.ignore(INT_MAX, '\n');

                    // If player selected a card from the future market
                }
                else if (plantIndex >= cardStack.futureMarketIndex && plantIndex < cardStack.visibleCards) {
                    Error("You can't buy from future market plants.");

                    // If player selected a valid index
                }
                else {
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

                    // Check if we drew the Step 3 card
                    if (cardStack.GetJustDrewStep() == 3) {
                        cout << "The step 3 card was drawn." << endl;

                        // Treat the step 3 card as though it was the highest plant in the market (only 7 plants are now visible)
                        cardStack.SetVisibleCards(7);
                        cardStack.ShuffleStack();
                    }

                    break;
                }

                // If initial bid
                if (initialBid) {
                    bid = cardStack.GetPlant(plantIndex)->GetPrice();
                    cout << *currentPlayer << " makes the initial bid." << endl;
                }
                else {
                    // For subsequent bids, need to enter the amount
                    cout << *currentPlayer << ", enter your bid amount (Enter 0 to pass): ";
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
                int nextIndex = (distance(players.begin(), find(players.begin(), players.end(), currentPlayer)) + 1) % players.size();

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

        // If we drew the step 3 card during any of the auction, only now do we adjust the size of the market and phase (at the end of all auctions)
        if (cardStack.GetJustDrewStep() == 3) {
            cardStack.AdjustForStep3();
            phase = 3;
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
        // TODO: same variable but different type on line 910!
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

                    allowed = currentPlayer->BuyResources(*rMarket, plant, resource, amount);

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
            cout << "\nWould you like to buy a city? (Y/N): ";
            cin >> temp;

            // Keep trying
            while (ToLower(temp) != "n" && ToLower(temp) != "y") {
                cout << "Please enter \"Y\" for yes and \"N\" for no: ";
                cin >> temp;
            }

            if (ToLower(temp) == "n") {
                cout << *currentPlayer << " has finished buying cities." << endl;
                break;
            }
            else {
                cout << "Enter the name of the city: ";
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
                }
                else {
                    // Buy the city
                    shared_ptr<House> newHouse = std::make_shared<House>(city);
                    currentPlayer->BuyHouse(newHouse);
                    cout << *currentPlayer << " has bought a house at " << temp << " for a total cost of " << std::to_string(cost) << " Elektro." << endl;
                }
            }
        }
    }

    // Replace power plants in the market if they have a price less or equal to the highest number of cities owned by a player
    int maxHouses = 0;
    for (shared_ptr<Player> p : players) {
        if (p->GetHouses().size() > maxHouses)
            maxHouses = p->GetHouses().size();
    }
    while (cardStack.GetPlant(0)->GetPrice() <= maxHouses) {
        cardStack.RemoveLowestVisible();
        cardStack.DrawPlant();
    }
    // Check if we drew the step 3 card
    if (cardStack.GetJustDrewStep() == 3) {
        cardStack.AdjustForStep3();
        cardStack.ShuffleStack();
        phase = 3;

        cout << "Entering phase 3." << endl;
    }

}

/// Step 5, bureaucracy
void Game::Bureaucracy() {

    // Check if we enter phase 2
    if (phase == 1) {
        int step2Cities = overview.GetRuleByNumOfPlayers(players.size()).step2Cities;

        // Check that a player has reached the number of cities for step 2
        for (shared_ptr<Player> p : players) {
            if (p->GetHouses().size() >= step2Cities) {
                // Go to phase 2
                phase = 2;

                // Replace lowest power plant from the market
                cardStack.RemoveLowestVisible();
                cardStack.DrawPlant();

                cout << "Entering phase 2." << endl;
                break;
            }
        }
    }



    // Update play order to best starts
    UpdatePlayOrder(false);
    
    // End of game cities
    int citiesEndOfGame = overview.GetRuleByNumOfPlayers(players.size()).citiesEndOfGame;

    // Winners
    vector<shared_ptr<Player>> winners;

    // Count number of cities that can be powered and get money
    // TODO let players move resources around
    for (shared_ptr<Player> p : playerOrder) {
        int numPoweredCities = 0;
        currentPlayer = p;
        cout << *currentPlayer << ", it is your turn to use your power plants." << endl;

        string input;
        for (shared_ptr<PowerPlantCard> plant : currentPlayer->GetPowerPlants()) {
            plant->PrintDetails();
            if (plant->GetTotalPlacedResources() >= plant->GetCapacity()) {
                cout << "Do you want to use the above plant? (Y/N): ";
                cin >> input;

                // Keep trying
                while (ToLower(input) != "n" && ToLower(input) != "y") {
                    cout << "Please enter \"Y\" for yes and \"N\" for no: ";
                    cin >> input;
                }

                // If don't want to power houses
                if (ToLower(input) == "n") {
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
                        int required = 0;

                        // While required is not satisfied
                        while (required < plant->GetCapacity()) {
                            
                            // Loop on active resources
                            for (auto resource : plant->GetActiveResources()) {

                                // If required satisfied
                                if (required == plant->GetCapacity()) break;
                                
                                // If doesn't have this resource
                                if (plant->GetPlacedResource(resource) == 0) continue;

                                // Usable placed resources
                                int usablePlaced = std::min(plant->GetPlacedResource(resource), plant->GetCapacity() - required);
                                cout << "How many " << GetResourceName(resource) << " out of " << usablePlaced << " usuable (might have more) would you like to power ? ";

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
                                        
                                } while (consume < 0 || consume > usablePlaced);

                                // Consume
                                cout << "Consumed " << consume << " " << GetResourceName(resource) << " from this power plant." << endl;
                                required += consume;
                                plant->ConsumeResources(resource, consume);
                            }
                        }
                    }
                }
            }
            else {
                cout << "Not enough resources to use this plant.\n" << endl;
            }
        }

        // Take min between number of houses that can be powered and the actual number of houses
        int playerHouses = currentPlayer->GetHouses().size();
        numPoweredCities = std::min(numPoweredCities, playerHouses);

        // Get money
        cout << *currentPlayer << " powered " << std::to_string(numPoweredCities) << " house(s), earning " << std::to_string(overview.GetPayment(numPoweredCities)) << " Elektro." << endl;
        currentPlayer->SetElektro(currentPlayer->GetElektro() + overview.GetPayment(numPoweredCities));

        // If game over
        if (citiesEndOfGame <= numPoweredCities) {
            gameOver = true;
            winners.push_back(currentPlayer);
        }
    }

    // If game is over
    if (gameOver) {

        if (winners.size() == 1)
            winner = winners[0];

        // If more than one winner
        else if(winners.size() > 1) {
            
            // Get max
            int maxMoney = winners[0]->GetElektro();
            for (int i = 1; i < winners.size(); i++) {
                if (winners[i]->GetElektro() > maxMoney)
                    maxMoney = winners[i]->GetElektro();
            }

            // Take the max only
            for (int i = 0; i < winners.size(); i++)
                if (winners[i]->GetElektro() != maxMoney)
                    winners.erase(winners.begin() + i);

            // If only one winner
            if (winners.size() == 1) {
                winner = winners[0];
            
            // If more than one winner
            } else if(winners.size() > 1) {
                
                int maxCitiesIndex = 0;
                for (int i = 1; i < winners.size(); i++) {
                    if (winners[i]->GetHouses().size() > winners[maxCitiesIndex]->GetHouses().size())
                        maxCitiesIndex = i;
                }
                winner = winners[maxCitiesIndex];
            }
        }
    }

    // Change the visible power plants
    if (phase == 1 || phase == 2) {
        // Place highest plant under stack 
        cardStack.PlaceHighestVisibleAtEndOfStack();
        cardStack.DrawPlant();
    }
    else if (phase == 3) {
        // Remove lowest plant from the game
        cardStack.RemoveLowestVisible();
        cardStack.DrawPlant();
    }

    // Check if we drew the step 3 card
    if (cardStack.GetJustDrewStep() == 3) {
        cardStack.AdjustForStep3();
        cardStack.ShuffleStack();
        phase = 3;
        
        cout << "Entering phase 3." << endl;
    }


    // Restock raw materials
    for (int i = 0; i < res::total; i++) {
        Resource resource = static_cast<Resource>(COAL + i);
        rMarket->AddResource(resource, overview.GetRuleByNumOfPlayers(players.size()).GetResourceAt(phase, resource));
    }
}

/// Runs the main game loop
void Game::PlayGame() {

    while (!gameOver) {

        // Display turn number
        cout << "Turn " << fullTurn << endl;

        // Display players info
        for (auto p : players) p->DisplayStatus();

        // Reset step counter
        playStep = 0;

        // Step 1
        cout << endl << *overview.GetSteps()[playStep++] << endl;

        // Update players order ASC order
        UpdatePlayOrder(false);

        cout << "The order is:" << endl;
        for (auto p : playerOrder)  cout << *p << " ";
        cout << endl;

        // Resources
        PrintRemainingResources();

        // Step 2
        cout << endl << *overview.GetSteps()[playStep++] << endl;

        // Auction
        AuctionPlants();

        // Display players info
        for (auto p : players) p->DisplayStatus();

        // Step 3
        cout << endl << *overview.GetSteps()[playStep++] << endl;
        
        // Buy resources
        BuyRawMaterials();

        // Display players info
        for (auto p : players) p->DisplayStatus();

        // Step 4
        cout << endl << *overview.GetSteps()[playStep++] << endl;
        
        // Buy houses
        BuyCities();

        // Display players info
        for (auto p : players) p->DisplayStatus();

        // Step 5
        cout << endl << *overview.GetSteps()[playStep++] << endl;

        // Bureaucracy
        // Note: The winner is announced in Step 5 and not in Step 4
        Bureaucracy();

        // Print score
        PrintScore();

        // Increase turn
        fullTurn++;
    };
}

void Game::PrintScore() const
{

    vector<shared_ptr<Player>> tmpPlayer = players;

    sort(tmpPlayer.begin(), tmpPlayer.end(), [](shared_ptr<Player> p1, shared_ptr<Player> p2) { return !comparePlayerPriority(p1, p2); });

    cout << endl << "Displaying the score track:" << endl;
    for (auto player : tmpPlayer)
        cout << *player << " score: " << player->GetHouses().size() << endl;
    cout << endl;
}

void Game::PrintRemainingResources() {
    std::map<Resource, int> total;

    for (int i = 0; i < res::total; i++) {
        total[static_cast<Resource>(i)] = rMarket->GetCapacityResource(static_cast<Resource>(i));
    }

    for (int i = 0; i < res::total; i++) {

        for (auto player : players) {
            total[static_cast<Resource>(i)] -= player->GetResources(static_cast<Resource>(i));
        }
    }

    for (int i = 0; i < res::total; i++) {
        total[static_cast<Resource>(i)] -= rMarket->GetNbResource(static_cast<Resource>(i));
    }

    cout << "\nRemaining resources:" << endl;
    for (auto mapData : total) {
        cout << GetResourceName(mapData.first) << ": " << mapData.second << endl;
    }
    cout << endl;
}