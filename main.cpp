#include "Player.h"
#include "Resource.h"
#include "PowerPlantCard.h"

#include <iostream>
#include <string>
#include <memory>

using std::make_shared;

int main(int argc, char *argv[]) {
	Player p1("Hello", make_shared<HouseColor>().get(), 0);
	
	std::shared_ptr<PowerPlantCard> card1 = make_shared<PowerPlantCard>(0, 0, 2);
	card1->AddResource(COAL);
	card1->AddResource(OIL);

	std::shared_ptr<PowerPlantCard> card2 = make_shared<PowerPlantCard>(0, 0, 1);
	card2->AddResource(COAL);

	p1.AddPowerPlant(card1.get());
	p1.AddPowerPlant(card2.get());

	std::cout << p1.AddResource(COAL, 2) << std::endl;
	std::cout << p1.AddResource(OIL, 4) << std::endl;

	return 0;
}