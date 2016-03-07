#include "Config.h"
#include <QtCore>
#include "Helpers.h"
#include "StepCard.h"
#include "PowerPlantCard.h"
#include "Game.h"

using std::string;
using std::map;
using std::vector;
using std::shared_ptr;
using std::make_shared;
using std::stoi;

bool Config::LoadIoData(pugi::xml_document& xml)
{
    pathPrefix = map<IO, string>();

    if (!xml.child("config").child("io"))
        return false;

    for (auto pathNode : xml.select_nodes("//io/path"))
    {
        string forAttribute = pathNode.node().attribute("for").value();
        string valueAttribute = pathNode.node().attribute("value").value();
        
        if (ToLower(forAttribute) == "maps")
            pathPrefix[MAPS] = valueAttribute;
        else if (ToLower(forAttribute) == "cards")
            pathPrefix[CARDS] = valueAttribute;
        else if (ToLower(forAttribute) == "colors")
            pathPrefix[COLORS] = valueAttribute;
    }

    return true;
}

bool Config::LoadMaps(pugi::xml_document& xml)
{
    maps = vector<shared_ptr<Map>>();

    if (!xml.child("config").child("maps"))
        return false;

    for (auto mapNode : xml.select_nodes("//maps/map"))
    {
        string fileAttribute = mapNode.node().attribute("file").value();
        auto map = make_shared<Map>(pathPrefix[MAPS] + fileAttribute);
        maps.push_back(map);
    }

    return true;
}

bool Config::LoadResourceMarket(pugi::xml_document& xml)
{
    initialResource = map<Resource, int>();
    
    if (!xml.child("config").child("market"))
        return false;

    for (auto resourceNode : xml.select_nodes("//market/resource"))
    {
        string nameAttribute = resourceNode.node().attribute("name").value();
        auto initialAttribute = stoi(resourceNode.node().attribute("initial").value());

        if (ToLower(nameAttribute) == "coal")
            initialResource[COAL] = initialAttribute;
        else if (ToLower(nameAttribute) == "oil")
            initialResource[OIL] = initialAttribute;
        else if (ToLower(nameAttribute) == "garbage")
            initialResource[GARBAGE] = initialAttribute;
        else if (ToLower(nameAttribute) == "uranium")
            initialResource[URANIUM] = initialAttribute;
    }

    for (auto resource : initialResource)
        game->GetResourceMarket().AddResource(resource.first, resource.second);

    return true;
}

bool Config::LoadCards(pugi::xml_document& xml) const
{
    if (!xml.child("config").child("cards"))
        return false;

    for (auto standardCardNode : xml.select_nodes("//cards/powerPlantCard"))
    {
        auto priceAttribute = stoi(standardCardNode.node().attribute("price").value());
        auto resourcesAttribute = stoi(standardCardNode.node().attribute("resources").value());
		auto powerAttribute = stoi(standardCardNode.node().attribute("power").value());
		auto ignoreFirstAttribute = ToLower(standardCardNode.node().attribute("ignore_first").value());
		auto positionAttribute = ToLower(standardCardNode.node().attribute("position").value());
		//TODO: FARZAD also add the image path to the card
        string imageAttribute = standardCardNode.node().attribute("image").value();

        auto powerPlantCard = make_shared<PowerPlantCard>(priceAttribute,
                                                          resourcesAttribute, powerAttribute);
		powerPlantCard->SetIgnoreFirst(ignoreFirstAttribute != "no");
		powerPlantCard->SetPosition(positionAttribute);

        for (auto resourceNode : standardCardNode.node().children("resource"))
        {
            string nameAttribute = resourceNode.attribute("name").value();

            if (ToLower(nameAttribute) == "coal")
                powerPlantCard->AddActiveResource(COAL);
            else if (ToLower(nameAttribute) == "oil")
                powerPlantCard->AddActiveResource(OIL);
            else if (ToLower(nameAttribute) == "garbage")
                powerPlantCard->AddActiveResource(GARBAGE);
            else if (ToLower(nameAttribute) == "uranium")
                powerPlantCard->AddActiveResource(URANIUM);
        }

        game->GetAllCards().push_back(powerPlantCard);
		game->GetCardStack().GetCards().push_back(powerPlantCard);
    }

    for (auto stepCardNode : xml.select_nodes("//cards/stepCard"))
    {
        auto stepAttribute = stoi(stepCardNode.node().attribute("step").value());
		auto ignoreFirstAttribute = ToLower(stepCardNode.node().attribute("ignore_first").value());
		auto positionAttribute = ToLower(stepCardNode.node().attribute("position").value());
		//TODO: FARZAD also add the image path to the card
        string imageAttribute = stepCardNode.node().attribute("image").value();

        auto stepCard = make_shared<StepCard>(stepAttribute);
		stepCard->SetIgnoreFirst(ignoreFirstAttribute != "no");
		stepCard->SetPosition(positionAttribute);

        game->GetAllCards().push_back(stepCard);
		game->GetCardStack().GetCards().push_back(stepCard);
    }

    return true;
}

bool Config::LoadOverviewCard(pugi::xml_document& xml) const
{
    if (!xml.child("config").child("overview"))
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

bool Config::LoadElektro(pugi::xml_document& xml) const
{
    auto moneyNode = xml.child("config").child("money");

    if (!moneyNode)
        return false;

    int amountAttribute = stoi(moneyNode.attribute("amount").value());
    game->SetInitElektro(amountAttribute);

    return true;
}

bool Config::LoadColors(pugi::xml_document& xml) const
{
    if (!xml.child("config").child("colors"))
        return false;

    for (auto houseColorElement : xml.select_nodes("//colors/color"))
    {
        string nameAttribute = houseColorElement.node().attribute("name").value();
        string imageAttribute = houseColorElement.node().attribute("color").value();
        shared_ptr<HouseColor> houseColor = make_shared<HouseColor>(nameAttribute, imageAttribute);
        game->GetHouseColor().push_back(houseColor);
    }

    return true;
}

bool Config::LoadFile(Game* game, string filePath)
{
    this->game = game;
    pugi::xml_document configXml;

    QFile configXmlFile(filePath.c_str());

    if (!configXmlFile.open(QFile::ReadOnly))
    {
        Error("Could not open file: " + filePath);
        return false;
    }

    QString xmlContent = configXmlFile.readAll();
    auto result = configXml.load_string(xmlContent.toStdString().c_str());

    if (result.status != pugi::status_ok)
    {
        Error("Could not read XML content for configuration");
        Error("Reason: " + string(result.description()) + "\n");
        return false;
    }
    
    if (!LoadIoData(configXml))
    {
        Error("Could not read IO data from the config file\n");
        return false;
    }

    if (!LoadMaps(configXml))
    {
        Error("Could not read map data from the config file\n");
        return false;
    }

    if (!LoadResourceMarket(configXml))
    {
        Error("Could not read resource market data from the config file\n");
        return false;
    }

    if (!LoadCards(configXml))
    {
        Error("Could not read card data from the config file\n");
        return false;
    }

    if (!LoadOverviewCard(configXml))
    {
        Error("Could not read overview card data from the config file\n");
        return false;
    }

    if (!LoadElektro(configXml))
    {
        Error("Could not read money data from the config file\n");
        return false;
    }

    if (!LoadColors(configXml))
    {
        Error("Could not read color data from the config file\n");
        return false;
    }

    return true;
}
