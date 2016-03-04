#include <QFile>
#include "GameStatus.h"
#include "Helpers.h"
#include "StepCard.h"
#include "Config.h"

using std::shared_ptr;
using std::make_shared;
using std::vector;
using std::stoi;

bool GameStatus::LoadMap(pugi::xml_document& xml)
{
    if (!xml.child("game").child("map"))
        return false;

    auto mapNode = xml.child("game").child("map");
    string fileAttribute = mapNode.attribute("file").value();
    map = make_shared<Map>(fileAttribute);
    return true;
}

bool GameStatus::LoadColors(pugi::xml_document& xml)
{
    colors = vector<shared_ptr<HouseColor>>();

    if (!xml.child("game").child("colors"))
        return false;

    for (auto colorNode : xml.select_nodes("//colors/color"))
    {
        string nameAttribute = colorNode.node().attribute("name").value();
        string imageAttribute = colorNode.node().attribute("image").value();
        auto color = make_shared<HouseColor>(nameAttribute, imageAttribute);
        colors.push_back(color);
    }

    return true;
}

bool GameStatus::LoadPlayers(pugi::xml_document& xml)
{
    if (!xml.child("players").child("player"))
        return false;

    for (auto playerNode : xml.select_nodes("//player"))
    {
        string playerNameAttribute = playerNode.node().attribute("name").value();
        string playerColorAttribute = playerNode.node().attribute("color").value();
        auto elektro = stoi(playerNode.node().attribute("elektro").value());

        // Read the color and try to find the corresponding color in the collection
        HouseColor* color = nullptr;
        for (int i = 0; i < colors.size(); i++)
            if (colors[i]->getName() == playerColorAttribute)
            {
                color = colors[i].get();
                break;
            }

        if (!color)
        {
            Error("color '" + playerColorAttribute + "' specified for player '" + playerNameAttribute +
                  "' is not valid\n");
            return false;
        }

        auto player = make_shared<Player>(playerNameAttribute, color, elektro);

        // Read resource values and add them
        for (auto resourceNode : playerNode.node().select_nodes("//resource"))
        {
            string resourceNameAttribute = resourceNode.node().attribute("name").value();
            auto resourceAmountAttribute = stoi(resourceNode.node().attribute("amount").value());
            player->AddResource(GetResourceByName(resourceNameAttribute), resourceAmountAttribute);
        }

        // Read power plants and add them
        for (auto powerPlantNode : playerNode.node().select_nodes("//powerplant"))
        {
            auto priceAttribute = stoi(powerPlantNode.node().attribute("price").value());

            PowerPlantCard* playerCard = nullptr;
            for (auto card : Config::GetInstance().GetCards())
            {
                auto powerPlantCard = dynamic_cast<PowerPlantCard*>(card.get());
                if (powerPlantCard && powerPlantCard->GetPrice() == priceAttribute)
                {
                    playerCard = powerPlantCard;
                    break;
                }
            }

            // The specified card couldn't be found
            if (!playerCard)
            {
                Error("Card with specified price: '" + std::to_string(priceAttribute) + "' is not valid!");
                return false;
            }

            if (playerCard)
                player->AddPowerPlant(playerCard);
        }

        players.push_back(player);
    }

    return true;
}

bool GameStatus::LoadOrderedPlayers(pugi::xml_document& xml)
{
    orderedPlayers = vector<Player*>();

    if (!xml.child("game").child("orderedPlayers"))
        return false;

    for (auto playerNode : xml.select_nodes("//orderedPlayers/player"))
    {
        string nameAttribute = playerNode.node().attribute("name").value();

        Player* player = nullptr;
        for (auto p : players)
            if (p->GetName() == nameAttribute)
            {
                player = p.get();
                break;
            }

        // Specified player couldn't be found in the list of players
        if (!player)
        {
            Error("Player with name '" + nameAttribute + "' couldn't be found" +
                  "in the list of players\n");
            return false;
        }

        orderedPlayers.push_back(player);
    }

    return true;
}

