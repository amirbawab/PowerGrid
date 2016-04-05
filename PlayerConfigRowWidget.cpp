#include "PlayerConfigRowWidget.h"

PlayerConfigRowWidget::PlayerConfigRowWidget(string name) {
		
	// Init components
	playerName = new QLineEdit(QString::fromStdString(name));
	hBoxLayout = new QHBoxLayout();

	// Set id
	setObjectName("row_label_h4_bg");
	playerName->setObjectName("transparent_h5_bg");

	// Set layout
	setLayout(hBoxLayout);

	// Create house
	houseButton = new QPushButton();
	houseButton->setObjectName("coloredHouse");
	connect(houseButton, SIGNAL(clicked()), this, SLOT(OnHouseClick()));

	// Add component
	hBoxLayout->addWidget(playerName, 0, Qt::AlignLeft);
	hBoxLayout->addWidget(houseButton, 0, Qt::AlignRight);
}

PlayerConfigRowWidget::~PlayerConfigRowWidget() {
	delete playerName;
}

// Set and refresh house color
void PlayerConfigRowWidget::SetHouseColorIndex(int houseColorIndex) {
	this->houseColorIndex = houseColorIndex;
	houseButton->setIcon(QIcon(houseColors[houseColorIndex]->getImage().c_str()));
	houseButton->setIconSize(QSize(30, 30));
}

void PlayerConfigRowWidget::UpdatePlayer() {
	player->SetName(GetPlayerName());
	player->SetColor(GetHouseColor());
}


/**************
* SLOTS
**************/

// Next color
void PlayerConfigRowWidget::OnHouseClick() {
	SetHouseColorIndex((houseColorIndex + 1) % houseColors.size());
}

/// This method is required when Q_OBJECT is added
/// Without this method, the CSS will not be applied
void PlayerConfigRowWidget::paintEvent(QPaintEvent *pe) {
	QStyleOption o;
	o.initFrom(this);
	QPainter p(this);
	style()->drawPrimitive(
		QStyle::PE_Widget, &o, &p, this);
};