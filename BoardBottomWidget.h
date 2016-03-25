#pragma once

#include <QWidget>
#include <QGridLayout>

class BoardBottomWidget : public QWidget {
public:
	BoardBottomWidget();
	~BoardBottomWidget();
private:
	QGridLayout *gridLayout;
};

