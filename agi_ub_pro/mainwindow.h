#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include "libagi_ub.h"
#include "wtextern.h"
#include "libwtevt_json.h"
#include <QMainWindow>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#include <QJsonValue>
#include <QFile>
#include <QDebug>
#include <qtimer.h>
#include <mutex>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
public:
    int start_ub_pro();
    int stop_ub_pro();
private:
    void append_logout(QString s_log);
    void append_logout(const char* s_log);
    void append_logout(std::string s_log);

    void start_load_dev();
    void stop_load_dev();

private:
    QJsonObject GBKToJson(const char *jsonString);
    QJsonObject QstringToJson(QString jsonString);
    QString     JsonToQstring(QJsonObject jsonObject);

    QByteArray  utf8ToGbk(const QByteArray &utf8);
    QByteArray  gbkToUtf8(const QByteArray &gbk);
    QByteArray  gbkToUtf8(const char *gbk);

    int32       proc_evt_json_callback(const char *ptr_evt_class_name, const char *ptr_evt_account_name, const char *ptr_evt_json);

    int32       proc_evt_json_data(QString s_class, QString s_account, const QJsonObject& json_evt);

    int32		proc_account_event(QString str_evt_account_name, const QJsonObject& json_evt_data, const QJsonObject& json_evt_time);
    int32		proc_dialog_event(QString str_evt_account_name, const QJsonObject& json_evt_data, const QJsonObject& json_evt_time);

    int32       read_evt_json_data();
signals:
    // 自定义信号
    void append_to_logout(QString s);

private:
    static int32 AGI_API func_evt_json_callback(void *ptr_user_data, const char *ptr_evt_class_name, const char *ptr_evt_account_name, const char *ptr_evt_json);
private:
    Ui::MainWindow *ui;

    std::string m_log;
    std::mutex  m_mtx;
    bool    m_update_log;
    QTimer *m_t_UpdateUI_Timer; //ui定时器

private:
    //定时读取事件的管道id
    int32  m_nEvt_pipe_id;
private slots:
    void on_updateui_timer() ; //定时溢出处理槽函数
private slots:
   void proc_menu_command(QAction *action);
};
#endif // MAINWINDOW_H
