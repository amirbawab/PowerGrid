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

bool GameStatus::SaveGameFile(string gameFilePath)
{
    pugi::xml_document document;
    auto game = document.append_child("game");
    PopulateMap(game);
    PopulateOrderedPlayers(game);
    PopulateResourceMarket(game);
    PopulateCardDeck(game);
    PopulateColors(game);
    return document.save_file(gameFilePath.c_str());
}

void GameStatus::PopulateMap(pugi::xml_node& game)
{
    auto mapNode = game.append_child("map");
    auto fileAttribute = mapNode.append_attribute("file");
    // TODO: do we need the 'description' attribute???
//    auto descriptionAttribute = mapNode.append_attribute("description");
    fileAttribute.set_value(map->GetFileName().c_str());
}

void GameStatus::PopulateOrderedPlayers(pugi::xml_node& game)
{
    auto orderedPlayersNode = game.append_child("orderedPlayers");
    for (auto player : orderedPlayers)
    {
        auto playerNode = orderedPlayersNode.append_child("player");
        auto nameAttribute = playerNode.append_attribute("name");
        nameAttribute.set_value(player->GetName().c_str());
    }
}

void GameStatus::PopulateResourceMarket(pugi::xml_node& game)
{
    auto marketNode = game.append_child("market");
    // TODO: resource market should return its number of resources
}

void GameStatus::PopulateCardDeck(pugi::xml_node& game)
{
    auto cardDeckNode = game.append_child("cardDeck");

    PowerPlantCard* powerPlantCard;
    StepCard* stepCard;
    for (auto card : cardDeck)
    {
        powerPlantCard = dynamic_cast<PowerPlantCard*>(card.get());
        stepCard = dynamic_cast<StepCard*>(card.get());

        // If we have a power-plant card
        if (powerPlantCard)
        {
            // Append the node and the attributes
            auto powerPlantCardNode = cardDeckNode.append_child("powerPlantCard");
            auto priceAttribute = powerPlantCardNode.append_attribute("price");
            auto imageAttribute = powerPlantCardNode.append_attribute("image");
            auto resourcesAttribute = powerPlantCardNode.append_attribute("resources");
            auto powerAttribute = powerPlantCardNode.append_attribute("power");

            // Set attribute values
            priceAttribute.set_value(powerPlantCard->GetPrice());
            imageAttribute.set_value(powerPlantCard->GetImagePath().c_str());
            resourcesAttribute.set_value(powerPlantCard->GetActiveResources().size());
            powerAttribute.set_value(powerPlantCard->GetPower());

            for (auto resource : powerPlantCard->GetActiveResources())
            {
                // Append the node and the attributes
                auto resourceNode = powerPlantCardNode.append_child("resource");
                auto resourceNameAttribute = resourceNode.append_attribute("name");

                // Set attribute values
                resourceNameAttribute.set_value(GetResourceName(resource).c_str());
            }
        }

        // If we have a step card
        if (stepCard)
        {
            // Append the node and the attributes
            auto stepCardNode = cardDeckNode.append_child("stepCard");
            auto stepAttribute = stepCardNode.append_attribute("step");
            auto imageAttribute = stepCardNode.append_attribute("image");

            // Set attribute values
            stepAttribute.set_value(stepCard->GetStep());
            imageAttribute.set_value(stepCard->GetImagePath().c_str());
        }
    }
}

void GameStatus::PopulateColors(pugi::xml_node& game)
{
    auto colorsNode = game.append_child("colors");
    
    for (auto color : colors)
    {
        // Append the node and the attributes
        auto colorNode = colorsNode.append_child("color");
        auto nameAttribute = colorNode.append_attribute("name");
        auto imageAttribute = colorNode.append_attribute("image");

        // Set attribute values
        nameAttribute.set_value(color->getName().c_str());
        imageAttribute.set_value(color->getImage().c_str());
    }
}

bool GameStatus::SavePlayersFile(string playersFilePath)
{
    pugi::xml_document document;
    auto players = document.append_child("players");
    PopulatePlayers(players);
    return document.save_file(playersFilePath.c_str());
}

