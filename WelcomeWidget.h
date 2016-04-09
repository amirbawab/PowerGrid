#pragma once

#include "TemplateLogoWidget.h"
#include "WelcomeMenuWidget.h"

class WelcomeWidget : public TemplateLogoWidget{
public:
	WelcomeWidget();
	~WelcomeWidget();
	void setOnNewGame(const QObject *, const char*) const;
	void setOnLoadGame(const QObject *, const char*) const;
	void setOnExitGame(const QObject *, const char*) const;
private:
	WelcomeMenuWidget *welcomeMenuWidget;
};

