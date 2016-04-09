#include "House.h"

House::House()
{
}

House::House(std::shared_ptr<City> city)
{
    this->city = city;
    price = city->GetHousePrice();
}

House::~House()
{
}
