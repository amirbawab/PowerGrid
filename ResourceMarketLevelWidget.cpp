#include "ResourceMarketLevelWidget.h"
#include <QGraphicsOpacityEffect>
#include "Game.h"

ResourceMarketLevelWidget::ResourceMarketLevelWidget() {

    // Init components
    gridLayout = new QGridLayout();
    costLabel = new QLabel();
    costLabel->setAlignment(Qt::AlignCenter);

    // Set layout
    setLayout(gridLayout);
    
    // Configure components
    gridLayout->setSpacing(0);
    
    // Set id
    costLabel->setObjectName("resource_cost_label");

    // Add cost
    gridLayout->addWidget(costLabel, 0, 0, 1, 3, Qt::AlignCenter);

    // Icon dimensions
    iconWidth = 50;
    iconHeight = 50;
}

void ResourceMarketLevelWidget::CreateUraniumLevelMarket() {
    
    QPushButton *button = new QPushButton();
    button->setObjectName("resourceButton");
    button->setIconSize(QSize(iconWidth, iconHeight));
    gridLayout->addWidget(button, 1, 0, Qt::AlignCenter);
    uraniumButtons.push_back(button);
}

void ResourceMarketLevelWidget::CreateFullLevelMarket() {

    // Add coal
    for (int i = 0; i < 3; i++) {
        QPushButton *button = new QPushButton();
        button->setObjectName("resourceButton");
        button->setIconSize(QSize(iconWidth, iconHeight));
        coalButtons.push_back(button);
        gridLayout->addWidget(coalButtons[i], 1, i, Qt::AlignCenter);
    }

    // Add oil
    for (int i = 0; i < 3; i++) {
        QPushButton *button = new QPushButton();
        button->setObjectName("resourceButton");
        button->setIconSize(QSize(iconWidth, iconHeight));
        oilButtons.push_back(button);
        gridLayout->addWidget(oilButtons[i], 2, i, Qt::AlignCenter);
    }

    // Add garbage
    for (int i = 0; i < 3; i++) {
        QPushButton *button = new QPushButton();
        button->setObjectName("resourceButton");
        button->setIconSize(QSize(iconWidth, iconHeight));
        garbageButtons.push_back(button);
        gridLayout->addWidget(garbageButtons[i], 3, i, Qt::AlignCenter);
    }
    
    // Add uranium
    for (int i = 0; i < 1; i++) {
        QPushButton *button = new QPushButton();
        button->setObjectName("resourceButton");
        button->setIconSize(QSize(iconWidth, iconHeight));
        uraniumButtons.push_back(button);
        gridLayout->addWidget(uraniumButtons[i], 4, 1, Qt::AlignCenter);
    }
}

ResourceMarketLevelWidget::~ResourceMarketLevelWidget() {
    for (int i = 0; i < coalButtons.size(); i++)
        delete coalButtons[i];

    for (int i = 0; i < oilButtons.size(); i++)
        delete oilButtons[i];

    for (int i = 0; i < garbageButtons.size(); i++)
        delete garbageButtons[i];

    for (int i = 0; i < uraniumButtons.size(); i++)
        delete uraniumButtons[i];
}

void ResourceMarketLevelWidget::Refresh() {
    
    // Update cost
    costLabel->setText(QString::fromStdString(std::to_string(level->GetCost()) + " Elektro"));

    // Draw coal
    int totalCoal = level->GetCounter(COAL);
    for (int i = coalButtons.size()-1; i >= 0; i--) {
        
        // Fade resource in step 3
        if (Game::getInstance().GetStep() == 3) {
            QGraphicsOpacityEffect * effect = new QGraphicsOpacityEffect(this);
            effect->setOpacity(0.55);
            coalButtons[i]->setGraphicsEffect(effect);
        }

        if (totalCoal == 0) {
            coalButtons[i]->setIcon(QIcon(":/PowerGrid/Resources/resources/coal_bw.png"));
        } else {
            coalButtons[i]->setIcon(QIcon(":/PowerGrid/Resources/resources/coal.png"));
            totalCoal--;
        }
    }

    // Draw oil
    int totalOil = level->GetCounter(OIL);
    for (int i = oilButtons.size() - 1; i >= 0; i--) {
        
        // Fade resource in step 3
        if (Game::getInstance().GetStep() == 3) {
            QGraphicsOpacityEffect * effect = new QGraphicsOpacityEffect(this);
            effect->setOpacity(0.55);
            oilButtons[i]->setGraphicsEffect(effect);
        }
        
        if (totalOil == 0) {
            oilButtons[i]->setIcon(QIcon(":/PowerGrid/Resources/resources/oil_bw.png"));
        }
        else {
            oilButtons[i]->setIcon(QIcon(":/PowerGrid/Resources/resources/oil.png"));
            totalOil--;
        }
    }

    // Draw garbage
    int totalGarbage = level->GetCounter(GARBAGE);
    for (int i = garbageButtons.size() - 1; i >= 0; i--) {
        
        // Fade resource in step 3
        if (Game::getInstance().GetStep() == 3) {
            QGraphicsOpacityEffect * effect = new QGraphicsOpacityEffect(this);
            effect->setOpacity(0.55);
            garbageButtons[i]->setGraphicsEffect(effect);
        }
        
        if (totalGarbage == 0) {
            garbageButtons[i]->setIcon(QIcon(":/PowerGrid/Resources/resources/garbage_bw.png"));
        }
        else {
            garbageButtons[i]->setIcon(QIcon(":/PowerGrid/Resources/resources/garbage.png"));
            totalGarbage--;
        }
    }

    // Draw uranium
    int totalUranim = level->GetCounter(URANIUM);
    for (int i = uraniumButtons.size() - 1; i >= 0; i--) {
        
        // Fade resource in step 3
        if (Game::getInstance().GetStep() == 3) {
            QGraphicsOpacityEffect * effect = new QGraphicsOpacityEffect(this);
            effect->setOpacity(0.55);
            uraniumButtons[i]->setGraphicsEffect(effect);
        }
        
        if (totalUranim == 0) {
            uraniumButtons[i]->setIcon(QIcon(":/PowerGrid/Resources/resources/uranium_bw.png"));
        }
        else {
            uraniumButtons[i]->setIcon(QIcon(":/PowerGrid/Resources/resources/uranium.png"));
            totalUranim--;
        }
    }
}