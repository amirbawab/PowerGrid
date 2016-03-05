#include "Player.h"
#include <iostream>
#include "Helpers.h"

using std::cout;
using std::endl;
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

bool Player::BuyHouse(House& house)
{
    int housePrice = house.GetPrice();

    if (elektro < housePrice)
    {
        Error("Player " + name + " with color '" + color->getName() + "' doesn't have" +
              " enough elektra to buy the house with price: " +
              to_string(housePrice) + "\n");
        return false;
    }

    if (!house.GetCity()->AddHouse())
        return false;

    elektro -= housePrice;
    houses.push_back(std::make_shared<House>(house));
    return true;
}

void Player::DisplayStatus() const
{
    string output;
    string separator;
    string message = "Displaying status for Player '" + name +
        "' with color '" + color->getName() + "':\n";

    for (int i = 0; i < message.length(); i++)
        separator += "-";

    separator += "\n";
    output += separator + message;
    output += "\tTotal money: " + to_string(elektro) + "\n";
    output += "\tNumber of houses: " + to_string(houses.size()) + "\n";
    output += separator;
    cout << output;
}

/// Add power plant if possible
bool Player::AddPowerPlant(std::shared_ptr<PowerPlantCard> powerPlant) {
	if (powerPlants.size() == 3) return false;
	powerPlants.push_back(powerPlant);
	return true;
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
