#pragma once

#include <memory>
#include "Map.h"
#include "Player.h"
#include "ResourceMarket.h"
#include "Game.h"

class GameStatus
{
    GameStatus() {}

    std::shared_ptr<Map> map;
    std::vector<std::shared_ptr<Player>> players;
    std::vector<Player*> orderedPlayers;
    std::shared_ptr<ResourceMarket> resourceMarket;
    std::vector<std::shared_ptr<Card>> cardDeck;
    std::vector<std::shared_ptr<Card>> allCards;
    std::vector<std::shared_ptr<HouseColor>> colors;

    bool LoadMap(pugi::xml_document& xml);
    bool LoadPlayers(pugi::xml_document& xml);
    bool LoadOrderedPlayers(pugi::xml_document& xml);
    bool LoadResourceMarket(pugi::xml_document& xml);
    bool LoadCardDeck(pugi::xml_document& xml);
    bool LoadAllCards(pugi::xml_document& xml);
    bool LoadColors(pugi::xml_document& xml);

    bool SaveGameFile(string gameFilePath);
    void PopulateMap(pugi::xml_node& game);
    void PopulateOrderedPlayers(pugi::xml_node& game);
    void PopulateResourceMarket(pugi::xml_node& game);
    void PopulateCardDeck(pugi::xml_node& game);
    void PopulateColors(pugi::xml_node& game);

    bool SavePlayersFile(string playersFilePath);
    void PopulatePlayers(pugi::xml_node& playersNode);

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
    void SetMap(std::shared_ptr<Map> map) { this->map = map; }

    std::vector<std::shared_ptr<HouseColor>> GetColors() const { return colors; }
    void SetColors(std::vector<std::shared_ptr<HouseColor>> colors) { this->colors = colors; }

    std::vector<std::shared_ptr<Player>> GetPlayers() const { return players; }
    void SetPlayers(std::vector<std::shared_ptr<Player>> players) { this->players = players; }

    std::vector<Player*> GetOrderedPlayers() const { return orderedPlayers; }

    std::shared_ptr<ResourceMarket> GetResourceMarket() const { return resourceMarket; }
    void SetResourceMarket(std::shared_ptr<ResourceMarket> resourceMarket) { this->resourceMarket = resourceMarket; }

    std::vector<std::shared_ptr<Card>> GetCardDesk() const { return cardDeck; }
    void SetCardDesk(std::vector<std::shared_ptr<Card>> cardDeck) { this->cardDeck = cardDeck; }

    bool Init(std::shared_ptr<Game> game, string configFilePath);
    bool LoadFile(std::shared_ptr<Game> game, string gameFilePath, string playersFilePath);
    bool SaveFile(std::shared_ptr<Game> game,string gameFilePath, string playersFilePath);
};
