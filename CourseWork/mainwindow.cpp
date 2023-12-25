#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QLabel>
#include <QDesktopWidget>
#include <QScreen>
#include <QRect>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    user_counter = 0;
    m_loginSuccesfull = false;

    ui->setupUi(this);

    db = new DataBase();
    db->connectToDataBase();

    model = new QSqlTableModel;
    model->setTable(TABLE);
    model->setHeaderData(0, Qt::Horizontal, "id");

    ui->tableView->setModel(model);
    ui->tableView->setColumnHidden(0, true);
    ui->tableView->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableView->setSelectionMode(QAbstractItemView::SingleSelection);

    updateTable();

    editForm = new EditRecord();
    editForm->setWindowModality(Qt::ApplicationModal);
    addForm = new AddRecord();
    addForm->setWindowModality(Qt::ApplicationModal);
    secondWindow = new Tickets();
    addSession = new AddSession();

    connect(secondWindow, &Tickets::firstWindow, this, &MainWindow::enableMainWindowSlot);
    connect(addSession, &AddSession::mainWindow, this, &MainWindow::enableMainWindowSlot);

    connect(addForm,  SIGNAL(sendToWidget(QByteArray, QString, QString, int, QString, QString, QString, QDate, qint8, QString)), this,
            SLOT(addRecordDataBase(QByteArray, QString, QString, int, QString, QString, QString, QDate, qint8, QString)));

    connect(this,     SIGNAL(sendForEdit(int, QByteArray, QString, QString, int, QString, QString, QString, QDate, qint8, QString)), editForm,
            SLOT(catchInfo(int, QByteArray, QString, QString, int, QString, QString, QString, QDate, qint8, QString)));

    connect(editForm, SIGNAL(sendToWidgetUpdate(int, QByteArray, QString, QString, int, QString, QString, QString, QDate, qint8, QString)), this,
            SLOT(editRecordDataBase(int, QByteArray, QString, QString, int, QString, QString, QString, QDate, qint8, QString)));

    connect(this,     SIGNAL(sendForTicket(int)), secondWindow,
            SLOT(catchInfo(int)));


    connect(ui->menu1_add_record,  SIGNAL(triggered()), this, SLOT(slotAdd()));
    connect(ui->addButton,         SIGNAL(clicked()), this, SLOT(slotAdd()));
    connect(ui->menu1_edit_record, SIGNAL(triggered()), this, SLOT(slotEdit()));
    connect(ui->editButton,        SIGNAL(clicked()), this, SLOT(slotEdit()));
    connect(ui->menu1_del_record,  SIGNAL(triggered()), this, SLOT(slotDel()));
    connect(ui->delButton,         SIGNAL(clicked()), this, SLOT(slotDel()));
    connect(ui->menu1_exit,        SIGNAL(triggered()), qApp, SLOT(quit()));
    connect(ui->menu2_help,        SIGNAL(triggered()), this, SLOT(slotInfo()));
    connect(ui->menu2_about,       SIGNAL(triggered()), this, SLOT(slotAbout()));

    connect(&ui_Auth, SIGNAL(login_button_clicked()),this, SLOT(authorizeUser()));
    connect(&ui_Auth, SIGNAL(destroyed()),this, SLOT(show()));
    connect(&ui_Auth, SIGNAL(register_button_clicked()),this,SLOT(registerWindowShow()));
    connect(&ui_Reg,  SIGNAL(register_button_clicked2()),this,SLOT(registerUser()));
    connect(&ui_Reg,  SIGNAL(destroyed()),&ui_Auth, SLOT(show()));
}


MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::updateTable(){
    currRow = -1;
    currID = -1;
    model->select();
    model->setSort(8,Qt::AscendingOrder);
    if (currRow == -1){
        ui->menu1_edit_record->setEnabled(false);
        ui->menu1_del_record->setEnabled(false);
        ui->editButton->setEnabled(false);
        ui->delButton->setEnabled(false);
    }
    else{
        ui->menu1_edit_record->setEnabled(true);
        ui->menu1_del_record->setEnabled(true);
        ui->editButton->setEnabled(true);
        ui->delButton->setEnabled(true);
    }
    if (model->rowCount() == 0){
        ui->label->show();
        ui->tableView->hide();
        return;
    }
    ui->label->hide();
    ui->tableView->show();
    QPixmap pic = QPixmap();
    for (int i = 0; i < model->rowCount(); i++){
        pic.loadFromData(model->data(model->index(i, 1)).toByteArray());
        QLabel *imageLabel = new QLabel();
        imageLabel->setPixmap(pic.scaled(150, 400, Qt::KeepAspectRatio));
        ui->tableView->setIndexWidget(model->index(i, 1), imageLabel);
    }
    ui->tableView->horizontalHeader()->setSectionResizeMode(1, QHeaderView::ResizeToContents);
    ui->tableView->horizontalHeader()->setSectionResizeMode(2, QHeaderView::Stretch);
    ui->tableView->horizontalHeader()->setSectionResizeMode(3, QHeaderView::Stretch);
    ui->tableView->horizontalHeader()->setSectionResizeMode(4, QHeaderView::ResizeToContents);
    ui->tableView->horizontalHeader()->setSectionResizeMode(5, QHeaderView::Stretch);
    ui->tableView->horizontalHeader()->setSectionResizeMode(6, QHeaderView::ResizeToContents);
    ui->tableView->horizontalHeader()->setSectionResizeMode(7, QHeaderView::Stretch);
    ui->tableView->horizontalHeader()->setSectionResizeMode(8, QHeaderView::ResizeToContents);
    ui->tableView->horizontalHeader()->setSectionResizeMode(9, QHeaderView::ResizeToContents);
    ui->tableView->horizontalHeader()->setSectionResizeMode(10, QHeaderView::Stretch);

    ui->tableView->verticalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    ui->tableView->setEditTriggers(QAbstractItemView::NoEditTriggers);
}

void MainWindow::slotAdd(){
    addForm->show();
}

void MainWindow::addRecordDataBase(const QByteArray &pic, const QString &name, const QString &author, const int &releaseYear, const QString &description, const QString &type,  const QString &genres,  const QDate &viewDate, const qint8 &score, const QString &comment)
{
    db->insertIntoTable(pic, name, author, releaseYear, description, type, genres, viewDate, score, comment);
    updateTable();
    currRow = -1;
    currID = -1;
}


void MainWindow::slotEdit(){

    //        qDebug() << currRow;
    if(currRow != -1)
    {
        QString name = model->data(model->index(currRow, 2)).toString();
        QString author = model->data(model->index(currRow, 3)).toString();
        int releaseYear = model->data(model->index(currRow, 4)).toDate().year();

        QString description = model->data(model->index(currRow, 5)).toString();

        QString type =  model->data(model->index(currRow, 6)).toString();
        QString genres =  model->data(model->index(currRow, 7)).toString();


        QDate viewDate = model->data(model->index(currRow, 8)).toDate();

        qint8 score =  (qint8) model->data(model->index(currRow, 9)).toInt();
        QString comment = model->data(model->index(currRow, 10)).toString();

        QByteArray inByteArray = model->data(model->index(currRow, 1)).toByteArray();
        emit sendForEdit(currID, inByteArray, name, author, releaseYear, description, type, genres, viewDate, score, comment);
        editForm->show();
    }

}

void MainWindow::editRecordDataBase(const int &id, const QByteArray &pic, const QString &name, const QString &author, const int &releaseYear, const QString &description, const QString &type,  const QString &genres,  const QDate &viewDate, const qint8 &score, const QString &comment)
{
    currRow = -1;
    currID = -1;
    db->editInTable(id,pic, name, author, releaseYear, description, type, genres, viewDate, score, comment);
    updateTable();
}

void MainWindow::slotDel(){
    if(currRow != -1)
    {
        db->deleteFromDatabase(currID);
        updateTable();
        QMessageBox::information(nullptr, "Уведомление", "запись успешно удалена");
        currRow = -1;
        currID = -1;
    }

}


void MainWindow::slotInfo()
{
    HelpInformation* form = new HelpInformation();
    form->setWindowModality(Qt::ApplicationModal);
    form->show();
}

void MainWindow::slotAbout()
{
    QMessageBox::about(this, "О программе", "Версия: 0.0.1 Alpha\n\nРазработчик: Яцук Максим, ИП-117\n\n            © 2023 уч.год, СибГУТИ");
}

