#include "addsession.h"
#include "ui_addsession.h"

AddSession::AddSession(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::AddSession)
{
    ui->setupUi(this);

    ui->hall_num_spinBox->setRange(MIN_HALL_NUM, MAX_HALL_NUM);
    sess_id = 0;
}

AddSession::~AddSession()
{
    delete ui;
}


void AddSession::on_confirm_pushButton_clicked()
{
    sess_id++;

    QString insertQuery = "INSERT INTO session (session_id, film_id, hall, date, time, duration) "
                          "VALUES (:session_id_value, :film_id_value, :hall_value, :date_value, :time_value, :duration_value)";

    QSqlQuery query;
    query.prepare(insertQuery);

    query.bindValue(":session_id_value", sess_id); // session_id_value - значение для session_id
    query.bindValue(":film_id_value", db_id); // film_id_value - значение для film_id
    query.bindValue(":hall_value", ui->hall_num_spinBox->value()); // hall_value - значение для hall
    query.bindValue(":date_value", ui->calendarWidget->selectedDate()); // date_value - значение для date
    query.bindValue(":time_value", ui->film_timeEdit->time()); // time_value - значение для time
    query.bindValue(":duration_value", ui->duration_timeEdit->time()); // duration_value - значение для duration

    if(query.exec()) {
        qDebug() << "Данные успешно добавлены в таблицу session";
    } else {
        qDebug() << "Ошибка при добавлении данных в таблицу session:" << query.lastError().text();
    }

    this->close();      // Закрываем окно
    emit mainWindow();
}
