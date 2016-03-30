#pragma once

#include "HouseColor.h"
#include <qwidget.h>
#include <QLineEdit>
#include <QHBoxLayout>
#include <string>
#include <QPushButton>
#include <vector>
#include <QPaintEvent>
#include <QStyleOption>
#include <QPainter>

using std::string;

class PlayerConfigRowWidget : public QWidget {
	Q_OBJECT
private:
	QLineEdit *playerName;
	QHBoxLayout *hBoxLayout;
	QPushButton *houseButton;
	std::vector<HouseColor*> houseColors;
	int houseColorIndex;
	void paintEvent(QPaintEvent *pe);
	
public:
	PlayerConfigRowWidget(string name);
	~PlayerConfigRowWidget();
	void SetPlayerName(string name);
	string GetPlayerName() const { return playerName->text().toStdString(); }
	void SetHouseColors(std::vector<HouseColor*> &houseColors) { this->houseColors = houseColors; }
	void SetHouseColorIndex(int houseColorIndex);

private slots:
	void OnHouseClick();
};

