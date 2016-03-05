#include "Map.h"
#include <iostream>
#include "Helpers.h"

using std::string;
using std::cout;
using std::cerr;
using std::endl;
using std::vector;
using std::map;
using std::shared_ptr;
using std::unique_ptr;
using std::make_shared;
using std::make_unique;

bool Map::LoadFile()
{
    pugi::xml_document mapXml;
    auto result = mapXml.load_file(fileName.c_str());

    if (result.status != pugi::status_ok)
    {
        Error("Could not open file: '" + fileName + "'");
        Error("Reason: " + string(result.description()) + "\n");
        return false;
    }

	name = mapXml.child("map").attribute("name").value();

    if (!LoadCities(mapXml))
    {
        Error("Could not read city data from the file");
        Error("Please make sure the file is correctly formatted\n");
        return false;
    }

    if (!LoadConnections(mapXml))
    {
        Error("Could not read connection data from the file");
        Error("Please make sure the file is correctly formatted\n");
        return false;
    }

    return true;
}

bool Map::SaveFile(string fileName)
{
    pugi::xml_document document;
    pugi::xml_node map = document.append_child("map");
    PopulateCities(map);
    PopulateConnections(map);
    return document.save_file(fileName.c_str());
}

void Map::DisplayMap() const
{
    cout << "-------------------------------------------------------------" << endl;
    cout << "Regions:" << endl;

    int regionIndex = 1;
    int cityIndex;
    for (auto region = regions.begin(); region != regions.end(); ++region, ++regionIndex)
    {
        cout << "\t" << regionIndex << ". '" << region->get()->GetName() <<
            "' contains the following cities:" << endl;

        cityIndex = 1;
        for (auto city = cities.begin(); city != cities.end(); ++city)
            if (city->get()->GetRegion()->GetName() == region->get()->GetName())
            {
                cout << "\t\t" << cityIndex << ". " << city->get()->GetName() << endl;
                cityIndex++;
            }
    }

    cout << endl <<  "-------------------------------------------------------------" << endl;
    cout << "Connections:" << endl;
    int connectionIndex = 0;
    for (auto connection = connectionList.begin(); connection != connectionList.end(); ++connection)
    {
        cout << cities[connectionIndex++]->GetName() << ":" << endl;

        for (auto mapIt = connection->begin(); mapIt != connection->end(); ++mapIt)
            if (mapIt->second != -1)
                cout << "\t=== " << mapIt->second << " ==> " << cities[mapIt->first]->GetName() << endl;

        cout << endl;
    }
}

vector<Connection> Map::GetConnections(string city)
{
    auto connections = vector<Connection>();
    int cityIndex = GetCityIndex(city);
    
    if (cityIndex == -1)
        return connections;

    for (auto conn : connectionList[cityIndex])
    {
        shared_ptr<City> first = cities[cityIndex];
        shared_ptr<City> second = cities[conn.first];
        int cost = conn.second;
        connections.push_back(Connection(first, second, cost));
    }

    return connections;
}

shared_ptr<City> Map::GetOppositeOf(const Connection& connection, string city)
{
    if (connection.getFirst() == city)
        return connection.getSecond();
    if (connection.getSecond() == city)
        return connection.getFirst();

    return nullptr;
}


bool Map::AddConnection(City first, City second, int cost)
{
    if (cities.size() == 0)
    {
        Error("Cannot add a connection to a map with no cities!\n");
        return false;
    }

    int firstCity = GetCityIndex(first.GetName());
    int secondCity = GetCityIndex(second.GetName());
    if (firstCity == -1 || secondCity == -1)
    {
        Error("At least one of the cities cannot be found. Unable to add a connection\n");
        return false;
    }

    if (connectionList[firstCity][secondCity] != -1 || connectionList[secondCity][firstCity] != -1)
    {
        Error("It seems that we have duplicate entries for the connection between '" +
            first.GetName() + "' and '" + second.GetName() + "'\n");
        return false;
    }

    connectionList[firstCity][secondCity] = cost;
    connectionList[secondCity][firstCity] = cost;
    return true;
}

int Map::GetCityIndex(const string cityName) const
{
    auto city = find(cities.begin(), cities.end(), cityName);

    if (city == cities.end())
        return -1;

    return distance(cities.begin(), city);
}

