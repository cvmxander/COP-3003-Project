#pragma once
#include <QDialog>
#include <QSqlDatabase>

class ResultsScreen : public QDialog {
    Q_OBJECT
public:
    explicit ResultsScreen(QWidget* parent = nullptr);
    virtual ~ResultsScreen() = default; //virtual destructor to be overriden in derived classes

    virtual void loadResults() = 0; //abstract so derived classes must override

protected:
    QSqlDatabase votingInfo; //connected database
};
