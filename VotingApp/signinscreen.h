#pragma once
#include <QMainWindow>
#include <QSqlDatabase>

namespace Ui { class SignInScreen; }

class SignInScreen : public QMainWindow {
    Q_OBJECT
public:
    explicit SignInScreen(QWidget* parent = nullptr);
    ~SignInScreen();

private slots:
    void on_buttonSignIn_clicked();
    void on_buttonRegister_clicked();
    void onActionTriggered();

    void on_buttonExit_clicked();

private:
    Ui::SignInScreen* ui;
    QSqlDatabase votingInfo;
    int actionNum = 1;
    QAction *toggleAction;
};
