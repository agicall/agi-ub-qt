#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "qdatetime.h"
#include <qtextcodec.h>
#include "qmessagebox.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    connect(menuBar(), SIGNAL(triggered(QAction*)), this, SLOT(proc_menu_command(QAction*)));

    m_nEvt_pipe_id = 0;
    m_log.reserve(1024*10);

    m_update_log = false;

    m_t_UpdateUI_Timer=new QTimer(this);
    m_t_UpdateUI_Timer->stop();
    m_t_UpdateUI_Timer->setInterval (100) ;//设置定时周期，单位：毫秒
    connect(m_t_UpdateUI_Timer,SIGNAL(timeout()),this,SLOT(on_updateui_timer()));
    m_t_UpdateUI_Timer->start();
}

MainWindow::~MainWindow()
{
    m_t_UpdateUI_Timer->stop();
    stop_ub_pro();
    delete ui;
}

void MainWindow::proc_menu_command(QAction *action)
{
    if (action->objectName() == QString::fromLocal8Bit("action_menu_exit"))
    {
        qApp->exit();
    }
    else if (action->objectName() == QString::fromLocal8Bit("action_menu_about"))
    {
        QMessageBox::warning(this, QStringLiteral("关于"),QStringLiteral("关于usb电话录音拨号盒"),QMessageBox::Ok | QMessageBox::Cancel);
    }
}

void MainWindow::append_logout(std::string s_log)
{
    if (s_log.empty()) return;

    QDateTime current_date_time = QDateTime::currentDateTime();
    std::string current_date = current_date_time.toString("yyyy.MM.dd hh:mm:ss.zzz ").toLocal8Bit().data();

    std::lock_guard<std::mutex> s(m_mtx);
    if (m_log.length() > 4096)
    {
        m_log = m_log.substr(0, 4096/2);
    }
    m_log = current_date + s_log  + "\r\n" + m_log;
    m_update_log = true;
}

void MainWindow::append_logout(const char* s_log)
{
    if (!s_log) return;
    append_logout((std::string)s_log);
}

void MainWindow::append_logout(QString s_log)
{
    append_logout(s_log.toLocal8Bit().data());
}

void MainWindow::start_load_dev()
{
    int ret = agi_ub_create(0, 0, 0);
    if (ret != ERR_NO)
    {
        append_logout("设备未找到,请插入设备,系统会自动连接...");
    }
    else
    {
        append_logout("设备找到,正在连接,请稍后...");
    }
}

void MainWindow::stop_load_dev()
{
    append_logout("停止加载设备...");
    agi_ub_destroy(0);
}

int MainWindow::start_ub_pro()
{
    char szVer[1024] = { 0 };
    //读取库版本号
    agi_ub_version(szVer, 1024);
    //全局初始化引擎
    int ret = agi_ub_init(0);
    if (ret != ERR_NO)
    {
        append_logout((std::string("*启动usb语音盒引擎失败:") + szVer + "["+std::to_string(ret)+"]").c_str());
    }
    else
    {
        append_logout((std::string("->启动usb语音盒引擎成功:") + szVer).c_str());
        //注册从子线程里回调事件通知函数,所有事件类别和账号事件都需要
        //evt_create_json_cb(func_evt_json_callback, this, "", "");

        //使用定时读取事件,和回调方式只需要一种就可以,所有事件类别和账号事件都需要
        m_nEvt_pipe_id = evt_create_json_pipe("","");

        start_load_dev();
    }
    return ERR_NO;
}

int MainWindow::stop_ub_pro()
{
    if (m_nEvt_pipe_id != 0)
    {
        evt_destroy_json_pipe(m_nEvt_pipe_id);
        m_nEvt_pipe_id = 0;
    }
    stop_load_dev();
    append_logout("停止usb语音盒引擎");
    agi_ub_uninit();
    return ERR_NO;
}

