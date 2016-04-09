#pragma once

#include <qwidget.h>
#include <qgridlayout.h>
#include <qpushbutton.h>

class TemplateNextBackWidget : public QWidget {
public:
	TemplateNextBackWidget();
	~TemplateNextBackWidget();
	void setOnNext(const QObject *, const char*);
	void setOnBack(const QObject *, const char*);
protected:
	QGridLayout *centerLayout;
private:
	class ButtonsWidget : public QWidget {
	public:
		ButtonsWidget();
		~ButtonsWidget();
		QPushButton *nextBtn, *backBtn;
	private:
		QHBoxLayout *hBoxLayout;
	};
	QGridLayout *gridLayout;
	ButtonsWidget *buttonsWidget;
	QWidget *centerWidget;
};

