#include "ResourceMarketLevel.h"

ResourceMarketLevel::ResourceMarketLevel(int oilCapacity, int coalCapacity, int uraniumCapacity, int garbageCapacity) : capacity(MAX_VALUE), counter(MAX_VALUE) {
	capacity[ResourceMarket::Resouce::OIL] = oilCapacity;
	capacity[ResourceMarket::Resouce::COAL] = coalCapacity;
	capacity[ResourceMarket::Resouce::GARBAGE] = garbageCapacity;
	capacity[ResourceMarket::Resouce::URANIUM] = uraniumCapacity;
};

ResourceMarketLevel::~ResourceMarketLevel() {
	// Nothing
}

/// Add 1 to a specific resource
bool ResourceMarketLevel::addResource(ResourceMarket::Resouce resource) {
	if (counter[resource] == capacity[resource]) return false;
	++counter[resource];
	return true;
}