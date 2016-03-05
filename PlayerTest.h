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
};

