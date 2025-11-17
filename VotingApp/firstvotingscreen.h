#pragma once //alternative to #ifndef/#define
#include "votingscreen.h" //header for base class
#include "firstresults.h" //header for next window
#include <QButtonGroup> //required class for manipulating button groups
#include <QString> //required class for manipulating strings

namespace Ui { class FirstVotingScreen; }

class FirstVotingScreen : public VotingScreen {
    Q_OBJECT
public:
    explicit FirstVotingScreen(QWidget* parent = nullptr); //constructor to build ui from .ui file
    ~FirstVotingScreen(); //destructor

    void receiveData(const QString& username); //receives username from previous screen

protected:
    void loadResults() override {} //overrides from base class

signals:
    void sendDataToResults(const QString &username); //passes username to next window

private slots:
    void on_submitButton_clicked(); //connects clicking button to code
    void on_butResults_clicked(); // ^^

    void on_buttonExit_clicked();

private:
    Ui::FirstVotingScreen* ui; //ptr to this .ui
    QButtonGroup* groupChicken; //button group ptr
    QButtonGroup* groupBeef; // ^^
    QButtonGroup* groupPasta; // ^^

    QString currentUser; //stores active username
    FirstResults *firstResults = nullptr; //ptr to next window
    void insertVote(QString vote[]); //method to insert votes into database
    void setCurrentUser(const QString& username) { currentUser = username; } //assigns current username
};
