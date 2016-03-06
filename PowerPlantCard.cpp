#include "PowerPlantCard.h"

PowerPlantCard::~PowerPlantCard() {
	// Nothing
}

/// Place resrouces if possible
bool PowerPlantCard::PlaceResource(Resource resource, int amount) {

	// If resource not allowed or capacity overflow, return false
	if (activeResources.find(resource) == activeResources.end() || GetTotalPlacedResources() + amount > capacity)
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