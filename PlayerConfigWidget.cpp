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

	// TODO Read input from previous page
	rows.push_back(new PlayerConfigRowWidget());
	rows.push_back(new PlayerConfigRowWidget());
	rows.push_back(new PlayerConfigRowWidget());
	rows.push_back(new PlayerConfigRowWidget());
	rows.push_back(new PlayerConfigRowWidget());
	rows.push_back(new PlayerConfigRowWidget());

	// Set id
	titleLabel->setObjectName("global_h2");

	// Set layout
	rowsWidget->setLayout(vBoxLayout);

	// Add rows to the rows widget
	for (int i = 0; i < rows.size(); i++)
		vBoxLayout->addWidget(rows[i], i, Qt::AlignCenter);

	// Add components
	centerLayout->addWidget(titleLabel, 0, 0, Qt::AlignCenter);
	centerLayout->addWidget(rowsWidget, 1, 0, Qt::AlignCenter);
}

PlayerConfigWidget::~PlayerConfigWidget() {
	delete titleLabel;
	delete vBoxLayout;
	delete rowsWidget;
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