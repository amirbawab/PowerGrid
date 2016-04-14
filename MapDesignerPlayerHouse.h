#pragma once
#include "qgraphicsitem.h"

class MapDesignerPlayer;

class MapDesignerPlayerHouse : public QGraphicsEllipseItem
{
    MapDesignerPlayer * player;

public:
    MapDesignerPlayerHouse();
};

