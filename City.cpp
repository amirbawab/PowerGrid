#include "City.h"
#include <algorithm>

bool operator==(const std::shared_ptr<City>& first, const std::shared_ptr<City>& second)
{
    return first->name == second->name;
}

bool operator==(const std::shared_ptr<City>& first, const City& other)
{
    return first->name == other.name;
}

bool operator==(const std::shared_ptr<City>& city, const std::string& cityName)
{
    return city->name == cityName;
}

void City::RemoveConnection(std::shared_ptr<Connection> connection)
{
    using std::remove;
    connections.erase(remove(connections.begin(), connections.end(), connection));
}

bool City::AddHouse(House* house)
{
    if (houses.size() == MAXIMUM_HOUSES)
        return false;

    houses.push_back(house);
    return true;
}

int City::GetHousePrice() const
{
    switch (houses.size())
    {
        case 0:
            return firstHousePrice;
        case 1:
            return secondHousePrice;
        case 2:
            return thirdHousePrice;
        default:
            return 0;
    }
}

bool City::IsFull() const
{
    return houses.size() == MAXIMUM_HOUSES;
}

bool City::IsConnectedTo(std::string cityName) {
    for (auto connection = connections.begin(); connection != connections.end(); ++connection)
        if ((*connection)->GetFirst()->GetName() == cityName || (*connection)->GetSecond()->GetName() == cityName)
            return true;
    return false;
}