#include "PlayerConfigWidget.h"
#include "Game.h"
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
		PlayerConfigRowWidget *row = new PlayerConfigRowWidget("Player " + std::to_string(i + 1));
		row->SetHouseColors(Game::getInstance().GetHouseColor());
		row->SetPlayer(Game::getInstance().GetPlayers()[i]);
		row->SetHouseColorIndex(i % Game::getInstance().GetHouseColor().size());
		rows.push_back(row);
		vBoxLayout->addWidget(rows[i], i, Qt::AlignCenter);
	}
}

bool PlayerConfigWidget::HasError() {

	// Check if two players have the same color
	for (int i = 0; i < rows.size(); i++) {
		for (int j = i + 1; j < rows.size(); j++) {
			if (rows[i]->GetHouseColor() == rows[j]->GetHouseColor())
				return true;
		}
	}
	return false;
}

void PlayerConfigWidget::UpdatePlayersInfo() {
	for (int i = 0; i < rows.size(); i++) {
		rows[i]->UpdatePlayer();
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