#pragma once
#include "GenericTest.h"
#include "CardStack.h"
#include <string>

class CardStackTest : public GenericTest{

public:
	CardStackTest() {}
	~CardStackTest() {}
	bool run();
private:
	bool test_SetCards();
	bool test_GetPlant();
	bool test_RemovePlant();
	bool test_DrawPlant();


};