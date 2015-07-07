#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "treerobotconstant.h"
#include <QtWidgets>
#include <QObject>
#include <QLabel>
#include <QPushButton>
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    //serialPort initialize
    serial = new serialPort(this);
    ui->comboBox->addItems(serial->getSerialPortList());
    QObject::connect(serial,&serialPort::eulerDataUpdated,this,&MainWindow::updatedEulerDataFromSP);

    //velocity ui
    //sliderVelocity = new SliderVelocity(this);
    //sliderVelocity->setRange(0,100);
    ui->horizontalSliderVelocity->setRange(0,100);
    ui->spinBoxVelocity->setRange(0,100);
    //QObject::connect(sliderVelocity,&SliderVelocity::valueChanged,ui->spinBoxVelocity,&QSpinBox::setValue);
    QObject::connect(ui->horizontalSliderVelocity,&SliderVelocity::valueChanged,ui->spinBoxVelocity,&QSpinBox::setValue);
    void (QSpinBox::*spinBoxSignal)(int) = &QSpinBox::valueChanged;
    QObject::connect(ui->spinBoxVelocity,spinBoxSignal,ui->horizontalSliderVelocity,&SliderVelocity::setValue);
    //QObject::connect(ui->spinBoxVelocity,spinBoxSignal,sliderVelocity,&SliderVelocity::setValue);
    QObject::connect(ui->horizontalSliderVelocity,&SliderVelocity::signalvalueChanged,this,&MainWindow::velocityChanged);
    //QObject::connect(ui->spinBoxVelocity,spinBoxSignal,this,&MainWindow::velocityChanged);
    //QObject::connect(sliderVelocity,&SliderVelocity::signalvalueChanged,this,&MainWindow::velocityChanged);

    //status bar set

    msgLabel = new QLabel;
    msgLabel->setMinimumSize(msgLabel->sizeHint());
    msgLabel->setAlignment(Qt::AlignHCenter);
    statusBar()->addWidget(msgLabel);
    //statusBar()->setStyleSheet(QString("QStatusBar::item{border: 0px}"));


    QObject::connect(ui->pushButtonSend,&QPushButton::clicked,this,&MainWindow::sendDataBySP);
    QObject::connect(ui->pushButtonOpenPort,&QPushButton::clicked,this,&MainWindow::openSerialPort);
    QObject::connect(ui->pushButtonClosePort,&QPushButton::clicked,this,&MainWindow::closeSerialPort);

    this->installEventFilter(this);
//==================kint======================
    // 开启一个1秒定时器，返回其ID
    id1 = startTimer(50);
    QObject::connect(this,&MainWindow::imageReady,this,&MainWindow::imgShow);
    camera_init_finish =false;
//==================kint======================//
}

MainWindow::~MainWindow()
{
    delete ui;
}
void MainWindow::updatedEulerDataFromSP(const QString &y,const QString &p,const QString &r)
{
    ui->labelR1FromSP->setText(p);
    ui->labelR2FromSP->setText(r);
    ui->labelR3FromSP->setText(y);
}
void MainWindow::sendDataBySP()
{
    QString str = ui->textEdit->toPlainText();
    serial->writeData(str.toLocal8Bit());
}
void MainWindow::CVDataSyn()
{

}

void MainWindow::openSerialPort()
{
    serial->openSerialPort(ui->comboBox->currentText());
    msgLabel->setText(serial->getPortStatus());
}
void MainWindow::closeSerialPort()
{
    serial->closeSerialPort();
    msgLabel->setText(serial->getPortStatus());
}
void MainWindow::velocityChanged(int v)
{
    serial->writeData(SET_VELOCITY);
    serial->writeData(v);
    qDebug()<<"Velocity: "<<v;
}

bool MainWindow::eventFilter(QObject *obj, QEvent *event)
{
    if(event->type()==QEvent::KeyPress)
    {
        handleKeyPressed((QKeyEvent *)event);
        return true;
    }
    else if(event->type()==QEvent::KeyRelease)
    {
        handleKeyRelease((QKeyEvent *)event);
        return true;
    }
    else
    {
        return QObject::eventFilter(obj,event);
    }
}
void MainWindow::handleKeyRelease(QKeyEvent *event)
{
    //msgLabel->setText(QObject::tr("NULL"));
    ui->pushButtonKey_S->setStyleSheet("background-color:#ffffff");
    ui->pushButtonKey_W->setStyleSheet("background-color:#ffffff");
}

