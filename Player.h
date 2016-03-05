#pragma once

#include "House.h"
#include "HouseColor.h"
#include "PowerPlantCard.h"
#include "Resource.h"
#include "CardStack.h"
#include <memory>
#include <vector>

using std::shared_ptr;

class Player
{
private:
    std::string name;
    int elektro = 50;
    std::vector<std::shared_ptr<House>> houses;
	std::vector<std::shared_ptr<PowerPlantCard>> powerPlants;
	std::vector<int> resources;
	std::shared_ptr<HouseColor> color;

public:
    Player();
    Player(std::string name, std::shared_ptr<HouseColor> color, int elektro);
    ~Player();

    int GetElektro() const { return elektro; }
    void SetElektro(int elektro) { this->elektro = elektro; }
	bool HasElektro(int elektro) { return (this->elektro >= elektro); }

    std::string GetName() const { return name; }
    void SetName(std::string name) { this->name = name; }

    std::shared_ptr<HouseColor> const& GetColor() const { return color; }
    void SetColor(std::shared_ptr<HouseColor> color) { this->color = color; }

	std::vector<std::shared_ptr<House>> const& GetHouses() const { return houses; }
	std::vector<std::shared_ptr<PowerPlantCard>> const& GetPowerPlants() const { return powerPlants; }
	int GetResources(Resource resource) const { return resources[resource]; }
	
	int GetHighestPowerPlant();
	bool AddPowerPlant(std::shared_ptr<PowerPlantCard> powerPlant);
	void ReplacePowerPlant(shared_ptr<PowerPlantCard>, int);
	bool BuyPowerPlant(CardStack&, int, int);

    bool BuyHouse(House& house);

    void DisplayStatus() const;
};

inline std::ostream& operator<<(std::ostream& stream, const Player& player)
{
    return stream << player.GetName() << "[" << player.GetColor()->getName() << "]";
}
