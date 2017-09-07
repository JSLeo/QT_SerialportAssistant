#include "serialmain.h"
#include "ui_serialmain.h"
#include <QPropertyAnimation>

SerialMain::SerialMain(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::SerialMain)
{
    ui->setupUi(this);
    labst= new QLabel("CLOSE", this);
    btst= new QLabel("", this);

//    btst->setFrameShadow(QFrame::Sunken);
//    btst->setFrameShape(QFrame::Box);
    labst->setFrameShape(QFrame::Box);
    labst->setStyleSheet("color:'red'");
    //statusBar()->addPermanentWidget(btst);
    statusBar()->addPermanentWidget(labst);
    QList<QSerialPortInfo> splist = qportif.availablePorts();
    for(int i=0;i<splist.count();i++)
    {

        ui->spname->addItem(splist[i].portName(),i);
    }
    statusBar()->showMessage(QString("Find ")+QString::number(splist.count())+"  Ports",5000);
    qport.setBaudRate(921600);
    qport.setDataBits(QSerialPort::Data8);
    qport.setParity(QSerialPort::NoParity);
    qport.setStopBits(QSerialPort::OneStop);
    qport.setFlowControl(QSerialPort::NoFlowControl);
    qport.setReadBufferSize(2048);
    connect(&qport,SIGNAL(readyRead()),this, SLOT(readyReadSlot()));
}

void SerialMain::readyReadSlot()
{
    QByteArray arr ;
    arr = qport.readAll();
    if(ui->r_c->isChecked())
        ui->Rec->appendPlainText(arr.toHex());
    else
        ui->Rec->appendPlainText(QString(arr));

}
void SerialMain::ck()
{
   btst->setFrameShape(QFrame::Box);
}

SerialMain::~SerialMain()
{
    delete ui;
}
void  SerialMain::formatString(QString &org, int n, const QChar &ch)
{
    int size= org.size();
    int space= qRound(size*1.0/n+0.5)-1;
    if(space<=0)
        return;
    for(int i=0,pos=n;i<space;++i,pos+=(n+1))
    {
        org.insert(pos,ch);
    }
}
QByteArray SerialMain::hexStringtoByteArray(QString hex)
{
    QByteArray ret;
    hex=hex.trimmed();
    formatString(hex,2,' ');
    QStringList sl=hex.split(" ");
    foreach(QString s,sl)
    {
        if(!s.isEmpty())
            ret.append((char)s.toInt(0,16)&0xFF);
    }
    return ret;
}
void SerialMain::on_pushButton_clicked()
{
    QByteArray sd;
    if(ui->t_c->isChecked())
        sd=hexStringtoByteArray(ui->t->toPlainText());
    else
        sd=ui->t->toPlainText().toLocal8Bit();

    int ret=qport.write(sd);
    statusBar()->showMessage("Send=>"+QString::number(ret)+"bytes",2000);
}

//void SerialMain::on_pushButton_2_clicked()
//{
////    QPropertyAnimation *animation = new QPropertyAnimation(ui->Rec, "geometry");
////      animation->setDuration(500);//持续时间
////      animation->setStartValue(QRect(0, 0, 100, 30));
////      animation->setEndValue(QRect(250, 250, 100, 30));

////      animation->start();
//}

void SerialMain::on_Op_clicked()
{
    if(qport.isOpen())
        qport.close();
    qport.open(QIODevice::ReadWrite);
    if(qport.isOpen()){
        labst->setStyleSheet("color:'green'");
        labst->setText("OPEN: "+qport.portName() +"-"+QString::number(qport.baudRate()));
         statusBar()->showMessage("OPEN: "+qport.portName(),1000);
    }else
    {
        labst->setStyleSheet("color:'red'");
        labst->setText("CLOSE");
        statusBar()->showMessage("OPEN: "+qport.portName() +"Fail",1000);
    }
}

void SerialMain::on_spname_currentTextChanged(const QString &arg1)
{
    statusBar()->showMessage(arg1,500);
    qport.setPortName(arg1);
}

void SerialMain::on_comboBox_2_currentIndexChanged(const QString &arg1)
{
    qport.setBaudRate(arg1.toInt());
}
