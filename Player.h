#pragma once

#include "House.h"
#include "HouseColor.h"
#include "PowerPlantCard.h"
#include "Resource.h"
#include "CardStack.h"
#include "ResourceMarket.h"
#include <memory>
#include <vector>

using std::shared_ptr;
using std::vector;
using std::string;

class Player
{
private:
    string name;
    int elektro = 50;
    vector<shared_ptr<House>> houses;
	vector<shared_ptr<PowerPlantCard>> powerPlants;
	vector<int> resources;
	shared_ptr<HouseColor> color;

public:
    Player();
    Player(string name, shared_ptr<HouseColor> color, int elektro);
    ~Player();

    int GetElektro() const { return elektro; }
    void SetElektro(int elektro) { this->elektro = elektro; }
	bool HasElektro(int elektro) { return (this->elektro >= elektro); }

    string GetName() const { return name; }
    void SetName(string name) { this->name = name; }

    shared_ptr<HouseColor> const& GetColor() const { return color; }
    void SetColor(shared_ptr<HouseColor> color) { this->color = color; }

	vector<shared_ptr<House>> const& GetHouses() const { return houses; }
	vector<shared_ptr<PowerPlantCard>> const& GetPowerPlants() const { return powerPlants; }
	int GetResources(Resource resource) const { return resources[resource]; }
	
	int GetHighestPowerPlant();
	bool AddPowerPlant(shared_ptr<PowerPlantCard> powerPlant);
	void ReplacePowerPlant(shared_ptr<PowerPlantCard>, int);
	bool BuyPowerPlant(CardStack&, int, int);

	bool BuyResources(ResourceMarket&, shared_ptr<PowerPlantCard>, Resource, int);
	bool BuyResources(ResourceMarket&, shared_ptr<PowerPlantCard>, string, int);

    bool BuyHouse(shared_ptr<House> house);

    void DisplayStatus() const;
};

inline std::ostream& operator<<(std::ostream& stream, const Player& player)
{
    return stream << player.GetName() << "[" << player.GetColor()->getName() << "]";
}
