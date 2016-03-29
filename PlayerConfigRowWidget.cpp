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

	// Create logo
	houseLabel = new QLabel();
	houseLabel->setContentsMargins(-1,-1,10,-1);

	// Add component
	hBoxLayout->addWidget(playerName, 0, Qt::AlignLeft);
	hBoxLayout->addWidget(houseLabel, 0, Qt::AlignRight);
}

PlayerConfigRowWidget::~PlayerConfigRowWidget() {
	delete playerName;
}
	
void PlayerConfigRowWidget::SetHouseColor(HouseColor *houseColor) {
	this->houseColor = houseColor;
	houseLabel->setPixmap(QPixmap(houseColor->getImage().c_str()));
}