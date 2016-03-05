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
void ResourceMarket::AddResource(Resource resource, int amount) {
	int right = levels.size()-1;
	while (right >= 0 && amount > 0) {
		if (levels[right]->addResource(resource))
			amount--;
		else
			right--;
	}
}

void ResourceMarket::AddResource(string resourceName, int amount)
{
    AddResource(GetResourceByName(resourceName), amount);
}