// QJsonValue value = obj.value(key);
int32	MainWindow::proc_account_event(QString str_evt_account_name, const QJsonObject& json_evt_data, const QJsonObject& json_evt_time)
{
    QString s_log,qs;
    //QString s_time = JsonToQstring(json_evt_time);
    QString s_evt_name = json_evt_data["evt_name"].toString();
    if (s_evt_name == "status")
    {
        QString s_value = json_evt_data["value"].toString();
        if (s_value == "online")
        {
            s_log = QStringLiteral("->设备在线");
        }
        else if (s_value == "offline")
        {
            s_log = QStringLiteral("**设备掉线**:[") + json_evt_data["mode"].toString()+"]";
        }else if (s_value == "free")
        {
            s_log = QStringLiteral("->设备空闲状态");
        }
        else if (s_value == "busy")
        {
            s_log = QStringLiteral("->设备工作中");
        }
        else if (s_value == "ring_on")
        {
            s_log = QStringLiteral("->来电铃声[开始]响");
        }
        else if (s_value == "ring_off")
        {
            s_log = QStringLiteral("->来电铃声[停止]响");
        }
        else if (s_value == "busy_tone")
        {
            qs = QString("[count:%1] [duration:%2ms]").arg(json_evt_data["count"].toInt()).arg(json_evt_data["duration"].toInt());
            s_log = QStringLiteral("->检测到忙音信号") + qs;
        }
    }
    else if (s_evt_name == "phone")
    {
        QString s_value = json_evt_data["value"].toString();
        if (s_value == "offhook")
        {
            s_log = QStringLiteral("->电话摘机");
        }
        else if (s_value == "hangup")
        {
            s_log = QStringLiteral("->电话挂机");
        }
        else if (s_value == "flash")
        {
            s_log = QStringLiteral("->电话排插簧");
        }
    }
    else if (s_evt_name == "do_ctrl")
    {
        int value = json_evt_data["value"].toInt();
        QString s_ctrl_name = json_evt_data["ctrl_name"].toString();
        qs = QString("[%1->%2]").arg(s_ctrl_name).arg(json_evt_data["duration"].toInt()).arg(value);
        s_log = QStringLiteral("->设备控制状态改变:") + qs;
    }
    else if (s_evt_name == "api")
    {
        QString s_value = json_evt_data["value"].toString();
        if (s_value == "offhook")
        {
            s_log = QStringLiteral("->软件api摘机");
        }
        else if (s_value == "hangup")
        {
            s_log = QStringLiteral("->软件api挂机");
        }
        else if (s_value == "flash")
        {
            s_log = QStringLiteral("->软件api排插簧");
        }
    }
    else if (s_evt_name == "dial_step")
    {
        QString s_operator = json_evt_data["operator"].toString();
        QString s_mode = json_evt_data["mode"].toString();
        QString s_step_name = json_evt_data["step_name"].toString();
        if (s_mode == "dial_dtmf")
        {
            if (s_step_name == "begin")
            {
                s_log = QStringLiteral("->发送DTMF:") + json_evt_data["code"].toString();
            }
            return ERR_NO;
        }
        if (s_step_name == "begin") s_step_name = QStringLiteral("启动拨号");
        else if (s_step_name == "waittone") s_step_name = QStringLiteral("等待拨号音");
        else if (s_step_name == "dialtone") s_step_name = QStringLiteral("检测到拨号音");
        else if (s_step_name == "timeout") s_step_name = QStringLiteral("检测拨号音超时");
        else if (s_step_name == "dialing") s_step_name = QStringLiteral("正在拨号中");
        else if (s_step_name == "stop") s_step_name = QStringLiteral("拨号主动停止");
        else if (s_step_name == "end")
        {
            s_step_name = QStringLiteral("拨号结束");
            if (s_operator == "phone")
            {
                s_step_name += QString("[end_mode:%1]").arg(json_evt_data["end"].toInt());//拨号结束的检测方式
            }
            //---演示设置自定义参数例子,通话事件里会在call_param参数里返回
            QJsonObject json_call_param;
            json_call_param["name"] = "jiujiu";
            json_call_param["url"] = "agicall.com";
            agi_ub_action_param(str_evt_account_name.toLocal8Bit().data(), "set_call_param", JsonToQstring(json_call_param).toLocal8Bit().data(), nullptr, 0);
        }
        if (s_operator == "api") s_operator = QStringLiteral("软件拨号");
        if (s_operator == "phone") s_operator = QStringLiteral("话机拨号");

        s_log = s_operator + ":" + s_step_name + " -> " + json_evt_data["code"].toString();
    }
    else if (s_evt_name == "logout")
    {
        s_log = json_evt_data["value"].toString();
    }

    if (!s_log.isEmpty())
    {
        append_logout("[" + str_evt_account_name + "] " + s_log);
    }
    return ERR_NO;
}

