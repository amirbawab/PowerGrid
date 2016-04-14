#include "MapWidget.h"
#include <QPainter>
#include "MapDesignerWindow.h"

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
    mapLabel = new QLabel();
    
    // Add maps
    mapLabels.push_back("USA");
    mapLabels.push_back("Canada");
    mapLabels.push_back("Germany");

    mapLabel->setText(mapLabels[currentMap].c_str());
    mapLabel->setObjectName("map_label");

    // Add id
    titleLabel->setObjectName("global_h2");
    nextMapBtn->setObjectName("right_arrow");
    previousMapBtn->setObjectName("left_arrow");

    // Connect buttons
    connect(nextMapBtn, &QPushButton::clicked, [=]() {
        currentMap = (++currentMap) % mapLabels.size();
        qDebug(("Next map: " + std::to_string(currentMap)).c_str());
        mapLabel->setText(mapLabels[currentMap].c_str());
    });

    // Connect buttons
    connect(previousMapBtn, &QPushButton::clicked, [=]() {
        currentMap == 0 ? currentMap = mapLabels.size() - 1 : --currentMap;
        qDebug(("Previous map: " + std::to_string(currentMap)).c_str());
        mapLabel->setText(mapLabels[currentMap].c_str());
    });

    // Set layout
    setLayout(gridLayout);

    // Add components
    gridLayout->addWidget(titleLabel, 0, 0, 1, 3, Qt::AlignCenter);
    gridLayout->addWidget(previousMapBtn, 1, 0, Qt::AlignRight);
    gridLayout->addWidget(mapLabel, 1, 1, Qt::AlignCenter);
    gridLayout->addWidget(nextMapBtn, 1, 2, Qt::AlignLeft);

    // Add map components
    auto vLayout = new QVBoxLayout();
    auto hLayout = new QHBoxLayout();
    createMapButton = new QPushButton("Create New Map ...");
    loadMapButton = new QPushButton("Load Custom Map ...");
    useCustomMap = new QCheckBox();
    useCustomMapLabel = new QLabel("Use Custom Map");
    selectedFileLabel = new QLabel("<b>Selected File:</b>");

    createMapButton->setObjectName("mapButton");
    loadMapButton->setObjectName("mapButton");
    useCustomMap->setObjectName("mapButton");
    useCustomMapLabel->setObjectName("mapButton");
    selectedFileLabel->setObjectName("mapFileLabel");
    selectedFileLabel->setWordWrap(true);
    selectedFileLabel->setVisible(false);
    loadMapButton->setEnabled(false);
    loadMapButton->setVisible(false);

    vLayout->addSpacerItem(new QSpacerItem(0, 20));
    vLayout->addWidget(createMapButton);
    vLayout->addSpacerItem(new QSpacerItem(0, 100));

    useCustomMap->setFixedSize(50, 50);
    hLayout->addWidget(useCustomMap);
    hLayout->addWidget(useCustomMapLabel);
    vLayout->addLayout(hLayout);

    vLayout->addSpacerItem(new QSpacerItem(0, 20));
    vLayout->addWidget(loadMapButton);
    vLayout->addSpacerItem(new QSpacerItem(0, 20));
    vLayout->addWidget(selectedFileLabel);

    gridLayout->addLayout(vLayout, 2, 0, 1, 3);

    // Connect signals/slots
    connect(createMapButton, &QPushButton::clicked, []()
    {
        auto mapDesigner = new MapDesignerWindow();
        mapDesigner->setWindowModality(Qt::ApplicationModal);
        mapDesigner->show();
    });

    connect(useCustomMap, &QCheckBox::clicked, [=](bool checked)
    {
        loadMapButton->setEnabled(checked);
        loadMapButton->setVisible(checked);
        selectedFileLabel->setVisible(checked);
        useCustomMapSelected = checked;
    });

    connect(loadMapButton, &QPushButton::clicked, [=]()
    {
        customMapFileName = QFileDialog::getOpenFileName(
            this, "Map File Location", "Resources/config/map", "XML Files (*.xml)");
        selectedFileLabel->setText("<b>Selected File:</b> " + (customMapFileName.isEmpty() ?
                                   "<font color='red'><b>NONE</b></font>" :
                                   "<font color='black'>" + customMapFileName + "</font>"));
    });
}

MapWidget::SelectMapWidget::~SelectMapWidget() {
    delete nextMapBtn;
    delete previousMapBtn;
    delete titleLabel;
    delete gridLayout;
    delete mapLabel;
    delete useCustomMap;
    delete useCustomMapLabel;
    delete loadMapButton;
    delete createMapButton;
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