void MainWindow::on_tableView_clicked(const QModelIndex &index)
{
    ui->menu1_edit_record->setEnabled(true);
    ui->menu1_del_record->setEnabled(true);
    ui->editButton->setEnabled(true);
    ui->delButton->setEnabled(true);
    currID = model->data(model->index(index.row(), 0)).toInt();
    currRow = index.row();
}



void MainWindow::authorizeUser()
{
    m_username = ui_Auth.getLogin();
    m_userpass = ui_Auth.getPass();

    QString str_t = " SELECT * "
                    " FROM userlist "
                    " WHERE name = '%1'";
    // int db_number = 0;

    QString username = "";
    QString userpass = "";

    int xPos = 0;
    int yPos = 0;

    int width = 0;
    int length = 0;

    db_input = str_t.arg(m_username);

    QSqlQuery query;
    QSqlRecord rec;

    if(!query.exec(db_input))
    {
        qDebug() << "Unable to execute query - exiting" << query.lastError() << " : " << query.lastQuery();
    }
    rec = query.record();
    query.next();
    user_counter = query.value(rec.indexOf("number")).toInt();
    username = query.value(rec.indexOf("name")).toString();
    userpass = query.value(rec.indexOf("pass")).toString();

    if(m_username != username || m_userpass != userpass)
    {
        qDebug() << "Password missmatch" << username;
        QMessageBox::information(nullptr, "Уведомление", "Пароль не совпадает!!!");
        m_loginSuccesfull = false;
    }
    else if(m_username.length() == 0 || m_userpass.length() == 0){
        qDebug() << "Empty input!" << username;
        QMessageBox::information(nullptr, "Уведомление", "Заполните оба поля!!!");
        m_loginSuccesfull = false;
    }
    else
    {
        m_loginSuccesfull = true;

        xPos = query.value(rec.indexOf("xpos")).toInt();
        yPos = query.value(rec.indexOf("ypos")).toInt();
        width = query.value(rec.indexOf("width")).toInt();
        length = query.value(rec.indexOf("length")).toInt();

        ui_Auth.close();
        ui_Reg.close();

        this->setGeometry(xPos, yPos ,width, length);
        this->showMaximized();
    }
}

void MainWindow::registerWindowShow()
{
    ui_Auth.hide();
    ui_Reg.show();
}

void MainWindow::registerUser()
{
    if(ui_Reg.checkPass())
    {
        QSqlQuery query;
        QSqlRecord rec;
        QString str_t = "SELECT COUNT(*) "
                        "FROM userlist;";
        db_input = str_t;
        if(!query.exec(db_input))
        {
            qDebug() << "Unable to get number " << query.lastError() << " : " << query.lastQuery();
            return;
        }
        else
        {
            query.next();
            rec = query.record();
            user_counter = rec.value(0).toInt();
            qDebug() << user_counter;
        }


        m_username = ui_Reg.getName();
        m_userpass = ui_Reg.getPass();
        user_counter++;
        str_t = "INSERT INTO userlist(number, name, pass, xpos, ypos, width, length)"
                "VALUES(%1, '%2', '%3', %4, %5, %6, %7);";
        db_input = str_t .arg(user_counter)
                .arg(m_username)
                .arg(m_userpass)
                .arg(0)
                .arg(0)
                .arg(800)
                .arg(400);

        if(!query.exec(db_input))
        {
            qDebug() << "Unable to insert data"  << query.lastError() << " : " << query.lastQuery();
        }
        else
        {
            ui_Reg.hide();
            ui_Auth.show();
        }
    }
    else
    {
        qDebug() << "Confirm password coorectly";
    }
}

void MainWindow::display()
{
    ui_Auth.show();
}



void MainWindow::enableMainWindowSlot()
{
        setEnabled(true); // Включить активность главного окна
}

void MainWindow::on_ticketsButton_clicked()
{
    if(currRow != -1)
    {
        QString name = model->data(model->index(currRow, 2)).toString();
        QString author = model->data(model->index(currRow, 3)).toString();

        secondWindow->db_id = currID;
        secondWindow->windowPreparation(name);
        secondWindow->show();

        this->setEnabled(false);
    }

//    secondWindow->show();  // Показываем окно
//    this->close();    // Закрываем основное окно
}

void MainWindow::on_sessionButton_clicked()
{
    if(currRow != -1)
    {
        addSession->db_id = currID;
        addSession->show();

        this->setEnabled(false);
    }
}


