#pragma once

#include <qwidget.h>
#include <QVBoxLayout>
#include <qpushbutton.h>

class WelcomeMenuWidget : public QWidget {
	Q_OBJECT
public:
	WelcomeMenuWidget();
	~WelcomeMenuWidget();
	void setOnNewGame(const QObject *, const char*);
	void setOnLoadGame(const QObject *, const char*);
	void setOnExitGame(const QObject *, const char*);
private:
	QVBoxLayout *vBoxLayout;
	QPushButton *newGameBtn, *loadGameBtn, *exitBtn;
	void paintEvent(QPaintEvent *pe);
};

