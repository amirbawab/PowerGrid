#include "MapDesignerWindow.h"
#include "MapDesignerGraphicsView.h"
#include <iostream>

using std::cout;
using std::endl;
using std::vector;

MapDesignerWindow::MapDesignerWindow()
{
    setFixedSize(1300, 800);

    centralWidget = new QWidget(this);
    layout = new QGridLayout(centralWidget);
    graphicsView = new MapDesignerGraphicsView();

    addCityButton = new QPushButton("Add City");
    changeRegionColorButton = new QPushButton("Change Region Color ...");

    addConnectionButton = new QPushButton("Add Connection");

    layout->addWidget(graphicsView, 0, 0);

    auto vLayout = new QVBoxLayout();
    auto hLayout = new QHBoxLayout();
    hLayout->addWidget(addCityButton);
    hLayout->addWidget(changeRegionColorButton);
    auto regionColorLabel = new QLabel("Current Region Color:");
    regionColorLabel->setAlignment(Qt::AlignCenter);
    hLayout->addWidget(regionColorLabel);
    regionColor = new QLabel();
    regionColor->setFixedWidth(50);

    regionColor->setAutoFillBackground(true);
    hLayout->addWidget(regionColor);

    vLayout->addLayout(hLayout);
    vLayout->addWidget(addConnectionButton);

    layout->addLayout(vLayout, 1, 0);

    setCentralWidget(centralWidget);

    statusBar = new QStatusBar(centralWidget);
    setStatusBar(statusBar);

    connect(addCityButton          , SIGNAL(clicked()), graphicsView, SLOT(OnAddCity()));
    connect(addConnectionButton    , SIGNAL(clicked()), graphicsView, SLOT(OnAddConnection()));
    connect(graphicsView           , SIGNAL(DisplayMessage(QString)), this, SLOT(OnDisplayMessage(QString)));
    connect(graphicsView           , SIGNAL(ClearMessage()), this, SLOT(OnClearMessage()));
    connect(changeRegionColorButton, SIGNAL(clicked()), this, SLOT(OnChangeRegionColor()));
}

MapDesignerWindow::~MapDesignerWindow()
{
}

void MapDesignerWindow::OnDisplayMessage(QString message) const
{
    statusBar->showMessage(message);
}

void MapDesignerWindow::OnClearMessage() const
{
    statusBar->clearMessage();
}

void MapDesignerWindow::OnChangeRegionColor() const
{
    auto selectedRegionColor = QColorDialog::getColor();

    if (!selectedRegionColor.isValid())
        return;

    (dynamic_cast<MapDesignerGraphicsView*>(graphicsView))->SetRegionColor(selectedRegionColor);

    auto colorRegionPalette(palette());
    colorRegionPalette.setColor(QPalette::Background, selectedRegionColor);
    regionColor->setPalette(colorRegionPalette);
}
