#include "resultsscreen.h"

ResultsScreen::ResultsScreen(QWidget* parent)
    : QDialog(parent)
{
    votingInfo = QSqlDatabase::database();
}
