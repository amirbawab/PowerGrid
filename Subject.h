#pragma once

#include "Observer.h"
#include <memory>
#include <vector>

class Subject
{
private:
	std::vector<std::shared_ptr<Observer>> observers;

public:
	void Attach(std::shared_ptr<Observer> observer) {
		if (std::find(observers.begin(), observers.end(), observer) == observers.end())
			observers.push_back(observer);
	}

	void Detach(std::shared_ptr<Observer> observer) {
		auto it = std::find(observers.begin(), observers.end(), observer);
		if (it != observers.end()) {
			observers.erase(it);	
		}
	}

	void Notify() {
		for (auto observer : observers)
			observer->Update();
	}

};
