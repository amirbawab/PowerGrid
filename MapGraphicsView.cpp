#include "MapGraphicsView.h"

MapGraphicsView::MapGraphicsView() {
	
	// Set id
	setObjectName("mapGraphicsView");

	// Init components
	graphicsScene = new QGraphicsScene();

	// Set scene
	setScene(graphicsScene);

	// Dummy drawing
	graphicsScene->addEllipse(0, -100, 300, 60, QPen(Qt::black), QBrush(Qt::green));
}

MapGraphicsView::~MapGraphicsView() {
	delete graphicsScene;
}

void MapGraphicsView::wheelEvent(QWheelEvent* event) {
	// Don't zoom if nothing is in the scene
	if (scene()->items().size() == 0)
		return;

	setTransformationAnchor(AnchorUnderMouse);
	setDragMode(ScrollHandDrag);

	if (event->delta() > 0) {
		scaleSteps++;
		scale(scaleFactor, scaleFactor);
	}
	else {
		scaleSteps--;
		scale(1 / scaleFactor, 1 / scaleFactor);
	}
}