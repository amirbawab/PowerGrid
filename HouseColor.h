#pragma once

#include <string>

using std::string;

class HouseColor {
private:
	string name;
	string image;
public:
	HouseColor() {};
	HouseColor(string name, string image);
	string getName() const { return name; };
	string getImage() const { return image; };
	~HouseColor();
};

