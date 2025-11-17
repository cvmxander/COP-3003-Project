#include "secondresults.h"
#include "ui_secondresults.h"
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>
#include <QPixmap>

SecondResults::SecondResults(QWidget *parent)
    : VotingScreen(parent)
    , ui(new Ui::SecondResults)
{
    ui->setupUi(this);
    this->showMaximized();
}

SecondResults::~SecondResults() {
    delete ui;
}

void SecondResults::loadResults() {
    // Total votes
    QSqlQuery totalQuery(votingInfo);
    if (!totalQuery.exec("SELECT COUNT(*) FROM votingInfo")) return;

    int total = 0;
    if (totalQuery.next()) total = totalQuery.value(0).toInt();
    if (total == 0) return; //gets total number of votes

    // Candidate votes
    QSqlQuery qry;
    qry.prepare("SELECT Party, Candidate, COUNT(Candidate) AS vote_count FROM votingInfo "
                "GROUP BY Candidate ORDER BY vote_count DESC");
    if (!qry.exec()) {
        qDebug() << "Query failed: " << qry.lastError().text();
    }
    QString candidate, party, highestCandidate;
    double highestVote = 0;
    while (qry.next()) {
        candidate = qry.value("Candidate").toString(); //gets candidate from sqlite
        party = qry.value("Party").toString(); //gets party from sqlite
        int count = qry.value("vote_count").toInt();
        double percent = (static_cast<double>(count)/total)*100.0; //calculates percentage for progress bar
        QString textPercent = QString("%1%").arg(percent, 0, 'f', 2); //creates string to put percentage in label
        if (party == "Chicken") { //ensures values are matched to the proper ui object
            ui->labelChicken->setText(candidate);
            ui->progressChicken->setValue(percent);
            ui->labelChickenPercent->setText(textPercent);
        } else if (party == "Beef") {
            ui->labelBeef->setText(candidate);
            ui->progressBeef->setValue(percent);
            ui->labelBeefPercent->setText(textPercent);
        } else {
            ui->labelPasta->setText(candidate);
            ui->progressPasta->setValue(percent);
            ui->labelPastaPercent->setText(textPercent);
        }
        if (percent > highestVote) {
            highestVote = percent;
            highestCandidate = candidate;
        }
    }
    ui->labelWinner->setText(highestCandidate);

    QPixmap chickenOrig(":/icons/img/chicken-icon.png");
    QPixmap chicken = chickenOrig.scaled(50, 50, Qt::KeepAspectRatio);
    ui->chickenPic->setPixmap(chicken);
    QPixmap beefOrig(":/icons/img/beef-icon.png");
    QPixmap beef = beefOrig.scaled(50, 50, Qt::KeepAspectRatio);
    ui->beefPic->setPixmap(beef);
    QPixmap pastaOrig(":/icons/img/pasta-icon.png");
    QPixmap pasta = pastaOrig.scaled(50, 50, Qt::KeepAspectRatio);
    ui->pastaPic->setPixmap(pasta);
}

void SecondResults::on_pushButton_clicked()
{
    close();
    QApplication::quit();
}