int Map::GetRegionIndex(const string regionName) const
{
    auto region = find(regions.begin(), regions.end(), regionName);

    if (region == regions.end())
        return -1;

    return distance(regions.begin(), region);
}

bool Map::LoadCities(pugi::xml_document& xml)
{
    cities = vector<shared_ptr<City>>();
    regions = vector<shared_ptr<Region>>();

    auto mapNode = xml.child("map");
    if (!mapNode)
        return false;

    for (auto cityNode : xml.select_nodes("/map/cities/city"))
    {
        string cityName = cityNode.node().attribute("name").value();
        string regionName = cityNode.node().attribute("region").value();

        // We can't have two cities with the same name
        if (GetCityIndex(cityName) != -1)
        {
            Error("It seems that the input file contains two cities with the same name!\n");
            return false;
        }

        shared_ptr<City> city = make_shared<City>(cityName);

        shared_ptr<Region> cityRegion;
        int regionIndex = GetRegionIndex(regionName);
        if (regionIndex == -1)
        {
            cityRegion = make_shared<Region>(regionName);
            city->SetRegion(cityRegion);
            regions.push_back(cityRegion);
        }
        else
            city->SetRegion(regions[regionIndex]);

        cities.push_back(city);
    }

    return true;
}

bool Map::LoadConnections(pugi::xml_document& xml)
{
    if (cities.size() == 0)
    {
        Error("Can't load connections when the city collection is empty\n");
        return false;
    }

    connections = vector<unique_ptr<Connection>>();
    connectionList = vector<map<int, int>>(cities.size());

    // Initialize the connectionList to contain all -1's
    for (int fromCity = 0; fromCity < connectionList.size(); fromCity++)
        for (int toCity = 0; toCity < cities.size(); toCity++)
            connectionList[fromCity][toCity] = -1;

    auto mapNode = xml.child("map");
    if (!mapNode)
        return false;

    string firstCity, secondCity;
    int firstCityIndex, secondCityIndex;
    int cost;
    for (auto connection : xml.select_nodes("//connections/connection"))
    {
        firstCity = connection.node().attribute("first").value();
        secondCity = connection.node().attribute("second").value();
        cost = std::stoi(connection.node().attribute("cost").value());

        if (!AddConnection(firstCity, secondCity, cost))
            return false;

        firstCityIndex = GetCityIndex(firstCity);
        secondCityIndex = GetCityIndex(secondCity);
        connections.push_back(make_unique<Connection>(cities[firstCityIndex],
                                                      cities[secondCityIndex], cost));
    }

    return true;
}

void Map::PopulateCities(pugi::xml_node& map)
{
    pugi::xml_node citiesNode = map.append_child("cities");
    for (int cityIndex = 0; cityIndex < cities.size(); cityIndex++)
    {
        pugi::xml_node cityNode = citiesNode.append_child("city");
        pugi::xml_attribute cityNameAttribute = cityNode.append_attribute("name");
        pugi::xml_attribute cityRegionAttribute = cityNode.append_attribute("region");
        cityNameAttribute.set_value(cities[cityIndex]->GetName().c_str());
        cityRegionAttribute.set_value(cities[cityIndex]->GetRegion()->GetName().c_str());
    }
}

void Map::PopulateConnections(pugi::xml_node& map)
{
    pugi::xml_node connectionsNode = map.append_child("connections");
    for (int connectionIndex = 0; connectionIndex < connections.size(); connectionIndex++)
    {
        pugi::xml_node connectionNode = connectionsNode.append_child("connection");
        pugi::xml_attribute connectionFirstAttribute = connectionNode.append_attribute("first");
        pugi::xml_attribute connectionSecondAttribute = connectionNode.append_attribute("second");
        pugi::xml_attribute connectionCostAttribute = connectionNode.append_attribute("cost");
        connectionFirstAttribute.set_value(connections[connectionIndex]->getFirst()->GetName().c_str());
        connectionSecondAttribute.set_value(connections[connectionIndex]->getSecond()->GetName().c_str());
        connectionCostAttribute.set_value(connections[connectionIndex]->getCost());
    }
}
