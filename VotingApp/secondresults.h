#ifndef SECONDRESULTS_H
#define SECONDRESULTS_H

#include "votingscreen.h"
#include <QMap>
#include <QProgressBar>
#include <QLabel>

namespace Ui {
class SecondResults;
}

class SecondResults : public VotingScreen
{
    Q_OBJECT
public:
    explicit SecondResults(QWidget *parent = nullptr);
    ~SecondResults() override;

    void loadResults() override;

private slots:
    void on_pushButton_clicked();

private:
    Ui::SecondResults *ui;
};

#endif // SECONDRESULTS_H
