#pragma once

#include <QGraphicsView>
#include <QWheelEvent>
#include "DataStore.h"
#include "City.h"
#include "Connection.h"
#include <memory>

class MapGraphicsView : public QGraphicsView {
public:
	MapGraphicsView();
	~MapGraphicsView();
    void Refresh();
private:
    std::unique_ptr<QGraphicsScene> graphicsScene;
    int scaleSteps = 0;
	float scaleFactor = 1.15f;
protected:
	void wheelEvent(QWheelEvent* event) override;
};

