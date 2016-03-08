#include "PowerPlantCard.h"

PowerPlantCard::~PowerPlantCard() {
	// Nothing
}

/// Place resrouces if possible
bool PowerPlantCard::PlaceResource(Resource resource, int amount) {

	// If resource not allowed or capacity overflow, return false
	if (activeResources.find(resource) == activeResources.end() || GetTotalPlacedResources() + amount >  2 * capacity)
		return false;

	// Add amount
	placedResources[resource] += amount;

	// Added!
	return true;
}

/// Get total in map
int PowerPlantCard::GetTotalPlacedResources() {
	int total = 0;
	for (auto iterator = placedResources.begin(); iterator != placedResources.end(); iterator++)
		total += iterator->second;
	return total;
}

/// Consumes the number of resources needed to power the plant (for single resource plants)
bool PowerPlantCard::ConsumeResources() {
	if (activeResources.size() == 1 && capacity < GetTotalPlacedResources()) {
		for (Resource r : activeResources) {
			placedResources[r] -= capacity;
		}
		return true;
	}
	else
		return false;
}

/// Consumes the specified number of resources needed to power the plant (for hybrid resource plants)
bool PowerPlantCard::ConsumeResources(Resource resource1, int amount1, Resource resource2, int amount2) {
	return true;
}

/// Print details about the card
void PowerPlantCard::PrintDetails() {
	std::cout << std::endl;
	std::cout << "-----------------------------------------------" << std::endl;
	std::cout << "Price: " << GetPrice() << ", Power: " << GetPower() << ", Capacity: " << GetCapacity();

	// If takes resources
	if (GetActiveResources().size() > 0) {
		std::cout << ", Resource(s): ";
		for (Resource resource : GetActiveResources())
			std::cout << " " << GetResourceName(resource);
	}
	std::cout << std::endl;

	// If has resources placed
	if (GetPlacedResources().size() > 0) {
		std::cout << "\tPlaced resources:";
		for (auto resource : GetPlacedResources()) {
			if (resource.second > 0)
				std::cout << GetResourceName(resource.first) << ": " << resource.second << std::endl;
		}
	}
	else {
		std::cout << "No resources placed." << std::endl;
	}
	std::cout << "-----------------------------------------------" << std::endl;
	std::cout << std::endl;
}