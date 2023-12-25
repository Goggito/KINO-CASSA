#ifndef TICKETS_H
#define TICKETS_H

#include <QDialog>
#include <QSqlDatabase>
#include <QtSql>
#include <QtDebug>
#include <QFileInfo>
#include <QPushButton>
#include <QVBoxLayout>

#define DB_TICKET_NAME "ticket_db.db"

#define VVOD_FAM "^[А-Я][а-я]+([а-я])"
#define HALL_ROWS 4
#define HALL_SEATS 10
#define HALL_BUTT_WIDTH 35
#define HALL_BUTT_HEIGH 20
#define HALL_BUTT_X 145
#define HALL_BUTT_Y 170

namespace Ui {
class Tickets;
}

class Tickets : public QDialog
{
    Q_OBJECT

public:
    QSqlDatabase mydb;

    void connClose();
    bool connOpen();
    void windowPreparation(QString name);

    void clearCalendarMark();
    QList<int> myQStringToInt(QString myString);

    int db_id;

public:
    explicit Tickets(QWidget *parent = 0);
    ~Tickets();

    QString name;
    QString author;

signals:
    void firstWindow();  // Сигнал для активации первого окна

public slots:
    void catchInfo(const int &id);


private slots:
    void on_pushButton_clicked();
    bool on_pushButton_2_clicked();
    void on_pushButton_3_clicked();

    void on_calendarWidget_selectionChanged();

    void on_comboBox_Time_currentIndexChanged(int index);

private:
    Ui::Tickets *ui;
    QPushButton * SeatsARR[HALL_ROWS][HALL_SEATS];


protected:
    void closeEvent(QCloseEvent *event) override;
};

#endif // TICKETS_H
