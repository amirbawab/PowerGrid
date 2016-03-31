#include "BoardBottomWidget.h"

// BoardBottomWidget class

BoardBottomWidget::BoardBottomWidget() {

	// Init components
	gridLayout = new QGridLayout();
	boardProfileWidget = new BoardProfileWidget();
	boardPlayerPowerPlantsWidget = new BoardPlayerPowerPlantsWidget();

	// Set layout
	setLayout(gridLayout);

	// Set id
	setObjectName("boardBottomWidget");

	// Add components
	gridLayout->addWidget(boardProfileWidget, 0, 0, Qt::AlignCenter);
	gridLayout->addWidget(boardPlayerPowerPlantsWidget, 0, 1, Qt::AlignCenter);
}

BoardBottomWidget::~BoardBottomWidget() {
	delete boardProfileWidget;
	delete gridLayout;
}

void BoardBottomWidget::Refresh() {
	boardProfileWidget->Refresh();
	boardPlayerPowerPlantsWidget->Refresh();
}

// BoardProfileWidget class

BoardProfileWidget::BoardProfileWidget() {

	// Init components
	gridLayout = new QGridLayout();
	playerNameLabel = new QLabel();
	playerPictureLabel = new QLabel();
	playerElektroLabel = new QLabel();
	
	// Set id
	playerNameLabel->setObjectName("board_bottom_subtitle");
	playerElektroLabel->setObjectName("board_bottom_subtitle_s");
	
	// Set layout
	setLayout(gridLayout);

	// Add components
	gridLayout->addWidget(playerPictureLabel, 0, 0, Qt::AlignBottom | Qt::AlignCenter);
	gridLayout->addWidget(playerNameLabel, 1, 0, Qt::AlignTop | Qt::AlignCenter);
	gridLayout->addWidget(playerElektroLabel, 2, 0, Qt::AlignTop | Qt::AlignCenter);
}

BoardProfileWidget::~BoardProfileWidget() {
	delete playerNameLabel;
	delete playerPictureLabel;
	delete gridLayout;
}

void BoardProfileWidget::Refresh() {
	playerNameLabel->setText(DataStore::getInstance().currentPlayer->GetName().c_str());
	playerPictureLabel->setPixmap(QPixmap(DataStore::getInstance().currentPlayer->GetColor()->getImage().c_str()));
	playerElektroLabel->setText((std::to_string(DataStore::getInstance().currentPlayer->GetElektro()) + " Elektro").c_str());
}

// BoardResourcePowerPlantWiget

BoardResourcePowerPlantWiget::BoardResourcePowerPlantWiget() {
	
	// Init components
	gridLayout = new QGridLayout();

	// Configure components
	gridLayout->setContentsMargins(20, 40, 20, 40);
	gridLayout->setSpacing(0);

	// Set layout
	setLayout(gridLayout);
}

BoardResourcePowerPlantWiget::~BoardResourcePowerPlantWiget() {
	delete gridLayout;
}

void BoardResourcePowerPlantWiget::Refresh() {
	
	// Clear old components
	for (int i = 0; i < resourceLables.size(); i++) {
		gridLayout->removeWidget(resourceLables[i]);
		delete resourceLables[i];
		resourceLables.erase(resourceLables.begin() + i);
	}

	// Get resources for the player
	int coalNum = powerPlantCard->GetPlacedResource(COAL);
	int oilNum = powerPlantCard->GetPlacedResource(OIL);
	int garbageNum = powerPlantCard->GetPlacedResource(GARBAGE);
	int uraniumNum = powerPlantCard->GetPlacedResource(URANIUM);

	// Row col
	int row = 0;
	int col = 0;
	
	// Add resources to widget
	for (int i = 0; i < coalNum; i++) {
		QLabel *label = new QLabel();
		label->setPixmap(QPixmap(":/PowerGrid/Resources/resources/coal.png"));
		resourceLables.push_back(label);
		gridLayout->addWidget(label, row, col, Qt::AlignCenter);

		row += col == 2 ? 1 : 0;
		col = (col + 1) % 3;
	}

	for (int i = 0; i < oilNum; i++) {
		QLabel *label = new QLabel();
		label->setPixmap(QPixmap(":/PowerGrid/Resources/resources/oil.png"));
		resourceLables.push_back(label);
		gridLayout->addWidget(label, row, col, Qt::AlignCenter);

		row += col == 2 ? 1 : 0;
		col = (col + 1) % 3;
	}

	for (int i = 0; i < garbageNum; i++) {
		QLabel *label = new QLabel();
		label->setPixmap(QPixmap(":/PowerGrid/Resources/resources/garbage.png"));
		resourceLables.push_back(label);
		gridLayout->addWidget(label, row, col, Qt::AlignCenter);

		row += col == 2 ? 1 : 0;
		col = (col + 1) % 3;
	}

	for (int i = 0; i < uraniumNum; i++) {
		QLabel *label = new QLabel();
		label->setPixmap(QPixmap(":/PowerGrid/Resources/resources/uranium.png"));
		resourceLables.push_back(label);
		gridLayout->addWidget(label, row, col, Qt::AlignCenter);

		row += col == 2 ? 1 : 0;
		col = (col + 1) % 3;
	}
}

void BoardResourcePowerPlantWiget::paintEvent(QPaintEvent *e) {
	QPainter painter(this);
	painter.drawPixmap(0, 0, QPixmap(powerPlantCard->GetImagePath().c_str()).scaled(size()));
	QWidget::paintEvent(e);
}

// BoardPlayerPowerPlantsWidget class

BoardPlayerPowerPlantsWidget::BoardPlayerPowerPlantsWidget() {
	
	// Init components
	gridLayout = new QGridLayout();

	// Set layout
	setLayout(gridLayout);
}

BoardPlayerPowerPlantsWidget::~BoardPlayerPowerPlantsWidget() {
	delete gridLayout;
}

void BoardPlayerPowerPlantsWidget::Refresh() {
	
	// Clear old components
	for (int i = 0; i < playerPowerPlantsWidgets.size(); i++) {
		gridLayout->removeWidget(playerPowerPlantsWidgets[i]);
		delete playerPowerPlantsWidgets[i];
		playerPowerPlantsWidgets.erase(playerPowerPlantsWidgets.begin() + i);
	}

	// Get power plants
	std::vector<std::shared_ptr<PowerPlantCard>> cards = DataStore::getInstance().currentPlayer->GetPowerPlants();

	// Add power plants
	for (int i = 0; i < cards.size(); i++) {
		BoardResourcePowerPlantWiget *label = new BoardResourcePowerPlantWiget();
		label->setMinimumSize(200, 200);
		label->SetPowerPlantCard(cards[i]);
		label->Refresh();
		playerPowerPlantsWidgets.push_back(label);
		gridLayout->addWidget(label, 0, i, Qt::AlignCenter);
		label->repaint();
	}
}