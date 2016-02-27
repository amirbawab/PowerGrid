#pragma once

#include <qwidget.h>
#include <qgridlayout.h>
#include <QHBoxLayout>
#include <qlabel.h>
#include <qpushbutton.h>

class TemplateNextBackWidget : public QWidget {
public:
	TemplateNextBackWidget();
	~TemplateNextBackWidget();
	void setOnNext(const QObject *, const char*);
	void setOnBack(const QObject *, const char*);
protected:
	QGridLayout *gridLayout;
private:
	class ButtonsWidget : public QWidget {
	public:
		ButtonsWidget();
		~ButtonsWidget();
		QPushButton *nextBtn, *backBtn;
	private:
		QHBoxLayout *hBoxLayout;
	};
	ButtonsWidget *buttonsWidget;
};

