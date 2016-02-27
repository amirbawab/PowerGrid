#pragma once

#include "TemplateLogoWidget.h"
#include "WelcomeMenuWidget.h"

class WelcomeWidget : public TemplateLogoWidget{
public:
	WelcomeWidget();
	~WelcomeWidget();
	void setOnNewGame(const QObject *, const char*);
	void setOnLoadGame(const QObject *, const char*);
	void setOnExitGame(const QObject *, const char*);
private:
	WelcomeMenuWidget *welcomeMenuWidget;
};

