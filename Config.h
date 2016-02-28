#pragma once

#include "Map.h"
#include <memory>
#include <vector>

class Card;

enum Resource
{
    COAL,
    OIL,
    GARBAGE,
    URANIUM
};

class Config
{
private:
    Config() {}

    std::vector<std::shared_ptr<Map>> maps;
    std::map<Resource, int> totalResource;
    std::map<Resource, int> initialResource;
    std::vector<std::shared_ptr<Card>> cards;

public:
    static Config& GetInstance()
    {
        static Config instance;
        return instance;
    }
    Config(const Config&) = delete;
    void operator=(const Config&) = delete;

    ~Config() { }
};

