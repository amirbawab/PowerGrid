#include "Overview.h"

Overview::Overview() : overviewRules(MAX_NB_PLAYERS-1)
{
    // Setup
    Setup();
}


Overview::~Overview()
{
}

/// Sets up the number of resources to add (...and other things maybe?)
void Overview::Setup() {

    // TODO To be read from files
    // 2.1 Players
    overviewRules[0].region = 3;
    overviewRules[0].randomeRemove = 8;
    overviewRules[0].maxPowerPlant = 4;
    overviewRules[0].step2Cities = 10;
    overviewRules[0].citiesEndOfGame = 21;

    // 2.2 Players
    overviewRules[0].resourceStep[0][COAL] = 3;
    overviewRules[0].resourceStep[0][OIL] = 2;
    overviewRules[0].resourceStep[0][GARBAGE] = 1;
    overviewRules[0].resourceStep[0][URANIUM] = 1;

    overviewRules[0].resourceStep[1][COAL] = 4;
    overviewRules[0].resourceStep[1][OIL] = 2;
    overviewRules[0].resourceStep[1][GARBAGE] = 2;
    overviewRules[0].resourceStep[1][URANIUM] = 1;

    overviewRules[0].resourceStep[2][COAL] = 3;
    overviewRules[0].resourceStep[2][OIL] = 4;
    overviewRules[0].resourceStep[2][GARBAGE] = 3;
    overviewRules[0].resourceStep[2][URANIUM] = 1;

    // 3.1 Players
    overviewRules[1].region = 3;
    overviewRules[1].randomeRemove = 8;
    overviewRules[1].maxPowerPlant = 3;
    overviewRules[1].step2Cities = 7;
    overviewRules[1].citiesEndOfGame = 17;

    // 3.2 Players
    overviewRules[1].resourceStep[0][COAL] = 4;
    overviewRules[1].resourceStep[0][OIL] = 2;
    overviewRules[1].resourceStep[0][GARBAGE] = 1;
    overviewRules[1].resourceStep[0][URANIUM] = 1;

    overviewRules[1].resourceStep[1][COAL] = 5;
    overviewRules[1].resourceStep[1][OIL] = 3;
    overviewRules[1].resourceStep[1][GARBAGE] = 2;
    overviewRules[1].resourceStep[1][URANIUM] = 1;

    overviewRules[1].resourceStep[2][COAL] = 3;
    overviewRules[1].resourceStep[2][OIL] = 4;
    overviewRules[1].resourceStep[2][GARBAGE] = 3;
    overviewRules[1].resourceStep[2][URANIUM] = 1;

    // 4.1 Players
    overviewRules[2].region = 4;
    overviewRules[2].randomeRemove = 4;
    overviewRules[2].maxPowerPlant = 3;
    overviewRules[2].step2Cities = 7;
    overviewRules[2].citiesEndOfGame = 17;

    // 4.2 Players
    overviewRules[2].resourceStep[0][COAL] = 5;
    overviewRules[2].resourceStep[0][OIL] = 3;
    overviewRules[2].resourceStep[0][GARBAGE] = 2;
    overviewRules[2].resourceStep[0][URANIUM] = 1;

    overviewRules[2].resourceStep[1][COAL] = 6;
    overviewRules[2].resourceStep[1][OIL] = 4;
    overviewRules[2].resourceStep[1][GARBAGE] = 3;
    overviewRules[2].resourceStep[1][URANIUM] = 2;

    overviewRules[2].resourceStep[2][COAL] = 4;
    overviewRules[2].resourceStep[2][OIL] = 5;
    overviewRules[2].resourceStep[2][GARBAGE] = 4;
    overviewRules[2].resourceStep[2][URANIUM] = 2;

    // 5.1 Players
    overviewRules[3].region = 5;
    overviewRules[3].randomeRemove = 0;
    overviewRules[3].maxPowerPlant = 3;
    overviewRules[3].step2Cities = 7;
    overviewRules[3].citiesEndOfGame = 15;

    // 5.2 Players
    overviewRules[3].resourceStep[0][COAL] = 5;
    overviewRules[3].resourceStep[0][OIL] = 4;
    overviewRules[3].resourceStep[0][GARBAGE] = 3;
    overviewRules[3].resourceStep[0][URANIUM] = 2;

    overviewRules[3].resourceStep[1][COAL] = 7;
    overviewRules[3].resourceStep[1][OIL] = 5;
    overviewRules[3].resourceStep[1][GARBAGE] = 3;
    overviewRules[3].resourceStep[1][URANIUM] = 3;

    overviewRules[3].resourceStep[2][COAL] = 5;
    overviewRules[3].resourceStep[2][OIL] = 6;
    overviewRules[3].resourceStep[2][GARBAGE] = 5;
    overviewRules[3].resourceStep[2][URANIUM] = 2;

    // 6.1 Players
    overviewRules[4].region = 5;
    overviewRules[4].randomeRemove = 0;
    overviewRules[4].maxPowerPlant = 3;
    overviewRules[4].step2Cities = 6;
    overviewRules[4].citiesEndOfGame = 14;

    // 6.2 Players
    overviewRules[4].resourceStep[0][COAL] = 7;
    overviewRules[4].resourceStep[0][OIL] = 5;
    overviewRules[4].resourceStep[0][GARBAGE] = 3;
    overviewRules[4].resourceStep[0][URANIUM] = 2;

    overviewRules[4].resourceStep[1][COAL] = 9;
    overviewRules[4].resourceStep[1][OIL] = 6;
    overviewRules[4].resourceStep[1][GARBAGE] = 5;
    overviewRules[4].resourceStep[1][URANIUM] = 3;

    overviewRules[4].resourceStep[2][COAL] = 6;
    overviewRules[4].resourceStep[2][OIL] = 7;
    overviewRules[4].resourceStep[2][GARBAGE] = 6;
    overviewRules[4].resourceStep[2][URANIUM] = 3;
}


/// Add overview step and return it
std::shared_ptr<Overview::OverviewStep> Overview::AddStep(int step, std::string title) {
    auto overviewStep = std::make_shared<OverviewStep>(step, title);
    steps.push_back(overviewStep);
    return overviewStep;
}