#pragma once

#include "TemplateNextBackWidget.h"
#include "PlayerConfigRowWidget.h"
#include <vector>
#include <QLabel>

class PlayerConfigWidget : public TemplateNextBackWidget {
	Q_OBJECT
private:
	void paintEvent(QPaintEvent *pe) override;
	vector<PlayerConfigRowWidget*> rows;
	QWidget *rowsWidget;
	QLabel *titleLabel;
	QVBoxLayout *vBoxLayout;
public:
	PlayerConfigWidget();
	~PlayerConfigWidget();
	void SetNumberOfPlayers(int num);
	bool HasError();
	void UpdatePlayersInfo();
};

