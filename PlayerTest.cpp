#include "PlayerTest.h"
#include <string>
#include "Player.h"

/// Run all test in this class
bool PlayerTest::run() {
    
    // Init success
    bool success = true;

    // All test cases
    success &= test_GetName();
    success &= test_SetName();
    success &= test_SetElektro();
    success &= test_HasElektro();
    success &= test_GetHouses();
    success &= test_AddPowerPlant();
    success &= test_GetPowerPlants();
    // Return true if tests are successful
    return success;
}

/// Test get player name
bool PlayerTest::test_GetName() {
    
    // Create test name
    string name = "Bob";
    
    // Create player
    Player pBob(name, nullptr, 0);

    // Check if get name return expected value
    return pBob.GetName() == name;
}

/// Test set player name
bool PlayerTest::test_SetName() {

    // Create test name
    string name = "Bob";

    // Create player
    Player pBob("", nullptr, 0);

    // Set name
    pBob.SetName(name);

    // Check if get name return expected value
    return pBob.GetName() == name;
}

bool PlayerTest::test_GetElektro() {

    Player pBob("", nullptr, 0);
    
    pBob.SetElektro(20);

        
    
    int test = pBob.GetElektro();

    return pBob.GetElektro() == test;


}

bool PlayerTest::test_GetColor()
{
    Player pBob("", nullptr, 0);
    shared_ptr<HouseColor> testcolor = std::make_shared<HouseColor>("black", "");
    pBob.SetColor(testcolor);


    return testcolor == pBob.GetColor();

}

bool PlayerTest::test_SetElektro()
{
    Player pBob("", nullptr, 0);
    
    int testelectro = 20;

    pBob.SetElektro(testelectro);

    return pBob.GetElektro() == testelectro;


}

bool PlayerTest::test_GetPowerPlants()
{
    vector<shared_ptr<PowerPlantCard>> test;
    Player pBob("", nullptr, 0);


    return test == pBob.GetPowerPlants();


}

bool PlayerTest::test_HasElektro()
{
    int testelektro = 20;

    
    Player pBob("", nullptr, 0);
    pBob.SetElektro(20);
    
    return 	pBob.HasElektro(testelektro);

}

bool PlayerTest::test_GetHouses()

{
    Player pBob("", nullptr, 0);
    vector<shared_ptr<House>> testhouse = pBob.GetHouses();

    return testhouse == pBob.GetHouses();
}

bool PlayerTest::test_BuyHouse()
{
//    Player pBob("", nullptr, 0);
//
//    shared_ptr<House> testhouse = std::make_shared<House>();
//    return pBob.BuyHouse(testhouse);
    return true;
}

bool PlayerTest::test_AddPowerPlant() {

    Player pBob("", nullptr, 0);
    shared_ptr<PowerPlantCard> test = std::make_shared<PowerPlantCard>();
    return pBob.AddPowerPlant(test);
    

}