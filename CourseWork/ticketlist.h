#ifndef TICKETLIST_H
#define TICKETLIST_H

#include <QWidget>
#include <QStringListModel>
#include <QListView>
#include <QStandardItemModel>
#include <QStandardItem>

namespace Ui {
class TicketList;
}

class TicketList : public QWidget
{
    Q_OBJECT

public:
    explicit TicketList(QWidget *parent = 0);
    ~TicketList();

    void populateListView();

    void updateTable(const QString &time,
                     const QString &date,
                     const QString &film,
                     const QString &hall,
                     const QString &surname);

    QStandardItemModel *model;

private:
    Ui::TicketList *ui;
};

#endif // TICKETLIST_H
