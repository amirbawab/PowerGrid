#include "ResourceMarketLevelWidget.h"

ResourceMarketLevelWidget::ResourceMarketLevelWidget() {

	// Init components
	gridLayout = new QGridLayout();
	costLabel = new QLabel("10 Elektro");
	costLabel->setAlignment(Qt::AlignCenter);

	// Set layout
	setLayout(gridLayout);
	
	// Configure layout
	gridLayout->setSpacing(0);

	// Icon dimensions
	iconWidth = 50;
	iconHeight = 50;
}

void ResourceMarketLevelWidget::createUraniumLevelMarket() {
	
	// Add cost
	gridLayout->addWidget(costLabel, 0, 0, Qt::AlignCenter);
	
	QPushButton *button = new QPushButton();
	button->setObjectName("resourceButton");
	button->setIconSize(QSize(iconWidth, iconHeight));
	button->setIcon(QIcon(":/PowerGrid/Resources/resources/uranium.png"));
	gridLayout->addWidget(button, 1, 0, Qt::AlignCenter);
}

void ResourceMarketLevelWidget::createFullLevelMarket() {
	
	// Add cost
	gridLayout->addWidget(costLabel, 0, 0, 1, 3, Qt::AlignCenter);

	// Add coil
	for (int i = 0; i < 3; i++) {
		QPushButton *button = new QPushButton();
		button->setObjectName("resourceButton");
		button->setIconSize(QSize(iconWidth, iconHeight));
		button->setIcon(QIcon(":/PowerGrid/Resources/resources/coal.png"));
		coalButtons.push_back(button);
	}

	for (int i = 0; i < coalButtons.size(); i++)
		gridLayout->addWidget(coalButtons[i], 1, i, Qt::AlignCenter);


	// Add oil
	for (int i = 0; i < 3; i++) {
		QPushButton *button = new QPushButton();
		button->setObjectName("resourceButton");
		button->setIconSize(QSize(iconWidth, iconHeight));
		button->setIcon(QIcon(":/PowerGrid/Resources/resources/oil.png"));
		oilButtons.push_back(button);
	}

	for (int i = 0; i < oilButtons.size(); i++)
		gridLayout->addWidget(oilButtons[i], 2, i, Qt::AlignCenter);

	// Add garbage
	for (int i = 0; i < 3; i++) {
		QPushButton *button = new QPushButton();
		button->setObjectName("resourceButton");
		button->setIconSize(QSize(iconWidth, iconHeight));
		button->setIcon(QIcon(":/PowerGrid/Resources/resources/garbage.png"));
		garbageButtons.push_back(button);
	}

	for (int i = 0; i < garbageButtons.size(); i++)
		gridLayout->addWidget(garbageButtons[i], 3, i, Qt::AlignCenter);


	// Add uranium
	for (int i = 0; i < 1; i++) {
		QPushButton *button = new QPushButton();
		button->setObjectName("resourceButton");
		button->setIconSize(QSize(iconWidth, iconHeight));
		button->setIcon(QIcon(":/PowerGrid/Resources/resources/uranium.png"));
		uraniumButtons.push_back(button);
	}

	for (int i = 0; i < uraniumButtons.size(); i++)
		gridLayout->addWidget(uraniumButtons[i], 4, 1, Qt::AlignCenter);
}

ResourceMarketLevelWidget::~ResourceMarketLevelWidget() {
	for (int i = 0; i < coalButtons.size(); i++)
		delete coalButtons[i];

	for (int i = 0; i < oilButtons.size(); i++)
		delete oilButtons[i];

	for (int i = 0; i < garbageButtons.size(); i++)
		delete garbageButtons[i];

	for (int i = 0; i < uraniumButtons.size(); i++)
		delete uraniumButtons[i];
}