bool GameStatus::LoadResourceMarket(pugi::xml_document& xml)
{
    resourceMarket = make_shared<ResourceMarket>();
    if (!xml.child("game").child("market"))
        return false;

    for (auto resourceNode : xml.select_nodes("//market/resource"))
    {
        string nameAttribute = resourceNode.node().attribute("name").value();
        auto amountAttribute = stoi(resourceNode.node().attribute("amount").value());
        resourceMarket->AddResource(nameAttribute, amountAttribute);
    }

    return true;
}

bool GameStatus::LoadCardDeck(pugi::xml_document& xml)
{
    cardDeck = vector<shared_ptr<Card>>();

    auto cardDeckNode = xml.child("game").child("cardDeck");
    if (!cardDeckNode)
        return false;

    for (auto cardNode : cardDeckNode.children())
    {
        if (cardNode.name() == "powerPlantCard")
        {
            auto priceAttribute = stoi(cardNode.attribute("price").value());
            string imageAttribute = cardNode.attribute("image").value();
            auto resourcesAttribute = stoi(cardNode.attribute("resources").value());
            auto powerAttribute = stoi(cardNode.attribute("power").value());
            // TODO: add image attribute to the card
            auto card = make_shared<PowerPlantCard>(priceAttribute,
                                                                powerAttribute, resourcesAttribute);

            for (auto resourceNode : cardNode.children("resource"))
            {
                string nameAttribute = resourceNode.attribute("name").value();
                card->AddResource(nameAttribute);
            }

            cardDeck.push_back(card);
        }
        else if (cardNode.name() == "stepCard")
        {
            auto stepAttribute = stoi(cardNode.attribute("step").value());
            string imageAttribute = cardNode.attribute("image").value();
            // TODO: add image attribute to the card
            auto card = make_shared<StepCard>(stepAttribute);
            cardDeck.push_back(card);
        }
    }

    return true;
}

bool GameStatus::ReadFile(string gameFilePath, string playersFilePath)
{
    pugi::xml_document gameXml;
    pugi::xml_document playersXml;

    QFile gameXmlFile(gameFilePath.c_str());
    QFile playersXmlFile(playersFilePath.c_str());

    if (!gameXmlFile.open(QIODevice::ReadOnly))
    {
        Error("Could not open file: " + gameFilePath);
        return false;
    }

    if (!playersXmlFile.open(QIODevice::ReadOnly))
    {
        Error("Could not open file: " + playersFilePath);
        return false;
    }

    QString gameXmlContent = gameXmlFile.readAll();
    QString playersXmlContent = playersXmlFile.readAll();

    // Read game content from file
    auto result = gameXml.load_string(gameXmlContent.toStdString().c_str());
    if (result.status != pugi::status_ok)
    {
        Error("Could not read XML content for saved game");
        Error("Reason: " + string(result.description()) + "\n");
        return false;
    }

    // Read players content from file
    result = playersXml.load_string(playersXmlContent.toStdString().c_str());
    if (result.status != pugi::status_ok)
    {
        Error("Could not read XML content for players");
        Error("Reason: " + string(result.description()) + "\n");
        return false;
    }

    if (!LoadMap(gameXml))
    {
        Error("Could not read map data from the game file\n");
        return false;
    }

    if (!LoadColors(gameXml))
    {
        Error("Could not read color data from the game file\n");
        return false;
    }

    if (!LoadPlayers(playersXml))
    {
        Error("Could not read player data from players file\n");
        return false;
    }

    if (!LoadOrderedPlayers(gameXml))
    {
        Error("Could not read oredered players from the game file\n");
        return false;
    }

    if (!LoadResourceMarket(gameXml))
    {
        Error("Could not read resource market data from the game file\n");
        return false;
    }

    if (!LoadCardDeck(gameXml))
    {
        Error("Could not read card deck data from the game file\n");
        return false;
    }

    return true;
}
