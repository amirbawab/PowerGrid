#include "Region.h"

bool operator==(const std::shared_ptr<Region>& first, const std::shared_ptr<Region>& second)
{
    return first->name == second->name;
}

bool operator==(const std::shared_ptr<Region>& region, const std::string& regionName)
{
    return region->name == regionName;
}

bool operator==(const std::shared_ptr<Region>& first, const Region& second)
{
    return first->name == second.name;
}
