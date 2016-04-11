#pragma once

#include <QWidget>
#include <QGridLayout>
#include <QStackedWidget>
#include <QPushButton>
#include "PowerPlantModeWidget.h"
#include "ResourceMarketModeWidget.h"
#include "OverviewModeWidget.h"
#include "MapModeWidget.h"

class BoardCenterWidget : public QWidget {
    Q_OBJECT;
public:
    BoardCenterWidget();
    ~BoardCenterWidget();
    void Refresh() const;
    void DrawMap() const;
    PowerPlantModeWidget *GetPowerPlantModeWidget() const { return powerPlantModeWidget; }
    ResourceMarketModeWidget *GetResourceMarketModeWidget() const { return resourceMarketModeWidget; }
    MapModeWidget* GetMapModeWidget() const { return mapModeWidget; }

private:
    QGridLayout *gridLayout;
    QStackedWidget *centerStackedWidget;
    QWidget *modeWidget;
    QPushButton *powerPlantsModeButton, *mapModeButton, *resourceMarketModeButton, *overviewModeButton;
    QVBoxLayout *vBoxLayout;
    PowerPlantModeWidget *powerPlantModeWidget;
    ResourceMarketModeWidget* resourceMarketModeWidget;
    OverviewModeWidget *overviewModeWidget;
    MapModeWidget* mapModeWidget;
    int powerPlantModeWidgetIndex, resourceMarketModeWidgetIndex, overviewModeWidgetIndex, mapModeWidgetIndex;
    void paintEvent(QPaintEvent *pe) override;

private slots:
    void onMapMode() const;
    void onPowerPlantMode() const;
    void onResourceMarketMode() const;
    void onOverviewMode() const;
};

