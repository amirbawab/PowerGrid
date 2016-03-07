#include "Overview.h"

Overview::Overview()
{
	Setup();
}


Overview::~Overview()
{
}

/// Sets up the number of resources to add (...and other things maybe?)
void Overview::Setup() {
	// Resizes the vectors to the correct number of players and steps
	nbResourceToAdd.resize(MAX_NB_PLAYERS - 1);
	for (vector<map<Resource, int>> v : nbResourceToAdd) {
		v.resize(NB_STEPS);
	}
	// Initialize the values
	// 2 players
	SetNbResourceToAdd(2, 1, Resource::COAL, 3);
	SetNbResourceToAdd(2, 2, Resource::COAL, 4);
	SetNbResourceToAdd(2, 3, Resource::COAL, 3);
	SetNbResourceToAdd(2, 1, Resource::OIL, 2);
	SetNbResourceToAdd(2, 2, Resource::OIL, 2);
	SetNbResourceToAdd(2, 3, Resource::OIL, 4);
	SetNbResourceToAdd(2, 1, Resource::GARBAGE, 1);
	SetNbResourceToAdd(2, 2, Resource::GARBAGE, 2);
	SetNbResourceToAdd(2, 3, Resource::GARBAGE, 3);
	SetNbResourceToAdd(2, 1, Resource::URANIUM, 1);
	SetNbResourceToAdd(2, 2, Resource::URANIUM, 1);
	SetNbResourceToAdd(2, 3, Resource::URANIUM, 1);
	// 3 players
	SetNbResourceToAdd(2, 1, Resource::COAL, 4);
	SetNbResourceToAdd(2, 2, Resource::COAL, 5);
	SetNbResourceToAdd(2, 3, Resource::COAL, 3);
	SetNbResourceToAdd(2, 1, Resource::OIL, 2);
	SetNbResourceToAdd(2, 2, Resource::OIL, 3);
	SetNbResourceToAdd(2, 3, Resource::OIL, 4);
	SetNbResourceToAdd(2, 1, Resource::GARBAGE, 1);
	SetNbResourceToAdd(2, 2, Resource::GARBAGE, 2);
	SetNbResourceToAdd(2, 3, Resource::GARBAGE, 3);
	SetNbResourceToAdd(2, 1, Resource::URANIUM, 1);
	SetNbResourceToAdd(2, 2, Resource::URANIUM, 1);
	SetNbResourceToAdd(2, 3, Resource::URANIUM, 1);
	// 4 players
	SetNbResourceToAdd(2, 1, Resource::COAL, 5);
	SetNbResourceToAdd(2, 2, Resource::COAL, 6);
	SetNbResourceToAdd(2, 3, Resource::COAL, 4);
	SetNbResourceToAdd(2, 1, Resource::OIL, 3);
	SetNbResourceToAdd(2, 2, Resource::OIL, 4);
	SetNbResourceToAdd(2, 3, Resource::OIL, 5);
	SetNbResourceToAdd(2, 1, Resource::GARBAGE, 2);
	SetNbResourceToAdd(2, 2, Resource::GARBAGE, 3);
	SetNbResourceToAdd(2, 3, Resource::GARBAGE, 4);
	SetNbResourceToAdd(2, 1, Resource::URANIUM, 1);
	SetNbResourceToAdd(2, 2, Resource::URANIUM, 2);
	SetNbResourceToAdd(2, 3, Resource::URANIUM, 2);
	// 5 players
	SetNbResourceToAdd(2, 1, Resource::COAL, 5);
	SetNbResourceToAdd(2, 2, Resource::COAL, 7);
	SetNbResourceToAdd(2, 3, Resource::COAL, 5);
	SetNbResourceToAdd(2, 1, Resource::OIL, 4);
	SetNbResourceToAdd(2, 2, Resource::OIL, 5);
	SetNbResourceToAdd(2, 3, Resource::OIL, 6);
	SetNbResourceToAdd(2, 1, Resource::GARBAGE, 3);
	SetNbResourceToAdd(2, 2, Resource::GARBAGE, 3);
	SetNbResourceToAdd(2, 3, Resource::GARBAGE, 5);
	SetNbResourceToAdd(2, 1, Resource::URANIUM, 2);
	SetNbResourceToAdd(2, 2, Resource::URANIUM, 3);
	SetNbResourceToAdd(2, 3, Resource::URANIUM, 2);
	// 6 players
	SetNbResourceToAdd(2, 1, Resource::COAL, 7);
	SetNbResourceToAdd(2, 2, Resource::COAL, 9);
	SetNbResourceToAdd(2, 3, Resource::COAL, 6);
	SetNbResourceToAdd(2, 1, Resource::OIL, 5);
	SetNbResourceToAdd(2, 2, Resource::OIL, 6);
	SetNbResourceToAdd(2, 3, Resource::OIL, 7);
	SetNbResourceToAdd(2, 1, Resource::GARBAGE, 3);
	SetNbResourceToAdd(2, 2, Resource::GARBAGE, 5);
	SetNbResourceToAdd(2, 3, Resource::GARBAGE, 6);
	SetNbResourceToAdd(2, 1, Resource::URANIUM, 2);
	SetNbResourceToAdd(2, 2, Resource::URANIUM, 3);
	SetNbResourceToAdd(2, 3, Resource::URANIUM, 3);
}


/// Add overview step and return it
std::shared_ptr<Overview::OverviewStep> Overview::AddStep(int step, std::string title) {
    auto overviewStep = std::make_shared<OverviewStep>(step, title);
	steps.push_back(overviewStep);
	return overviewStep;
}

/// Get payment amount for a given number of houses
int Overview::GetPayment(int numCities) {
	if (numCities >= 0 && numCities < NB_PAYMENT_LEVELS)
		return payments[numCities];
	else
		return -1;
};

/// Get the number of cities required to end the game 
int Overview::GetNbCitiesToEnd(int numPlayers) {
	if (numPlayers >= 2 && numPlayers <= MAX_NB_PLAYERS)
		return nbCitiesToEnd[numPlayers - 2];
	else
		return -1;
}

/// Get the number of resources to add of a given type
int Overview::GetNbResourceToAdd(int numPlayers, int step, Resource resource) {
	if ((numPlayers >= 2 && numPlayers <= MAX_NB_PLAYERS) &&
		(step >= 1 && step <= 3))
		return nbResourceToAdd[numPlayers - 2][step - 1][resource];
	else
		return -1;
}

/// Sets the number of resources to be added
void Overview::SetNbResourceToAdd(int numPlayers, int step, Resource resource, int amount) {
	nbResourceToAdd[numPlayers - 2][step - 1][resource] = amount;
}