#pragma once
#include "votingscreen.h"
#include <QButtonGroup>
#include <QString>

namespace Ui { class SecondVotingScreen; }

class SecondVotingScreen : public VotingScreen {
    Q_OBJECT
public:
    explicit SecondVotingScreen(QWidget* parent = nullptr);
    ~SecondVotingScreen();

    void receiveData(const QString& username);

protected:
    void loadResults() override {} // not used for voting

private slots:
    void on_butSubmit_clicked();
    void on_butResults_clicked();

    void on_buttonExit_clicked();

private:
    Ui::SecondVotingScreen* ui;
    QButtonGroup* groupFinals;
    QString currentUser;
    void setCurrentUser(const QString& username) { currentUser = username; }
};
