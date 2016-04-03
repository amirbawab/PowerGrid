#include "Map.h"
#include <iostream>
#include "Helpers.h"
#include <QFile>
#include <limits>
#include <queue>
#include <stack>

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

Map::Map(string fileName) : fileName{ fileName } {
    
    // Load file
    LoadFile();
}

bool Map::LoadFile()
{
    pugi::xml_document mapXml;

    QFile mapXmlFile(fileName.c_str());

    if (!mapXmlFile.open(QFile::ReadOnly)) {
        Error("Could not open file: " + fileName);
        return false;
    }

    QString mapXmlContent = mapXmlFile.readAll();

    // Read map content from file
    auto result = mapXml.load_string(mapXmlContent.toStdString().c_str());
    if (result.status != pugi::status_ok) {
        Error("Could not read XML content for map");
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
    map.append_attribute("name").set_value(GetName().c_str());
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
            if (city->second->GetRegion()->GetName() == region->get()->GetName())
            {
                cout << "\t\t" << cityIndex << ". " << city->second->GetName() << endl;
                cityIndex++;
            }
    }

    cout << endl <<  "-------------------------------------------------------------" << endl;
    cout << "Connections:" << endl;
    
    for (auto city = cities.begin(); city != cities.end(); ++city) {

        auto cityName = city->second->GetName();
        cout << cityName << ":" << endl;

        for (auto connection = city->second->GetConnections().begin(); connection != city->second->GetConnections().end(); ++connection)
        {
            auto firstCity = connection->get()->GetFirst()->GetName();
            auto secondCity = connection->get()->GetSecond()->GetName();
            cout << "\t=== " << connection->get()->GetCost() << " ===> ";
            cout << (firstCity == cityName ? secondCity : firstCity) << endl;
        }

        cout << endl;
    }
}

shared_ptr<City> Map::GetOppositeOf(const Connection& connection, string city)
{
    if (connection.GetFirst() == city)
        return connection.GetSecond();
    if (connection.GetSecond() == city)
        return connection.GetFirst();

    return nullptr;
}


