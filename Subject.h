#pragma once

#include "Observer.h"
#include <vector>

class Subject
{
private:
    vector<Observer*> observers;

public:
    void Attach(Observer *observer) {
        if (find(observers.begin(), observers.end(), observer) == observers.end())
            observers.push_back(observer);
    }

    void Detach(Observer *observer) {
        auto it = find(observers.begin(), observers.end(), observer);
        if (it != observers.end()) {
            observers.erase(it);	
        }
    }

    void Notify() {
        for (auto observer : observers)
            observer->Update();
    }

};