int32	MainWindow::proc_dialog_event(QString str_evt_account_name, const QJsonObject& json_evt_data, const QJsonObject& json_evt_time)
{
    QString s_log,qs;
    //QString s_time = JsonToQstring(json_evt_time);
    QString s_evt_name = json_evt_data["evt_name"].toString();
    if (s_evt_name == "status")
    {
        QJsonObject json_dialog = json_evt_data["dialog"].toObject();
        QJsonObject json_callid = json_dialog["callid"].toObject();

        QString s_step_name = json_evt_data["step_name"].toString();
        QString s_operator = json_dialog["operator"].toString();
        QString s_dialog_type = json_dialog["dialog_type"].toString();
        QString s_code = json_callid["code"].toString();

        if (s_dialog_type == "in")//呼入来电
        {
            if (s_step_name == "in_create")
            {
                s_log = QStringLiteral("<--创建新的呼入");
            }
            else if (s_step_name == "in_trying")
            {
                s_log = QStringLiteral("<--呼入尝试接收来电号码");
            }
            else if (s_step_name == "in_invite")
            {
                s_log = QStringLiteral("<--来电号码呼入:[") + s_code + "]";
            }
            else if (s_step_name == "in_ringing")//超时没号码响铃,或者已经收到号码响铃
            {
                s_log = QStringLiteral("<--呼入铃声");
            }
            else if (s_step_name == "in_connected")
            {
                s_log = QStringLiteral("<--呼入接通:[") + s_code + "]";
            }
            else if (s_step_name == "in_finished")
            {
                s_log = QStringLiteral("<--呼入结束");
            }
        }
        else if (s_dialog_type == "out")//呼出去电
        {
            if (s_step_name == "out_create")
            {
                s_log = QStringLiteral("-->创建新的呼出");
            }
            else if (s_step_name == "out_invite")
            {
                s_log = QStringLiteral("-->正在呼叫目标号码:[") + s_code + "]";
            }
            else if (s_step_name == "out_ringing")
            {
                s_log = QStringLiteral("-->呼出对方回铃音");
            }
            else if (s_step_name == "out_connected")
            {
                QString s_method = json_evt_data["method"].toString();//只有使用极性反转 [polarity] 检测到的才是可靠信号,其它方式仅作参考
                s_log = QStringLiteral("-->呼出接通:[") + s_method + "]";
            }
            else if (s_step_name == "out_finished")
            {
                s_log = QStringLiteral("-->呼出结束");
            }
        }
        else
        {
            s_log = QStringLiteral("-->**异常状态,不应该发生**");
        }
    }
    else if (s_evt_name == "recfile")
    {
        QString s_step_name = json_evt_data["step_name"].toString();
        if (s_step_name == "start")
        {
            s_log = QStringLiteral("@开始录音:[") + json_evt_data["file"].toString() + "]";
        }
        else if (s_step_name == "stop")
        {
            s_log = QStringLiteral("@停止录音:[") + json_evt_data["file"].toString() + "] [" + QString::number(json_evt_data["duration"].toInt()) +"ms]";
        }
        else if (s_step_name == "failed")
        {
            s_log = QStringLiteral("@启动录音失败:[") + json_evt_data["file"].toString() + "] [reason:" + QString::number(json_evt_data["reason"].toInt()) + "]";
        }
    }
    else if (s_evt_name == "playfile")
    {
        QString s_step_name = json_evt_data["step_name"].toString();
        if (s_step_name == "start")
        {
            s_log = QStringLiteral("#开始放音:[") + json_evt_data["file"].toString() + "]";
        }
        else if (s_step_name == "stop")
        {
            s_log = QStringLiteral("#停止放音:[") + json_evt_data["file"].toString() + "]";
        }
        else if (s_step_name == "finished")
        {
            s_log = QStringLiteral("#放音结束:[") + json_evt_data["file"].toString() + "]";
        }
        else if (s_step_name == "failed")
        {
            s_log = QStringLiteral("#放音失败:[") + json_evt_data["file"].toString() + "]";
        }
    }
    else if (s_evt_name == "dtmf_char")
    {
        s_log = QStringLiteral("$收到一个按键:[") + json_evt_data["dtmf"].toString() + "]";
    }
    else if (s_evt_name == "dtmf_cmd")
    {
        s_log = QStringLiteral("$收到一串连续按键:[") + json_evt_data["dtmf"].toString() + "]";
    }
    else if (s_evt_name == "dtmf_result")
    {
        s_log = QStringLiteral("$正在接收连续按键:[") + json_evt_data["dtmf"].toString() + "]";
    }

    if (!s_log.isEmpty())
    {
        append_logout("[" + str_evt_account_name + "] " + s_log);
    }
    return ERR_NO;
}

