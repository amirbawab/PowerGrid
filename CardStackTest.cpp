#include"CardStackTest.h"

bool CardStackTest::run() 
{
	bool success = true;

	// All test cases
	success &= test_SetCards();
	success &= test_DrawPlant();
	success &= test_RemovePlant();
	
	// Return true if tests are successful
	return success;


}

bool CardStackTest::test_SetCards()
{
	std::vector<std::shared_ptr<Card>> testcolor = std::vector<std::shared_ptr<Card>>();
	CardStack bob;
	bob.SetCards(testcolor);
	
	return testcolor == bob.GetCards();

}

bool CardStackTest::test_DrawPlant()
{
	CardStack bob;

	bob.DrawPlant();
	std::vector<std::shared_ptr<Card>> cards;

	return cards[0] == NULL;



}

bool CardStackTest::test_RemovePlant() {

	CardStack bob;

	bob.RemovePlant(1);

	std::vector<std::shared_ptr<Card>> visiblePlants;

	return visiblePlants[1] == NULL;
}

bool CardStackTest::test_GetPlant()
{



}