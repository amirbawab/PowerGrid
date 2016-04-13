#pragma once

#include "City.h"
#include "Connection.h"
#include "Region.h"
#include "Player.h"
#include <vector>
#include <map>
#include <pugixml.hpp>
#include <memory>
#include <set>

class Map
{
public:
    explicit Map(string fileName);
    Map() {}

    static const int INVALID_VALUE = -1;

    vector<shared_ptr<Region>>& GetRegions() { return regions; }
    std::map<string, shared_ptr<City>> const& GetCities() const { return cities; }
    vector<shared_ptr<Connection>> const& GetConnections() const { return connections; }
    string GetFileName() const { return fileName; }
    string GetName() const { return name; }

    bool SaveFile(string fileName);
    void DisplayMap() const;

    static shared_ptr<City> GetOppositeOf(const Connection& connection, string city);
    int GetShortestPath(string fromCity, string toCity);
    int GetShortestPath(shared_ptr<Player>, string toCity);

    shared_ptr<City> GetCityByName(string cityName);

    bool AddConnection(shared_ptr<City> first, shared_ptr<City> second, int cost);
    void RemoveConnectionByCity(shared_ptr<City> city);

    void AddCity(shared_ptr<City> city) { cities[city->GetName()] = city; }

    void RemoveRegionByCity(shared_ptr<City> city);
    void RemoveRegion(shared_ptr<Region> region);
    void RemoveRegion(string regionName);

    std::set<shared_ptr<Region>>& GetRemovedRegions() { return removedRegions; }

private:
    string fileName;
    string name;

    vector<shared_ptr<Region>> regions;
    std::set<shared_ptr<Region>> removedRegions;
    std::map<string, shared_ptr<City>> cities;
    vector<shared_ptr<Connection>> connections;

    int GetRegionIndex(const string regionName) const;
    bool LoadCities(pugi::xml_document& xml);
    bool LoadConnections(pugi::xml_document& xml);
    void PopulateCities(pugi::xml_node& map);
    void PopulateConnections(pugi::xml_node& map);
    bool LoadFile();
};

