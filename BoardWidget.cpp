#include "BoardWidget.h"
#include <QMessageBox>

BoardWidget::BoardWidget() {

    // Init components
    gridLayout = new QGridLayout();
    boardTopWidget = new BoardTopWidget();
    boardBottomWidget = new BoardBottomWidget();
    boardCenterWidget = new BoardCenterWidget();

    // Set layout
    setLayout(gridLayout);

    // Set id
    setObjectName("templateBoardWidget");

    // Set margin
    gridLayout->setContentsMargins(0, 0, 0, 0);
    
    // Connect card
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

    // Connect ok
    connect(boardBottomWidget->GetBoardMessage()->GetStepTwoPanel()->GetOkButton(), &QPushButton::clicked, [=]() {
        qDebug("Ok clicked");

        if (!boardCenterWidget->GetPowerPlantModeWidget()->GetSelectedCard()) {
            QMessageBox::critical(this, "No Power Plant Selected!", "Please select a power plant");
        
        } else {
            // Cast
            auto powerPlantCard = std::dynamic_pointer_cast<PowerPlantCard>(
                Game::getInstance().GetCardStack().GetVisibleCards()[boardCenterWidget->GetPowerPlantModeWidget()->GetSelectedCardIndex()]);

            // If found
            if (powerPlantCard) {
                if (boardBottomWidget->GetBoardMessage()->GetStepTwoPanel()->GetCounterWidget()->GetValueAsInt() < powerPlantCard->GetPrice()) {
                    QMessageBox::critical(this, "Price Error", "Please select a price greater than or equal to the selected card");
                }
                else {
                    Game::getInstance().Step2PickPlant2(boardCenterWidget->GetPowerPlantModeWidget()->GetSelectedCardIndex(), 
                        boardBottomWidget->GetBoardMessage()->GetStepTwoPanel()->GetCounterWidget()->GetValueAsInt(), false);
                }
            }
        }
    });
    
    // Connect skip
    connect(boardBottomWidget->GetBoardMessage()->GetStepTwoPanel()->GetSkipButton(), &QPushButton::clicked, [=]() {
        qDebug("Skip clicked");
        Game::getInstance().Step2PickPlant2(-1, -1, true);
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
    boardTopWidget->Refresh();
    boardCenterWidget->Refresh();
    boardBottomWidget->Refresh();
}

void BoardWidget::DrawMap() {
    boardCenterWidget->DrawMap();
}