void MainWindow::handleKeyPressed(QKeyEvent *event)
{
    switch(event->key())
    {
    case Qt::Key_S:
        serial->writeData(SET_DIRECTION_UP);
        msgLabel->setText(QObject::tr("Key_S"));
        ui->pushButtonKey_S->setStyleSheet("background-color:#0055ff");
        qDebug()<<"Key_S";
        break;
    case Qt::Key_W:
        serial->writeData(SET_DIRECTION_DOWN);
        msgLabel->setText(QObject::tr("Key_W"));
        ui->pushButtonKey_W->setStyleSheet("background-color:#0055ff");
        qDebug()<<"Key_W";
        break;
    case Qt::Key_1:
        serial->writeData(SET_DIRECTION_LEFT_LOCK);
        msgLabel->setText(QObject::tr("Key_1"));
        if(ui->checkBoxKey_1->checkState()==Qt::Checked)
        {
            ui->checkBoxKey_1->setCheckState(Qt::Unchecked);
        }
        else
        {
            ui->checkBoxKey_1->setCheckState(Qt::Checked);
        }
        qDebug()<<"Key_1";
        break;
    case Qt::Key_2:
        serial->writeData(SET_DIRECTION_MIDDLE_LOCK);
        msgLabel->setText(QObject::tr("Key_2"));
        if(ui->checkBoxKey_2->checkState()==Qt::Checked)
        {
            ui->checkBoxKey_2->setCheckState(Qt::Unchecked);
        }
        else
        {
            ui->checkBoxKey_2->setCheckState(Qt::Checked);
        }
        qDebug()<<"Key_2";
        break;
    case Qt::Key_3:
        serial->writeData(SET_DIRECTION_RIGHT_LOCK);
        msgLabel->setText(QObject::tr("Key_3"));
        if(ui->checkBoxKey_3->checkState()==Qt::Checked)
        {
            ui->checkBoxKey_3->setCheckState(Qt::Unchecked);
        }
        else
        {
            ui->checkBoxKey_3->setCheckState(Qt::Checked);
        }
        qDebug()<<"Key_3";
        break;
    case Qt::Key_4:
        serial->writeData(SET_TOP_CLAW_LOCK);
        msgLabel->setText(QObject::tr("Key_4"));
        if(ui->checkBoxKey_4->checkState()==Qt::Checked)
        {
            ui->checkBoxKey_4->setCheckState(Qt::Unchecked);
        }
        else
        {
            ui->checkBoxKey_4->setCheckState(Qt::Checked);
        }
        qDebug()<<"Key_4";
        break;
    case Qt::Key_5:
        serial->writeData(SET_TRAIL_CLAW_LOCK);
        msgLabel->setText(QObject::tr("Key_5"));
        if(ui->checkBoxKey_5->checkState()==Qt::Checked)
        {
            ui->checkBoxKey_5->setCheckState(Qt::Unchecked);
        }
        else
        {
            ui->checkBoxKey_5->setCheckState(Qt::Checked);
        }
        qDebug()<<"Key_5";
        break;
    }
}

//================================kint======================

void MainWindow::timerEvent(QTimerEvent *event)
{
    // 判断是哪个定时器
    if(event->timerId() == id1 )
    {
       qDebug() << "timer1";
        if(camera_init_finish) //初始判断
        {
            bool run_state = pCam_qr_estimation->run();
            if (!run_state)  // 重插式
            {
                cout<<"============================================================="<<endl;
                cout<<"waitting for replus the usb camera !!"<<endl;
                cout<<"replus the usb camera ,camera will reinit and be ok !!"<<endl;
                cout<<"============================================================="<<endl;
                cv::waitKey(1000);
                pCam_qr_estimation->Init(configure_file,camera_ID);
            }
            else //  释放信号
            {
               // pCam_qr_estimation->pQrDetect_->t_[0];
                qimg = Mat2QImage(pCam_qr_estimation->frame);
                img_test = QImage2Mat(qimg);
                emit imageReady();
           }
        }
        else
        {
            qDebug() << "  camera init not finish ";
            init(configure_file,camera_ID);
        }
    }
    else
    {
        qDebug() << "timer erro  ";
    }
}
void MainWindow::init(char* addr,int camId)
{
    camera_ID = camId;
    configure_file = addr;
   pCam_qr_estimation = new CameraEstimation;
   bool camera_state =  pCam_qr_estimation->Init(addr,camId) ;
   if(!camera_state)
   {
       cout<<"usb  camera does not plug in !"<<endl;
       cout<<"please close program  and plug in usb camera ,then restart it !"<<endl;
       //getchar();
       camera_init_finish = false;
   }
   else
   {
       cout <<"camera open !"<<endl;
       camera_init_finish = true;
   }
}


QImage MainWindow::Mat2QImage(cv::Mat const& src)
{
     cv::Mat temp; // make the same cv::Mat
     cvtColor(src, temp,CV_BGR2RGB); // cvtColor Makes a copt, that what i need
     QImage dest((const uchar *) temp.data, temp.cols, temp.rows, temp.step, QImage::Format_RGB888);
     dest.bits(); // enforce deep copy, see documentation
     // of QImage::QImage ( const uchar * data, int width, int height, Format format )
     return dest;
}

cv::Mat MainWindow::QImage2Mat(QImage const& src)
{
     cv::Mat tmp(src.height(),src.width(),CV_8UC3,(uchar*)src.bits(),src.bytesPerLine());
     cv::Mat result; // deep copy just in case (my lack of knowledge with open cv)
     cvtColor(tmp, result,CV_BGR2RGB);
     return result;
}
void MainWindow::imgShow()
{
    //qimg.load("/home/yhzhao/Desktop/12.png");
        ui->labelImage->setPixmap(QPixmap::fromImage(qimg));

    ui->labelR1FromCV->setText(QString("%1").arg(pCam_qr_estimation->pQrDetect_->r_[0], 0,'g',4));
    ui->labelR2FromCV->setText(QString("%1").arg(pCam_qr_estimation->pQrDetect_->r_[1], 0,'g',4));
    ui->labelR3FromCV->setText(QString("%1").arg(pCam_qr_estimation->pQrDetect_->r_[2], 0,'g',4));

    ui->labelT1FromCV->setText(QString("%1").arg(pCam_qr_estimation->pQrDetect_->t_[0], 0,'g',4));
    ui->labelT2FromCV->setText(QString("%1").arg(pCam_qr_estimation->pQrDetect_->t_[1], 0,'g',4));
    ui->labelT3FromCV->setText(QString("%1").arg(pCam_qr_estimation->pQrDetect_->t_[2], 0,'g',4));
    qDebug()<<"Tigger";
}

//================================kint======================//



