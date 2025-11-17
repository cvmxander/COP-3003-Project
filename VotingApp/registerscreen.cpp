#include "registerscreen.h"
#include "ui_registerscreen.h"
#include <QSqlQuery>
#include <QRandomGenerator>
#include <QMessageBox>
#include <QDebug>
#include <QSqlError>

RegisterScreen::RegisterScreen(QWidget* parent)
    : QDialog(parent)
    , ui(new Ui::RegisterScreen)
{
    ui->setupUi(this);
    this->showMaximized();
}

RegisterScreen::~RegisterScreen() { delete ui; }

void RegisterScreen::on_buttonRegister_clicked() {
    QString firstName = ui->lineFirst->text();//gets info from user to store in database
    QString lastName = ui->lineLast->text();
    QString username = ui->lineUsername->text();
    QString password = ui->linePassword->text();
    QString confirm = ui->lineConfirm->text();

    if (password != confirm) { ui->label->setText("Passwords do not match"); return; } //ensures password matches

    QSqlQuery qry(votingInfo);
    qry.prepare("SELECT Username FROM votingInfo WHERE Username = :username");
    qry.bindValue(":username", username);
    qry.exec();
    if (qry.next()) { ui->label->setText("Username already exists"); return;}

    // Generate unique ID
    int randNum;
    bool exists;
    do {
        randNum = QRandomGenerator::global()->bounded(1000000);
        qry.prepare("SELECT ID FROM votingInfo WHERE ID = :id");
        qry.bindValue(":id", randNum);
        qry.exec();
        exists = qry.next();
    } while (exists); //generates unique ID

    qry.prepare("INSERT INTO votingInfo (FirstName, LastName, ID, Username, Password) "
                "VALUES (:first, :last, :id, :username, :password)");
    qry.bindValue(":first", firstName);
    qry.bindValue(":last", lastName);
    qry.bindValue(":id", randNum);
    qry.bindValue(":username", username);
    qry.bindValue(":password", password);

    if (!qry.exec()) qDebug() << "Insert failed:" << qry.lastError().text();
    else {
        QMessageBox::information(this,"Success","Registered successfully"); //ensures information is properly stored into sqlite
        close();
        if (parentWidget()) parentWidget()->show();
    }
}

void RegisterScreen::on_buttonExit_clicked()
{
    QApplication::quit();
}

