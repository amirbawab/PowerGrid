#include "Config.h"
#include <QtCore>
#include "Helpers.h"
#include "Card.h"

using std::string;
using std::map;
using std::vector;
using std::shared_ptr;
using std::make_shared;
using std::stoi;

bool Config::LoadIoData(pugi::xml_document& xml)
{
    io = map<IO, string>();

    if (!xml.child("config").child("io"))
        return false;

    for (auto pathNode : xml.select_nodes("//io/path"))
    {
        string forAttribute = pathNode.node().attribute("for").value();
        string valueAttribute = pathNode.node().attribute("value").value();
        
        if (ToLower(forAttribute) == "maps")
            io[MAPS] = valueAttribute;
        else if (ToLower(forAttribute) == "cards")
            io[CARDS] = valueAttribute;
        else if (ToLower(forAttribute) == "colors")
            io[COLORS] = valueAttribute;
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
        shared_ptr<Map> map = make_shared<Map>(io[MAPS] + fileAttribute);
        maps.push_back(map);
    }

    return true;
}

bool Config::LoadResourceMarket(pugi::xml_document& xml)
{
    totalResource = map<Resource, int>();
    initialResource = map<Resource, int>();
    
    if (!xml.child("config").child("market"))
        return false;

    for (auto resourceNode : xml.select_nodes("//market/resource"))
    {
        string nameAttribute = resourceNode.node().attribute("name").value();
        int totalAttribute = stoi(resourceNode.node().attribute("total").value());
        int initialAttribute = stoi(resourceNode.node().attribute("initial").value());

        if (ToLower(nameAttribute) == "coal")
        {
            totalResource[COAL] = totalAttribute;
            initialResource[COAL] = initialAttribute;
        } else if (ToLower(nameAttribute) == "oil")
        {
            totalResource[OIL] = totalAttribute;
            initialResource[OIL] = initialAttribute;
        } else if (ToLower(nameAttribute) == "garbage")
        {
            totalResource[GARBAGE] = totalAttribute;
            initialResource[GARBAGE] = initialAttribute;
        } else if (ToLower(nameAttribute) == "uranium")
        {
            totalResource[URANIUM] = totalAttribute;
            initialResource[URANIUM] = initialAttribute;
        }
    }

    return true;
}

bool Config::LoadCards(pugi::xml_document& xml)
{
    cards = vector<shared_ptr<Card>>();

    if (!xml.child("config").child("cards"))
        return false;

    for (auto standardCardNode : xml.select_nodes("//cards/standardCard"))
    {
        int priceAttribute = stoi(standardCardNode.node().attribute("price").value());
        int resourcesAttribute = stoi(standardCardNode.node().attribute("resources").value());
        int powerAttribute = stoi(standardCardNode.node().attribute("power").value());
        //TODO: also add the image path to the card
        string imageAttribute = standardCardNode.node().attribute("image").value();

        auto standardCard = make_shared<StandardCard>(priceAttribute,
                                                      resourcesAttribute, powerAttribute);

        for (auto resourceNode : standardCardNode.node().children("resource"))
        {
            string nameAttribute = resourceNode.attribute("name").value();

            if (ToLower(nameAttribute) == "coal")
                standardCard->AddResource(COAL);
            else if (ToLower(nameAttribute) == "oil")
                standardCard->AddResource(OIL);
            else if (ToLower(nameAttribute) == "garbage")
                standardCard->AddResource(GARBAGE);
            else if (ToLower(nameAttribute) == "uranium")
                standardCard->AddResource(URANIUM);
        }

        cards.push_back(standardCard);
    }

    for (auto stepCardNode : xml.select_nodes("//cards/stepCard"))
    {
        int stepAttribute = stoi(stepCardNode.node().attribute("step").value());
        //TODO: also add the image path to the card
        string imageAttribute = stepCardNode.node().attribute("image").value();

        auto stepCard = make_shared<StepCard>(stepAttribute);
        cards.push_back(stepCard);
    }

    return true;
}

bool Config::LoadOverviewCards(pugi::xml_document& xml)
{
    //TODO: read overview card data from the file
    return true;
}

bool Config::LoadElektro(pugi::xml_document& xml)
{
    auto moneyNode = xml.child("config").child("money");

    if (!moneyNode)
        return false;

    int amountAttribute = stoi(moneyNode.attribute("amount").value());
    elektro = amountAttribute;
    return true;
}

bool Config::ReadFile(string filePath)
{
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
        Error("Could not open file: '" + filePath + "'");
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

    if (!LoadOverviewCards(configXml))
    {
        Error("Could not read overview card data from the config file\n");
        return false;
    }

    if (!LoadElektro(configXml))
    {
        Error("Could not read money data from the config file\n");
        return false;
    }

    return true;
}
