#include "Player.h"
#include "Resource.h"
#include "PowerPlantCard.h"

#include <iostream>
#include <string>
#include <memory>

using std::make_shared;

int main(int argc, char *argv[]) {
	Player p1("Hello", make_shared<HouseColor>().get(), 0);
	
	return 0;
}