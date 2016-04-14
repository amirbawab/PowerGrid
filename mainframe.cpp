#include "MainFrame.h"
#include "Game.h"
#include <QDesktopWidget>
#include <QApplication>
#include <QMessageBox>
#include <QFileDialog>

MainFrame::MainFrame(string title) : QMainWindow() {

    // Set title
    setWindowTitle(title.c_str());

    // Init widgets
    centerStackWidget = new QStackedWidget();
    welcomeWidget = new WelcomeWidget();
    mapWidget = new MapWidget();
    playerConfigWidget = new PlayerConfigWidget();
    boardWidget = new BoardWidget();

    // Configure welcome widget
    welcomeWidget->setOnExitGame(this, SLOT(close()));
    welcomeWidget->setOnNewGame(this, SLOT(onNewGame()));
    welcomeWidget->setOnLoadGame(this, SLOT(onLoadGame()));

    // Configure map widget
    mapWidget->setOnBack(this, SLOT(onMapBack()));
    mapWidget->setOnNext(this, SLOT(onMapNext()));

    // Configure player widget
    playerConfigWidget->setOnBack(this, SLOT(onPlayerConfigBack()));
    playerConfigWidget->setOnNext(this, SLOT(onPlayerConfigNext()));

    // Center 
    setCentralWidget(centerStackWidget);

    // Add default stack
    welcomeWidgetIndex = centerStackWidget->addWidget(welcomeWidget);
    mapWidgetIndex = centerStackWidget->addWidget(mapWidget);
    playerConfigWidgetIndex = centerStackWidget->addWidget(playerConfigWidget);
    boardWidgetIndex = centerStackWidget->addWidget(boardWidget);

    // Configure frame
    resize(QDesktopWidget().availableGeometry(this).size() * 0.9); // 70% of width
    setGeometry(QStyle::alignedRect(Qt::LeftToRight, Qt::AlignCenter, this->size(), QApplication::desktop()->availableGeometry()));
    show();
}

MainFrame::~MainFrame() {
    delete welcomeWidget;
    delete mapWidget;
    delete playerConfigWidget;
    delete centerStackWidget;
}

/// SLOTS ///

void MainFrame::onNewGame() {
    qDebug("Starting a new game");
    centerStackWidget->setCurrentIndex(mapWidgetIndex);
    Game::getInstance().NewGame();
}

void MainFrame::onLoadGame() {
    auto fileName = QFileDialog::getOpenFileName(this, "Save Game Location",
                                                 "Please Specify Save Game Location", "XML Files (*.xml)");
    if (fileName.isEmpty())
        return;

    Game::getInstance().LoadGame(fileName.toStdString());
    switch (Game::getInstance().GetStep())
    {
        case 1:
            Game::getInstance().Step1Start();
            break;
        case 2:
            Game::getInstance().Step2Start();
            break;
        case 3:
            Game::getInstance().Step3Start();
            break;
        case 4:
            Game::getInstance().Step4Start();
            break;
        default:
            Game::getInstance().Step5Start();
            break;
    }
    
    boardWidget->Refresh();
    centerStackWidget->setCurrentIndex(boardWidgetIndex);
}

void MainFrame::onMapBack() {
    qDebug("Back from map selection screen");
    centerStackWidget->setCurrentIndex(welcomeWidgetIndex);
}

void MainFrame::onMapNext() {
    qDebug("Next from map selection screen");
    
    if (mapWidget->GetSelectMapWidget()->useCustomMapSelected)
    {
        auto mapFileName = mapWidget->GetSelectMapWidget()->customMapFileName;

        if (mapFileName.isEmpty())
        {
            QMessageBox::critical(
                this, "Custom Map Error", QString("You have selected to use a custom")
                .append(", but you didn't select a file"));

            return;
        }

        Game::getInstance().ConfigNewGame(mapFileName.toStdString(),
                                          mapWidget->GetNumberOfPlayers(), true);
    }
    else
        Game::getInstance().ConfigNewGame(mapWidget->GetSelectedMap(), mapWidget->GetNumberOfPlayers());

    centerStackWidget->setCurrentIndex(playerConfigWidgetIndex);
    playerConfigWidget->SetNumberOfPlayers(mapWidget->GetNumberOfPlayers());
    boardWidget->DrawMap();
}

void MainFrame::onPlayerConfigBack() {
    qDebug("Back from player config screen");
    centerStackWidget->setCurrentIndex(mapWidgetIndex);
}

void MainFrame::onPlayerConfigNext() {
    qDebug("Next from player config screen");

    // If no errors, go to next page
    if (!playerConfigWidget->HasError()) {
        playerConfigWidget->UpdatePlayersInfo();
        Game::getInstance().StartGame();
        Game::getInstance().Phase0Start();
        
        boardWidget->Refresh();
        centerStackWidget->setCurrentIndex(boardWidgetIndex);
    } else {
        QMessageBox::critical(this, "Try again", "Players should have distinct house colors.", QMessageBox::Ok);
        qDebug("Errors found on this page.");
    }
}

void MainFrame::Update() {
    // Display error message
    if (Game::getInstance().GetErrorMessageText() != "") {
        QMessageBox::critical(this, Game::getInstance().GetErrorMessageTextTitle().c_str(), Game::getInstance().GetErrorMessageText().c_str());
        Game::getInstance().SetErrorMessageTextBox("", "");
    }

    // Display info message
    if (Game::getInstance().GetInfoMessageText() != "") {
        QMessageBox::information(this, Game::getInstance().GetInfoMessageTextTitle().c_str(), Game::getInstance().GetInfoMessageText().c_str());
        Game::getInstance().SetInfoMessageTextBox("", "");
    }

    boardWidget->Refresh();
}

/// This method is required when Q_OBJECT is added
/// Without this method, the CSS will not be applied
void MainFrame::paintEvent(QPaintEvent *pe) {
    QStyleOption o;
    o.initFrom(this);
    QPainter p(this);
    style()->drawPrimitive(
        QStyle::PE_Widget, &o, &p, this);
};