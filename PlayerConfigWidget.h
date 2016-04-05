#pragma once

#include "TemplateNextBackWidget.h"
#include "PlayerConfigRowWidget.h"
#include "Player.h"
#include <vector>
#include "DataStore.h"
#include <memory>

class PlayerConfigWidget : public TemplateNextBackWidget {
	Q_OBJECT
private:
	void paintEvent(QPaintEvent *pe);
	std::vector<PlayerConfigRowWidget*> rows;
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