void GameStatus::PopulatePlayers(pugi::xml_node& playersNode)
{
    for (auto player : players)
    {
        // Append the node and the attributes
        auto playerNode = playersNode.append_child("player");
        auto nameAttribute = playerNode.append_attribute("name");
        auto colorAttribute = playerNode.append_attribute("color");
        auto elektroAttribute = playerNode.append_attribute("elektro");

        // Set attribute values
        nameAttribute.set_value(player->GetName().c_str());
        colorAttribute.set_value(player->GetColor()->getName().c_str());
        elektroAttribute.set_value(player->GetElektro());

        // If the player has houses, add them
        if (player->GetHouses().size() > 0)
        {
            auto housesNode = playerNode.append_child("houses");
            for (auto house : player->GetHouses())
            {
                // Append the node and the attributes
                auto houseNode = housesNode.append_child("house");
                auto cityAttribute = houseNode.append_attribute("city");
                auto priceAttribute = houseNode.append_attribute("price");

                // Set attribute values
                cityAttribute.set_value(house->GetCity()->GetName().c_str());
                priceAttribute.set_value(house->GetPrice());
            }
        }

        // If the player has power plants, add them
        if (player->GetPowerPlants().size() > 0)
        {
            auto powerPlantsNode = playerNode.append_child("powerplants");
            for (auto powerPlant : player->GetPowerPlants())
            {
                // Append the node and the attributes
                auto powerPlantNode = powerPlantsNode.append_child("powerplant");
                auto priceAttribute = powerPlantNode.append_attribute("price");

                // Set attribute values
                priceAttribute.set_value(powerPlant->GetPrice());

                // Add resources, if any
                for (auto resource : powerPlant->GetPlacedResources())
                {
                    // Append the node and the attributes
                    auto resourceNode = powerPlantNode.append_child("resource");
                    auto resourceNameAttribute = resourceNode.append_attribute("name");
                    auto amountAttribute = resourceNode.append_attribute("amount");

                    // Set attribute values
                    resourceNameAttribute.set_value(GetResourceName(resource.first).c_str());
                    amountAttribute.set_value(resource.second);
                }
            }
        }
    }
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
        std::shared_ptr<HouseColor> color = nullptr;

        for (int i = 0; i < colors.size(); i++)
            if (colors[i]->getName() == playerColorAttribute)
            {
                color = colors[i];
                break;
            }

        // No corresponding color could be found
        if (!color)
        {
            Error("color '" + playerColorAttribute + "' specified for player '" + playerNameAttribute +
                  "' is not valid\n");
            return false;
        }

        auto player = make_shared<Player>(playerNameAttribute, color, elektro);

        // Read power plants and add them
        for (auto powerPlantNode : playerNode.node().select_nodes("//powerplant"))
        {
            auto priceAttribute = stoi(powerPlantNode.node().attribute("price").value());

            std::shared_ptr<PowerPlantCard> playerCard = nullptr;
            for (auto card : Config::GetInstance().GetCards())
            {
                std::shared_ptr<PowerPlantCard> powerPlantCard = std::dynamic_pointer_cast<PowerPlantCard>(card);
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

            player->AddPowerPlant(playerCard);

            // Read resource values and put them in the card
            for (auto resourceNode : powerPlantNode.node().select_nodes("//resource"))
            {
                string resourceNameAttribute = resourceNode.node().attribute("name").value();
                auto amountAttribute = stoi(resourceNode.node().attribute("amount").value());
                playerCard->PlaceResource(resourceNameAttribute, amountAttribute);
            }
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
            Error("Player with name '" + nameAttribute + "' couldn't be found " +
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
        if (ToLower(cardNode.name()) == "powerplantcard")
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
                card->AddActiveResource(nameAttribute);
            }

            cardDeck.push_back(card);
        }
        else if (ToLower(cardNode.name()) == "stepcard")
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

bool GameStatus::LoadFile(string gameFilePath, string playersFilePath)
{
    pugi::xml_document gameXml;
    pugi::xml_document playersXml;

    QFile gameXmlFile(gameFilePath.c_str());
    QFile playersXmlFile(playersFilePath.c_str());

    if (!gameXmlFile.open(QFile::ReadOnly))
    {
        Error("Could not open file: " + gameFilePath);
        return false;
    }

    if (!playersXmlFile.open(QFile::ReadOnly))
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

bool GameStatus::SaveFile(string gameFilePath, string playersFilePath)
{
    if (!SaveGameFile(gameFilePath))
    {
        Error("Could not save game file to the path '" + gameFilePath + "'\n");
        return false;
    }

    if (!SavePlayersFile(playersFilePath))
    {
        Error("Could not save players file to the path '" + playersFilePath + "'\n");
        return false;
    }

    return true;
}
