#pragma once

#include "ResourceMarket.h"
#include <vector>

class ResourceMarketLevel {
private:
	const static int MAX_VALUE = 4;
	std::vector<int> capacity;
	std::vector<int> counter;
public:
	ResourceMarketLevel() {};
	ResourceMarketLevel(int oilCapacity, int coalCapacity, int uraniumCapacity, int garbageCapacity);
	~ResourceMarketLevel();
	int GetCapacity(ResourceMarket::Resouce resource) const { return capacity[resource]; };
	int GetCounter(ResourceMarket::Resouce resource) const { return counter[resource]; };
	bool addResource(ResourceMarket::Resouce resource);
};

