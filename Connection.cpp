#include "Connection.h"

bool Connection::operator==(const Connection& other) const
{
    return ((this->first == other.first && this->second == other.second) ||
        (this->second == other.first && this->first == other.second)) &&
        this->cost == other.cost;
}

bool operator==(const std::shared_ptr<Connection>& first,
                const std::shared_ptr<Connection>& second)
{
    return ((first->first == second->first && first->second == second->second) ||
        (first->second == second->first && first->first == second->second)) &&
        first->cost == second->cost;
}

bool operator==(const std::shared_ptr<Connection>& first,
                const Connection& second)
{
    return ((first->first == second.first && first->second == second.second) ||
        (first->second == second.first && first->first == second.second)) &&
        first->cost == second.cost;
}
