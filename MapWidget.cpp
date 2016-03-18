#include "MapWidget.h"

MapWidget::MapWidget() {
	
	// Init components
	selectMapWidget = new SelectMapWidget();
	selectNumberOfPlayerWidget = new SelectNumberOfPlayerWidget();

	// Add component to center
	centerLayout->addWidget(selectMapWidget, 0, 0, Qt::AlignCenter);
	centerLayout->addWidget(selectNumberOfPlayerWidget, 0, 1, Qt::AlignCenter);

	// Connect
	connect(selectNumberOfPlayerWidget->plusBtn, SIGNAL(clicked()), this, SLOT(increment()));
	connect(selectNumberOfPlayerWidget->minusBtn, SIGNAL(clicked()), this, SLOT(decrement()));
}

MapWidget::~MapWidget() {
	delete selectNumberOfPlayerWidget;
	delete selectMapWidget;
}

MapWidget::SelectMapWidget::SelectMapWidget() {
	
	// Init components
	gridLayout = new QGridLayout(this);
	nextMapBtn = new QPushButton();
	previousMapBtn = new QPushButton();
	titleLabel = new QLabel("Select map");

	// Add USA map
	// This has to be loaded from files later
	usaMapLabel = new QLabel();
	usaMapLabel->setPixmap(QPixmap(":/PowerGrid/Resources/map/usa_map_prev.jpg"));

	// Add id
	titleLabel->setObjectName("global_h2");
	nextMapBtn->setObjectName("right_arrow");
	previousMapBtn->setObjectName("left_arrow");

	// Set layout
	setLayout(gridLayout);

	// Add components
	gridLayout->addWidget(titleLabel, 0, 0, 1, 3, Qt::AlignCenter);
	gridLayout->addWidget(previousMapBtn, 1, 0, Qt::AlignRight);
	gridLayout->addWidget(usaMapLabel, 1, 1, Qt::AlignCenter);
	gridLayout->addWidget(nextMapBtn, 1, 2, Qt::AlignLeft);
}

MapWidget::SelectMapWidget::~SelectMapWidget() {
	delete nextMapBtn;
	delete previousMapBtn;
	delete titleLabel;
	delete gridLayout;
	delete usaMapLabel;
}

MapWidget::SelectNumberOfPlayerWidget::SelectNumberOfPlayerWidget() {

	// Init components
	gridLayout = new QGridLayout(this);
	numberOfPlayersLabel = new QLabel("2");
	plusBtn = new QPushButton("+");
	minusBtn = new QPushButton("-");
	titleLabel = new QLabel("Number of players");

	// Set layout
	setLayout(gridLayout);

	// Add id
	titleLabel->setObjectName("global_h2");
	plusBtn->setObjectName("global_h2_button");
	minusBtn->setObjectName("global_h2_button");
	numberOfPlayersLabel->setObjectName("num_label_h4_bg");

	// Add components
	gridLayout->addWidget(titleLabel, 0, 0, 1, 3, Qt::AlignCenter);
	gridLayout->addWidget(minusBtn, 1, 0, Qt::AlignRight);
	gridLayout->addWidget(numberOfPlayersLabel, 1, 1, Qt::AlignCenter);
	gridLayout->addWidget(plusBtn, 1, 2, Qt::AlignLeft);
}

MapWidget::SelectNumberOfPlayerWidget::~SelectNumberOfPlayerWidget() {
	delete numberOfPlayersLabel;
	delete plusBtn;
	delete minusBtn;
	delete gridLayout;
}

/// SLOTS ///

void MapWidget::increment() {
	// Get value as int
	int val = std::stoi(selectNumberOfPlayerWidget->numberOfPlayersLabel->text().toStdString());
	
	// MAX 6
	if (val < 6) val++;

	// Update
	selectNumberOfPlayerWidget->numberOfPlayersLabel->setText(QString::fromStdString(std::to_string(val)));
}

void MapWidget::decrement() {
	// Get value as int
	int val = std::stoi(selectNumberOfPlayerWidget->numberOfPlayersLabel->text().toStdString());

	// MIN 2
	if (val > 2) val--;

	// Update
	selectNumberOfPlayerWidget->numberOfPlayersLabel->setText(QString::fromStdString(std::to_string(val)));
}

/// This method is required when Q_OBJECT is added
/// Without this method, the CSS will not be applied
void MapWidget::paintEvent(QPaintEvent *pe) {
	QStyleOption o;
	o.initFrom(this);
	QPainter p(this);
	style()->drawPrimitive(
		QStyle::PE_Widget, &o, &p, this);
};