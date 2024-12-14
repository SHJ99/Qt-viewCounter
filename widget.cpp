#include "widget.h"
#include "ui_widget.h"

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);

    QObject::connect(&socket, &QSslSocket::connected, this, &Widget::doConneted);
    QObject::connect(&socket, &QSslSocket::disconnected, this, &Widget::doDisconneted);
    QObject::connect(&socket, &QSslSocket::readyRead, this, &Widget::doReadyread);
}

Widget::~Widget()
{
    delete ui;
}

QString Widget::parser(const QString &str)
{
    QRegularExpression regex(R"("viewCount":\{"simpleText":"조회수 ([\d,]+)회"\})");
    QRegularExpressionMatch match = regex.match(str);
    if (match.hasMatch()) {
        QString viewCount = match.captured(1);
        return viewCount.replace(",", "");
    }
    return "";
}

void Widget::on_pbRequest_clicked()
{
    QString host = "www.youtube.com";
    socket.connectToHostEncrypted(host, 443);
}


void Widget::on_pbClear_clicked()
{
    ui->teView->clear();
}

void Widget::doConneted()
{
    QString msg = "connect "+ socket.peerAddress().toString();
    ui->leStatus->clear();
    //ui->leStatus->insert(msg);

    QString url = ui->leHost->text();
    QString vid;
    if(url.contains("v=")){
        vid = url.split("v=").last().split("&").first();
        QString request = QString(
                              "GET /watch?v=%1 HTTP/1.1\r\n"
                              "Host: www.youtube.com\r\n"
                              "Connection: close\r\n\r\n").arg(vid);
        socket.write(request.toUtf8());

        ui->leStatus->insert(msg);
    }
    else{
        ui->leStatus->clear();
        ui->leStatus->insert("url error");
    }
}

void Widget::doDisconneted()
{
    QString msg = "disconnect "+ socket.peerAddress().toString();
    ui->leStatus->clear();
    ui->leStatus->insert(msg);
}

void Widget::doReadyread()
{
    QByteArray response = socket.readAll();
    //while (socket.waitForReadyRead(5000)) { response += socket.readAll();  }

    QString html = QString::fromUtf8(response);
    QString viewer = parser(html);

    if(!viewer.isEmpty()){
        ui->teView->insertPlainText(viewer);
    }
    //else{ ui->teView->insertPlainText("fail viewer count"); }
}

