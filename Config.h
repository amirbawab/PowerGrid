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
    Config* instance;
    Config();

    std::vector<std::shared_ptr<Map>> maps;
    std::map<Resource, int> totalResource;
    std::map<Resource, int> initialResource;
    std::vector<std::shared_ptr<Card>> cards;

public:
    Config* GetInstance()
    {
        if (!instance)
            instance = new Config();
        return instance;
    }
    ~Config()
    {
        delete instance;
    }
};

