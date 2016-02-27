#pragma once

#include <string>
#include <Config.h>

class Card
{
private:
    std::string imagePath;

    std::string GetImagePath() const { return imagePath; }

public:
    Card();
    Card(std::string imagePath) : imagePath{ imagePath } {}
    ~Card();
};

class StandardCard : public Card
{
private:
    int price = 0;
    int numberOfResources = 0;
    int power = 0;
    std::vector<Resource> resources;

public:
    StandardCard() {}
    StandardCard(int price, int numberOfResources, int power) :
        price{ price }, numberOfResources{ numberOfResources }, power{ power } { }
    
    int GetPrice() const { return price; }
    int GetNumberOfResources() const { return numberOfResources; }
    int GetPower() const { return power; }
    std::vector<Resource> GetResources() const { return resources; }
};

class StepCard : public Card
{
private:
    int step = 0;

public:
    StepCard() {}
    StepCard(int step) : step{ step } {}
    int GetStep() const { return step; }
};
