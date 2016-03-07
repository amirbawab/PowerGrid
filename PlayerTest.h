#pragma once
#include "GenericTest.h"
#include "Player.h"
#include <string>

class PlayerTest : public GenericTest {
public:
	PlayerTest() {}
	~PlayerTest() {}
	bool run();
private:
	bool test_GetName();
	bool test_SetName();
	bool test_GetElektro();
	bool test_GetColor();
	bool test_SetElektro();
	bool test_GetPowerPlants();
	bool test_HasElektro();
	bool test_GetHouses();
	bool test_BuyHouse();
	bool test_AddPowerPlant();
};

