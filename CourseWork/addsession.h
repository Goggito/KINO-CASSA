#ifndef ADDSESSION_H
#define ADDSESSION_H

#include <QWidget>
#include <QSql>
#include <QSqlQuery>
#include <QSqlError>
#include <QSqlDatabase>
#include <QFile>
#include <QDate>
#include <QDebug>

#define MIN_HALL_NUM 1
#define MAX_HALL_NUM 5

namespace Ui {
class AddSession;
}

class AddSession : public QWidget
{
    Q_OBJECT

public:
    explicit AddSession(QWidget *parent = 0);
    ~AddSession();

    int db_id;
    int sess_id;

private:
    Ui::AddSession *ui;

private slots:
    void on_confirm_pushButton_clicked();

signals:
    void mainWindow();

protected:
    void closeEvent(QCloseEvent *event) override {
        QWidget::closeEvent(event);
        this->close();      // Закрываем окно
        emit mainWindow(); // И вызываем сигнал на активацию главного окна
    }
};



#endif // ADDSESSION_H
