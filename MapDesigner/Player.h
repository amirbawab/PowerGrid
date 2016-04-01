#pragma once
#include "PlayerHouse.h"

class Player
{
    std::vector<PlayerHouse> houses;

public:
    Player() {}
    ~Player() {}

    std::vector<PlayerHouse>& GetHouses() { return houses; }
};

