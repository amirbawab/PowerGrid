#pragma once

#include "TemplateNextBackWidget.h"

class PlayerConfigWidget : public TemplateNextBackWidget {
	Q_OBJECT
private:
	void paintEvent(QPaintEvent *pe);
public:
	PlayerConfigWidget();
	~PlayerConfigWidget();
};

