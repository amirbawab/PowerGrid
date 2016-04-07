#pragma once

#include "City.h"
#include "Connection.h"
#include "Region.h"
#include "Player.h"
#include <vector>
#include <map>
#include <pugixml.hpp>
#include <memory>
#include <string>

class Map
{
public:
    explicit Map(std::string fileName);
    Map() {}

    static const int INVALID_VALUE = -1;

    std::vector<std::shared_ptr<Region>>& GetRegions() { return regions; }
    std::map<std::string, std::shared_ptr<City>> const& GetCities() const { return cities; }
    std::vector<std::shared_ptr<Connection>> const& GetConnections() const { return connections; }
    std::string GetFileName() const { return fileName; }
    std::string GetName() const { return name; }

    bool SaveFile(std::string fileName);
    void DisplayMap() const;

    static std::shared_ptr<City> GetOppositeOf(const Connection& connection, std::string city);
    int GetShortestPath(std::string fromCity, std::string toCity);
    int GetShortestPath(shared_ptr<Player>, string toCity);

    std::shared_ptr<City> GetCityByName(std::string cityName);
    bool AddConnection(std::shared_ptr<City> first, std::shared_ptr<City> second, int cost);
    void AddCity(std::shared_ptr<City> city) { cities[city->GetName()] = city; }

private:
    std::string fileName;
    std::string name;
    std::vector<std::shared_ptr<Region>> regions;
    std::map<std::string, std::shared_ptr<City>> cities;
    std::vector<std::shared_ptr<Connection>> connections;
    int GetRegionIndex(const std::string regionName) const;
    bool LoadCities(pugi::xml_document& xml);
    bool LoadConnections(pugi::xml_document& xml);
    void PopulateCities(pugi::xml_node& map);
    void PopulateConnections(pugi::xml_node& map);
    bool LoadFile();
};

