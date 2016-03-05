#include "PlayerTest.h"

/// Run all test in this class
bool PlayerTest::run() {
	
	// Init success
	bool success = true;

	// All test cases
	success &= test_GetName();
	success &= test_SetName();

	// Return true if tests are successful
	return success;
}

/// Test get player name
bool PlayerTest::test_GetName() {
	
	// Create test name
	std::string name = "Bob";
	
	// Create player
	Player pBob(name, nullptr, 0);

	// Check if get name return expected value
	return pBob.GetName() == name;
}

/// Test set player name
bool PlayerTest::test_SetName() {

	// Create test name
	std::string name = "Bob";

	// Create player
	Player pBob("", nullptr, 0);

	// Set name
	pBob.SetName(name);

	// Check if get name return expected value
	return pBob.GetName() == name;
}