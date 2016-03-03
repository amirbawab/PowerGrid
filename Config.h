#pragma once

#include "Map.h"
#include "Card.h"
#include "PowerPlantCard.h"
#include "StepCard.h"
#include "Resource.h"
#include <memory>
#include <vector>
#include <pugixml.hpp>

enum IO
{
    MAPS,
    CARDS,
    COLORS
};

class Config
{
private:
    Config() {}

    std::vector<std::shared_ptr<Map>> maps;
    std::map<Resource, int> totalResource;
    std::map<Resource, int> initialResource;
    std::vector<std::shared_ptr<Card>> cards;

    std::map<IO, std::string> io;
    int elektro = 0;
    //TODO: add a vector of colors

    bool LoadIoData(pugi::xml_document& xml);
    bool LoadMaps(pugi::xml_document& xml);
    bool LoadResourceMarket(pugi::xml_document& xml);
    bool LoadCards(pugi::xml_document& xml);
    bool LoadOverviewCards(pugi::xml_document& xml);
    bool LoadElektro(pugi::xml_document& xml);
    //TODO: load colors

public:
    static Config& GetInstance()
    {
        static Config instance;
        return instance;
    }
    Config(const Config&) = delete;
    void operator=(const Config&) = delete;
    ~Config() { }

    std::vector<std::shared_ptr<Map>> const& GetMaps() const { return maps; }
    std::map<Resource, int> const& GetTotalResource() const { return totalResource; }
    std::map<Resource, int> const& GetInitialResource() const { return initialResource; }
    std::vector<std::shared_ptr<Card>> const& GetCards() const { return cards; }

    bool ReadFile(std::string filePath);
};

