#include "City.h"
#include <iostream>

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

int City::GetHousePrice() const
{
    switch (houses)
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

bool City::AddHouse()
{
    if (houses == MAX_VALUE)
        return false;

    houses++;
    return true;
}

bool City::IsFull() {
	return houses == MAX_VALUE;
}

bool City::IsConnectedTo(std::string cityName) {
	for (auto connection = connections.begin(); connection != connections.end(); ++connection)
		if ((*connection)->GetFirst()->GetName() == cityName || (*connection)->GetSecond()->GetName() == cityName)
			return true;
	return false;
}