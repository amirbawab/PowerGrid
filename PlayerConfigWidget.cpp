#include "PlayerConfigWidget.h"
#include <QStyleOption>
#include <QPainter>

PlayerConfigWidget::PlayerConfigWidget() {

}

PlayerConfigWidget::~PlayerConfigWidget() {

}

/// This method is required when Q_OBJECT is added
/// Without this method, the CSS will not be applied
void PlayerConfigWidget::paintEvent(QPaintEvent *pe) {
	QStyleOption o;
	o.initFrom(this);
	QPainter p(this);
	style()->drawPrimitive(
		QStyle::PE_Widget, &o, &p, this);
};