#include "House.h"
#include "City.h"

House::House(std::shared_ptr<City> city, std::shared_ptr<HouseColor> color) :
    city{ city }, color{ color }
{
    price = city->GetHousePrice();
}

House::~House()
{
}
