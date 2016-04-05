#pragma once
#include "PlayerHouse.h"
#include <memory>

class Player
{
    std::vector<std::unique_ptr<PlayerHouse>> houses;

public:
    Player() {}
    ~Player() {}

    std::vector<std::unique_ptr<PlayerHouse>>& GetHouses() { return houses; }
};

