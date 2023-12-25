#include "tickets.h"
#include "ui_tickets.h"
#include <QMessageBox>
#include <iostream>
#include <QCalendarWidget>
#include <QCloseEvent>
#include <QString>


Tickets::Tickets(QWidget *parent) : QDialog(parent),
    ui(new Ui::Tickets)
{
    ui->setupUi(this);

    //проверка на ввод фамилии
    QRegExp proverka1(VVOD_FAM);
    QRegExpValidator *valiName = new QRegExpValidator(proverka1, this);
    ui->txt_surname->setValidator(valiName);

    int seats_num = 1;
    const char * seats_num_char;

    for (int x = 0; x < HALL_ROWS; x++)
    {
        for (int y = 0; y < HALL_SEATS; y++)
        {
            QString seats_qstr = QString::number(seats_num);
            std::string seats_str = std::to_string(seats_num);
            seats_num_char = seats_str.c_str();

            SeatsARR[x][y] = new QPushButton(seats_num_char, this);
            SeatsARR[x][y]->setCheckable(true);
            ui->SeatsLayout->addWidget(SeatsARR[x][y], x + 1, y + 1);

            connect(SeatsARR[x][y],
                    &QPushButton::clicked,
                    this,
                    [=](){ui->txt_places->insert(seats_qstr); ui->txt_places->insert(" "); SeatsARR[x][y]->setEnabled(false);});


            qDebug() << "Ticket cicle [" << " " << x << " " << y << " " << seats_num_char << "]";
            seats_num++;
        }
    }
}

Tickets::~Tickets()
{
    delete ui;
}


void Tickets::catchInfo(const int &id)
{
    db_id = id;
}

/* Перевод строки с номерами мест в массив int
 * */
QList<int> Tickets::myQStringToInt(QString myString)
{
    QStringList stringList = myString.split(" "); // Разделение строки на подстроки по пробелам

    QList<int> intList; // Создание списка для хранения целых чисел

    // Преобразование каждой подстроки в целое число и добавление в список intList
    for(const QString& str : stringList) {
        bool ok;
        int number = str.toInt(&ok);
        if (ok) {
            intList.append(number);
        } else {
            // Обработка ошибок при преобразовании
            qDebug() << "Ошибка преобразования строки в число:" << str;
        }
    }

    return intList;
}

/* Очистка кадендаря
 * */
void Tickets::clearCalendarMark()
{
    QSqlQuery query;
    query.prepare("SELECT hall, date, time, duration FROM session WHERE film_id = :id");
    query.bindValue(":id", db_id); // Привязка значения film_id

    // Выполнение запроса
    if(query.exec()) {
        while(query.next()) {
            QDate date = query.value("date").toDate(); // Получение значения поля date

            QTextCharFormat textFormat;
            textFormat.setForeground(Qt::white);
            ui->calendarWidget->setDateTextFormat(date, textFormat);

            qDebug() << "date:" << date ;
        }
    } else {
        qDebug() << "Ошибка выполнения запроса:" << query.lastError().text();
    }
    qDebug() << "Ticket cicle start _id = " << db_id << " " << name;
}

/* Получение данных из БД и их подстановка
 * */
void Tickets::windowPreparation(QString name)
{
    QSqlQuery query1;
    query1.prepare("SELECT hall, date, time, duration FROM session WHERE film_id = :id");
    query1.bindValue(":id", db_id); // Привязка значения film_id

    if(query1.exec()) {
        while(query1.next()) {
            int hall = query1.value("hall").toInt(); // Получение значения поля hall
            QDate date = query1.value("date").toDate(); // Получение значения поля date
            QTime time = query1.value("time").toTime(); // Получение значения поля time
            QTime duration = query1.value("duration").toTime(); // Получение значения поля duration

            QTextCharFormat textFormat;
            textFormat.setForeground(Qt::yellow);
            ui->calendarWidget->setDateTextFormat(date, textFormat);

            qDebug() << "hall:" << hall << "date:" << date << "time:" << time << "duration:" << duration;
            // Делайте что-то с полученными данными
        }
    } else {
        qDebug() << "Ошибка выполнения запроса:" << query1.lastError().text();
    }
    qDebug() << "Ticket cicle start _id = " << db_id << " " << name;

    ui->txt_film_name->insert(name);
}

/* Обновление времени при нажатии на определенную дату
 * */
void Tickets::on_calendarWidget_selectionChanged()
{
    // Очистка выбора времени
    ui->comboBox_Time->clear();

    // Заполнение новым временем
    QDate selectedDate = ui->calendarWidget->selectedDate(); // Получение выбранной даты
    qDebug() << "Выбранная дата изменена:" << selectedDate.toString("yyyy-MM-dd");

    QSqlQuery query;
    query.prepare("SELECT session_id, time FROM session WHERE film_id = :filmId AND date = :date");
    query.bindValue(":filmId", db_id);
    query.bindValue(":date", selectedDate);


    if(query.exec()) {
        while(query.next()) {
            QTime time = query.value("time").toTime();
            int session_id = query.value("session_id").toInt();
            ui->comboBox_Time->addItem(time.toString("hh:mm:ss"), QVariant(session_id));
        }
    } else {
        qDebug() << "Ошибка выполнения запроса:" << query.lastError().text();
    }
    qDebug() << "Ticket cicle start _id = " << db_id << " " << name;

}

