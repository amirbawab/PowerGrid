#pragma once

#include "Resource.h"
#include "Card.h"
#include <set>

class PowerPlantCard : public Card {
private:
	int price = 0;
	int power = 0;
	int capacity = 0;
	std::set<Resource> resources;
public:
	PowerPlantCard() {};
	PowerPlantCard(int price, int power, int capacity) : price(price), power(power), capacity(capacity) {};
	~PowerPlantCard();
	std::set<Resource> const& GetResources() const { return resources; };
	void AddResource(Resource resource) { resources.insert(resource); }
	int GetPrice() const { return price; };
	int GetPower() const { return power; };
	int GetCapcity() const { return capacity; };
};

