#pragma once

#include "Resource.h"
#include "Card.h"
#include <set>
#include <map>

class PowerPlantCard : public Card {
private:
	int price = 0;
	int power = 0;
	int capacity = 0;  // number of resources needed to power the plant (but it can hold up to 2 * capacity)
	std::set<Resource> activeResources;
	std::map<Resource, int> placedResources;
	
public:
	PowerPlantCard() {}
	PowerPlantCard(int price, int power, int capacity) : price(price), power(power), capacity(capacity) {}
	~PowerPlantCard();
	
	void AddActiveResource(Resource resource) { activeResources.insert(resource); }
	void AddActiveResource(string resourceName) { AddActiveResource(GetResourceByName(resourceName)); }
	bool PlaceResource(Resource resource, int amount);
	bool PlaceResource(string resourceName, int amount) { return PlaceResource(GetResourceByName(resourceName), amount); }

	std::set<Resource> const& GetActiveResources() const { return activeResources; }
	std::map<Resource, int> const& GetPlacedResources() const { return placedResources; }
	int GetPrice() const { return price; }
	int GetPower() const { return power; }
	int GetCapacity() const { return capacity; }
	int GetTotalPlacedResources();
	bool ConsumeResources(Resource resource1, int);
	int GetPlacedResource(Resource resource) { return placedResources[resource]; }
	
	void PrintDetails();
};

inline std::ostream& operator<<(std::ostream& stream, const PowerPlantCard& card) {
	stream << "Price: " << card.GetPrice() << ", Power: " << card.GetPower() << ", Capacity: " << card.GetCapacity();
	
	// If takes resources
	if (card.GetActiveResources().size() > 0) {
		stream << ", Resource(s): ";
		for (Resource resource : card.GetActiveResources())
			stream << " " << GetResourceName(resource);
	}
	return stream;
}
