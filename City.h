#pragma once

#include <string>
#include "Region.h"
#include <memory>

class City
{
public:
    City() {}
    City(std::string name) : name(name) {}

    std::string GetName() const { return name; }
    void SetName(std::string name) { this->name = name; }

    std::shared_ptr<Region> GetRegion() const { return region; }
    void SetRegion(std::shared_ptr<Region> region) { this->region = region; }

    int GetNumberOfHouses() const { return houses; }

    int GetHousePrice() const;
    bool AddHouse();

    bool operator==(const City& other) const { return this->name == other.name; }
    bool operator==(const std::string& cityName) const { return this->name == cityName; }
    friend bool operator==(const std::shared_ptr<City>& first, const std::shared_ptr<City>& second);
    friend bool operator==(const std::shared_ptr<City>& first, const City& second);
    friend bool operator==(const std::shared_ptr<City>& city, const std::string& cityName);

private:
    int firstHousePrice = 10;
    int secondHousePrice = 15;
    int thirdHousePrice = 20;
    int houses = 0;
    std::string name = "";
    std::shared_ptr<Region> region;
};

