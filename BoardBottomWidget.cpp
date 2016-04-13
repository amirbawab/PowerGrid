#include "BoardBottomWidget.h"
#include <QGraphicsOpacityEffect>
#include "Game.h"
#include <QPainter>
#include <QStyleOption>
#include <QPushButton>

// BoardBottomWidget class

BoardBottomWidget::BoardBottomWidget() {

    // Init components
    gridLayout = new QGridLayout();
    boardProfileWidget = new BoardProfileWidget();
    boardPlayerPowerPlantsWidget = new BoardPlayerPowerPlantsWidget();
    boardMessageWidget = new BoardMessageWidget();
    
    // Set layout
    setLayout(gridLayout);

    // Set id
    setObjectName("boardBottomWidget");

    // Add components
    gridLayout->addWidget(boardProfileWidget, 0, 0, Qt::AlignCenter);
    gridLayout->addWidget(boardPlayerPowerPlantsWidget, 0, 1, Qt::AlignCenter);
    gridLayout->addWidget(boardMessageWidget, 0, 2);

    // Add spaces
    gridLayout->setColumnStretch(0, 1);
    gridLayout->setColumnStretch(1, 4);
    gridLayout->setColumnStretch(2, 4);
}

BoardBottomWidget::~BoardBottomWidget() {
    delete boardProfileWidget;
    delete gridLayout;
}

void BoardBottomWidget::Refresh() const
{
    boardProfileWidget->Refresh();
    boardPlayerPowerPlantsWidget->Refresh();
    boardMessageWidget->Refresh();
}

// BoardProfileWidget class

BoardProfileWidget::BoardProfileWidget() {

    // Init components
    gridLayout = new QGridLayout();
    playerNameLabel = new QLabel();
    playerPictureLabel = new QLabel();
    playerElektroLabel = new QLabel();
    
    // Set id
    playerNameLabel->setObjectName("board_bottom_subtitle");
    playerElektroLabel->setObjectName("board_bottom_subtitle_s");
    
    // Set layout
    setLayout(gridLayout);

    // Add components
    gridLayout->addWidget(playerPictureLabel, 0, 0, Qt::AlignBottom | Qt::AlignCenter);
    gridLayout->addWidget(playerNameLabel, 1, 0, Qt::AlignTop | Qt::AlignCenter);
    gridLayout->addWidget(playerElektroLabel, 2, 0, Qt::AlignTop | Qt::AlignCenter);
}

BoardProfileWidget::~BoardProfileWidget() {
    delete playerNameLabel;
    delete playerPictureLabel;
    delete gridLayout;
}

void BoardProfileWidget::Refresh() const
{
    auto player = Game::getInstance().GetCurrentPlayer();
    playerNameLabel->setText(Game::getInstance().GetCurrentPlayer()->GetName().c_str());
    playerPictureLabel->setPixmap(QPixmap(Game::getInstance().GetCurrentPlayer()->GetColor()->getImage().c_str()));
    playerElektroLabel->setText((std::to_string(Game::getInstance().GetCurrentPlayer()->GetElektro()) + " Elektro").c_str());
}

// BoardResourcePowerPlantWiget

BoardResourcePowerPlantWiget::BoardResourcePowerPlantWiget() {
    
    // Init components
    gridLayout = new QGridLayout();

    // Configure components
    gridLayout->setContentsMargins(20, 40, 20, 40);
    gridLayout->setSpacing(0);

    // Set layout
    setLayout(gridLayout);
}

BoardResourcePowerPlantWiget::~BoardResourcePowerPlantWiget() {
    delete gridLayout;
}

