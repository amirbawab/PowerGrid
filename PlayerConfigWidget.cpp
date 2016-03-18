#include "PlayerConfigWidget.h"
#include <QStyleOption>
#include <QPainter>

PlayerConfigWidget::PlayerConfigWidget() {

	// Init components
	rowsWidget = new QWidget;
	vBoxLayout = new QVBoxLayout;
	titleLabel = new QLabel("Players configurations");

	// Set rows spacing
	vBoxLayout->setSpacing(10);

	// Set id
	titleLabel->setObjectName("global_h2");

	// Set layout
	rowsWidget->setLayout(vBoxLayout);

	// Add components
	centerLayout->addWidget(titleLabel, 0, 0, Qt::AlignCenter);
	centerLayout->addWidget(rowsWidget, 1, 0, Qt::AlignCenter);
}

PlayerConfigWidget::~PlayerConfigWidget() {
	delete titleLabel;
	delete vBoxLayout;
	delete rowsWidget;
}

void PlayerConfigWidget::SetNumberOfPlayers(int num) {

	// Clear old components
	for (int i = rows.size()-1; i >= 0; i--) {
		vBoxLayout->removeWidget(rows[i]);
		delete rows[i];
		rows.erase(rows.begin() + i);
	}
		
	// Add rows to the rows widget
	for (int i = 0; i < num; i++) {
		rows.push_back(new PlayerConfigRowWidget("Player " + std::to_string(i + 1)));
		vBoxLayout->addWidget(rows[i], i, Qt::AlignCenter);
	}
}

/// This method is required when Q_OBJECT is added
/// Without this method, the CSS will not be applied
void PlayerConfigWidget::paintEvent(QPaintEvent *pe) {
	QStyleOption o;
	o.initFrom(this);
	QPainter p(this);
	style()->drawPrimitive(
		QStyle::PE_Widget, &o, &p, this);
};