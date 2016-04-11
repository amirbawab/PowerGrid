#pragma once

#include <QWidget>
#include "MapGraphicsView.h"
#include <QGridLayout>

class MapModeWidget : public QWidget {
public:
    MapModeWidget();
    ~MapModeWidget();
    void DrawMap() const;
    void Refresh() const;

    MapGraphicsView* GetMapGraphicsView() const { return mapGraphicsView; }

private:
    MapGraphicsView *mapGraphicsView;
    QGridLayout *gridLayout;
};