void BoardResourcePowerPlantWiget::Refresh() {
    
    // Clear old components
    for (int i = 0; i < resourceLables.size(); i++) {
        gridLayout->removeWidget(resourceLables[i]);
        delete resourceLables[i];
        resourceLables.erase(resourceLables.begin() + i);
    }

    // Get resources for the player
    int coalNum = powerPlantCard->GetPlacedResource(COAL);
    int oilNum = powerPlantCard->GetPlacedResource(OIL);
    int garbageNum = powerPlantCard->GetPlacedResource(GARBAGE);
    int uraniumNum = powerPlantCard->GetPlacedResource(URANIUM);

    // Row col
    int row = 0;
    int col = 0;
    
    // Add resources to widget
    for (int i = 0; i < coalNum; i++) {
        QLabel *label = new QLabel();
        label->setPixmap(QPixmap(":/PowerGrid/Resources/resources/coal.png"));
        resourceLables.push_back(label);
        gridLayout->addWidget(label, row, col, Qt::AlignCenter);

        row += col == 2 ? 1 : 0;
        col = (col + 1) % 3;
    }

    for (int i = 0; i < oilNum; i++) {
        QLabel *label = new QLabel();
        label->setPixmap(QPixmap(":/PowerGrid/Resources/resources/oil.png"));
        resourceLables.push_back(label);
        gridLayout->addWidget(label, row, col, Qt::AlignCenter);

        row += col == 2 ? 1 : 0;
        col = (col + 1) % 3;
    }

    for (int i = 0; i < garbageNum; i++) {
        QLabel *label = new QLabel();
        label->setPixmap(QPixmap(":/PowerGrid/Resources/resources/garbage.png"));
        resourceLables.push_back(label);
        gridLayout->addWidget(label, row, col, Qt::AlignCenter);

        row += col == 2 ? 1 : 0;
        col = (col + 1) % 3;
    }

    for (int i = 0; i < uraniumNum; i++) {
        QLabel *label = new QLabel();
        label->setPixmap(QPixmap(":/PowerGrid/Resources/resources/uranium.png"));
        resourceLables.push_back(label);
        gridLayout->addWidget(label, row, col, Qt::AlignCenter);

        row += col == 2 ? 1 : 0;
        col = (col + 1) % 3;
    }
}

void BoardResourcePowerPlantWiget::paintEvent(QPaintEvent *e) {
    QPainter painter(this);
    painter.drawPixmap(0, 0, QPixmap(powerPlantCard->GetImagePath().c_str()).scaled(size()));
    QWidget::paintEvent(e);
}

void BoardResourcePowerPlantWiget::SetOpacity(float opacity) {
    QGraphicsOpacityEffect * effect = new QGraphicsOpacityEffect(this);
    effect->setOpacity(opacity);
    setGraphicsEffect(effect);
}

// BoardPlayerPowerPlantsWidget class

BoardPlayerPowerPlantsWidget::BoardPlayerPowerPlantsWidget() {
    
    // Init components
    gridLayout = new QGridLayout();

    // Set layout
    setLayout(gridLayout);
}

BoardPlayerPowerPlantsWidget::~BoardPlayerPowerPlantsWidget() {
    delete gridLayout;
}

void BoardPlayerPowerPlantsWidget::Refresh() {
    
    // Clear old components
    for (int i = 0; i < playerPowerPlantsWidgets.size(); i++) {
        gridLayout->removeWidget(playerPowerPlantsWidgets[i]);
        delete playerPowerPlantsWidgets[i];
        playerPowerPlantsWidgets.erase(playerPowerPlantsWidgets.begin() + i);
    }

    // Get power plants
    std::vector<std::shared_ptr<PowerPlantCard>> cards = Game::getInstance().GetCurrentPlayer()->GetPowerPlants();

    // Add power plants
    for (int i = 0; i < cards.size(); i++) {
        BoardResourcePowerPlantWiget *label = new BoardResourcePowerPlantWiget();
        label->setMinimumSize(200, 200);
        label->SetPowerPlantCard(cards[i]);
        label->Refresh();

        // Highlight if active power plant
        if (Game::getInstance().GetPowerPlantIndex() != i) {
            label->SetOpacity(0.5);
        }

        playerPowerPlantsWidgets.push_back(label);
        gridLayout->addWidget(label, 0, i, Qt::AlignCenter);
        label->repaint();
    }
}

// BoardMessageWidget class

