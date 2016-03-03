#pragma once

#include <string>

using std::string;

class HouseColor {
private:
	string name;
	string image;
public:
	HouseColor();
	HouseColor(string, string);
	string getName();
	string getImage();
	~HouseColor();
};

