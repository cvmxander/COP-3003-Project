#include "firstresults.h" //header for this class
#include "ui_firstresults.h" //defines ui elements from .ui file
#include "secondvotingscreen.h" //header for next window
#include <QSqlQuery> //used to manipulate sql commands
#include <QSqlError> //sql error messages
#include <QMap> //store data pairs
#include <QProgressBar>
#include <QLabel>
#include <QPixmap>
#include <QDebug> //outputting messages in terminal (similar to cout in normal c++)

FirstResults::FirstResults(QWidget *parent)
    : VotingScreen(parent) //calls base class constructor
    , ui(new Ui::FirstResults) //manages all widgets in .ui file
{
    ui->setupUi(this); //connects ui to this class
    this->showMaximized();
    loadResults(); //loads results as soon as window opens
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

FirstResults::~FirstResults() {
    delete ui; //destructor
}

void FirstResults::receiveData(const QString &username) {
    ui->labelName->setText(username); //receives username from previous screen
}

// Navigate to SecondVotingScreen
void FirstResults::on_pushButton_clicked() {
    close(); //closes current window
    secondVotingScreen = new SecondVotingScreen(this); //creates next window

    connect(this, &FirstResults::sendData,secondVotingScreen, &SecondVotingScreen::receiveData); //passes username to next screen

    emit sendData(ui->labelName->text()); //uses connect^^ to actually send username
    secondVotingScreen->show(); //displays next window
}

QMap<QString,double> FirstResults::calculatePercentages(const QString &column) {
    QMap<QString,double> map; //used to pair percentages with what they represent

    // Get total votes
    QSqlQuery qryTotal(votingInfo);
    if (!qryTotal.exec("SELECT COUNT(*) FROM votingInfo")) {
        qDebug() << "Failed to get total votes:" << qryTotal.lastError().text();
        return map;
    }

    int total = 0;
    if (qryTotal.next()) total = qryTotal.value(0).toInt();
    if (total == 0) return map;

    // Get votes per option
    QSqlQuery qry(votingInfo);
    qry.prepare(QString("SELECT %1, COUNT(%1) AS vote_count FROM votingInfo GROUP BY %1").arg(column));
    if (!qry.exec()) {
        qDebug() << "Failed to get votes:" << qry.lastError().text();
        return map;
    }

    while (qry.next()) {
        QString vote = qry.value(column).toString();
        int count = qry.value("vote_count").toInt();
        map[vote] = (static_cast<double>(count) / total) * 100.0;
    }

    return map;
}

// Load results and update UI
void FirstResults::loadResults() {
    auto results1 = calculatePercentages("FirstVote1");
    auto results2 = calculatePercentages("FirstVote2");
    auto results3 = calculatePercentages("FirstVote3");

    auto updateUI = [](const QMap<QString,double> &results,
                           const QMap<QString,QProgressBar*> &bars,
                           const QMap<QString,QLabel*> &labels) {
        for (auto it = results.begin(); it != results.end(); ++it) {
            const QString &vote = it.key();
            double percent = it.value();
            QString textPercent = QString("%1%").arg(percent, 0, 'f', 2);

            if (bars.contains(vote)) bars[vote]->setValue(percent);
            if (labels.contains(vote)) labels[vote]->setText(textPercent);
        }
    };

    // Category 1: Chicken
    updateUI(results1,
             {{"Fried Chicken", ui->progressFried}, //updates progress bar
              {"Crispy Chicken Sandwich", ui->progressCrispy},
              {"Chicken Parmesan", ui->progressParm},
              {"Grilled Chicken", ui->progressGrilled}},
             {{"Fried Chicken", ui->labelFried}, //updates label showing precise percentage because bars only take ints
              {"Crispy Chicken Sandwich", ui->labelCrispy},
              {"Chicken Parmesan", ui->labelParm},
              {"Grilled Chicken", ui->labelGrilled}});

    // Category 2: Beef
    updateUI(results2,
             {{"Cheeseburgers", ui->progressBurger},
              {"Steak", ui->progressSteak},
              {"Meatloaf", ui->progressMeatloaf},
              {"Beef Tacos", ui->progressTaco}},
             {{"Cheeseburgers", ui->labelBurger},
              {"Steak", ui->labelSteak},
              {"Meatloaf", ui->labelMeatloaf},
              {"Beef Tacos", ui->labelTaco}});

    // Category 3: Pasta
    updateUI(results3,
             {{"Spaghetti", ui->progressSpaghetti},
              {"Fettuccine Alfredo", ui->progressAlfredo},
              {"Lasagna", ui->progressLasagna},
              {"Macaroni and Cheese", ui->progressMac}},
             {{"Spaghetti", ui->labelSpaghetti},
              {"Fettuccine Alfredo", ui->labelAlfredo},
              {"Lasagna", ui->labelLasagna},
              {"Macaroni and Cheese", ui->labelMac}});
}

void FirstResults::on_buttonExit_clicked()
{
    QApplication::quit();
}

