#pragma once

#include <memory>
#include <string>
#include <vector>

class OverviewStep;

class Overview
{
private:

    std::vector<std::shared_ptr<OverviewStep>> steps;

public:
    class OverviewStep
    {
    public:
        int step;
        std::string title;
        std::vector<std::string> info;
        OverviewStep(int step, std::string title) : step { step}, title {title} {}
        void AddInfo(std::string infoString) { info.push_back(infoString); }
    };

    Overview();
    ~Overview();
};
