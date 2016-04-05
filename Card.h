#pragma once

#include <string>

class Card
{
private:
    std::string imagePath;
    bool ignoreFirst;
    std::string position;
public:
    Card();
    Card(std::string imagePath) : imagePath{ imagePath } {}
    virtual ~Card();
    std::string GetImagePath() const { return imagePath; }
    std::string GetPosition() { return position; }
    void SetPosition(std::string position) { this->position = position; }
    bool GetIgnoreFirst() { return ignoreFirst; }
    void SetIgnoreFirst(bool ignoreFirst) { this->ignoreFirst = ignoreFirst; }
    void SetImagePath(std::string imagePath) { this->imagePath = imagePath; }
};