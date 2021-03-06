#include "ResourceMarketLevel.h"

ResourceMarketLevel::ResourceMarketLevel(
    int coalCapacity, int oilCapacity, int garbageCapacity, int uraniumCapacity, int cost) :
    capacity(res::total), counter(res::total) {

    capacity[OIL] = oilCapacity;
    capacity[COAL] = coalCapacity;
    capacity[GARBAGE] = garbageCapacity;
    capacity[URANIUM] = uraniumCapacity;
    this->cost = cost;
};

ResourceMarketLevel::~ResourceMarketLevel() {
    // Nothing
}

/// Add 1 to a specific resource
bool ResourceMarketLevel::AddResource(Resource resource) {
    if (counter[resource] == capacity[resource]) return false;
    ++counter[resource];
    return true;
}

/// Remove 1 from a specific resource
bool ResourceMarketLevel::RemoveResource(Resource resource) {
    if (counter[resource] == 0) return false;
    --counter[resource];
    return true;
}