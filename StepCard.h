#pragma once

#include "Card.h"

class StepCard : public Card {
private:
	int step = 0;

public:
	StepCard() {}
	~StepCard() {}
	explicit StepCard(int step) : step{ step } {}
	int GetStep() const { return step; }
};
