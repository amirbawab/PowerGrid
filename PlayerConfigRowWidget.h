#pragma once

#include <qwidget.h>
#include <QLineEdit>
#include <QHBoxLayout>
#include <string>
#include <QLabel>

using std::string;

class PlayerConfigRowWidget : public QWidget {
private:
	QLineEdit *playerName;
	QHBoxLayout *hBoxLayout;
	QLabel *houseLabel;
public:
	PlayerConfigRowWidget(string name);
	~PlayerConfigRowWidget();
	void SetPlayerName(string name);
	string GetPlayerName() const { return playerName->text().toStdString(); }
};

