#pragma once

#include "House.h"
#include <memory>
#include <vector>

class Player
{
private:
    std::string color;
    std::string name;
    int elektro = 50;
    std::vector<std::shared_ptr<House>> houses;

public:
    Player();
    Player(std::string name, std::string color, int elektro);
    ~Player();

    std::string GetName() const { return name; }
    void SetName(std::string name) { this->name = name; }

    std::string GetColor() const { return color; }
    void SetColor(std::string color) { this->color = color; }

    std::vector<std::shared_ptr<House>> GetHouses() const { return houses; }

    bool BuyHouse(House& house);
    void DisplayStatus() const;
};

inline std::ostream& operator<<(std::ostream& stream, const Player& player)
{
    return stream << player.GetName() << "[" << player.GetColor() << "]";
}
