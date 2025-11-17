#include <QApplication>
#include <QSqlDatabase>
#include <QSqlError>
#include <QDebug>
#include "signinscreen.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    // 1. Set up SQLite database connection globally
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("C:/Users/alexj/OneDrive - Florida Gulf Coast University/FGCU Coursework/Programming II Fall 2025/Sqlite Databases/votingInfo.db"); // this is the path on my pc, make sure you change it to match
    if (!db.open()) {
        qDebug() << "Failed to open database:" << db.lastError().text();
        return -1; // Exit if DB cannot be opened
    }

    // 2. Show the Sign In screen
    SignInScreen w;
    w.show();

    return a.exec();
}
