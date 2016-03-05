#include "ResourceMarketLevel.h"

ResourceMarketLevel::ResourceMarketLevel(int coalCapacity, int oilCapacity, int garbageCapacity, int uraniumCapacity, int cost) : capacity(res::total), counter(res::total) {
	capacity[Resource::OIL] = oilCapacity;
	capacity[Resource::COAL] = coalCapacity;
	capacity[Resource::GARBAGE] = garbageCapacity;
	capacity[Resource::URANIUM] = uraniumCapacity;
	this->cost = cost;
};

ResourceMarketLevel::~ResourceMarketLevel() {
	// Nothing
}

/// Add 1 to a specific resource
bool ResourceMarketLevel::addResource(Resource resource) {
	if (counter[resource] == capacity[resource]) return false;
	++counter[resource];
	return true;
}