#include "TemplateLogoWidget.h"

TemplateLogoWidget::TemplateLogoWidget() {

	// Init components
	gridLayout = new QGridLayout(this);

	// Set layout
	setLayout(gridLayout);

	// Set ID
	setObjectName("templateLogoWidget");

	// Create logo
	QLabel *logoLabel = new QLabel(this);
	logoLabel->setObjectName("templateLogo");
	logoLabel->setPixmap(QPixmap(":/PowerGrid/Resources/logo.png"));

	// Configure widget
	setContentsMargins(60, 60, 60, 60);

	// Add components
	gridLayout->addWidget(logoLabel, 0, 0);

	// Resize grid
	gridLayout->setRowStretch(1, 1);
}


TemplateLogoWidget::~TemplateLogoWidget() {
	delete gridLayout;
}
