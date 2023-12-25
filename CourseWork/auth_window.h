#ifndef AUTH_WINDOW_H
#define AUTH_WINDOW_H

#include <QWidget>

namespace Ui {
class auth_window;
}

class auth_window : public QWidget
{
    Q_OBJECT

public:
    explicit auth_window(QWidget *parent = 0);
    ~auth_window();
    QString getLogin();
    QString getPass();

signals:
    void login_button_clicked();
    void register_button_clicked();

private slots:
    void on_log_enter_butt_clicked();
    void on_log_registr_butt_clicked();
    void on_log_pass_line_textEdited(const QString &arg1);
    void on_log_name_line_textEdited(const QString &arg1);

private:
    Ui::auth_window *ui;
    QString m_username;
    QString m_userpass;
};

#endif // AUTH_WINDOW_H
