#include "OverviewModeWidget.h"
#include <QLabel>

OverviewModeWidget::OverviewModeWidget() {
	
	// Init component
	scrollArea = new QScrollArea();
	gridLayout = new QGridLayout();

	// Set layout
	setLayout(gridLayout);

	// Get image
	QLabel *imageLabel = new QLabel;
	QImage image(":/PowerGrid/Resources/overview/all.png");
	imageLabel->setPixmap(QPixmap::fromImage(image));
	imageLabel->setStyleSheet("background-color:transparent;");

	// Display image
	scrollArea = new QScrollArea;
	scrollArea->setWidget(imageLabel);
	scrollArea->setObjectName("overviewScrollArea");
	scrollArea->setAlignment(Qt::AlignCenter);

	// Add component
	gridLayout->addWidget(scrollArea, 0, 0);
}

OverviewModeWidget::~OverviewModeWidget() {
	delete scrollArea;
	delete gridLayout;
}
