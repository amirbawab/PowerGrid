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
void ResourceMarket::addResource(Resource resource, int amount) {
	int left = 0;
	while (left < levels.size() && amount > 0) {
		if (levels[left]->addResource(resource))
			amount--;
		else
			left++;
	}
}

void ResourceMarket::AddResource(string resourceName, int amount)
{
    addResource(GetResourceByName(resourceName), amount);
}
