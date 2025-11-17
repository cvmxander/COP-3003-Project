#ifndef VOTINGSCREEN_H
#define VOTINGSCREEN_H

#include <QDialog>
#include <QSqlDatabase>

class VotingScreen : public QDialog
{
    Q_OBJECT
public:
    explicit VotingScreen(QWidget *parent = nullptr);
    virtual ~VotingScreen() {}

    // Virtual function to be overridden by derived classes
    virtual void loadResults() = 0;

protected:
    QSqlDatabase votingInfo = QSqlDatabase::database(); // shared DB connection
};

#endif // VOTINGSCREEN_H
