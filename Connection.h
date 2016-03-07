#pragma once
#include <memory>

class City;
class Connection
{
public:
    Connection() {}
    Connection(std::shared_ptr<City> firstCity,
               std::shared_ptr<City> secondCity, int cost)
        : first(firstCity), second(secondCity), cost(cost) { }
    
    std::shared_ptr<City> GetFirst() const { return first; }
    void setFirst(std::shared_ptr<City> city) { first = city; }

    std::shared_ptr<City> GetSecond() const { return second; }
    void setSecond(std::shared_ptr<City> city) { second = city; }

    int GetCost() const { return cost; }
    void setCost(int cost) { this->cost = cost; }

    bool operator==(const Connection& other) const;
    friend bool operator==(const std::shared_ptr<Connection>& first,
                           const std::shared_ptr<Connection>& second);
    friend bool operator==(const std::shared_ptr<Connection>& first,
                           const Connection& second);

private:
    std::shared_ptr<City> first;
    std::shared_ptr<City> second;
    int cost = 0;
};

