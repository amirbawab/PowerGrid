#pragma once

#include "Resource.h"
#include <memory>
#include <string>
#include <vector>
#include <map>

using std::vector;
using std::map;

class Overview
{
public:

	// Overview step
    class OverviewStep
    {
    private:
        int step;
        std::string title;
        std::vector<std::string> info;
    public:
        OverviewStep(int step, std::string title) : step{ step }, title{ title } {}
        int GetStep() const { return step; }
        std::string GetTitle() const { return title; }
        void AddInfo(std::string infoString) { info.push_back(infoString); }
        std::vector<std::string> const& GetInfo() const { return info; }

		// To string
		friend std::ostream& operator<<(std::ostream& stream, const OverviewStep& step) {
			stream << "Step " << step.GetStep() << ": " << step.GetTitle() << std::endl;
			for (auto info : step.GetInfo())
				stream << "\t" << info << std::endl;
			return stream;
		}

    };

	// Overview rules
	struct OverviewRule {
	public:
		// Constructor
		OverviewRule() : resourceStep(NB_STEPS) {}

		// Table 1
		int region;
		int randomeRemove;
		int maxPowerPlant;
		int step2Cities;
		int citiesEndOfGame;
		int GetResourceAt(int phase, Resource resource) { return resourceStep[phase - 1][resource]; }
	private:
		// Table 2
		vector<map<Resource, int>> resourceStep;
		friend class Overview;
	};

	// Methods
    Overview();
    ~Overview();

    std::vector<std::shared_ptr<OverviewStep>> const& GetSteps() const { return steps; }
	std::shared_ptr<OverviewStep> AddStep(int step, std::string title);
	int GetPayment(int numCities) { return payments[numCities]; }
	OverviewRule GetRuleByNumOfPlayers(int numberOfPlayer) { return overviewRules[numberOfPlayer-2]; }

	void Setup();

private:
    std::vector<std::shared_ptr<OverviewStep>> steps;
	static const int MAX_NB_PLAYERS = 6;
	static const int NB_STEPS = 3;

	// Get money
	const vector<int> payments{ 10, 22, 33, 44, 54, 64, 73, 82, 90, 98, 105, 112, 118, 124, 129, 134, 138, 142, 145, 148, 150 };

	// Rules
	vector<OverviewRule> overviewRules;
};
