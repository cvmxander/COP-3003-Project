#ifndef FIRSTRESULTS_H
#define FIRSTRESULTS_H

#include "votingscreen.h" //header file for parent class
#include "secondvotingscreen.h" //header for pointer class for next window
#include <QMap> //used in calculatePercentages()
#include <QProgressBar> //used to manipulate progress bars in ui
#include <QLabel> //used to manipulate labels in ui

namespace Ui {
class FirstResults;
}

class FirstResults : public VotingScreen //declares FirstResults as a subclass of VotingScreen
{
    Q_OBJECT //qt requirement
public:
    explicit FirstResults(QWidget *parent = nullptr); //constructor
    ~FirstResults() override; //virtual destructor overriding the base class destructor

    void loadResults() override; //virtual function overriding loadResults() from base class

public slots:
    void receiveData(const QString &username); //receives username from previous window

signals:
    void sendData(const QString &username); //sends username to next window

private slots:
    void on_pushButton_clicked(); //defines method for clicking a button on the ui

    void on_buttonExit_clicked();

private:
    Ui::FirstResults *ui; //ptr to this class's ui
    SecondVotingScreen *secondVotingScreen = nullptr; //ptr to next window
    QMap<QString,double> calculatePercentages(const QString &column); //defined method used in .cpp file
};

#endif // FIRSTRESULTS_H
