#pragma once

#include "City.h"
#include "Connection.h"
#include "Region.h"
#include <vector>
#include <map>
#include <pugixml.hpp>
#include <memory>

class Map
{
public:
    Map(std::string fileName) : fileName{ fileName } {}
    std::vector<std::shared_ptr<Region>> GetRegions() const { return regions; }
    std::vector<std::shared_ptr<City>> GetCities() const { return cities; }
    std::vector<std::unique_ptr<Connection>> const& GetConnections() const { return connections; }
    bool LoadFile();
    bool SaveFile(std::string fileName);
    void DisplayMap() const;

    std::vector<Connection> GetConnections(std::string city);
    static std::shared_ptr<City> GetOppositeOf(const Connection& connection,
                                        std::string city);

private:
    std::string fileName;
    std::vector<std::shared_ptr<Region>> regions;
    std::vector<std::shared_ptr<City>> cities;
    std::vector<std::unique_ptr<Connection>> connections;
    std::vector<std::map<int, int>> connectionList;
    bool AddConnection(City first, City second, int cost);
    int GetCityIndex(const std::string cityName) const;
    int GetRegionIndex(const std::string regionName) const;
    bool LoadCities(pugi::xml_document& xml);
    bool LoadConnections(pugi::xml_document& xml);
    void PopulateCities(pugi::xml_node& map);
    void PopulateConnections(pugi::xml_node& map);
};

