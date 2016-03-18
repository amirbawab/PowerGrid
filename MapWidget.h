#pragma once

#include <QStyleOption>
#include <QPainter>
#include <string>

#include "TemplateNextBackWidget.h"

class MapWidget : public TemplateNextBackWidget {
	Q_OBJECT
private:
	class SelectMapWidget : public QWidget {
	private:
		QGridLayout *gridLayout;
	public:
		SelectMapWidget();
		~SelectMapWidget();
		QPushButton *nextMapBtn, *previousMapBtn;
		QLabel *titleLabel, *usaMapLabel;
	};

	class SelectNumberOfPlayerWidget : public QWidget {
	private:
		QGridLayout *gridLayout;
	public:
		SelectNumberOfPlayerWidget();
		~SelectNumberOfPlayerWidget();
		QPushButton *plusBtn, *minusBtn;
		QLabel *titleLabel, *numberOfPlayersLabel;
	};

	SelectMapWidget *selectMapWidget;
	SelectNumberOfPlayerWidget *selectNumberOfPlayerWidget;
	void paintEvent(QPaintEvent *);

private slots:
	void increment();
	void decrement();
public:
	MapWidget();
	~MapWidget();
	int GetNumberOfPlayers() { return std::stoi(selectNumberOfPlayerWidget->numberOfPlayersLabel->text().toStdString()); }
};

