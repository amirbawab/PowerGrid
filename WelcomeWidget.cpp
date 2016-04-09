#include "WelcomeWidget.h"

WelcomeWidget::WelcomeWidget() {
	
	// Init components
	welcomeMenuWidget = new WelcomeMenuWidget();

	// Add components
	gridLayout->addWidget(welcomeMenuWidget, 1, 0, Qt::AlignCenter);
}

WelcomeWidget::~WelcomeWidget() {
	delete welcomeMenuWidget;
}

void WelcomeWidget::setOnNewGame(const QObject *receiver, const char* slot) const {
	welcomeMenuWidget->setOnNewGame(receiver, slot);
}

void WelcomeWidget::setOnLoadGame(const QObject *receiver, const char* slot) const {
	welcomeMenuWidget->setOnLoadGame(receiver, slot);
}

void WelcomeWidget::setOnExitGame(const QObject *receiver, const char* slot) const {
	welcomeMenuWidget->setOnExitGame(receiver, slot);
}
