#pragma once

#include "ResourceMarketLevel.h"
#include <vector>
#include <memory>

class ResourceMarket {
private:
	std::vector<std::shared_ptr<ResourceMarketLevel>> levels; 
public:
	ResourceMarket();
	~ResourceMarket();
	void addResource(Resource resource, int amount);
    void AddResource(string resourceName, int amount);
};

