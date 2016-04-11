#include "BoardWidget.h"
#include <QMessageBox>
#include "Game.h"
#include <QDebug>

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

    // Connect region for phase 0
    connect(boardCenterWidget->GetMapModeWidget()->GetMapGraphicsView(), &MapGraphicsView::RegionSelected,
            [=](shared_ptr<Region> region)
    {
        qDebug() << "Selected region: " << region->GetName().c_str();

        // Set the region to remove in the game
        Game::getInstance().pickedRegion = region;
    });

    // Connect ok for phase 0
    connect(boardBottomWidget->GetBoardMessage()->GetPhase0Panel()->GetOkButton(), &QPushButton::clicked, [=]()
    {
        qDebug() << "OK (phase 0) clicked";

        // Reset selected in the map view
        boardCenterWidget->GetMapModeWidget()->GetMapGraphicsView()->ResetSelected();

        Game::getInstance().Phase0RemoveRegions2();
    });
    
    // Connect ok for step 1
    connect(boardBottomWidget->GetBoardMessage()->GetStepOnePanel()->GetOkButton(), &QPushButton::clicked, []()
    {
        qDebug() << "OK (step 1) clicked";
        Game::getInstance().Step2Start();
    });

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
        qDebug("Ok (step 2) clicked");

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
        qDebug("Skip (step 2) clicked");
       
        // If now bidding
        if (Game::getInstance().GetNowBidding())
            Game::getInstance().Step2Bid2();
        else
            Game::getInstance().Step2PickPlant2(true);
    });
    
    // Connect plus for step 3
    connect(boardBottomWidget->GetBoardMessage()->GetStepThreePanel()->GetCounterWidget(),
            &CounterWidget::plusPressed, [=](int value) {
        
        int available = Game::getInstance().GetResourceMarket()->GetNbResource(Game::getInstance().resourceIdentity);
        if (value > available) {
            QMessageBox::critical(this, "Resource Market Error",
                                  string("You cannot select more than the available amount of " +
                                         GetResourceName(Game::getInstance().resourceIdentity)).c_str());
            boardBottomWidget->GetBoardMessage()->GetStepThreePanel()->GetCounterWidget()->SetValue(std::to_string(available));
        }
        else
            boardCenterWidget->GetResourceMarketModeWidget()->ActivateResource(Game::getInstance().resourceIdentity, value);
    });

    // Connect minus for step 3
    connect(boardBottomWidget->GetBoardMessage()->GetStepThreePanel()->GetCounterWidget(),
            &CounterWidget::minusPressed, [=](int value) {

        boardCenterWidget->GetResourceMarketModeWidget()->ActivateResource(Game::getInstance().resourceIdentity, value);
    });

    // Connect ok for step 3
    connect(boardBottomWidget->GetBoardMessage()->GetStepThreePanel()->GetOkButton(), &QPushButton::clicked, [=]() {
        qDebug("Ok (step 3) clicked");

        Game::getInstance().Step3BuyingResources2(boardBottomWidget->GetBoardMessage()->GetStepThreePanel()->
                                                  GetCounterWidget()->GetValueAsInt());
    });

    // Connect skip for step 3
    connect(boardBottomWidget->GetBoardMessage()->GetStepThreePanel()->GetSkipButton(), &QPushButton::clicked, [=]() {
        qDebug("Skip (step 3) clicked");

        Game::getInstance().Step3BuyingResources2(0);
    });

    // Connect city for step 4
    connect(boardCenterWidget->GetMapModeWidget()->GetMapGraphicsView(), &MapGraphicsView::CitySelected,
            [=](shared_ptr<City> city)
    {
        qDebug() << "Selected city: " << city->GetName().c_str();

        // Display the city in the question label
        boardBottomWidget->GetBoardMessage()->GetQuestionLabel()->setText(
            QString("Selected city: <b>").append(city->GetName().c_str()).append("</b>"));

        // Set the city in the game
        Game::getInstance().pickedCity = city;
    });

    // Connect ok for step 4
    connect(boardBottomWidget->GetBoardMessage()->GetStepFourPanel()->GetOkButton(), &QPushButton::clicked, [=]()
    {
        qDebug("OK (step 4) clicked");

        // Reset selected in the map view
        boardCenterWidget->GetMapModeWidget()->GetMapGraphicsView()->ResetSelected();

        // Try to buy a house in the city
        Game::getInstance().Step4BuyingCities2();
    });

    // Connect skip for step 4
    connect(boardBottomWidget->GetBoardMessage()->GetStepFourPanel()->GetSkipButton(), &QPushButton::clicked, [=]()
    {
        qDebug("OK (step 4) clicked");
        
        // Reset selected in the map view
        boardCenterWidget->GetMapModeWidget()->GetMapGraphicsView()->ResetSelected();

        // Reset the value in the game and continue
        Game::getInstance().pickedCity.reset();
        Game::getInstance().Step4BuyingCities2();
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
