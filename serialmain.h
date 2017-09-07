#ifndef SERIALMAIN_H
#define SERIALMAIN_H

#include <QMainWindow>
#include <QtWidgets>
#include <QtSerialPort/QSerialPort>
#include <QtSerialPort/QSerialPortInfo>

namespace Ui {
class SerialMain;
}

class SerialMain : public QMainWindow
{
    Q_OBJECT

public:
    explicit SerialMain(QWidget *parent = 0);
    ~SerialMain();

private slots:
    void on_pushButton_clicked();
    void ck();
    void on_Op_clicked();
    void readyReadSlot();
    void on_spname_currentTextChanged(const QString &arg1);

    void on_comboBox_2_currentIndexChanged(const QString &arg1);

private:
    Ui::SerialMain *ui;
    QLabel *labst;
    QLabel *btst;
    QSerialPort qport;
    QSerialPortInfo qportif;
    QByteArray hexStringtoByteArray(QString hex);
    void formatString(QString &org, int n, const QChar &ch);
};

#endif // SERIALMAIN_H