/* Обновление мест в зависимости от времени
 * */
void Tickets::on_comboBox_Time_currentIndexChanged(int index)
{
    int session_id = ui->comboBox_Time->currentData().toInt();

    ui->txt_places->clear();
    for (int x = 0; x < HALL_ROWS; x++){
        for (int y = 0; y < HALL_SEATS; y++){
            SeatsARR[x][y]->setChecked(false);
            SeatsARR[x][y]->setEnabled(true);
        }
    }

    QSqlQuery query;
    query.prepare("SELECT place FROM booking WHERE session_id = :idSession");
    query.bindValue(":idSession", session_id);

    if (query.exec()) {
        while (query.next()) {
            QString place = query.value("place").toString();
            QList<int> place_arr = myQStringToInt(place);
            for(int num : place_arr)
            {
                for (int x = 0; x < HALL_ROWS; x++)
                {
                    for (int y = 0; y < HALL_SEATS; y++)
                    {
                        if(SeatsARR[x][y]->text().toInt() == num)
                        {
                            qDebug() << "arr " << SeatsARR[x][y]->text().toInt();
                            qDebug() << "num " << num;
                            SeatsARR[x][y]->setEnabled(false);
                        }
                    }
                }
            }
        }
    } else {
        qDebug() << "Ошибка выполнения запроса:" << query.lastError().text();
    }
}

/* Очистка при закрытии
 * */
void Tickets::closeEvent(QCloseEvent *event)
{
    QDialog::closeEvent(event);

    clearCalendarMark();

    this->close();
    emit firstWindow();
}



/* Кнопка ЗАКРЫТЬ
 * */
void Tickets::on_pushButton_clicked()
{
    clearCalendarMark();

    this->close();      // Закрываем окно
    emit firstWindow(); // И вызываем сигнал на открытие главного окна
}

/* Кнопка СОХРАНИТЬ
 * */
bool Tickets::on_pushButton_2_clicked()
{
    int session_id = 0;
    int film_id = 0;

    QString surname_str = ui->txt_surname->text();
    qDebug() << "Фамилия:" << surname_str;

    QString place_str = ui->txt_places->text();
    qDebug() << "Выбранные места:" << place_str;

    film_id = db_id;
    qDebug() << "ID Фильма:" << film_id;

    session_id = ui->comboBox_Time->currentData().toInt();
    qDebug() << "ID Сеанса:" << session_id;

    if(surname_str.length() == 0)
    {
        QMessageBox::information(nullptr, "Уведомление", "Заполните фамилию!");
        return false;
    }
    else if(place_str.length() == 0)
    {
        QMessageBox::information(nullptr, "Уведомление", "Выберите места!");
        return false;
    }
    else if(session_id == 0 || film_id == 0)
    {
        QMessageBox::information(nullptr, "Уведомление", "Выберите корректную дату!");
        return false;
    }

    QSqlQuery query;
    query.prepare("SELECT * FROM booking WHERE surname = :surname AND session_id = :sessionId");
    query.bindValue(":surname", surname_str); // Укажите нужную фамилию
    query.bindValue(":sessionId", session_id);   // Укажите нужный session_id

    if (query.exec())
    {
        while (query.next())
        {
            QMessageBox::information(nullptr, "Уведомление", "Фамилия совпадает с имеющейся в базе");
            return false;
        }
    } else {
        qDebug() << "Ошибка выполнения запроса:" << query.lastError().text();
    }

    QSqlQuery query2;
    query2.prepare("INSERT INTO booking (surname, film_id, session_id, place) "
                   "VALUES (:surname, :filmId, :sessionId, :place)");
    query2.bindValue(":surname", surname_str);
    query2.bindValue(":filmId", film_id);
    query2.bindValue(":sessionId", session_id);
    query2.bindValue(":place", place_str);

    if (query2.exec())
    {
        qDebug() << "Данные успешно добавлены в таблицу booking";
    }
    else
    {
        qDebug() << "Ошибка добавления данных:" << query2.lastError().text();
    }

    ui->txt_surname->clear();
    ui->comboBox_Time->setCurrentIndex(-1);
    ui->txt_places->clear();

    for (int x = 0; x < HALL_ROWS; x++){
        for (int y = 0; y < HALL_SEATS; y++){
            SeatsARR[x][y]->setChecked(false);
            SeatsARR[x][y]->setEnabled(true);
        }
    }

    clearCalendarMark();

    this->close();      // Закрываем окно
    emit firstWindow(); // И вызываем сигнал на открытие главного окна

    return true;
}

/* Кнопка ОТЧИСТИТЬ
 * */
void Tickets::on_pushButton_3_clicked()
{
    ui->txt_surname->clear();
    ui->comboBox_Time->setCurrentIndex(-1);
    ui->txt_places->clear();

    for (int x = 0; x < HALL_ROWS; x++){
        for (int y = 0; y < HALL_SEATS; y++){
            SeatsARR[x][y]->setChecked(false);
            SeatsARR[x][y]->setEnabled(true);
        }
    }
}


