#pragma once

#include <string>
#include <memory>

class Region
{
public:
    Region() {}
    Region(std::string name) : name(name) {}

    Region(const Region&) = delete;
    Region& operator=(const Region&) = delete;

    std::string GetName() const { return name; }
    void SetName(std::string name) { this->name = name; }

    bool operator==(const Region& other) const { return this->name == other.name; }
    bool operator==(const std::string& regionName) const { return this->name == regionName; }
    friend bool operator==(const std::shared_ptr<Region>& first,
                           const std::shared_ptr<Region>& second);
    friend bool operator==(const std::shared_ptr<Region>& region,
                           const std::string& regionName);
    friend bool operator==(const std::shared_ptr<Region>& first,
                           const Region& second);

private:
    std::string name = "";
};
