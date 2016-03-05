#pragma once

#include <memory>
#include "Map.h"
#include "Player.h"
#include "ResourceMarket.h"

class GameStatus
{
    GameStatus() {}

    std::shared_ptr<Map> map;
    std::vector<std::shared_ptr<Player>> players;
    std::vector<Player*> orderedPlayers;
    std::shared_ptr<ResourceMarket> resourceMarket;
    std::vector<std::shared_ptr<Card>> cardDeck;
    std::vector<std::shared_ptr<HouseColor>> colors;

    bool LoadMap(pugi::xml_document& xml);
    bool LoadPlayers(pugi::xml_document& xml);
    bool LoadOrderedPlayers(pugi::xml_document& xml);
    bool LoadResourceMarket(pugi::xml_document& xml);
    bool LoadCardDeck(pugi::xml_document& xml);
    bool LoadColors(pugi::xml_document& xml);

public:
    static GameStatus& GetInstance()
    {
        static GameStatus instance;
        return instance;
    }
    GameStatus(GameStatus const&) = delete;
    GameStatus& operator=(GameStatus const&) = delete;
    ~GameStatus() {}

    std::shared_ptr<Map> GetMap() const { return map; }
    std::vector<std::shared_ptr<HouseColor>> GetColors() const { return colors; }
    std::vector<std::shared_ptr<Player>> GetPlayers() const { return players; }
    std::vector<Player*> GetOrderedPlayers() const { return orderedPlayers; }
    std::shared_ptr<ResourceMarket> GetResourceMarket() const { return resourceMarket; }
    std::vector<std::shared_ptr<Card>> GetCardDesk() const { return cardDeck; }

    bool LoadFile(string gameFilePath, string playersFilePath);
};

