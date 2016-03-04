#include "Overview.h"

Overview::Overview()
{
}


Overview::~Overview()
{
}

/// Add overview step and return it
std::shared_ptr<Overview::OverviewStep> Overview::AddStep(int step, std::string title) {
    auto overviewStep = std::make_shared<OverviewStep>(step, title);
	steps.push_back(overviewStep);
	return overviewStep;
}