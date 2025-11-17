#pragma once
#include <QDialog> //class for popup windows
#include <QSqlDatabase> //used to communicate with database

namespace Ui { class RegisterScreen; }

class RegisterScreen : public QDialog {
    Q_OBJECT
public:
    explicit RegisterScreen(QWidget* parent = nullptr);
    ~RegisterScreen();

private slots:
    void on_buttonRegister_clicked();

    void on_buttonExit_clicked();

private:
    Ui::RegisterScreen* ui;
    QSqlDatabase votingInfo = QSqlDatabase::database();
};
