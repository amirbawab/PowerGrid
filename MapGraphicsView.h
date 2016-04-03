#pragma once

#include <QGraphicsView>
#include <QWheelEvent>

class MapGraphicsView : public QGraphicsView {
public:
	MapGraphicsView();
	~MapGraphicsView();
private:
	QGraphicsScene *graphicsScene;
	int scaleSteps = 0;
	float scaleFactor = 1.15f;
protected:
	void wheelEvent(QWheelEvent* event) override;

};

