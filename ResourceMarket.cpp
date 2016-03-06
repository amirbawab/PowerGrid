#include "ResourceMarket.h"
#include "Helpers.h"

ResourceMarket::ResourceMarket() : levels(12) {

	// Create the left 8 levels
	for (int i = 0; i < 8; i++)
		levels[i] = std::make_shared<ResourceMarketLevel>(ResourceMarketLevel(3, 3, 3, 1, i+1));

	// Create the left 8 levels
	for (int i = 0; i < 4; i++)
		levels[i + 8] = std::make_shared<ResourceMarketLevel>(ResourceMarketLevel(0, 0, 0, 1, 10 + i*2));
}

ResourceMarket::~ResourceMarket() {
	// Nothing
}

/// Add resource at most 'amount' amount
bool ResourceMarket::AddResource(Resource resource, int amount) {
	if (GetNbResource(resource) + amount > GetCapacityResource(resource))
		return false;

	int right = levels.size()-1;
	while (right >= 0 && amount > 0) {
		if (levels[right]->AddResource(resource))
			amount--;
		else
			right--;
	}
	return true;
}

bool ResourceMarket::AddResource(string resourceName, int amount)
{
    return AddResource(GetResourceByName(resourceName), amount);
}

/// Remove resources at most 'amount' amount
bool ResourceMarket::RemoveResource(Resource resource, int amount) {
	if (GetNbResource(resource) < amount)
		return false;

	int right = 0;
	while (right < levels.size() && amount > 0) {
		if (levels[right]->RemoveResource(resource))
			amount--;
		else
			right++;
	}
	return true;
}

bool ResourceMarket::RemoveResource(string resource, int amount) {
	return RemoveResource(GetResourceByName(resource), amount);
}

/// Gets the number of available resources of the input type
int ResourceMarket::GetNbResource(Resource re) {
	int total = 0;
	for (int i = 0; i < size(levels); i++) {
		total += levels[i]->GetCounter(re);
	}
	return total;
}

/// Gets the total capacity of the market to hold the resource of the input type
int ResourceMarket::GetCapacityResource(Resource re) {
	int total = 0;
	for (int i = 0; i < size(levels); i++) {
		total += levels[i]->GetCapacity(re);
	}
	return total;
}

int ResourceMarket::GetPrice(Resource resource, int amount) {
	int quantityLeft = amount;
	int totalPrice = 0;
	int quantityInLevel;
	// Check if there are enough resources
	if (GetNbResource(resource) < amount) {
		return 9999;
	}
}


