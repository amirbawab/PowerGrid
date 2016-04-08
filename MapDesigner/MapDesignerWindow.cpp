#include "MapDesignerWindow.h"
#include "MapDesignerGraphicsView.h"
#include <iostream>

using std::cout;
using std::endl;
using std::vector;

MapDesignerWindow::MapDesignerWindow()
{
    resize(QDesktopWidget().availableGeometry(this).size() * 0.9); // 90% of width
    // Center on screen
    setGeometry(QStyle::alignedRect(Qt::LeftToRight, Qt::AlignCenter,
                                    this->size(), QApplication::desktop()->availableGeometry()));

    centralWidget = new QWidget(this);
    layout = new QGridLayout(centralWidget);
    graphicsView = new MapDesignerGraphicsView();

    addCityButton = new QPushButton("Add City");
    deleteCityButton = new QPushButton("Delete City");
    changeRegionColorButton = new QPushButton("Change Region Color ...");

    addConnectionButton = new QPushButton("Add Connection");
    exportXML = new QPushButton("Export to XML ...");

    layout->addWidget(graphicsView, 0, 0);

    hLayout = new QHBoxLayout();
    hLayout->addWidget(addCityButton);
    hLayout->addWidget(deleteCityButton);
    hLayout->addSpacerItem(new QSpacerItem(50, 0));
    hLayout->addWidget(addConnectionButton);
    hLayout->addSpacerItem(new QSpacerItem(100, 0));

    hLayout->addWidget(changeRegionColorButton);
    regionColorLabel = new QLabel("Current Region Color:");
    regionColorLabel->setAlignment(Qt::AlignCenter);
    hLayout->addWidget(regionColorLabel);
    regionColor = new QLabel();
    regionColor->setFixedWidth(50);

    regionColor->setAutoFillBackground(true);
    hLayout->addWidget(regionColor);

    hLayout->addSpacerItem(new QSpacerItem(200, 0));
    hLayout->addWidget(exportXML);

    layout->addLayout(hLayout, 1, 0);

    setCentralWidget(centralWidget);

    statusBar = new QStatusBar(centralWidget);
    setStatusBar(statusBar);

    connect(addCityButton          , SIGNAL(clicked()), graphicsView, SLOT(OnAddCity()));
    connect(deleteCityButton       , SIGNAL(clicked()), graphicsView, SLOT(OnDeleteCity()));
    connect(addConnectionButton    , SIGNAL(clicked()), graphicsView, SLOT(OnAddConnection()));
    connect(graphicsView           , SIGNAL(DisplayMessage(QString)), this, SLOT(OnDisplayMessage(QString)));
    connect(graphicsView           , SIGNAL(ClearMessage()), this, SLOT(OnClearMessage()));
    connect(changeRegionColorButton, SIGNAL(clicked()), this, SLOT(OnChangeRegionColor()));
    connect(exportXML              , SIGNAL(clicked()), graphicsView, SLOT(OnExportXml()));

    auto colorRegionPalette = palette();
    colorRegionPalette.setColor(QPalette::Background,
                                (dynamic_cast<MapDesignerGraphicsView*>(graphicsView))->DEFAULT_REGION_COLOR);
    regionColor->setPalette(colorRegionPalette);
}

MapDesignerWindow::~MapDesignerWindow()
{
    delete layout;
    delete graphicsView;
    delete addCityButton;
    delete changeRegionColorButton;
    delete addConnectionButton;
    delete exportXML;
    delete statusBar;
    delete regionColor;
    delete regionColorLabel;
    delete hLayout;
    delete centralWidget;
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

    auto mapDesignerView = dynamic_cast<MapDesignerGraphicsView*>(graphicsView);
    auto validColor = mapDesignerView->SetRegionColor(selectedRegionColor);
    if (!validColor)
    {
        QMessageBox::warning(nullptr, "Invalid Color",
                             "Cannot have more than 6 region colors!");
        mapDesignerView->SetRegionColor(mapDesignerView->DEFAULT_REGION_COLOR);
        selectedRegionColor = mapDesignerView->DEFAULT_REGION_COLOR;
    }

    auto colorRegionPalette = palette();
    colorRegionPalette.setColor(QPalette::Background, selectedRegionColor);
    regionColor->setPalette(colorRegionPalette);

}
