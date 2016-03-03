#include "HouseColor.h"

HouseColor::HouseColor(string name, string image) {
	this->name = name;
	this->image = image;
}

HouseColor::~HouseColor() {
	// Nothing
}

/// Get image
string HouseColor::getImage() {
	return this->image;
}

/// Get name
string HouseColor::getName() {
	return this->name;
}
