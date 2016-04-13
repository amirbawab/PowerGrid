#include <QFile>
#include <memory>

#include "GameStatus.h"
#include "Helpers.h"
#include "StepCard.h"
#include "Config.h"
#include "Game.h"

using std::shared_ptr;
using std::make_shared;
using std::vector;
using std::stoi;
using std::dynamic_pointer_cast;

bool GameStatus::LoadMap(pugi::xml_document& xml) const
{
    if (!xml.child("game").child("map"))
        return false;

    auto mapNode = xml.child("game").child("map");
    string fileAttribute = mapNode.attribute("file").value();
    shared_ptr<Map> map = make_shared<Map>(fileAttribute);
    game->SetMap(map);

    return true;
}

bool GameStatus::LoadColors(pugi::xml_document& xml) const
{
    if (!xml.child("game").child("colors"))
        return false;

    for (auto colorNode : xml.select_nodes("//colors/color"))
    {
        string nameAttribute = colorNode.node().attribute("name").value();
        string imageAttribute = colorNode.node().attribute("image").value();
        auto color = make_shared<HouseColor>(nameAttribute, imageAttribute);
        game->GetHouseColor().push_back(color);
    }

    return true;
}

bool GameStatus::LoadFullTurn(pugi::xml_document& xml) const
{
    auto fullTurnAttribute = xml.child("game").attribute("full_turn");
    if (!fullTurnAttribute)
        return false;

    auto fullTrun = stoi(fullTurnAttribute.value());
    game->SetFullTurn(fullTrun);

    return true;
}

bool GameStatus::LoadPhase(pugi::xml_document& xml) const
{
    auto phaseAttribute = xml.child("game").attribute("phase");
    if (!phaseAttribute)
        return false;

    auto phase = stoi(phaseAttribute.value());
    game->SetPhase(phase);

    return true;
}

bool GameStatus::SaveGameFile(string gameFilePath) const
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

void GameStatus::PopulateMap(pugi::xml_node& gameXml) const
{
    auto mapNode = gameXml.append_child("map");
    auto fileAttribute = mapNode.append_attribute("file");
    fileAttribute.set_value(game->GetMap()->GetFileName().c_str());
}

void GameStatus::PopulateOrderedPlayers(pugi::xml_node& gameXml) const
{
    auto orderedPlayersNode = gameXml.append_child("orderedPlayers");
    for (auto player : game->GetPlayerOrder())
    {
        auto playerNode = orderedPlayersNode.append_child("player");
        auto colorAttribute = playerNode.append_attribute("color");
        colorAttribute.set_value(player->GetColor()->getName().c_str());
    }
}

void GameStatus::PopulateResourceMarket(pugi::xml_node& gameXml) const
{
    auto marketNode = gameXml.append_child("market");

    for (auto i = 0; i < res::total; i++)
    {
        auto resource = static_cast<Resource>(i);

        // Append the node and the attributes
        auto resourceNode = marketNode.append_child("resource");
        auto nameAttribute = resourceNode.append_attribute("name");
        auto amountAttribute = resourceNode.append_attribute("amount");

        // Set attributes values
        nameAttribute.set_value(GetResourceName(resource).c_str());
        amountAttribute.set_value(game->GetResourceMarket()->GetNbResource(resource));
    }
}

