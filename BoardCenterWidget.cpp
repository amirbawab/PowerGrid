#include "BoardCenterWidget.h"

BoardCenterWidget::BoardCenterWidget() {
	
	// Init components
	gridLayout = new QGridLayout();
	centerStackedWidget = new QStackedWidget();
	modeWidget = new QWidget();
	vBoxLayout = new QVBoxLayout();
	powerPlantsModeButton = new QPushButton();
	mapModeButton = new QPushButton();
	resourceModeButton = new QPushButton();
	overviewModeButton = new QPushButton();
	powerPlantModeWidget = new PowerPlantModeWidget();

	// Configure components
	modeWidget->setObjectName("modeWidget");
	powerPlantsModeButton->setObjectName("boardModeButton");
	mapModeButton->setObjectName("boardModeButton");
	resourceModeButton->setObjectName("boardModeButton");
	overviewModeButton->setObjectName("boardModeButton");

	// Set layout
	setLayout(gridLayout);
	modeWidget->setLayout(vBoxLayout);

	// Add to stack
	powerPlantModeWidgetIndex = centerStackedWidget->addWidget(powerPlantModeWidget);

	// Default active
	centerStackedWidget->setCurrentIndex(powerPlantModeWidgetIndex);

	// Add components
	vBoxLayout->addWidget(powerPlantsModeButton, 0);
	vBoxLayout->addWidget(mapModeButton, 0);
	vBoxLayout->addWidget(resourceModeButton, 0);
	vBoxLayout->addWidget(overviewModeButton, 0);
	gridLayout->addWidget(centerStackedWidget, 0, 0);
	gridLayout->addWidget(modeWidget, 0, 1, Qt::AlignRight);
}


BoardCenterWidget::~BoardCenterWidget() {
	delete powerPlantsModeButton;
	delete mapModeButton;
	delete resourceModeButton;
	delete overviewModeButton;
	delete powerPlantModeWidget;
	delete vBoxLayout;
	delete modeWidget;
	delete centerStackedWidget;
	delete gridLayout;
}
