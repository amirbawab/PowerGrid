#include "Player.h"
#include "Resource.h"
#include "PowerPlantCard.h"

#include <iostream>
#include <string>
#include <memory>

#include "GameStatus.h"
#include "Config.h"
#include "Game.h"

using std::make_shared;


int main(int argc, char *argv[]) {

	// Load configuration
    Config::GetInstance().LoadFile(":/test/Resources/config/Config.xml");
    
	// If loading saved game
	if(false)
		GameStatus::GetInstance().LoadFile("Resources/saved games/PG_2016-03-2/Game.xml",
                                       "Resources/saved games/PG_2016-03-2/Players.xml");

	// Create game
	Game game;

	return 0;
}