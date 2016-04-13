#include "Game.h"
#include "GameStatus.h"
#include <sstream>
#include <iomanip>
#include <QDir>

Game::Game() {

    // Log
    cout << "Game start!" << endl;

    // TODO Remove this ?
    // Save map
    //if (map->SaveFile("SavedMap.xml"))
    //	cout << "Map saved." << endl;
}

void Game::NewGame() {
    // Create a file name for saving
    auto t = time(nullptr);
    auto tm = *localtime(&t);
    std::stringstream ss;
    ss << std::put_time(&tm, "%d-%m-%Y-%m-%d_%H-%M-%S");
    string saveDirectory = "Resources/saved games";
    QDir currentDirectory(".");
    currentDirectory.mkdir(saveDirectory.c_str());
    this->savedFileName = currentDirectory.absolutePath().toStdString() +
        "/" + saveDirectory + "/PG-" + ss.str() + ".xml";
    
    // New game message
    cout << "New game selected" << endl;

    // Initialize components
    fullTurn = 1;
    phase = 0;
    playStep = 1;
}

void Game::ConfigNewGame(string map, int numberOfPlayers, bool customMap) {

    // Log
    cout << "Map selected: " << map << ". Number of players selected: " << numberOfPlayers << endl;

    Reset();

    // Initialize game
    GameStatus::GetInstance().Init(this, map, ":/PowerGrid/Resources/config/Config.xml", customMap);

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
void Game::LoadGame(string fileName) {

    this->savedFileName = fileName;

    Reset();

    // If loading saved game
    GameStatus::GetInstance().LoadFile(this, fileName);

    // Set the current player
    currentPlayer = playerOrder[0];

    Notify();
}


/// Used for determining the turn order
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
/// Phase 0, remove regions from the map
void Game::Phase0Start()
{
    regionsToRemove = map->GetRegions().size() - overview.GetRuleByNumOfPlayers(players.size()).region;
    Phase0RemoveRegions1();
}

void Game::Phase0RemoveRegions1()
{
    messageText = "Click on the region you want to remove, then press 'OK'" +
        string("<br />(remaining region(s): <font color='red'><b>") +
        std::to_string(regionsToRemove) + "</b></font>)";

    pickedRegion.reset();
    Notify();
}

void Game::Phase0RemoveRegions2()
{
    if (!pickedRegion)
        return Phase0RemoveRegions1();

    map->RemoveRegion(pickedRegion);

    // If more regions to remove
    if (--regionsToRemove > 0)
        return Phase0RemoveRegions1();

    // Phase is now 1
    phase = 1;

    Step1Start();

}

/// Step 1, sets the players in the proper turn order
void Game::UpdatePlayOrder(bool reverse) {
    sort(playerOrder.begin(), playerOrder.end(), comparePlayerPriority);

    if (reverse)
        std::reverse(playerOrder.begin(), playerOrder.end());
}

void Game::Step1Start() {
    

    cout << "Starting step 1 ..." << endl;
    messageText = "Player order updated. Press `OK` to continue ...";
    playStep = 1;

    // Notify GUI
    Notify();
}

void Game::Step2Start() {
    // GUI Message: "Step 2"
    cout << "Starting step 2 ..." << endl;
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
    messageText = "The highest bid is now <font color='red'><b>" +
        std::to_string(currentBid) + "</b></font>, enter your bid:";
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
                SetErrorMessageTextBox("Not Enough Money", "You don't enough money to place a bid of this amount");
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
            SetErrorMessageTextBox("Bidding Error", "Your bid must be greater than the current bid");
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
        cout << *currentPlayer << " won this auction for " << currentBid << endl;
        canBuy[currentPlayer.get()] = false;
        
        // Check if player already has 3 power plants
        if (currentPlayer->GetPowerPlants().size() == 3) {
            return Step2ReplacePlant1();
        }
        else {
            currentPlayer->BuyPowerPlant(cardStack, plantIndex, currentBid);
        }

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

void Game::Step2ReplacePlant1() {
    messageText = "Choose a power plant to replace";
    Notify();
}

void Game::Step2ReplacePlant2(int plantToReplaceIndex) {
    currentPlayer->ReplacePowerPlant(cardStack.GetPlant(plantIndex), plantToReplaceIndex);
    Step2BidEnd();
}

void Game::Step2BidEnd() {

    // Update info message in GUI
    SetInfoMessageTextBox("Winner", "<b>" + highestBidder->GetName() + "</b> won the bid for this round");

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
    currentBid = 0;

    // Else we go to the next player to pick a plant for auction
    return Step2PickPlant1();
}

void Game::Step2End() {

    // If we drew the step 3 card during any of the auction, only now do we adjust the size of the market and phase (at the end of all auctions)
    if (cardStack.GetJustDrewStep() == 3) {
        cardStack.AdjustForStep3();
        phase = 3;
    }

    // Save the game
    GameStatus::GetInstance().SaveFile(this, savedFileName);

    // Start step 3
    Step3Start();
}

void Game::Step3Start() {
    // GUI Message : "Step 3"
    cout << "Starting step 3 ..." << endl;
    playStep = 3;

    // Reorder players (worst starts)
    UpdatePlayOrder(true);

    // Reset attributes and start buying resources
    currentPlayer = playerOrder[0];
    resourceIndex = 0;
    powerPlantIndex = 0;

    auto rSet = currentPlayer->GetPowerPlants()[powerPlantIndex]->GetActiveResources();
    vector<Resource> temp;
    copy(rSet.begin(), rSet.end(), back_inserter(temp));
    resourceIdentity = temp[resourceIndex];

    Step3BuyingResources1();
}

void Game::Step3BuyingResources1() {
    messageText = "How many <font color='red'><b>" + GetResourceName(resourceIdentity) +
        "</b></font> would you like to buy for the selected<br />power plant?";
    Notify();
}

void Game::Step3BuyingResources2(int amount) {
    resourceAmount = amount;
    
    // Buy resources and check if allowed to do so
    bool allowed = currentPlayer->BuyResources(*rMarket, currentPlayer->GetPowerPlants()[powerPlantIndex], resourceIdentity, amount);
    if (!allowed) {
        SetErrorMessageTextBox("Amount Error", "You can't buy the amount of <b>" + std::to_string(amount) + " " +
                               GetResourceName(resourceIdentity) + "(s)</b> for this power plant!");
        return Step3BuyingResources1();
    }

    // Find next playing step
    // Check if there are other resources to iterate through
    vector<Resource> temp;
    auto rSet = currentPlayer->GetPowerPlants()[powerPlantIndex]->GetActiveResources();
    copy(rSet.begin(), rSet.end(), back_inserter(temp));
    resourceIndex++;
    
    if (resourceIndex >= temp.size()) {
        // If no more resources, go to next power plant
        powerPlantIndex++;
        resourceIndex = 0;
        resourceAmount = 0;

        if (powerPlantIndex >= currentPlayer->GetPowerPlants().size()) {
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
    rSet = currentPlayer->GetPowerPlants()[powerPlantIndex]->GetActiveResources();
    copy(rSet.begin(), rSet.end(), back_inserter(temp));
    resourceIdentity = temp[resourceIndex];
    resourceAmount = 0;

    return Step3BuyingResources1();
}

void Game::Step3End() {

}


void Game::Step4Start() {
    // GUI Message: "Step 4" 
    cout << "Starting step 4 ..." << endl;
    playStep = 4;

    // Reorder players (worst starts)
    UpdatePlayOrder(true);
    currentPlayer = playerOrder[0];

    Step4BuyingCities1();
}

void Game::Step4BuyingCities1() {
    messageText = "Select a city to buy, and press OK";
    pickedCity.reset();
    Notify();
}

void Game::Step4BuyingCities2() {

    if (!pickedCity) {
        // Player skipped, go to next player
        currentPlayer = playerOrder[GetNextPlayerIndex()];

        if (currentPlayer.get() == playerOrder[0].get()) {
            return Step4End();
        }

        return Step4BuyingCities1();
    }

    // Check if city is full
    if (pickedCity->GetNumberOfHouses() == phase) {
        SetErrorMessageTextBox("Buying City Error",
                               "Cannot buy a house. <b>" + pickedCity->GetName() +
                               "</b> is already saturated for this phase.");
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
        SetErrorMessageTextBox("Not Enough Money", "Not enough money to buy <b>" +
                               pickedCity->GetName() + "</b> for a total cost of <b>" +
                               std::to_string(cost) + "</b> Elektro.");
        return Step4BuyingCities1();
    }

    // Buy the city
    auto newHouse = std::make_shared<House>(pickedCity, currentPlayer->GetColor());
    newHouse->SetPrice(cost);
    currentPlayer->BuyHouse(newHouse);
    SetInfoMessageTextBox("Buying City Success", "<b>" + currentPlayer->GetName() +
                          "</b> has bought a house at <b>" + pickedCity->GetName() +
                          "</b> for a total cost of <b>" + std::to_string(cost) + "</b> Elektro.");

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

    // Save the game
    GameStatus::GetInstance().SaveFile(this, savedFileName);

    Step5Start();
}

void Game::Step5Start() {
    // GUI Message: "Step 5"
    cout << "Starting step 5 ..." << endl;
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
    numPoweredCities = 0;
    winners.clear();
    
    Step5UsingPlants1();
}

void Game::Step5UsingPlants1() {
    messageText = "Which power plant do you want to use?";
    step5SelectResource = false;
    Notify();
}

void Game::Step5UsingPlants2(std::shared_ptr<PowerPlantCard> pickedPlant2) {
    
    pickedPlant = pickedPlant2;

    // If skip, get paid and go to next player
    if (!pickedPlant) {

        // Take min between number of houses that can be powered and the actual number of houses
        int playerHouses = currentPlayer->GetHouses().size();
        numPoweredCities = std::min(numPoweredCities, playerHouses);

        // Get money
        cout << *currentPlayer << " powered " << std::to_string(numPoweredCities) << " house(s), earning " << std::to_string(overview.GetPayment(numPoweredCities)) << " Elektro." << endl;
        currentPlayer->SetElektro(currentPlayer->GetElektro() + overview.GetPayment(numPoweredCities));

        // If game over
        if (overview.GetRuleByNumOfPlayers(players.size()).citiesEndOfGame <= playerHouses) {
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
        if (pickedPlant->ConsumeResources(*pickedPlant->GetActiveResources().begin(), pickedPlant->GetCapacity())) {
            numPoweredCities += pickedPlant->GetPower();
            SetInfoMessageTextBox("Resources Consumed", "Resources consumed for the selected power plant");
        }
        else {
            SetErrorMessageTextBox("Resources Not Consumed", "Not enough resources to power the selected power plant");
        }
    }
    else {
        // If there is more than one resource, need to specify quantity of each
        return Step5ChoosingResource1();
    }

    // Pick another plant to use
    return Step5UsingPlants1();
}

void Game::Step5ChoosingResource1() {
    // GUI 
    messageText = "Click on the resources of the selected power plant";
    step5SelectResource = true;
    Notify();
}

void Game::Step5ChoosingResource2(vector<int> resourceAmounts) {
    
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

        int resourceAdded = overview.GetRuleByNumOfPlayers(players.size()).GetResourceAt(phase, resource);
        // Check that there are enough unused resources to add
        resourceAdded = std::min(resourceAdded, getUnusedResources(resource)); 
        rMarket->AddResource(resource, resourceAdded);
    }

    fullTurn++;

    // Save the game
    GameStatus::GetInstance().SaveFile(this, savedFileName);

    Step1Start();
}



void Game::GameEnd() {
    // GUI Message: "Game is over! The winner is " + winner 
}

int Game::getUnusedResources(Resource resource) {
    // This returns the number of resources that are not in the market or owned by players
    // Find total number of used resources
    int total = 0;

    // Add players' resources
    for (shared_ptr<Player> p : players) {
        total += p->GetResources(resource);
    }
    // Add market resources
    total += rMarket->GetNbResource(resource);

    // Get unused resources
    return rMarket->GetCapacityResource(resource) - total;
}

int Game::GetNextPlayerIndex() {
    return (distance(playerOrder.begin(), find(playerOrder.begin(), playerOrder.end(), currentPlayer)) + 1) % playerOrder.size();
}

void Game::Reset()
{
    allCards = vector<shared_ptr<Card>>();
    players = vector<shared_ptr<Player>>();
    houseColor = vector<shared_ptr<HouseColor>>();
    playerOrder = vector<shared_ptr<Player>>();
    rMarket = std::make_shared<ResourceMarket>();
    cardStack.ResetCards();
    cardStack.ResetVisiblePlants();
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

