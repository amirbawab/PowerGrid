#pragma once

#include <QWidget>
#include <QGridLayout>
#include <QLabel>
#include "Player.h"
#include <memory>
#include <QStackedWidget>
#include <QPushButton>
#include <map>

class BoardProfileWidget : public QWidget {
public:
    BoardProfileWidget();
    ~BoardProfileWidget();
    void Refresh() const;
private:
    QLabel *playerNameLabel;
    QLabel *playerPictureLabel;
    QLabel *playerElektroLabel;
    QGridLayout *gridLayout;
};

class BoardResourcePowerPlantWidget : public QPushButton {
public:
    BoardResourcePowerPlantWidget();
    ~BoardResourcePowerPlantWidget();
    void SetPowerPlantCard(shared_ptr<PowerPlantCard> powerPlantCard) { this->powerPlantCard = powerPlantCard; }
    void Refresh();
    void SetOpacity(float opacity);
    QLabel *textLabel;
    vector<int> resourceAmount;
    std::map<QPushButton*, int> resourceMap;
    std::map<QPushButton*, bool> resourceSelectedMap;
    bool enabled;
private:
    shared_ptr<PowerPlantCard> powerPlantCard;
    QGridLayout *gridLayout;
    vector<QPushButton*> resourceLables;
};

class BoardPlayerPowerPlantsWidget : public QWidget {
public:
    BoardPlayerPowerPlantsWidget();
    ~BoardPlayerPowerPlantsWidget();
    std::shared_ptr<PowerPlantCard> GetSelectedCard() { return selectedCard; }
    void Refresh();
    BoardResourcePowerPlantWidget* selectedWidget;
private:
    std::shared_ptr<PowerPlantCard> selectedCard;
    int selectedWidgetIndex;
    vector<BoardResourcePowerPlantWidget*> playerPowerPlantsWidgets;
    QGridLayout *gridLayout;
};

class CounterWidget : public QWidget {
    Q_OBJECT
public:
    CounterWidget();
    ~CounterWidget();
    QLabel *GetNumberLabel() const { return numberLabel; }
    int GetValueAsInt() const { return stoi(numberLabel->text().toStdString()); }
    QPushButton *GetPlusBtn() const { return plusBtn; }
    QPushButton *GetMinusBtn() const{ return minusBtn; }
    void SetValue(string text) const { numberLabel->setText(text.c_str()); }

private:
    QGridLayout *gridLayout;
    QPushButton *plusBtn, *minusBtn;
    QLabel *numberLabel;
    void paintEvent(QPaintEvent *pe) override;
public slots:
    void increment();
    void decrement();
signals:
    void plusPressed(int);
    void minusPressed(int);
};

class Phase0Panel : public QWidget {
public:
    Phase0Panel();
    ~Phase0Panel();
    QPushButton *GetOkButton() const { return okButton; }
private:
    QGridLayout *gridLayout;
    QPushButton *okButton;
};

class StepOnePanel : public QWidget {
public:
    StepOnePanel();
    ~StepOnePanel();
    QPushButton *GetOkButton() const { return okButton; }
private:
    QGridLayout *gridLayout;
    QPushButton *okButton;
};

class StepTwoPanel : public QWidget {
public:
    StepTwoPanel();
    ~StepTwoPanel();
    CounterWidget *GetCounterWidget() const { return counterWidget; }
    QPushButton *GetOkButton() const { return okButton; }
    QPushButton *GetSkipButton() const { return skipButton; }
private:
    QGridLayout *gridLayout;
    QPushButton *okButton;
    QPushButton *skipButton;
    CounterWidget *counterWidget;
};

class StepThreePanel : public QWidget {
Q_OBJECT
public:
    StepThreePanel();
    ~StepThreePanel();
    CounterWidget *GetCounterWidget() const { return counterWidget; }
    QPushButton *GetOkButton() const { return okButton; }
    QPushButton *GetSkipButton() const { return skipButton; }
private:
    QGridLayout *gridLayout;
    QPushButton *okButton;
    QPushButton *skipButton;
    CounterWidget *counterWidget;
};

class StepFourPanel : public QWidget {
public:
    StepFourPanel();
    ~StepFourPanel();
    QPushButton *GetOkButton() const { return okButton; }
    QPushButton *GetSkipButton() const { return skipButton; }
private:
    QGridLayout *gridLayout;
    QPushButton *okButton;
    QPushButton *skipButton;
};

class StepFivePanel : public QWidget {
public:
    StepFivePanel();
    ~StepFivePanel();
    QPushButton *GetOkButton() const { return okButton; }
    QPushButton *GetSkipButton() const { return skipButton; }
private:
    QGridLayout *gridLayout;
    QPushButton *okButton;
    QPushButton *skipButton;
};

class BoardMessageWidget : public QWidget {
public:
    BoardMessageWidget();
    ~BoardMessageWidget();
    void Refresh() const;

    QLabel         *GetQuestionLabel()  const { return questionLabel; }
    Phase0Panel    *GetPhase0Panel()    const { return phase0Panel; }
    StepOnePanel   *GetStepOnePanel()   const { return stepOnePanel; }
    StepTwoPanel   *GetStepTwoPanel()   const { return stepTwoPanel; }
    StepThreePanel *GetStepThreePanel() const { return stepThreePanel; }
    StepFourPanel  *GetStepFourPanel()  const { return stepFourPanel; }
    StepFivePanel  *GetStepFivePanel()  const { return stepFivePanel; }
private:
    QGridLayout *gridLayout;
    QLabel *questionLabel;
    Phase0Panel *phase0Panel;
    StepOnePanel *stepOnePanel;
    StepTwoPanel *stepTwoPanel;
    StepThreePanel *stepThreePanel;
    StepFourPanel *stepFourPanel;
    StepFivePanel *stepFivePanel;
    QStackedWidget *stepsWidget;
};

class BoardBottomWidget : public QWidget {
public:
    BoardBottomWidget();
    ~BoardBottomWidget();
    void Refresh() const;
    BoardMessageWidget *GetBoardMessage() const { return boardMessageWidget; }
    BoardPlayerPowerPlantsWidget *GetBoardPlayerPowerPlantsWidget() { return boardPlayerPowerPlantsWidget; }
private:
    QGridLayout *gridLayout;
    BoardProfileWidget *boardProfileWidget;
    BoardPlayerPowerPlantsWidget *boardPlayerPowerPlantsWidget;
    BoardMessageWidget *boardMessageWidget;
};