BoardMessageWidget::BoardMessageWidget() {
    
    // Init components
    gridLayout = new QGridLayout();
    questionLabel = new QLabel();
    phase0Panel = new Phase0Panel();
    stepOnePanel = new StepOnePanel();
    stepTwoPanel = new StepTwoPanel();
    stepThreePanel = new StepThreePanel();
    stepFourPanel = new StepFourPanel();
    stepFivePanel = new StepFivePanel();
    stepsWidget = new QStackedWidget();

    // Add steps to stack
    stepsWidget->addWidget(phase0Panel);
    stepsWidget->addWidget(stepOnePanel);
    stepsWidget->addWidget(stepTwoPanel);
    stepsWidget->addWidget(stepThreePanel);
    stepsWidget->addWidget(stepFourPanel);
    stepsWidget->addWidget(stepFivePanel);

    // Set active stack
    stepsWidget->setCurrentWidget(stepOnePanel);

    // Set layout
    setLayout(gridLayout);

    // Set id
    questionLabel->setObjectName("board_message");

    // Add components
    gridLayout->addWidget(questionLabel, 0, 0, Qt::AlignLeft);
    gridLayout->addWidget(stepsWidget, 1, 0, Qt::AlignCenter);

    // Configure spacing
    gridLayout->setRowStretch(0, 1);
    gridLayout->setRowStretch(1, 3);
}

BoardMessageWidget::~BoardMessageWidget() {
    delete gridLayout;
}

void BoardMessageWidget::Refresh() const
{
    questionLabel->setText(QString::fromStdString(Game::getInstance().GetMessageText()));

    // Set the value for counter
    stepTwoPanel->GetCounterWidget()->SetValue(std::to_string(Game::getInstance().currentBid));
    stepThreePanel->GetCounterWidget()->SetValue(std::to_string(Game::getInstance().resourceAmount));

    switch (Game::getInstance().GetStep()) {
    case 1:
        stepsWidget->setCurrentWidget(stepOnePanel);
        break;
    case 2:
        stepsWidget->setCurrentWidget(stepTwoPanel);
        break;
    case 3:
        stepsWidget->setCurrentWidget(stepThreePanel);
        break;
    case 4:
        stepsWidget->setCurrentWidget(stepFourPanel);
        break;
    case 5:
        stepsWidget->setCurrentWidget(stepFivePanel);
        break;
    default:
        break;
    }

    // If phase 0, use phase0 panel
    if (Game::getInstance().GetPhase() == 0)
        stepsWidget->setCurrentWidget(phase0Panel);
}

// Phase0Panel class
Phase0Panel::Phase0Panel()
{
    // Init components
    gridLayout = new QGridLayout();
    okButton = new QPushButton("OK");
    
    // Set id
    okButton->setObjectName("player_button");
    
    // Set layout
    setLayout(gridLayout);

    // Add components
    gridLayout->addWidget(okButton, 0, 0, Qt::AlignCenter);
}

Phase0Panel::~Phase0Panel()
{
    delete okButton;
    delete gridLayout;
}

// StepOneButtons class

StepOnePanel::StepOnePanel() {

    // Init components
    gridLayout = new QGridLayout();
    okButton = new QPushButton("OK");
    
    // Set id
    okButton->setObjectName("player_button");
    
    // Set layout
    setLayout(gridLayout);

    // Add components
    gridLayout->addWidget(okButton, 0, 0, Qt::AlignCenter);
}

StepOnePanel::~StepOnePanel() {
    delete okButton;
    delete gridLayout;
}

// StepTwoButtons class

StepTwoPanel::StepTwoPanel() {
    
    // Init components
    gridLayout = new QGridLayout();
    okButton = new QPushButton("OK");
    skipButton = new QPushButton("PASS");
    counterWidget = new CounterWidget();

    // Set id
    okButton->setObjectName("player_button");
    skipButton->setObjectName("player_button");

    // Set layout
    setLayout(gridLayout);

    // Add components
    gridLayout->addWidget(counterWidget, 0, 0, 1, 2, Qt::AlignCenter);
    gridLayout->addWidget(skipButton, 1, 0, Qt::AlignCenter);
    gridLayout->addWidget(okButton, 1, 1, Qt::AlignCenter);
}

StepTwoPanel::~StepTwoPanel() {
    delete okButton;
    delete skipButton;
    delete gridLayout;
}

// StepThreeButtons class

