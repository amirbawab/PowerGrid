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
	bool AddResource(Resource resource, int amount);
    bool AddResource(string resourceName, int amount);
	bool RemoveResource(Resource, int);
	bool RemoveResource(string, int);

	int GetNbResource(Resource);
	int GetCapacityResource(Resource);

	int GetPrice(Resource, int);
	int GetPrice(string, int);


};

