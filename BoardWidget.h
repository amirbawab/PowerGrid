#pragma once

#include <QWidget>
#include <QGridLayout>
#include "BoardTopWidget.h"
#include "BoardBottomWidget.h"
#include "BoardCenterWidget.h"
#include <QThread>

class BoardWidget : public QWidget {
public:
    BoardWidget();
    ~BoardWidget();
    void Refresh();
    void DrawMap();
private:
    class TopWidgetRefresher : public QThread
    {
        BoardTopWidget* widget = nullptr;
    public:
        TopWidgetRefresher(QObject* parent, BoardTopWidget* widget);
        void run() override;
    };

    class BottomWidgetRefresher : public QThread
    {
        BoardBottomWidget* widget = nullptr;
    public:
        BottomWidgetRefresher(QObject* parent, BoardBottomWidget* widget);
        void run() override;
    };

    class CenterWidgetRefresher : public QThread
    {
        BoardCenterWidget* widget = nullptr;
    public:
        CenterWidgetRefresher(QObject* parent, BoardCenterWidget* widget);
        void run() override;
    };

    QGridLayout *gridLayout;
    BoardTopWidget *boardTopWidget;
    BoardBottomWidget *boardBottomWidget;
    BoardCenterWidget *boardCenterWidget;

    TopWidgetRefresher* topRefresher = nullptr;
    BottomWidgetRefresher* bottomRefresher = nullptr;
    CenterWidgetRefresher* centerRefresher = nullptr;
};

