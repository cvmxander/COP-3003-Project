#include "firstvotingscreen.h"
#include "ui_firstvotingscreen.h"
#include "firstresults.h"
#include <QMessageBox>
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>
#include <QPixmap>

FirstVotingScreen::FirstVotingScreen(QWidget* parent)
    : VotingScreen(parent)
    , ui(new Ui::FirstVotingScreen)
{
    ui->setupUi(this);
    this->showMaximized();

    groupChicken = new QButtonGroup(this); //groups buttons so that only one choice can be selected per category
    groupChicken->addButton(ui->butFriedChicken);
    groupChicken->addButton(ui->butChickenSandwich);
    groupChicken->addButton(ui->butChickenParm);
    groupChicken->addButton(ui->butGrilledChicken);

    groupBeef = new QButtonGroup(this);
    groupBeef->addButton(ui->butBurger);
    groupBeef->addButton(ui->butSteak);
    groupBeef->addButton(ui->butMeatloaf);
    groupBeef->addButton(ui->butTaco);

    groupPasta = new QButtonGroup(this);
    groupPasta->addButton(ui->butSpaghetti);
    groupPasta->addButton(ui->butAlfredo);
    groupPasta->addButton(ui->butLasagna);
    groupPasta->addButton(ui->butMacNCheese);

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

FirstVotingScreen::~FirstVotingScreen() { delete ui; }

void FirstVotingScreen::receiveData(const QString& username) {
    setCurrentUser(username);
    QSqlQuery qry;
    qry.prepare("SELECT FirstName FROM votingInfo WHERE Username = :username");
    qry.bindValue(":username", username);
    if (!qry.exec()) qDebug() << "Could not find user: " << qry.lastError().text();
    if (qry.next()) {
        QString name = qry.value("FirstName").toString();
        ui->labelName->setText("Welcome " + name);
    }
}

void FirstVotingScreen::insertVote(QString vote[]) {
    QSqlQuery qry(votingInfo);
    QString voteInfo;
    for (int i = 0; i < 3; i++) {
        QString column = "FirstVote" + QString::number(i+1);
        qry.prepare(QString("UPDATE votingInfo SET %1 = :vote WHERE Username = :username").arg(column)); //inserts user vote into sqlite per category
        voteInfo = vote[i];
        qry.bindValue(":vote", voteInfo);
        qry.bindValue(":username", currentUser);
        if (!qry.exec()) qDebug() << "Update failed:" << qry.lastError().text();
    }
}

void FirstVotingScreen::on_submitButton_clicked() {
    if (!groupChicken->checkedButton() || !groupBeef->checkedButton() || !groupPasta->checkedButton()) {
        QMessageBox::warning(this,"Error","Select all categories before submitting!");
        return;
    }

    QString votes[3] = {groupChicken->checkedButton()->text(), groupBeef->checkedButton()->text(), groupPasta->checkedButton()->text()};
    QString *ptr = votes; //gets string from buttons and stores in an array to pass into insertVote() to insert into sqlite

    insertVote(ptr);

    ui->labelStatus->setText("Votes Submitted");
}

void FirstVotingScreen::on_butResults_clicked() {
    FirstResults* results = new FirstResults(nullptr); //opens next window
    connect(this, &FirstVotingScreen::sendDataToResults,
            results, &FirstResults::receiveData); //connects functions
    emit sendDataToResults(currentUser); //calls connected functions
    results->show(); //shows next window
    results->loadResults();
    close(); //closes current window
}

void FirstVotingScreen::on_buttonExit_clicked()
{
    QApplication::quit();
}

