#pragma once

#include <QWidget>
#include <QGridLayout>

class BoardWidget : public QWidget {
public:
	BoardWidget();
	~BoardWidget();

private:
	QGridLayout *gridLayout;
};

