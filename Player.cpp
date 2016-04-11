#include "Player.h"
#include <iostream>
#include "Helpers.h"
#include "City.h"

using std::cout;
using std::endl;
using std::cin;
using std::to_string;
using std::string;

Player::Player()
{
}

Player::Player(string name, std::shared_ptr<HouseColor> color, int elektro) :
    color {color}, name {name}, elektro {elektro}, resources(res::total)
{ }

Player::~Player()
{
}

bool Player::BuyHouse(shared_ptr<House> house)
{
    int housePrice = house->GetPrice();

    // Check for enough money
    if (elektro < housePrice)
    {
        Error("Player " + name + " with color '" + color->getName() + "' doesn't have" +
              " enough elektra to buy the house with price: " +
              to_string(housePrice) + "\n");
        return false;
    }

    // Check if already bought in this city
    for (auto tmpHouse : houses)
        if (tmpHouse->GetCity() == house->GetCity())
            return false;

    // If can't add (City saturated)
    if (!house->GetCity()->AddHouse(house.get()))
        return false;

    elektro -= housePrice;
    houses.push_back(house);
    return true;
}

void Player::DisplayStatus() const
{
    string output;
    string separator;
    string message = "Displaying status for Player '" + name +
        "' with color '" + color->getName() + "':\n";

    for (int i = 0; i < message.length() + 20; i++)
        separator += "-";

    separator += "\n";
    output += separator + message;
    output += "\tTotal money: " + to_string(elektro) + "\n";
    output += "\tNumber of houses: " + to_string(houses.size()) + "\n";
    
    // Print houses
    for (int i = 0; i < houses.size(); i++)
        output += "\t\tHouse #" + to_string(i + 1) + " - Name: " + houses[i]->GetCity()->GetName() + ", Price: " + to_string(houses[i]->GetPrice()) + "\n";

    output += "\tNumber of power plants: " + to_string(powerPlants.size()) + "\n";

    // Print power plants
    for (int i = 0; i < powerPlants.size(); i++) {

        // Cache
        std::shared_ptr<PowerPlantCard> powerPlant = powerPlants[i];

        output += "\t\tPowerplant #" + to_string(i + 1) + " - Price: " + to_string(powerPlant->GetPrice()) + ", Power: " + to_string(powerPlant->GetPower()) +",Capacity: " + to_string(powerPlant->GetCapacity());
        
        // If takes resources
        if (powerPlant->GetActiveResources().size() > 0) {
            output += ", Resource(s): ";
            for (Resource resource : powerPlant->GetActiveResources())
                output += " " + GetResourceName(resource);
            output += "\n";
        }

        // If has resources placed
        if (powerPlant->GetTotalPlacedResources() > 0) {
            output += "\t\t\tPlaced resources:\n";
            for (auto resource : powerPlant->GetPlacedResources()) {
                if (resource.second > 0)
                    output += "\t\t\t" + GetResourceName(resource.first) + ": " + to_string(resource.second) + "\n";
            }
        }
        else {
            output += "\t\t\tNo resources placed.\n";
        }
    }

    output += separator;
    cout << output;
}

/// Add power plant if possible
bool Player::AddPowerPlant(std::shared_ptr<PowerPlantCard> powerPlant) {
    if (powerPlants.size() == 3) return false;
    powerPlants.push_back(powerPlant);
    return true;
}

/// Replaces a power plant by another
void Player::ReplacePowerPlant(shared_ptr<PowerPlantCard> plant, int index) {
    powerPlants[index] = plant;
    // Transfer resources
    // to do
}

/// Buys a power plant from the available plants
bool Player::BuyPowerPlant(CardStack& cardStack, int index, int cost) {
    // Note that only indices 0-3 are available to buy (4-7 are future plants)
    if (index >= 0 && index <= 3 && HasElektro(cost)) {
        SetElektro(GetElektro() - cost);
        if (powerPlants.size() < 3)
            AddPowerPlant(cardStack.GetPlant(index));
        else {
            int i;
            do {
                cout << "Enter index of power plant to replace: " << endl;
                cin >> i;
                
                // Check invalid value
                if (!cin.good()) {
                    i = PG::INVALID;
                    cin.clear();
                    cin.ignore(INT_MAX, '\n');
                }

            } while (!(i >= 0 && i <= 2));
            ReplacePowerPlant(cardStack.GetPlant(index), i);
        }
            
        cardStack.RemovePlant(index);
        cardStack.DrawPlant();
        return true;
    }
    return false;
}

/// Get the number corresponding to the highest power plant
int Player::GetHighestPowerPlant() {
    int max = 0;
    for (std::shared_ptr<PowerPlantCard> plant : powerPlants) {
        if (plant->GetPrice() > max)
            max = plant->GetPrice();
    }
    return max;
}

string Player::GetInitials(int max) {
    string result = "";
    for (int i = 0; i < name.size() && max > 0; i++) {
        if (i == 0 && name[i] != ' ') {
            result += toupper(name[i]);
            max--;
        
        } else if (i > 0 && name[i-1] == ' ' && name[i] != ' ') {
            result += toupper(name[i]);
            max--;
        }
    }
    return result;
}


/// Buy the number of resource specified from the resource market
bool Player::BuyResources(ResourceMarket& rMarket, shared_ptr<PowerPlantCard> plant, Resource resource, int amount) {
    int price = rMarket.GetPrice(resource, amount);

    if (amount >= 0 && 
        HasElektro(price) && 
        (plant->GetTotalPlacedResources() + amount <= 2 * plant->GetCapacity()) &&
        (plant->GetActiveResources().find(resource) != plant->GetActiveResources().end())) {

        plant->PlaceResource(resource, amount);
        SetElektro(GetElektro() - amount);
        rMarket.RemoveResource(resource, amount);
        return true;
    }
    return false;
}

bool Player::BuyResources(ResourceMarket& rMarket, shared_ptr<PowerPlantCard> plant, string resource, int amount) {
    return BuyResources(rMarket, plant, GetResourceByName(resource), amount);
}