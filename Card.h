#pragma once

#include <string>

class Card
{
private:
    std::string imagePath;
    bool ignoreFirst = false;
    std::string position;
public:
    Card() {}
    explicit Card(std::string imagePath) : imagePath{ imagePath } {}
    virtual ~Card() {}
    std::string GetImagePath() const { return imagePath; }
    std::string GetPosition() const { return position; }
    void SetPosition(std::string position) { this->position = position; }
    bool GetIgnoreFirst() const { return ignoreFirst; }
    void SetIgnoreFirst(bool ignoreFirst) { this->ignoreFirst = ignoreFirst; }
    void SetImagePath(std::string imagePath) { this->imagePath = imagePath; }
};