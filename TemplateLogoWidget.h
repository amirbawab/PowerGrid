#pragma once

#include <qwidget.h>
#include <qlabel.h>
#include <qgridlayout.h>

class TemplateLogoWidget : public QWidget {
public:
	TemplateLogoWidget();
	~TemplateLogoWidget();
protected:
	QGridLayout *gridLayout;
};

