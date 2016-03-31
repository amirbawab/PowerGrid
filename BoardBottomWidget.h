#pragma once

#include <QWidget>
#include <QGridLayout>
#include <QLabel>
#include "Player.h"
#include <memory>
#include "DataStore.h"

class BoardProfileWidget : public QWidget {
public:
	BoardProfileWidget();
	~BoardProfileWidget();
	void Refresh();
private:
	QLabel *playerNameLabel;
	QLabel *playerPictureLabel;
	QGridLayout *gridLayout;
};

class BoardBottomWidget : public QWidget {
public:
	BoardBottomWidget();
	~BoardBottomWidget();
	void Refresh();
private:
	QGridLayout *gridLayout;
	BoardProfileWidget *boardProfileWidget;
};

