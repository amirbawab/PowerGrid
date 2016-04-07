#pragma once

#include "Observer.h"
#include <memory>
#include <vector>

class Subject
{
private:
    std::vector<Observer*> observers;

public:
    void Attach(Observer *observer) {
        if (std::find(observers.begin(), observers.end(), observer) == observers.end())
            observers.push_back(observer);
    }

    void Detach(Observer *observer) {
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
