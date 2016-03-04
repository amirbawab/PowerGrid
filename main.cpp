#include "Player.h"
#include "Resource.h"
#include "PowerPlantCard.h"

#include <iostream>
#include <string>
#include <memory>

int main(int argc, char *argv[]) {
	Player p1("Hello", HouseColor(), 0);
	
	std::shared_ptr<PowerPlantCard> card1 = std::make_shared<PowerPlantCard>(0, 0, 2);
	card1->AddResource(Resource::COAL);
	card1->AddResource(Resource::OIL);

	std::shared_ptr<PowerPlantCard> card2 = std::make_shared<PowerPlantCard>(0, 0, 1);
	card2->AddResource(Resource::COAL);

	p1.AddPowerPlant(card1);
	p1.AddPowerPlant(card2);

	std::cout << p1.AddResource(Resource::COAL, 2) << std::endl;
	std::cout << p1.AddResource(Resource::OIL, 4) << std::endl;

	return 0;
}