#pragma once

#include "Map.h"

class Game;

class GameStatus
{
    Game* game = nullptr;

    GameStatus() {}

    bool LoadMap(pugi::xml_document& xml) const;
    bool LoadPlayers(pugi::xml_document& xml) const;
    bool LoadOrderedPlayers(pugi::xml_document& xml) const;
    bool LoadResourceMarket(pugi::xml_document& xml) const;
    bool LoadAllCards(pugi::xml_document& xml) const;
    bool LoadVisibleCards(pugi::xml_document& xml) const;
    bool LoadCardDeck(pugi::xml_document& xml) const;
    bool LoadOverview(pugi::xml_document& xml) const;
    bool LoadColors(pugi::xml_document& xml) const;
    bool LoadFullTurn(pugi::xml_document& xml) const;
    bool LoadPhase(pugi::xml_document& xml) const;

    bool SaveGameFile(string gameFilePath) const;
    void PopulateMap(pugi::xml_node& gameXml) const;
    void PopulateOrderedPlayers(pugi::xml_node& gameXml) const;
    void PopulateResourceMarket(pugi::xml_node& gameXml) const;
    void PopulateCardDeck(pugi::xml_node& gameXml) const;
    void PopulateAllCards(pugi::xml_node& gameXml) const;
    void PopulateOverview(pugi::xml_node& gameXml) const;
    void PopulateColors(pugi::xml_node& gameXml) const;

    bool SavePlayersFile(string playersFilePath) const;
    void PopulatePlayers(pugi::xml_node& playersNode) const;

public:
    static GameStatus& GetInstance()
    {
        static GameStatus instance;
        return instance;
    }
    GameStatus(GameStatus const&) = delete;
    GameStatus& operator=(GameStatus const&) = delete;
    ~GameStatus() {}

    bool Init(Game* game, string mapName, string configFilePath, bool customMap = false) const;
    bool LoadFile(Game* game, string gameFilePath);
    bool SaveFile(Game* game, string gameFilePath, string playersFilePath);
};
