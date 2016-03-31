#pragma once

#include <QWidget>
#include <QGridLayout>
#include <vector>
#include <QPushButton>
#include <QLabel>
#include "ResourceMarketLevel.h"
#include <memory>

class ResourceMarketLevelWidget : public QWidget {
public:
	ResourceMarketLevelWidget();
	~ResourceMarketLevelWidget();
	void RefreshFullLevelMarket();
	void RefreshUraniumLevelMarket();
	int iconWidth;
	int iconHeight;
	void Refresh();
	void SetResourceMarketLevel(std::shared_ptr<ResourceMarketLevel> level) { this->level = level; }
private:
	QGridLayout *gridLayout;
	QLabel *costLabel;
	std::vector<QPushButton*> coalButtons;
	std::vector<QPushButton*> oilButtons;
	std::vector<QPushButton*> garbageButtons;
	std::vector<QPushButton*> uraniumButtons;
	std::shared_ptr<ResourceMarketLevel> level;
};

