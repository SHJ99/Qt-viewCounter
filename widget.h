#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QSslSocket>
#include <QRegularExpression>

QT_BEGIN_NAMESPACE
namespace Ui {
class Widget;
}
QT_END_NAMESPACE

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = nullptr);
    ~Widget();

    QSslSocket socket;

    QString parser(const QString &str);

public slots:
    void doConneted();
    void doDisconneted();
    void doReadyread();

private slots:
    void on_pbRequest_clicked();

    void on_pbClear_clicked();

private:
    Ui::Widget *ui;
};
#endif // WIDGET_H