void GameStatus::PopulateCardDeck(pugi::xml_node& gameXml) const
{
    auto cardDeckNode = gameXml.append_child("cardDeck");

    PowerPlantCard* powerPlantCard;
    StepCard* stepCard;
    for (auto card : game->GetCardStack().GetCards())
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

void GameStatus::PopulateAllCards(pugi::xml_node& gameXml) const
{
    auto cardsNode = gameXml.append_child("cards");

    for (auto card : game->GetAllCards())
    {
        auto powerPlantCard = dynamic_pointer_cast<PowerPlantCard>(card);
        auto stepCard = dynamic_pointer_cast<StepCard>(card);

        if (powerPlantCard)
        {
            // Append the node and the attributes
            auto powerPlantCardNode = cardsNode.append_child("powerPlantCard");
            auto priceAttribute = powerPlantCardNode.append_attribute("price");
            auto imageAttribute = powerPlantCardNode.append_attribute("image");
            auto resourcesAttribute = powerPlantCardNode.append_attribute("resources");
            auto powerAttribute = powerPlantCardNode.append_attribute("power");

            // Set the attribute values
            priceAttribute.set_value(powerPlantCard->GetPrice());
            imageAttribute.set_value(powerPlantCard->GetImagePath().c_str());
            resourcesAttribute.set_value(powerPlantCard->GetCapacity());
            powerAttribute.set_value(powerPlantCard->GetPower());

            for (auto resource : powerPlantCard->GetActiveResources())
            {
                // Append the node and the attributes
                auto resourceNode = powerPlantCardNode.append_child("resource");
                auto resourceNameAttribute = resourceNode.append_attribute("name");

                // Set the attribute values
                resourceNameAttribute.set_value(GetResourceName(resource).c_str());
            }
        }
        else if (stepCard)
        {
            // Append the node and the attributes
            auto stepCardNode = cardsNode.append_child("stepCard");
            auto stepAttribute = stepCardNode.append_attribute("step");
            auto imageAttribute = stepCardNode.append_attribute("image");

            // Set the attribute values
            stepAttribute.set_value(stepCard->GetStep());
            imageAttribute.set_value(stepCard->GetImagePath().c_str());
        }
    }
}

void GameStatus::PopulateOverview(pugi::xml_node& gameXml) const
{
    auto overviewNode = gameXml.append_child("overview");

    for (auto step : game->GetOverview().GetSteps())
    {
        // Append the node and the attributes
        auto stepNode = overviewNode.append_child("step");
        auto numberAttribute = stepNode.append_attribute("number");
        auto titleAttribute = stepNode.append_attribute("title");

        // Set the attribute values
        numberAttribute.set_value(step->GetStep());
        titleAttribute.set_value(step->GetTitle().c_str());

        for (auto info : step->GetInfo())
        {
            // Append the node and the attributes
            auto infoNode = stepNode.append_child("info");
            auto textAttribute = infoNode.append_attribute("text");

            // Set the attribute values
            textAttribute.set_value(info.c_str());
        }
    }
}

void GameStatus::PopulateColors(pugi::xml_node& gameXml) const
{
    auto colorsNode = gameXml.append_child("colors");
    
    for (auto color : game->GetHouseColor())
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

bool GameStatus::SavePlayersFile(string playersFilePath) const
{
    pugi::xml_document document;
    auto players = document.append_child("players");
    PopulatePlayers(players);
    return document.save_file(playersFilePath.c_str());
}

void GameStatus::PopulatePlayers(pugi::xml_node& playersNode) const
{
    for (auto player : game->GetPlayers())
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

bool GameStatus::LoadPlayers(pugi::xml_document& xml) const
{
    if (!xml.child("players").child("player"))
        return false;

    for (auto playerNode : xml.select_nodes("//player"))
    {
        string playerNameAttribute = playerNode.node().attribute("name").value();
        string playerColorAttribute = playerNode.node().attribute("color").value();
        auto elektro = stoi(playerNode.node().attribute("elektro").value());

        // Read the color and try to find the corresponding color in the collection
        shared_ptr<HouseColor> color = nullptr;

        for (int i = 0; i < game->GetHouseColor().size(); i++)
            if (game->GetHouseColor()[i]->getName() == playerColorAttribute)
            {
                color = game->GetHouseColor()[i];
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

        for (auto houseNode : playerNode.node().child("houses").children("house")) {
            auto cityAttribute = houseNode.attribute("city").value();
            auto priceAttribute = stoi(houseNode.attribute("price").value());

            shared_ptr<House> newHouse = make_shared<House>(
                game->GetMap()->GetCityByName(cityAttribute), player->GetColor());
            newHouse->SetPrice(priceAttribute);

            player->GetHouses().push_back(newHouse);
            game->GetMap()->GetCityByName(cityAttribute)->AddHouse(newHouse.get());
        }

        // Read power plants and add them
        for (auto powerPlantNode : playerNode.node().child("powerplants").children("powerplant"))
        {
            auto priceAttribute = stoi(powerPlantNode.attribute("price").value());

            shared_ptr<PowerPlantCard> playerCard = nullptr;
            for (auto card : game->GetAllCards())
            {
                shared_ptr<PowerPlantCard> powerPlantCard = dynamic_pointer_cast<PowerPlantCard>(card);
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
            for (auto resourceNode : powerPlantNode.children("resource"))
            {
                string resourceNameAttribute = resourceNode.attribute("name").value();
                auto amountAttribute = stoi(resourceNode.attribute("amount").value());
                playerCard->PlaceResource(resourceNameAttribute, amountAttribute);
            }
        }

        game->GetPlayers().push_back(player);
    }

    return true;
}

bool GameStatus::LoadOrderedPlayers(pugi::xml_document& xml) const
{
    if (!xml.child("game").child("orderedPlayers"))
        return false;

    for (auto playerNode : xml.select_nodes("//orderedPlayers/player"))
    {
        string colorAttribute = playerNode.node().attribute("color").value();

        shared_ptr<Player> player;
        for (auto p : game->GetPlayers())
            if (p->GetColor()->getName() == colorAttribute)
            {
                player = p;
                break;
            }

        // Specified player couldn't be found in the list of players
        if (!player)
        {
            Error("Player with color '" + colorAttribute + "' couldn't be found " +
                  "in the list of players\n");
            return false;
        }

        game->GetPlayerOrder().push_back(player);
    }

    return true;
}

bool GameStatus::LoadResourceMarket(pugi::xml_document& xml) const
{
    if (!xml.child("game").child("market"))
        return false;

    for (auto resourceNode : xml.select_nodes("//market/resource"))
    {
        string nameAttribute = resourceNode.node().attribute("name").value();
        auto amountAttribute = stoi(resourceNode.node().attribute("amount").value());
        game->GetResourceMarket()->AddResource(nameAttribute, amountAttribute);
    }

    return true;
}

bool GameStatus::LoadAllCards(pugi::xml_document& xml) const
{
    auto cardsNode = xml.child("game").child("cards");
    if (!cardsNode)
        return false;

    for (auto cardNode : cardsNode.children())
    {
        if (ToLower(cardNode.name()) == "powerplantcard")
        {
            auto priceAttribute = stoi(cardNode.attribute("price").value());
            string imageAttribute = cardNode.attribute("image").value();
            auto resourcesAttribute = stoi(cardNode.attribute("resources").value());
            auto powerAttribute = stoi(cardNode.attribute("power").value());
            // TODO: FARZAD add image attribute to the card
            auto card = make_shared<PowerPlantCard>(priceAttribute,
                                                                powerAttribute, resourcesAttribute);

            for (auto resourceNode : cardNode.children("resource"))
            {
                string nameAttribute = resourceNode.attribute("name").value();
                card->AddActiveResource(nameAttribute);
            }

            game->GetAllCards().push_back(card);
        }
        else if (ToLower(cardNode.name()) == "stepcard")
        {
            auto stepAttribute = stoi(cardNode.attribute("step").value());
            string imageAttribute = cardNode.attribute("image").value();
            // TODO: FARZAD add image attribute to the card
            auto card = make_shared<StepCard>(stepAttribute);
            game->GetAllCards().push_back(card);
        }
    }

    return true;
}

bool GameStatus::LoadVisibleCards(pugi::xml_document& xml) const
{
    auto visibleCardsNode = xml.child("game").child("visibleCards");
    if (!visibleCardsNode)
        return false;

    for (auto cardNode : visibleCardsNode.children())
    {
        if (ToLower(cardNode.name()) == "powerplantcard")
        {
            auto priceAttribute = stoi(cardNode.attribute("price").value());

            shared_ptr<PowerPlantCard> playerCard = nullptr;
            for (auto card : game->GetAllCards())
            {
                shared_ptr<PowerPlantCard> powerPlantCard = dynamic_pointer_cast<PowerPlantCard>(card);
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

            game->GetCardStack().GetVisibleCards().push_back(playerCard);
        }
    }

    return true;
}

bool GameStatus::LoadCardDeck(pugi::xml_document& xml) const
{
    auto cardDeckNode = xml.child("game").child("cardDeck");
    if (!cardDeckNode)
        return false;

    for (auto cardNode : cardDeckNode.children())
    {
        if (ToLower(cardNode.name()) == "powerplantcard")
        {
            auto priceAttribute = stoi(cardNode.attribute("price").value());

            shared_ptr<PowerPlantCard> playerCard = nullptr;
            for (auto card : game->GetAllCards())
            {
                shared_ptr<PowerPlantCard> powerPlantCard = dynamic_pointer_cast<PowerPlantCard>(card);
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

            game->GetCardStack().GetCards().push_back(playerCard);
        }
        else if (ToLower(cardNode.name()) == "stepcard")
        {
            auto stepAttribute = stoi(cardNode.attribute("step").value());

            shared_ptr<StepCard> cardDeskStepCard = nullptr;
            for (auto card : game->GetAllCards())
            {
                shared_ptr<StepCard> stepCard = dynamic_pointer_cast<StepCard>(card);
                if (stepCard && stepCard->GetStep() == stepAttribute)
                {
                    cardDeskStepCard = stepCard;
                    break;
                }
            }

            // The specified card couldn't be found
            if (!cardDeskStepCard)
            {
                Error("Card with specified step: '" + std::to_string(stepAttribute) + "' is not valid!");
                return false;
            }

            game->GetCardStack().GetCards().push_back(cardDeskStepCard);
        }
    }

    return true;
}

bool GameStatus::LoadOverview(pugi::xml_document& xml) const
{
    if (!xml.child("game").child("overview"))
        return false;

    for (auto stepNode : xml.select_nodes("//overview/step"))
    {
        int numberAttribute = stoi(stepNode.node().attribute("number").value());
        string titleAttribute = stepNode.node().attribute("title").value();

        auto overviewStep = game->GetOverview().AddStep(numberAttribute, titleAttribute);

        for (auto infoNode : stepNode.node().children("info"))
        {
            string textAttribute = infoNode.attribute("text").value();
            overviewStep->AddInfo(textAttribute);
        }
    }

    return true;
}

bool GameStatus::Init(Game* game, string mapName, string configFilePath, bool customMap) const
{
    // Reset game values
    game->Reset();

    if (!Config::GetInstance().LoadFile(game, configFilePath))
        return false;

    // Load from custom file
    if (customMap)
    {
        auto map = make_shared<Map>(mapName);
        game->SetMap(map);
    }
    // Get from the config file
    else
    {
        for (auto map : Config::GetInstance().GetMaps())
        {
            if (map->GetName() == mapName)
            {
                game->SetMap(map);
                break;
            }
        }
    }

    return true;
}

bool GameStatus::LoadFile(Game* game, string gameFilePath,
                          string playersFilePath)
{
    this->game = game;

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

    if (!LoadResourceMarket(gameXml))
    {
        Error("Could not read resource market data from the game file\n");
        return false;
    }

    if (!LoadAllCards(gameXml))
    {
        Error("Could not read data for all cards from the game file\n");
        return false;
    }

    if (!LoadVisibleCards(gameXml))
    {
        Error("Could not read visible card data from the game file\n");
        return false;
    }

    if (!LoadCardDeck(gameXml))
    {
        Error("Could not read card deck data from the game file\n");
        return false;
    }

    if (!LoadOverview(gameXml))
    {
        Error("Could not read card overview data from the game file\n");
        return false;
    }

    if (!LoadPlayers(playersXml)) {
        Error("Could not read player data from players file\n");
        return false;
    }

    if (!LoadOrderedPlayers(gameXml)) {
        Error("Could not read oredered players from the game file\n");
        return false;
    }

    if (!LoadFullTurn(gameXml))
    {
        Error("Could not read full turn value from the game file\n");
        return false;
    }

    if (!LoadPhase(gameXml))
    {
        Error("Could not read phase value from the game file\n");
        return false;
    }

    return true;
}

bool GameStatus::SaveFile(Game* game, string gameFilePath,
                          string playersFilePath)
{
    this->game = game;

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
