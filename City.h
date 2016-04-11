#pragma once

#include <string>
#include "Region.h"
#include "Connection.h"
#include <memory>
#include <vector>
#include "House.h"

class City
{
public:
    City() {}
    explicit City(string name) : name(name) {}

    string GetName() const { return name; }
    void SetName(string name) { this->name = name; }

    std::shared_ptr<Region> GetRegion() const { return region; }
    void SetRegion(std::shared_ptr<Region> region) { this->region = region; }

    std::vector<House*>& GetHouses() { return houses; }
    int GetNumberOfHouses() const { return houses.size(); }
    std::vector<std::shared_ptr<Connection>> const&  GetConnections() const { return connections; }

    void AddConnection(std::shared_ptr<Connection> connection) { connections.push_back(connection); }
    void RemoveConnection(std::shared_ptr<Connection> connection);

    bool AddHouse(House* house);

    int GetHousePrice() const;
    bool IsFull() const;
    bool IsConnectedTo(string cityName);

    void setX(int x) { this->x = x; }
    void setY(int y) { this->y = y; }
    void setWidth(int width) { this->width = width; }
    void setHeight(int height) { this->height = height; }
    
    int getX() const { return x; }
    int getY() const { return y; }
    int getWidth() const { return width; }
    int getHeight() const { return height; }

    void SetToRemove(bool toRemove) { this->toRemove = toRemove; }
    bool GetToRemove() const { return toRemove; }

    bool operator==(const City& other) const { return this->name == other.name; }
    bool operator==(const string& cityName) const { return this->name == cityName; }
    friend bool operator==(const std::shared_ptr<City>& first, const std::shared_ptr<City>& second);
    friend bool operator==(const std::shared_ptr<City>& first, const City& second);
    friend bool operator==(const std::shared_ptr<City>& city, const string& cityName);

private:
    static const int MAXIMUM_HOUSES = 3;

    bool toRemove = false;

    int firstHousePrice  = 10;
    int secondHousePrice = 15;
    int thirdHousePrice  = 20;

    int x = 0, y = 0, width = 0, height = 0;

    string name = "";
    std::shared_ptr<Region> region;
    std::vector<std::shared_ptr<Connection>> connections;
    std::vector<House*> houses;
};

