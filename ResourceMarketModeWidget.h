#pragma once

#include <QWidget>
#include <vector>
#include "DataStore.h"
#include "ResourceMarketLevelWidget.h"
#include <QGridLayout>

class ResourceMarketModeWidget : public QWidget {
public:
	ResourceMarketModeWidget();
	~ResourceMarketModeWidget();
	void Refresh();
private:
	std::vector<ResourceMarketLevelWidget*> resourceMarketLevelwidgets;
	QGridLayout *gridLayout;
};

