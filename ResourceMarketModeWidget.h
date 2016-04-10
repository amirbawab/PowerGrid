#pragma once

#include <QWidget>
#include <vector>
#include "ResourceMarketLevelWidget.h"

class ResourceMarketModeWidget : public QWidget {
public:
    ResourceMarketModeWidget();
    ~ResourceMarketModeWidget();
    void Refresh();
    void ActivateResource(Resource resource, int amount);
private:
    std::vector<ResourceMarketLevelWidget*> resourceMarketLevelwidgets;
    QGridLayout *gridLayout;
};

