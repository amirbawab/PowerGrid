#include "BoardWidget.h"
#include <QMessageBox>
#include "Game.h"

BoardWidget::BoardWidget() {

    // Init components
    gridLayout = new QGridLayout();
    boardTopWidget = new BoardTopWidget();
    boardBottomWidget = new BoardBottomWidget();
    boardCenterWidget = new BoardCenterWidget();

    // Initialize refresher threads
    topRefresher = new TopWidgetRefresher(this, boardTopWidget);
    bottomRefresher = new BottomWidgetRefresher(this, boardBottomWidget);
    centerRefresher = new CenterWidgetRefresher(this, boardCenterWidget);

    // Thread housekeeping
    connect(topRefresher   , &TopWidgetRefresher::finished   , topRefresher   , &QObject::deleteLater);
    connect(bottomRefresher, &BottomWidgetRefresher::finished, bottomRefresher, &QObject::deleteLater);
    connect(centerRefresher, &CenterWidgetRefresher::finished, centerRefresher, &QObject::deleteLater);

    // Set layout
    setLayout(gridLayout);

    // Set id
    setObjectName("templateBoardWidget");

    // Set margin
    gridLayout->setContentsMargins(0, 0, 0, 0);
    
    // Connect card for step 2
    connect(boardCenterWidget->GetPowerPlantModeWidget(), &PowerPlantModeWidget::CardSelected, [=](int index) {
        qDebug("Updating counter");

        // Cast
        auto powerPlantCard = std::dynamic_pointer_cast<PowerPlantCard>(Game::getInstance().GetCardStack().GetVisibleCards()[index]);

        // If found
        if (powerPlantCard) {
            boardBottomWidget->GetBoardMessage()->GetStepTwoPanel()->GetCounterWidget()->
                GetNumberLabel()->setText(std::to_string(powerPlantCard->GetPrice()).c_str());
        }
    });

    // Connect ok for step 2
    connect(boardBottomWidget->GetBoardMessage()->GetStepTwoPanel()->GetOkButton(), &QPushButton::clicked, [=]() {
        qDebug("Ok clicked");

        // If now bidding
        if (Game::getInstance().GetNowBidding()) {
            Game::getInstance().Step2Bid2(boardBottomWidget->GetBoardMessage()->GetStepTwoPanel()->GetCounterWidget()->GetValueAsInt());
        }
        else {
            if (!boardCenterWidget->GetPowerPlantModeWidget()->GetSelectedCard()) {
                QMessageBox::critical(this, "No Power Plant Selected!", "Please select a power plant");
            }
            else {
                // Cast
                auto powerPlantCard = std::dynamic_pointer_cast<PowerPlantCard>(
                    Game::getInstance().GetCardStack().GetVisibleCards()[boardCenterWidget->GetPowerPlantModeWidget()->GetSelectedCardIndex()]);

                // If found
                if (powerPlantCard) {
                    if (boardBottomWidget->GetBoardMessage()->GetStepTwoPanel()->GetCounterWidget()->GetValueAsInt() < powerPlantCard->GetPrice()) {
                        QMessageBox::critical(this, "Price Error", "Please select a price greater than or equal to the selected card");
                    }
                    else {
                        Game::getInstance().Step2PickPlant2(
                            false,
                            boardCenterWidget->GetPowerPlantModeWidget()->GetSelectedCardIndex(),
                            boardBottomWidget->GetBoardMessage()->GetStepTwoPanel()->GetCounterWidget()->GetValueAsInt());
                    }
                }
            }
        }
    });
    
    // Connect skip for step 2
    connect(boardBottomWidget->GetBoardMessage()->GetStepTwoPanel()->GetSkipButton(), &QPushButton::clicked, [=]() {
        qDebug("Skip clicked");
       
        // If now bidding
        if (Game::getInstance().GetNowBidding()) {
            Game::getInstance().Step2Bid2();
        } else {
            Game::getInstance().Step2PickPlant2(true);
        }
    });
    
    // Connect plus for step 3
    connect(boardBottomWidget->GetBoardMessage()->GetStepThreePanel()->GetCounterWidget(), &CounterWidget::plusPressed, [=](int value) {
        qDebug("Plus clicked");
        boardCenterWidget->GetResourceMarketModeWidget()->ActivateResource(Game::getInstance().resourceIdentity, value);
    });

    // Connect plus for step 3
    connect(boardBottomWidget->GetBoardMessage()->GetStepThreePanel()->GetCounterWidget(), &CounterWidget::minusPressed, [=](int value) {
        qDebug("Minus clicked");
        boardCenterWidget->GetResourceMarketModeWidget()->ActivateResource(Game::getInstance().resourceIdentity, value);
    });

    // Add components
    gridLayout->addWidget(boardTopWidget, 0, 0, Qt::AlignTop);
    gridLayout->addWidget(boardCenterWidget, 1, 0);
    gridLayout->addWidget(boardBottomWidget, 2, 0, Qt::AlignBottom);
}

BoardWidget::~BoardWidget() {
    delete boardTopWidget;
    delete boardBottomWidget;
    delete gridLayout;
}

void BoardWidget::Refresh() {
    
    // Refresh all widgets
    topRefresher->run();
    bottomRefresher->run();
    centerRefresher->run();

}

void BoardWidget::DrawMap() {
    boardCenterWidget->DrawMap();
}

BoardWidget::TopWidgetRefresher::TopWidgetRefresher(
    QObject* parent, BoardTopWidget* widget) : QThread(parent), widget{ widget }
{ }

void BoardWidget::TopWidgetRefresher::run()
{
    widget->Refresh();
}

BoardWidget::BottomWidgetRefresher::BottomWidgetRefresher(
    QObject* parent, BoardBottomWidget* widget) :
    QThread(parent), widget { widget }
{}

void BoardWidget::BottomWidgetRefresher::run()
{
    widget->Refresh();
}

BoardWidget::CenterWidgetRefresher::CenterWidgetRefresher(
    QObject* parent, BoardCenterWidget* widget) :
    QThread(parent), widget{ widget }
{}

void BoardWidget::CenterWidgetRefresher::run()
{
    widget->Refresh();
}
