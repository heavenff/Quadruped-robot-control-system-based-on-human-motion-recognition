#include "widget.h"
#include "ui_widget.h"
#include <QHostAddress>
#define cout qDebug() <<"{"<<__FILE__<<":"<<__LINE__<<"}"

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);

    //分配空间，指定父对象
    udpSocket = new QUdpSocket(this);

    //绑定
    static quint16 _port = 8017;
    udpSocket->bind(_port,QUdpSocket::DontShareAddress);
    cout<<"绑定";

    setWindowTitle("上位机");

    //当对方发送数据过来，自动触发readyread()信号
    connect(udpSocket,&QUdpSocket::readyRead,this,&Widget::dealMsg);
}

Widget::~Widget()
{
    delete ui;
}

void Widget::dealMsg()
{
    //读取对方发送的内容
    char buf[1024] = {0};
    QHostAddress cliAddr;//对方地址
    quint16 port;//对方端口
    qint64 len = udpSocket->readDatagram(buf,sizeof (buf),&cliAddr,&port);
    if(len > 0)
    {
        //格式化【192.168.2.2:8888】aaaa
        QString str = QString ("[%1:%2] %3").arg(cliAddr.toString())
                                            .arg(port)
                                            .arg(buf);
        //给编辑区设置内容
        ui->textEdit->setText(str);

    }
    cout<<"收到了消息";
}
void Widget::on_ButtonSend_clicked()
{
    //获取对方的IP和端口
    QString ip = ui->lineEditIP->text();
    qint16 port = ui->lineEditPort->text().toInt();

    //获取编辑区内容
    QString str = ui->textEdit->toPlainText();

    //给指定的IP发送数据
    udpSocket->writeDatagram(str.toUtf8(),QHostAddress(ip),port);
}

void Widget::on_ButtonAct1_clicked()
{
    //获取对方的IP和端口
    QString ip = ui->lineEditIP->text();
    qint16 port = ui->lineEditPort->text().toInt();

    //设置动作1发送的内容
//    QString str = "aaaa";
    char str[] = "aaaa";

    //给指定的IP发送数据
    udpSocket->writeDatagram(str,QHostAddress(ip),port);


}

void Widget::on_ButtonAct2_clicked()
{
    //获取对方的IP和端口
    QString ip = ui->lineEditIP->text();
    qint16 port = ui->lineEditPort->text().toInt();

    //设置动作2发送的内容
    QString str = "bbbb";

    //给指定的IP发送数据
    udpSocket->writeDatagram(str.toUtf8(),QHostAddress(ip),port);
}


void Widget::on_ButtonAct3_clicked()
{
    //获取对方的IP和端口
    QString ip = ui->lineEditIP->text();
    qint16 port = ui->lineEditPort->text().toInt();

    //设置动作3发送的内容
    QString str = "cccc";

    //给指定的IP发送数据
    udpSocket->writeDatagram(str.toUtf8(),QHostAddress(ip),port);

}

void Widget::on_ButtonAct4_clicked()
{
    //获取对方的IP和端口
    QString ip = ui->lineEditIP->text();
    qint16 port = ui->lineEditPort->text().toInt();

    //设置动作4发送的内容
    QString str = "dddd";

    //给指定的IP发送数据
    udpSocket->writeDatagram(str.toUtf8(),QHostAddress(ip),port);
}

void Widget::on_ButtonClose_clicked()
{
    this->close();
}