int32  MainWindow::proc_evt_json_callback(const char *ptr_evt_class_name, const char *ptr_evt_account_name, const char *ptr_evt_json)
{
    QString s_class = ptr_evt_class_name;
    QString s_account = ptr_evt_account_name;
    QJsonObject json_evt = GBKToJson(ptr_evt_json);
    return proc_evt_json_data(s_class, s_account, json_evt);
}

int32  MainWindow::proc_evt_json_data(QString s_class, QString s_account, const QJsonObject& json_evt)
{
    QJsonObject json_evt_data = json_evt["data"].toObject();
    QJsonObject json_evt_time = json_evt["time"].toObject();
    if (s_class == "account")
    {
        return proc_account_event(s_account, json_evt_data, json_evt_time);
    }
    if (s_account == "dialog")
    {
        return proc_dialog_event(s_account, json_evt_data, json_evt_time);
    }
    return ERR_NO;
}

int32 AGI_API MainWindow::func_evt_json_callback(void *ptr_user_data, const char *ptr_evt_class_name, const char *ptr_evt_account_name, const char *ptr_evt_json)
{
    MainWindow* pThis = (MainWindow*)ptr_user_data;
    return pThis->proc_evt_json_callback(ptr_evt_class_name, ptr_evt_account_name, ptr_evt_json);
}

int32    MainWindow::read_evt_json_data()
{
    if (m_nEvt_pipe_id != 0)
    {
        char szEvt_Data[4096]={0};
        while (evt_pop_json_buf_data(m_nEvt_pipe_id, szEvt_Data, 4096, 0) == ERR_NO)
        {
            QJsonObject json_evt = GBKToJson(szEvt_Data);
            QString s_class = json_evt["class"].toString();
            QString s_account = json_evt["account"].toString();
            proc_evt_json_data(s_class, s_account, json_evt);
        }
    }
    return ERR_NO;
}

void MainWindow::on_updateui_timer()
{
    read_evt_json_data();
    std::lock_guard<std::mutex> s(m_mtx);
    if (m_update_log)
    {
        m_update_log = false;
        ui->m_logout->setPlainText( QString::fromLocal8Bit(m_log.c_str()));
    }
}

QJsonObject MainWindow::GBKToJson(const char *jsonString)
{
    QByteArray utf8 = gbkToUtf8(jsonString);
    QJsonDocument jsonDocument = QJsonDocument::fromJson(utf8);
    if(jsonDocument.isNull())
    {
        qDebug()<< "parse json is error:"<< utf8.data();
    }
    QJsonObject jsonObject = jsonDocument.object();
    return jsonObject;
}

QJsonObject MainWindow::QstringToJson(QString jsonString)
{
    QJsonDocument jsonDocument = QJsonDocument::fromJson(jsonString.toLocal8Bit().data());
    if(jsonDocument.isNull())
    {
        qDebug()<< "parse json is error:"<< jsonString.toLocal8Bit().data();
    }
    QJsonObject jsonObject = jsonDocument.object();
    return jsonObject;
}

//没有换行
QString MainWindow::JsonToQstring(QJsonObject jsonObject)
{
    QJsonDocument document;
    document.setObject(jsonObject);
    QByteArray simpbyte_array = document.toJson(QJsonDocument::Compact);
    QString simpjson_str(simpbyte_array);
    return simpjson_str;
}

QByteArray MainWindow::utf8ToGbk(const QByteArray &utf8)
{
    // utf8 -> unicode -> gbk
    QTextCodec *utf8Codec = QTextCodec::codecForName("UTF-8");
    QString unicode = utf8Codec->toUnicode(utf8);

    QTextCodec *gbkCodec = QTextCodec::codecForName("GBK");
    QByteArray gbk = gbkCodec->fromUnicode(unicode);

    return gbk;
}

QByteArray MainWindow::gbkToUtf8(const char *gbk)
{
    // gbk -> unicode -> utf8
    QTextCodec *gbkCodec = QTextCodec::codecForName("GBK");
    QString unicode = gbkCodec->toUnicode(gbk);

    QTextCodec *utf8Codec = QTextCodec::codecForName("UTF-8");
    QByteArray utf8 = utf8Codec->fromUnicode(unicode);

    return utf8;
}
QByteArray MainWindow::gbkToUtf8(const QByteArray &gbk)
{
    // gbk -> unicode -> utf8
    QTextCodec *gbkCodec = QTextCodec::codecForName("GBK");
    QString unicode = gbkCodec->toUnicode(gbk);

    QTextCodec *utf8Codec = QTextCodec::codecForName("UTF-8");
    QByteArray utf8 = utf8Codec->fromUnicode(unicode);

    return utf8;
}
