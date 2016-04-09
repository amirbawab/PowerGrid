#pragma once

#include "HouseColor.h"
#include "Player.h"
#include <qwidget.h>
#include <QLineEdit>
#include <QHBoxLayout>
#include <string>
#include <vector>
#include <QPaintEvent>
#include <memory>
#include <QPushButton>

using std::string;

class PlayerConfigRowWidget : public QWidget {
    Q_OBJECT
private:
    QLineEdit *playerName;
    QHBoxLayout *hBoxLayout;
    QPushButton *houseButton;
    vector<shared_ptr<HouseColor>> houseColors;
    shared_ptr<Player> player;
    int houseColorIndex;
    void paintEvent(QPaintEvent *pe) override;
    
public:
    explicit PlayerConfigRowWidget(string name);
    ~PlayerConfigRowWidget();
    string GetPlayerName() const { return playerName->text().toStdString(); }
    void SetHouseColors(vector<shared_ptr<HouseColor>> &houseColors) { this->houseColors = houseColors; }
    void SetHouseColorIndex(int houseColorIndex);
    void SetPlayer(shared_ptr<Player> player) { this->player = player; }
    shared_ptr<Player> GetPlayer() const { return player; }
    shared_ptr<HouseColor> GetHouseColor() { return houseColors[houseColorIndex]; }
    void UpdatePlayer();

private slots:
    void OnHouseClick();
};

