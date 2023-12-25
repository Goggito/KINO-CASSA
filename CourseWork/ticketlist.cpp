#include "ticketlist.h"
#include "ui_ticketlist.h"



TicketList::TicketList(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::TicketList)
{
    ui->setupUi(this);

    QStandardItemModel *model = new QStandardItemModel(0, 5);
    model->setHorizontalHeaderLabels({"Время", "Дата", "Фильм", "Зал", "Фамилия"});


    ui->tableView->setModel(model);

    // Установка параметров отображения таблицы (опционально)
    ui->tableView->horizontalHeader()->setStretchLastSection(true); // Растягивать последний столбец
    ui->tableView->verticalHeader()->setVisible(false);
}

TicketList::~TicketList()
{
    delete ui;
}

void TicketList::populateListView()
{

}

void TicketList::updateTable(const QString &time,
                             const QString &date,
                             const QString &film,
                             const QString &hall,
                             const QString &surname)
{
    int rowCount = model->rowCount(); // Получаем текущее количество строк в модели

        // Создаем QStandardItem для каждой ячейки в новой строке
        QList<QStandardItem*> newRowItems;
        newRowItems << new QStandardItem(time);
        newRowItems << new QStandardItem(date);
        newRowItems << new QStandardItem(film);
        newRowItems << new QStandardItem(hall);
        newRowItems << new QStandardItem(surname);

        // Добавляем строку в модель
        model->insertRow(rowCount, newRowItems);
}
