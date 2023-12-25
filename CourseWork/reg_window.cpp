#include "reg_window.h"
#include "ui_reg_window.h"

reg_window::reg_window(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::reg_window)
{
    ui->setupUi(this);
}

reg_window::~reg_window()
{
    delete ui;
}

QString reg_window::getName()
{
    return m_userName;
}

QString reg_window::getPass()
{
    return m_userPass;
}

bool reg_window::checkPass()
{
    return (m_confirmation == m_userPass);
}

void reg_window::on_reg_confirm_butt_clicked()
{
    emit register_button_clicked2();
}

void reg_window::on_reg_name_line_textEdited(const QString &arg1)
{
    reg_window::m_userName = arg1;
}

void reg_window::on_reg_pass_line_textEdited(const QString &arg1)
{
    reg_window::m_userPass = arg1;
}

void reg_window::on_reg_pass2_line_textEdited(const QString &arg1)
{
    reg_window::m_confirmation = arg1;
}
