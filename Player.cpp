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

Player::Player(string name, HouseColor color, int elektro) :
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
        Error("Player " + name + " with color '" + color.getName() + "' doesn't have" +
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
        "' with color '" + color.getName() + "':\n";

    for (int i = 0; i < message.length(); i++)
        separator += "-";

    separator += "\n";
    output += separator + message;
    output += "\tTotal money: " + to_string(elektro) + "\n";
    output += "\tNumber of houses: " + to_string(houses.size()) + "\n";
    output += separator;
    cout << output;
}

bool Player::AddResource(Resource resource, int amount) {
	
	// Prepare tmp vectors
	std::vector<std::vector<std::vector<int>>> permutationLists(powerPlants.size());
	std::vector<int> tmpPermutation(res::total);

	// Calculate user card permutation
	for (int i = 0; i < powerPlants.size(); i++) {
		std::vector<std::vector<int>> permutation;
		std::vector<int> tmpVector(res::total);
		CalculatePermutation(permutation, powerPlants[i], tmpVector, 0, 0);
		permutationLists[i] = permutation;
	}

	// Print permutation result
	/*for (auto a : permutationLists) {
		for (auto b : a) {
			for (auto c : b) {
				std::cout << c << " ";
			}
			std::cout << endl;
		}
		std::cout << endl;
	}*/

	// Add resources by default
	resources[resource] += amount;
	
	// If cannot buy
	if (!CanAddResource(permutationLists, tmpPermutation, 0)) {
		resources[resource] -= amount;
		return false;
	}

	// Added
	return true;
}

/// Calculate the permutation of the power plant (twice capacity)
void Player::CalculatePermutation(std::vector<std::vector<int>> &permutation, std::shared_ptr<PowerPlantCard> powerPlant, std::vector<int> &tmpVector, int total, int index) {

	// If total generated
	if (total == powerPlant->GetCapcity() * 2) {
		permutation.push_back(tmpVector);
		return;
	}

	// If total not generated and out of bound
	if (index == res::total) return;

	// If resouce not found
	if (powerPlant->GetResources().find(Resource(index)) == powerPlant->GetResources().end()) {
		// Skip
		CalculatePermutation(permutation, powerPlant, tmpVector, total, index + 1);
	
	// If resource found
	} else {
		// Try 0, 1, ...
		for (int i = 0; i <= powerPlant->GetCapcity() * 2; i++) {

			// If less than total, keep trying
			if (total + i <= powerPlant->GetCapcity() * 2) {
				tmpVector[index] = i;
				CalculatePermutation(permutation, powerPlant, tmpVector, total + i, index + 1);
				tmpVector[index] = 0;
			}
			else break;
		}
	}
}

/// Checks if the current combination of resource and power plant is allowed (twice the amount)
bool Player::CanAddResource(std::vector<std::vector<std::vector<int>>> &permutationLists, std::vector<int> &tmpPermutation, int powerPlantIndex) {

	// Check if permutation resources are more than the existing ones
	bool valid = true;
	for (int i = 0; i < resources.size() && valid; i++)
		valid &= tmpPermutation[i] >= resources[i];
		
	// If valid
	if (valid) return true;

	// If no more powerplants
	if (powerPlantIndex == powerPlants.size()) return false;

	// Loop on permutation lists
	for (int i = 0; i < permutationLists[powerPlantIndex].size(); i++) {
		
		// Add resources to tmp
		for (int k = 0; k < res::total; k++)
			tmpPermutation[k] += permutationLists[powerPlantIndex][i][k];

		if (CanAddResource(permutationLists, tmpPermutation, powerPlantIndex + 1))
			return true;

		// Remove resources from tmp
		for (int k = 0; k < res::total; k++)
			tmpPermutation[k] -= permutationLists[powerPlantIndex][i][k];
	}

	// Default false
	return false;
}

bool Player::AddPowerPlant(std::shared_ptr<PowerPlantCard> powerPlant) {
	if (powerPlants.size() == 3) return false;
	powerPlants.push_back(powerPlant);
	return true;
}