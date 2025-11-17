#include "signinscreen.h"
#include "ui_signinscreen.h"
#include "firstvotingscreen.h"
#include "registerscreen.h"
#include <QMessageBox>
#include <QSqlQuery>
#include <QDebug>
#include <QSqlError>

SignInScreen::SignInScreen(QWidget* parent)
    : QMainWindow(parent)
    , ui(new Ui::SignInScreen)
{
    ui->setupUi(this);
    this->showMaximized();
    votingInfo = QSqlDatabase::database();
    if (!votingInfo.open()) this->statusBar()->showMessage("Failed to connect");
    else this->statusBar()->showMessage("Connected"); //ensures database is open and connected in order to properly sign in

    toggleAction = new QAction(QIcon(":/icons/img/eye-icon-closed.png"), "Show/Hide Password", this); //icon for show password
    ui->linePassword->addAction(toggleAction, QLineEdit::TrailingPosition);

    connect(toggleAction, &QAction::triggered, this, &SignInScreen::onActionTriggered);
}

SignInScreen::~SignInScreen() { delete ui;}

void SignInScreen::onActionTriggered() {
    if (actionNum == 1) {
        ui->linePassword->setEchoMode(QLineEdit::Normal);
        toggleAction->setIcon(QIcon(":/icons/img/eye-icon-open.png"));
        actionNum = 0;
    } else {
        ui->linePassword->setEchoMode(QLineEdit::Password);
        toggleAction->setIcon(QIcon(":/icons/img/eye-icon-closed.png"));
        actionNum = 1;
    }
}

void SignInScreen::on_buttonSignIn_clicked() {
    QString username = ui->lineUsername->text();
    QString password = ui->linePassword->text();

    QSqlQuery qry(votingInfo);
    qry.prepare("SELECT * FROM votingInfo WHERE Username = :username AND Password = :password");
    qry.bindValue(":username", username);
    qry.bindValue(":password", password);
    if (!qry.exec()) { qDebug() << qry.lastError().text(); return; }

    if (qry.next()) { //if login is correct, the next window opens and allows the user to submit votes linked to their account
        FirstVotingScreen* screen = new FirstVotingScreen(nullptr);
        screen->receiveData(username);
        screen->show();
        hide();
    } else {
        QMessageBox::warning(this,"Login Failed","Username or password incorrect");
    }
}

void SignInScreen::on_buttonRegister_clicked() {
    RegisterScreen* reg = new RegisterScreen(this);
    reg->show();
    hide();
}

void SignInScreen::on_buttonExit_clicked()
{
    QApplication::quit();
}

