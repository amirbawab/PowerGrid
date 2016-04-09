#pragma once

#include "Map.h"
#include "Resource.h"
#include <memory>
#include <vector>

class Game;

enum IO
{
    MAPS,
    CARDS,
    COLORS
};

class Config
{
    Game* game = nullptr;

    Config() {}

    vector<shared_ptr<Map>> maps;
    std::map<Resource, int> initialResource;
    std::map<IO, string> pathPrefix;

    bool LoadIoData(pugi::xml_document& xml);
    bool LoadMaps(pugi::xml_document& xml);
    bool LoadResourceMarket(pugi::xml_document& xml);
    bool LoadCards(pugi::xml_document& xml) const;
    bool LoadOverviewCard(pugi::xml_document& xml) const;
    bool LoadElektro(pugi::xml_document& xml) const;
    bool LoadColors(pugi::xml_document& xml) const;

public:
    static Config& GetInstance()
    {
        static Config instance;
        return instance;
    }
    Config(const Config&) = delete;
    void operator=(const Config&) = delete;
    ~Config() { }

    string GetPathPrefix(IO io) { return pathPrefix[io]; }
    vector<shared_ptr<Map>> const& GetMaps() const { return maps; }

    bool LoadFile(Game* game, string filePath);
};

