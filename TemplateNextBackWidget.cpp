#include "TemplateNextBackWidget.h"

TemplateNextBackWidget::TemplateNextBackWidget() {

	// Init components
	gridLayout = new QGridLayout(this);
	buttonsWidget = new ButtonsWidget();
	centerWidget = new QWidget;
	centerLayout = new QGridLayout;

	// Set ID
	setObjectName("templateLogoWidget");

	// Configure widget
	setContentsMargins(90, 60, 90, 60);

	// Set layout
	setLayout(gridLayout);
	centerWidget->setLayout(centerLayout);

	// Add component to template
	gridLayout->addWidget(centerWidget, 0, 0, 0);
	gridLayout->addWidget(buttonsWidget, 1, 0, 0);

	// Adjust stretch
	gridLayout->setRowStretch(0, 1);
}

TemplateNextBackWidget::~TemplateNextBackWidget() {
	delete gridLayout;
	delete centerLayout;
	delete centerWidget;
	delete buttonsWidget;
}

void TemplateNextBackWidget::setOnNext(const QObject *receiver, const char* slot) {
	qDebug("Next on Map page registered!");
	connect(buttonsWidget->nextBtn, SIGNAL(clicked()), receiver, slot);
}

void TemplateNextBackWidget::setOnBack(const QObject *receiver, const char* slot) {
	qDebug("Back on Map page registered!");
	connect(buttonsWidget->backBtn, SIGNAL(clicked()), receiver, slot);
}

TemplateNextBackWidget::ButtonsWidget::ButtonsWidget() {
	
	// Init components
	hBoxLayout = new QHBoxLayout(this);
	nextBtn = new QPushButton("Next");
	backBtn = new QPushButton("Back");
	
	// Set layout
	setLayout(hBoxLayout);

	// Set ID
	nextBtn->setObjectName("templateGenericButton");
	backBtn->setObjectName("templateGenericButton");

	// Add components
	hBoxLayout->addWidget(backBtn, 0, Qt::AlignLeft);
	hBoxLayout->addWidget(nextBtn, 0, Qt::AlignRight);
}

TemplateNextBackWidget::ButtonsWidget::~ButtonsWidget() {
	delete nextBtn;
	delete backBtn;
	delete hBoxLayout;
}