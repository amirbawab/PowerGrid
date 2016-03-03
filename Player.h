#pragma once

#include "House.h"
#include "HouseColor.h"
#include "PowerPlantCard.h"
#include "Resource.h"
#include <memory>
#include <vector>

class Player
{
private:
    std::string name;
    int elektro = 50;
    std::vector<std::shared_ptr<House>> houses;
	std::vector<std::shared_ptr<PowerPlantCard>> powerPlants;
	std::vector<int> resources;
	HouseColor color;

public:
    Player();
    Player(std::string name, HouseColor color, int elektro);
    ~Player();

    std::string GetName() const { return name; }
    void SetName(std::string name) { this->name = name; }

    HouseColor GetColor() const { return color; }
    void SetColor(HouseColor color) { this->color = color; }

	std::vector<std::shared_ptr<House>> const& GetHouses() const { return houses; }
	std::vector<std::shared_ptr<PowerPlantCard>> const& GetPowerPlants() const { return powerPlants; }
	int GetResources(Resource resource) const { return resources[resource]; }
	
	bool AddPowerPlant(std::shared_ptr<PowerPlantCard> powerPlant);
    bool BuyHouse(House& house);
    void DisplayStatus() const;
	bool AddResource(Resource resource);
};

inline std::ostream& operator<<(std::ostream& stream, const Player& player)
{
    return stream << player.GetName() << "[" << player.GetColor().getName() << "]";
}
