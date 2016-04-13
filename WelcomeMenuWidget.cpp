#include "WelcomeMenuWidget.h"

#include <QStyleOption>
#include <QPainter>

WelcomeMenuWidget::WelcomeMenuWidget() {

	// Init components
	vBoxLayout = new QVBoxLayout(this);
	newGameBtn = new QPushButton("New Game");
	loadGameBtn = new QPushButton("Load Game");
	exitBtn = new QPushButton("Exit");

	// Set layout
	setLayout(vBoxLayout);

	// Set unique id
	setObjectName("welcomeMenuWidget");

	// Configure widget
	setContentsMargins(-1, 40, -1, 40);

	// Add id for buttons
	newGameBtn->setObjectName("templateLogoGenericButton");
	loadGameBtn->setObjectName("templateLogoGenericButton");
	exitBtn->setObjectName("templateLogoGenericButton");

	// Add components
	vBoxLayout->addWidget(newGameBtn, 1, Qt::AlignCenter);
	vBoxLayout->addWidget(loadGameBtn, 1, Qt::AlignCenter);
	vBoxLayout->addWidget(exitBtn, 1, Qt::AlignCenter);
}

WelcomeMenuWidget::~WelcomeMenuWidget() {
	delete vBoxLayout;
	delete newGameBtn;
	delete loadGameBtn;
	delete exitBtn;
}

void WelcomeMenuWidget::setOnNewGame(const QObject *receiver, const char* slot) {
	qDebug("New Game registered!");
	connect(newGameBtn, SIGNAL(clicked()), receiver, slot);
}

void WelcomeMenuWidget::setOnLoadGame(const QObject *receiver, const char* slot) {
	qDebug("Load Game registered!");
	connect(loadGameBtn, SIGNAL(clicked()), receiver, slot);
}

void WelcomeMenuWidget::setOnExitGame(const QObject *receiver, const char* slot) {
	qDebug("Exit Game registered!");
	connect(exitBtn, SIGNAL(clicked()), receiver, slot);
}

/// This method is required when Q_OBJECT is added
/// Without this method, the CSS will not be applied
void WelcomeMenuWidget::paintEvent(QPaintEvent *pe) {
	QStyleOption o;
	o.initFrom(this);
	QPainter p(this);
	style()->drawPrimitive(
	QStyle::PE_Widget, &o, &p, this);
};