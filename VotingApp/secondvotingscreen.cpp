#include "secondvotingscreen.h"
#include "ui_secondvotingscreen.h"
#include "secondresults.h"
#include <QSqlQuery>
#include <QDebug>
#include <QSqlError>
#include <QMessageBox>
#include <QPixmap>

SecondVotingScreen::SecondVotingScreen(QWidget* parent)
    : VotingScreen(parent)
    , ui(new Ui::SecondVotingScreen)
{
    ui->setupUi(this);

    groupFinals = new QButtonGroup(this); //allows only one option to be selected
    groupFinals->addButton(ui->butChicken, 1);
    groupFinals->addButton(ui->butBeef, 2);
    groupFinals->addButton(ui->butPasta, 3);

    QSqlQuery qry;
    qry.prepare("SELECT FirstVote1, COUNT(FirstVote1) AS vote_count FROM votingInfo "
                "GROUP BY FirstVote1 ORDER BY vote_count DESC LIMIT 1");
    if (!qry.exec()) {
        qDebug() << "Query failed: " << qry.lastError().text();
    }
    QString topCandidate;
    if (qry.next()) {
        topCandidate = qry.value("FirstVote1").toString(); //creates poll for second stage using top results from stage one
    }
    ui->butChicken->setText(topCandidate);
    qry.prepare("SELECT FirstVote2, COUNT(FirstVote2) AS vote_count FROM votingInfo "
                "GROUP BY FirstVote2 ORDER BY vote_count DESC LIMIT 1");
    if (!qry.exec()) {
        qDebug() << "Query failed: " << qry.lastError().text();
    }
    QString topCandidate2;
    if (qry.next()) {
        topCandidate2 = qry.value("FirstVote2").toString(); //creates poll for second stage using top results from stage one
    }
    ui->butBeef->setText(topCandidate2);
    qry.prepare("SELECT FirstVote3, COUNT(FirstVote3) AS vote_count FROM votingInfo "
                "GROUP BY FirstVote3 ORDER BY vote_count DESC LIMIT 1");
    if (!qry.exec()) {
        qDebug() << "Query failed: " << qry.lastError().text();
    }
    QString topCandidate3;
    if (qry.next()) {
        topCandidate3 = qry.value("FirstVote3").toString(); //creates poll for second stage using top results from stage one
    }
    ui->butPasta->setText(topCandidate3);

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

SecondVotingScreen::~SecondVotingScreen() { delete ui; }

void SecondVotingScreen::receiveData(const QString& username) {
    setCurrentUser(username);
    ui->labelName->setText("Welcome " + username);
}

void SecondVotingScreen::on_butSubmit_clicked() {
    int selected = groupFinals->checkedId();
    if (selected == -1) {
        QMessageBox::warning(this,"Error","Select an option before submitting");
        return;
    }

    QString vote;
    if (selected == 1) vote = ui->butChicken->text();
    else if (selected == 2) vote = ui->butBeef->text();
    else if (selected == 3) vote = ui->butPasta->text();

    QSqlQuery qry(votingInfo);
    qry.prepare("UPDATE votingInfo SET Candidate = :vote WHERE Username = :username");
    qry.bindValue(":vote", vote);
    qry.bindValue(":username", currentUser);

    if (!qry.exec()) qDebug() << "Update failed:" << qry.lastError().text();
    else ui->labelStatus->setText("Vote Submitted!"); //submits candidate vote into sqlite

    if (selected == 1) {
        qry.prepare("UPDATE votingInfo SET Party = :vote WHERE Username = :username"); //submits party for the candidate into sqlite
        qry.bindValue(":vote", "Chicken");
        qry.bindValue(":username", currentUser);

        if (!qry.exec()) qDebug() << "Update failed:" << qry.lastError().text();
    }else if (selected == 2) {
        qry.prepare("UPDATE votingInfo SET Party = :vote WHERE Username = :username");
        qry.bindValue(":vote", "Beef");
        qry.bindValue(":username", currentUser);

        if (!qry.exec()) qDebug() << "Update failed:" << qry.lastError().text();
    }else if (selected == 3) {
        qry.prepare("UPDATE votingInfo SET Party = :vote WHERE Username = :username");
        qry.bindValue(":vote", "Pasta");
        qry.bindValue(":username", currentUser);

        if (!qry.exec()) qDebug() << "Update failed:" << qry.lastError().text();
    }
}

void SecondVotingScreen::on_butResults_clicked() {
    SecondResults* results = new SecondResults(this);
    results->show();
    results->loadResults();
    close();
}

void SecondVotingScreen::on_buttonExit_clicked()
{
    QApplication::quit();
}

