#pragma once

#include "Resource.h"
#include <vector>

class ResourceMarketLevel {
private:
	std::vector<int> capacity;
	std::vector<int> counter;
	int cost = 0;
public:
	ResourceMarketLevel() {};
	ResourceMarketLevel(int, int, int, int, int cost);
	~ResourceMarketLevel();
	int GetCapacity(Resource resource) const { return capacity[resource]; };
	int GetCounter(Resource resource) const { return counter[resource]; };
	bool AddResource(Resource);
	bool RemoveResource(Resource);
};

