#pragma once

#include <QWidget>
#include <QGridLayout>

class BoardTopWidget : public QWidget {
public:
	BoardTopWidget();
	~BoardTopWidget();
private:
	QGridLayout *gridLayout;
};

