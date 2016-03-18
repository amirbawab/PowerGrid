#pragma once

#include "TemplateNextBackWidget.h"
#include "PlayerConfigRowWidget.h"
#include <vector>

class PlayerConfigWidget : public TemplateNextBackWidget {
	Q_OBJECT
private:
	void paintEvent(QPaintEvent *pe);
public:
	PlayerConfigWidget();
	~PlayerConfigWidget();
	QWidget *rowsWidget;
	QLabel *titleLabel;
	QVBoxLayout *vBoxLayout;
	std::vector<PlayerConfigRowWidget*> rows;
};

