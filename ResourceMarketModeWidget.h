#pragma once

#include <QWidget>
#include <vector>
#include "ResourceMarketLevelWidget.h"
#include <QGridLayout>

class ResourceMarketModeWidget : public QWidget {
public:
	ResourceMarketModeWidget();
	~ResourceMarketModeWidget();
private:
	std::vector<ResourceMarketLevelWidget*> resourceMarketLevelwidgets;
	QGridLayout *gridLayout;
};

