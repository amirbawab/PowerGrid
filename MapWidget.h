#pragma once

#include <QStyleOption>
#include <QLabel>
#include <QCheckBox>
#include <string>

#include "TemplateNextBackWidget.h"

class MapWidget : public TemplateNextBackWidget {
    Q_OBJECT

public:
    MapWidget();
    ~MapWidget();

    class SelectMapWidget : public QWidget {
    private:
        QGridLayout *gridLayout;
    public:
        SelectMapWidget();
        ~SelectMapWidget();
        QPushButton *nextMapBtn, *previousMapBtn;
        QPushButton* createMapButton;
        QPushButton* loadMapButton;
        QCheckBox* useCustomMap;
        QLabel* useCustomMapLabel;
        QLabel *titleLabel, *mapLabel;
        std::vector<std::string> mapLabels;
        int currentMap = 0;
        std::string GetSelectedMap() { return mapLabels[currentMap]; }

        bool useCustomMapSelected = false;
        QString customMapFileName;
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

    std::string GetSelectedMap() const { return selectMapWidget->GetSelectedMap(); }
    int GetNumberOfPlayers() const { return stoi(selectNumberOfPlayerWidget->numberOfPlayersLabel->text().toStdString()); }
    SelectMapWidget* GetSelectMapWidget() const { return selectMapWidget; }

private:
    SelectMapWidget *selectMapWidget;
    SelectNumberOfPlayerWidget *selectNumberOfPlayerWidget;
    void paintEvent(QPaintEvent *) override;

private slots:
    void increment();
    void decrement();
public:
};

