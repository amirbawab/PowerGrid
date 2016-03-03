#pragma once

#include <string>

class Card
{
private:
    std::string imagePath;
public:
    Card();
    Card(std::string imagePath) : imagePath{ imagePath } {}
    ~Card();
	std::string GetImagePath() const { return imagePath; }
};