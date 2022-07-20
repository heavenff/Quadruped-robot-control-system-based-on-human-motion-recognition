#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QUdpSocket>

QT_BEGIN_NAMESPACE
namespace Ui { class Widget; }
QT_END_NAMESPACE

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = nullptr);
    ~Widget();

    void dealMsg();//槽函数，处理对方发过来的数据

private slots:
    void on_ButtonSend_clicked();

    void on_ButtonAct1_clicked();

    void on_ButtonAct2_clicked();

    void on_ButtonAct3_clicked();

    void on_ButtonAct4_clicked();

    void on_ButtonClose_clicked();

private:
    Ui::Widget *ui;

    QUdpSocket *udpSocket;
};
#endif // WIDGET_H
