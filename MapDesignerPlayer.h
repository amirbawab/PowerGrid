#pragma once
#include "MapDesignerPlayerHouse.h"
#include <memory>

class MapDesignerPlayer
{
    std::vector<std::unique_ptr<MapDesignerPlayerHouse>> houses;

public:
    MapDesignerPlayer() {}
    ~MapDesignerPlayer() {}

    std::vector<std::unique_ptr<MapDesignerPlayerHouse>>& GetHouses() { return houses; }
};

