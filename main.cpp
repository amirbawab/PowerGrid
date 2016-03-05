#include "Player.h"
#include "Resource.h"
#include "PowerPlantCard.h"

#include <iostream>
#include <string>
#include <memory>

#include "GameStatus.h"
#include "Config.h"

using std::make_shared;


int main(int argc, char *argv[]) {
	Player p1("Hello", make_shared<HouseColor>().get(), 0);
	
    Config::GetInstance().LoadFile(":/test/Resources/config/Config.xml");
    GameStatus::GetInstance().LoadFile("Resources/saved games/PG_2016-03-2/Game.xml",
                                       "Resources/saved games/PG_2016-03-2/Players.xml");
	return 0;
}