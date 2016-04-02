#pragma once
#include "qgraphicsitem.h"
#include <QBrush>

class Player;

class PlayerHouse : public QGraphicsEllipseItem
{
    Player * player;

public:
    PlayerHouse();
};

