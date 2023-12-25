#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMessageBox>
#include <QSqlTableModel>
#include <QDebug>

#include <QtSql/QSql>
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlQuery>
#include <QtSql/QSqlError>
#include <QtSql/QSqlRecord>

#include "addrecord.h"
#include "editrecord.h"
#include "helpinformation.h"
#include "database.h"

#include "auth_window.h"
#include "reg_window.h"

#include "tickets.h"
#include "addsession.h"


namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);

    void display();
    //bool connectDB();
    QSqlDatabase mw_db; // экземпляр подключения к БД

    ~MainWindow();

public slots:
    void slotAdd();
    void slotEdit();
    void slotDel();
    void slotInfo();
    void slotAbout();
    void updateTable();
    void addRecordDataBase(const QByteArray &pic, const QString &name, const QString &author, const int &releaseYear, const QString &description, const QString &type,  const QString &genres,  const QDate &viewDate, const qint8 &score, const QString &comment);
    void editRecordDataBase(const int &id, const QByteArray &pic, const QString &name, const QString &author, const int &releaseYear, const QString &description, const QString &type,  const QString &genres,  const QDate &viewDate, const qint8 &score, const QString &comment);

signals:
    void sendForEdit(const int &ip, const QByteArray &pic, const QString &name, const QString &author, const int &releaseYear, const QString &description, const QString &type,  const QString &genres,  const QDate &viewDate, const qint8 &score, const QString &comment);
    void sendForTicket(const int &ip);

private slots:
    void on_tableView_clicked(const QModelIndex &index);

    void authorizeUser();
    void registerWindowShow();
    void registerUser();

    void enableMainWindowSlot();

    void on_ticketsButton_clicked();
    void on_sessionButton_clicked();

private:
    Ui::MainWindow *ui;

    Tickets * secondWindow;
    AddRecord  * addForm;
    EditRecord * editForm;
    AddSession * addSession;

    DataBase * db;
    QSqlTableModel * model;
    int currRow = -1;
    int currID = -1;

    auth_window ui_Auth; // окно аутентификации
    reg_window ui_Reg; // окно регистрации
    QString m_username; // строки для обработки
    QString m_userpass; // пользовательского ввода
    QString db_input; // строка для отправки запроса к БД
    int user_counter; // счетчик пользователей
    bool m_loginSuccesfull; // флаг успешной авторизации
};

#endif // MAINWINDOW_H
