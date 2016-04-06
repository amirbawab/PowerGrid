#include "MapModeWidget.h"

MapModeWidget::MapModeWidget() {
    
    // Init component
    gridLayout = new QGridLayout();
    mapGraphicsView = new MapGraphicsView();

    // Set layout
    setLayout(gridLayout);

    // Add components
    gridLayout->addWidget(mapGraphicsView, 0, 0);
}

MapModeWidget::~MapModeWidget() {
    delete mapGraphicsView;
    delete gridLayout;
}

void MapModeWidget::DrawMap() {
    mapGraphicsView->DrawMap();
}

void MapModeWidget::Refresh() {
    mapGraphicsView->Refresh();
}