bool Map::AddConnection(std::shared_ptr<City> first, std::shared_ptr<City> second, int cost)
{
    if (cities.size() == 0)
    {
        Error("Cannot add a connection to a map with no cities!\n");
        return false;
    }

    if (cities.find(first->GetName()) == cities.end() || cities.find(second->GetName()) == cities.end())
    {
        Error("At least one of the cities cannot be found. Unable to add a connection\n");
        return false;
    }

    if (first->IsConnectedTo(second->GetName()))
    {
        Error("It seems that we have duplicate entries for the connection between '" +
            first->GetName() + "' and '" + second->GetName() + "'\n");
        return false;
    }

    // Create a connection
    std::shared_ptr<Connection> connection = std::make_shared<Connection>(first, second, cost);

    // Add it to cities
    first->AddConnection(connection);
    second->AddConnection(connection);

    // Add to list of connections
    connections.push_back(connection);

    return true;
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
    cities = std::map<std::string, std::shared_ptr<City>>();
    regions = vector<shared_ptr<Region>>();

    auto mapNode = xml.child("map");
    if (!mapNode)
        return false;

    for (auto cityNode : xml.select_nodes("/map/cities/city"))
    {
        string cityName = cityNode.node().attribute("name").value();
        string regionName = cityNode.node().attribute("region").value();

        // We can't have two cities with the same name
        if (cities.find(cityName) != cities.end())
        {
            Error("It seems that the input file contains two cities with the same name!\n");
            return false;
        }

        shared_ptr<City> city = make_shared<City>(cityName);

        shared_ptr<Region> cityRegion;
        int regionIndex = GetRegionIndex(regionName);
        if (regionIndex == INVALID_VALUE)
        {
            cityRegion = make_shared<Region>(regionName);
            city->SetRegion(cityRegion);
            regions.push_back(cityRegion);
        }
        else
            city->SetRegion(regions[regionIndex]);

        // Add city to map
        cities[cityName] = city;
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

    connections = vector<std::shared_ptr<Connection>>();

    auto mapNode = xml.child("map");
    if (!mapNode)
        return false;

    string firstCity, secondCity;
    int cost;
    for (auto connection : xml.select_nodes("//connections/connection"))
    {
        firstCity = connection.node().attribute("first").value();
        secondCity = connection.node().attribute("second").value();
        cost = std::stoi(connection.node().attribute("cost").value());

        if (cities.find(firstCity) == cities.end() || cities.find(secondCity) == cities.end()) {
            Error("One of the cities " + firstCity + " or " + secondCity + " is not registered!\n");
            return false;
        }

        // Get cities
        std::shared_ptr<City> first = cities[firstCity];
        std::shared_ptr<City> second = cities[secondCity];

        if (!AddConnection(first, second, cost))
            return false;
    }

    return true;
}

void Map::PopulateCities(pugi::xml_node& map)
{
    pugi::xml_node citiesNode = map.append_child("cities");
    for (auto city = cities.begin(); city != cities.end(); ++city)
    {
        pugi::xml_node cityNode = citiesNode.append_child("city");
        pugi::xml_attribute cityNameAttribute = cityNode.append_attribute("name");
        pugi::xml_attribute cityRegionAttribute = cityNode.append_attribute("region");
        cityNameAttribute.set_value(city->first.c_str());
        cityRegionAttribute.set_value(city->second->GetRegion()->GetName().c_str());
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
        connectionFirstAttribute.set_value(connections[connectionIndex]->GetFirst()->GetName().c_str());
        connectionSecondAttribute.set_value(connections[connectionIndex]->GetSecond()->GetName().c_str());
        connectionCostAttribute.set_value(connections[connectionIndex]->GetCost());
    }
}

/// Get the shortest path between two cities
int Map::GetShortestPath(std::string fromCity, std::string toCity) {

    // Status
    static const int UNVISITED = 0;
    static const int VISITING = 1;
    static const int VISITED = 2;

    class  CityCostComparable {
    public:
        int cost = std::numeric_limits<int>::max();
        int visited = UNVISITED;
        string parent;
        std::shared_ptr<City> city;
        CityCostComparable(std::shared_ptr<City> city) : city(city) {};
    };

    struct CityCostComparator {
        bool operator()(const std::shared_ptr<CityCostComparable> lhs, const std::shared_ptr<CityCostComparable> rhs) const {
            return lhs->cost > rhs->cost;
        }
    };

    // If any city was not found
    if (cities.find(fromCity) == cities.end() || cities.find(toCity) == cities.end()) {
        Error("Cannot find " + fromCity + " or " + toCity + " in Dijkstra!");
        return PG::INVALID;
    }

    // Prepare required information
    std::map<std::string, std::shared_ptr<CityCostComparable>> citiesComparable;

    // Create comparable cities
    for (auto city = cities.begin(); city != cities.end(); ++city) {
        citiesComparable[city->first] = std::make_shared<CityCostComparable>(cities[city->first]);
    }

    // Create priority queue
    std::priority_queue<std::shared_ptr<CityCostComparable>, std::vector<std::shared_ptr<CityCostComparable>>, CityCostComparator> pQueue;

    // Prepare first city
    citiesComparable[fromCity]->cost = 0;
    citiesComparable[fromCity]->visited = VISITING;
    pQueue.push(citiesComparable[fromCity]);

    // Start
    while (!pQueue.empty()) {
        
        // Get top
        std::shared_ptr<CityCostComparable> topCityCostComparable = pQueue.top();

        // Mark visited
        topCityCostComparable->visited = VISITED;

        // If found my target, no need to continue calculating the other path
        if (topCityCostComparable->city->GetName() == toCity) {

            // Prepare stack
            std::stack<string> path;
            auto tmpCity = citiesComparable[toCity];

            // Print the path
            while (tmpCity) {
                path.push(tmpCity->city->GetName());
                tmpCity = citiesComparable[tmpCity->parent];
            }
            std::cout << "Possible path:";
            while (!path.empty()) { 
                std::cout << "\t" << path.top();
                path.pop();
            }

            std::cout << std::endl;

            return topCityCostComparable->cost;
        }

        // Remove top
        pQueue.pop();

        // Get neighbor connections
        std::vector<std::shared_ptr<Connection>> topConnections = topCityCostComparable->city->GetConnections();
        
        // Loop on connections
        for (auto connection = topConnections.begin(); connection != topConnections.end(); ++connection) {
            
            // Get opposite city
            std::shared_ptr<City> oppositeCity = GetOppositeOf(**connection, topCityCostComparable->city->GetName());

            // Get city cost comparable
            std::shared_ptr<CityCostComparable> oppositeCityCostComparable = citiesComparable[oppositeCity->GetName()];

            // If not visited
            if (oppositeCityCostComparable->visited != VISITED) {

                // Calculate path
                int pathCost = (*connection)->GetCost() + topCityCostComparable->cost;

                // If new path is less then update
                if (pathCost < oppositeCityCostComparable->cost) {

                    // If unvisited
                    if (oppositeCityCostComparable->visited == UNVISITED) {
                        oppositeCityCostComparable->cost = pathCost;
                        oppositeCityCostComparable->parent = topCityCostComparable->city->GetName();

                    // If visiting
                    } else {
                        // Empty queue into a tmp vector, make the changes then reput everything into the queue
                        std::vector<std::shared_ptr<CityCostComparable>> tmpVector;
                        while (!pQueue.empty()) { tmpVector.push_back(pQueue.top()); pQueue.pop(); }
                        oppositeCityCostComparable->cost = pathCost;
                        oppositeCityCostComparable->parent = topCityCostComparable->city->GetName();
                        while (!tmpVector.empty()) { pQueue.push(tmpVector[tmpVector.size() - 1]); tmpVector.pop_back(); }
                    }
                }

                // If unvisited, add to queue
                if (oppositeCityCostComparable->visited == UNVISITED)
                    pQueue.push(oppositeCityCostComparable);

                // Mark visiting
                oppositeCityCostComparable->visited = VISITING;
            }
        }
    }
    
    // Invalid
    return INVALID_VALUE;
}

/// Get the smallest cost for a player to connect to a specified city
int Map::GetShortestPath(shared_ptr<Player> player, string toCity) {
    
    // If no houses
    if (player->GetHouses().size() == 0) return 0;

    // Default is from first house to target
    int minCost = GetShortestPath(player->GetHouses()[0]->GetCity()->GetName(), toCity);

    // Loop from index 1
    for (int i = 1; i < player->GetHouses().size(); i++) {
        int pathCost = GetShortestPath(player->GetHouses()[i]->GetCity()->GetName(), toCity);
        if (minCost > pathCost)
            minCost = pathCost;
    }
    return minCost;
}


/// Get city by name or nullptr
std::shared_ptr<City> Map::GetCityByName(std::string cityName) {
    if (cities.find(cityName) == cities.end()) return nullptr;
    return cities[cityName];
}