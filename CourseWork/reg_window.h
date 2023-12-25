#ifndef REG_WINDOW_H
#define REG_WINDOW_H

#include <QWidget>

namespace Ui {
class reg_window;
}

class reg_window : public QWidget
{
    Q_OBJECT

public:
    explicit reg_window(QWidget *parent = 0);
    ~reg_window();
    QString getName();
    QString getPass();
    bool checkPass();

signals:
    void register_button_clicked2();

private slots:
    void on_reg_confirm_butt_clicked();
    void on_reg_name_line_textEdited(const QString &arg1);
    void on_reg_pass_line_textEdited(const QString &arg1);
    void on_reg_pass2_line_textEdited(const QString &arg1);

private:
    Ui::reg_window *ui;
    QString m_userName;
    QString m_userPass;
    QString m_confirmation;
};

#endif // REG_WINDOW_H