StepThreePanel::StepThreePanel() {

    // Init components
    gridLayout = new QGridLayout();
    okButton = new QPushButton("OK");
    skipButton = new QPushButton("SKIP");
    counterWidget = new CounterWidget();

    // Set id
    okButton->setObjectName("player_button");
    skipButton->setObjectName("player_button");

    // Set layout
    setLayout(gridLayout);

    // Add components
    gridLayout->addWidget(counterWidget, 0, 0, 1, 2, Qt::AlignCenter);
    gridLayout->addWidget(skipButton, 1, 0, Qt::AlignCenter);
    gridLayout->addWidget(okButton, 1, 1, Qt::AlignCenter);
}

StepThreePanel::~StepThreePanel() {
    delete okButton;
    delete skipButton;
    delete gridLayout;
}

// StepFourButtons class

StepFourPanel::StepFourPanel() {

    // Init components
    gridLayout = new QGridLayout();
    okButton = new QPushButton("OK");
    skipButton = new QPushButton("END");
    
    // Set id
    okButton->setObjectName("player_button");
    skipButton->setObjectName("player_button");

    // Set layout
    setLayout(gridLayout);

    // Add components
    gridLayout->addWidget(skipButton, 1, 0, Qt::AlignCenter);
    gridLayout->addWidget(okButton, 1, 1, Qt::AlignCenter);
}

StepFourPanel::~StepFourPanel() {
    delete okButton;
    delete skipButton;
    delete gridLayout;
}

// StepFiveButtons class

StepFivePanel::StepFivePanel() {

    // Init components
    gridLayout = new QGridLayout();
    okButton = new QPushButton("OK");
    skipButton = new QPushButton("END");

    // Set id
    okButton->setObjectName("player_button");
    skipButton->setObjectName("player_button");

    // Set layout
    setLayout(gridLayout);

    // Add components
    gridLayout->addWidget(skipButton, 0, 0, Qt::AlignCenter);
    gridLayout->addWidget(okButton, 0, 1, Qt::AlignCenter);
    
}

StepFivePanel::~StepFivePanel() {
    delete okButton;
    delete skipButton;
    delete gridLayout;
}

// CounterWidget class

CounterWidget::CounterWidget() {
    
    // Init components
    gridLayout = new QGridLayout();
    plusBtn = new QPushButton("+");
    minusBtn = new QPushButton("-");
    numberLabel = new QLabel("0");

    // Set id
    minusBtn->setObjectName("counter_symbol");
    plusBtn->setObjectName("counter_symbol");
    numberLabel->setObjectName("counter_lable");

    // Connect
    connect(plusBtn, SIGNAL(clicked()), this, SLOT(increment()));
    connect(minusBtn, SIGNAL(clicked()), this, SLOT(decrement()));

    // Set layout
    setLayout(gridLayout);

    // Add components
    gridLayout->addWidget(minusBtn, 0, 0, Qt::AlignRight);
    gridLayout->addWidget(numberLabel, 0, 1, Qt::AlignCenter);
    gridLayout->addWidget(plusBtn, 0, 2, Qt::AlignLeft);
}

CounterWidget::~CounterWidget() {
    delete plusBtn;
    delete minusBtn;
    delete numberLabel;
    delete gridLayout;
}

/// SLOTS ///

void CounterWidget::increment()
{
    // Get value as int
    int val = std::stoi(numberLabel->text().toStdString());

    // MAX 500
    if (val < 500) val++;

    // Update
    numberLabel->setText(QString::fromStdString(std::to_string(val)));

    // Emit
    emit plusPressed(val);
}

void CounterWidget::decrement()
{
    // Get value as int
    int val = std::stoi(numberLabel->text().toStdString());

    // MIN 0
    if (val > 0) val--;

    // Update
    numberLabel->setText(QString::fromStdString(std::to_string(val)));

    // Emit
    emit minusPressed(val);
}

/// This method is required when Q_OBJECT is added
/// Without this method, the CSS will not be applied
void CounterWidget::paintEvent(QPaintEvent *pe) {
    QStyleOption o;
    o.initFrom(this);
    QPainter p(this);
    style()->drawPrimitive(
        QStyle::PE_Widget, &o, &p, this);
};