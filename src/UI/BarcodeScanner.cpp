//
// Created by Maxim on 21.03.2023.
//

#include "BarcodeScanner.h"

device::BarcodeScanner::BarcodeScanner(QWidget *parent) : QWidget(parent), serial_(new QSerialPort(this)),
ui_(new Ui::BarcodeScannerSettings())
{
    ui_->setupUi(this);

    updatePortList();

    ui_->boundRate_cbox->addItems({"1200", "2400", "4800", "9600", "19200", "38400", "57600", "115200", "Неизвестно"});
    ui_->boundRate_cbox->setCurrentText("9600");

    ui_->bitCount_cbox->addItems({"5", "6", "7", "8"});
    ui_->bitCount_cbox->setCurrentText("8");

    ui_->parity_cbox->addItems({"Нет", "Чётное", "Нечётное", "Пробел", "Метка", "Неизвестно"});
    ui_->parity_cbox->setCurrentText("Нет");

    ui_->stopBit_cbox->addItems({"1", "1.5", "2", "Неизвестно"});
    ui_->stopBit_cbox->setCurrentText("1");

    ui_->flowControl_cbox->addItems({"Нет", "RTS/CTS", "XON/XOFF", "Неизвестно"});
    ui_->flowControl_cbox->setCurrentText("Нет");

    //ui_->prefix_cbox->addItems({});
    //ui_->suffix_cbox->addItems({});

    serial_->setPortName(ui_->port_cbox->currentText());
    serial_->setBaudRate(QSerialPort::Baud9600);
    serial_->setDataBits(QSerialPort::Data8);
    serial_->setParity(QSerialPort::NoParity);
    serial_->setStopBits(QSerialPort::OneStop);
    serial_->setFlowControl(QSerialPort::NoFlowControl);
    serial_->open(QIODevice::ReadOnly);

    QObject::connect(ui_->port_cbox, &QComboBox::currentTextChanged, this, &device::BarcodeScanner::setPortName);
    QObject::connect(ui_->boundRate_cbox, &QComboBox::currentTextChanged, this, &device::BarcodeScanner::setBoundRate);
    QObject::connect(ui_->bitCount_cbox, &QComboBox::currentTextChanged, this, &device::BarcodeScanner::setBitCount);
    QObject::connect(ui_->parity_cbox, &QComboBox::currentTextChanged, this, &device::BarcodeScanner::setParity);
    QObject::connect(ui_->flowControl_cbox, &QComboBox::currentTextChanged, this, &device::BarcodeScanner::setFlowControl);
    QObject::connect(ui_->updPortList_btn, &QPushButton::clicked, this, &device::BarcodeScanner::updatePortList);
}

device::BarcodeScanner::~BarcodeScanner()
{
    delete ui_;
    delete serial_;
    this->disconnect();
}

void device::BarcodeScanner::serialReceive()
{
    QByteArray ba;
    ba = serial_->readAll();
    std::string result = ba.toStdString();
    serialEmit(result);
}

void device::BarcodeScanner::closeEvent(QCloseEvent *event)
{
    this->hide();
}

void device::BarcodeScanner::setPortName(const QString& portName)
{
    serial_->setPortName(portName);
}

void device::BarcodeScanner::setBoundRate(const QString &boundRate)
{
    switch (boundRate.toInt())
    {
        case 1200:
        {
            serial_->setBaudRate(QSerialPort::Baud1200);
            break;
        }
        case 2400:
        {
            serial_->setBaudRate(QSerialPort::Baud2400);
            break;
        }
        case 4800:
        {
            serial_->setBaudRate(QSerialPort::Baud4800);
            break;
        }
        case 9600:
        {
            serial_->setBaudRate(QSerialPort::Baud9600);
            break;
        }
        case 19200:
        {
            serial_->setBaudRate(QSerialPort::Baud19200);
            break;
        }
        case 38400:
        {
            serial_->setBaudRate(QSerialPort::Baud38400);
            break;
        }
        case 57600:
        {
            serial_->setBaudRate(QSerialPort::Baud57600);
            break;
        }
        case 115200:
        {
            serial_->setBaudRate(QSerialPort::Baud115200);
            break;
        }
        default:
        {
            serial_->setBaudRate(QSerialPort::UnknownBaud);
            break;
        }
    }
}

void device::BarcodeScanner::setBitCount(const QString &bitCount)
{
    switch (bitCount.toInt())
    {
        case 5:
        {
            serial_->setDataBits(QSerialPort::Data5);
            break;
        }
        case 6:
        {
            serial_->setDataBits(QSerialPort::Data6);
            break;
        }
        case 7:
        {
            serial_->setDataBits(QSerialPort::Data7);
            break;
        }
        case 8:
        {
            serial_->setDataBits(QSerialPort::Data8);
            break;
        }
        default:
        {
            break;
        }
    }
}

void device::BarcodeScanner::setParity(const QString &parity)
{
    if(parity == QString::fromStdWString(L"Нет"))
    {
        serial_->setParity(QSerialPort::NoParity);
    }
    else if(parity == QString::fromStdWString(L"Чётное"))
    {
        serial_->setParity(QSerialPort::EvenParity);
    }
    else if(parity == QString::fromStdWString(L"Нечётное"))
    {
        serial_->setParity(QSerialPort::OddParity);
    }
    else if(parity == QString::fromStdWString(L"Пробел"))
    {
        serial_->setParity(QSerialPort::SpaceParity);
    }
    else if(parity == QString::fromStdWString(L"Метка"))
    {
        serial_->setParity(QSerialPort::MarkParity);
    }
    else if(parity == QString::fromStdWString(L"Неизвестно"))
    {
        serial_->setParity(QSerialPort::UnknownParity);
    }
}

void device::BarcodeScanner::setStopBits(const QString &stopBits)
{
    if(stopBits == QString::fromStdWString(L"1"))
    {
        serial_->setStopBits(QSerialPort::OneStop);
    }
    else if(stopBits == QString::fromStdWString(L"1.5"))
    {
        serial_->setStopBits(QSerialPort::OneAndHalfStop);
    }
    else if(stopBits == QString::fromStdWString(L"2"))
    {
        serial_->setStopBits(QSerialPort::TwoStop);
    }
    else if(stopBits == QString::fromStdWString(L"Неизвестно"))
    {
        serial_->setStopBits(QSerialPort::UnknownStopBits);
    }
}

void device::BarcodeScanner::setFlowControl(const QString &flowControl)
{
    if(flowControl == QString::fromStdWString(L"Нет"))
    {
        serial_->setFlowControl(QSerialPort::NoFlowControl);
    }
    else if(flowControl == QString::fromStdWString(L"RTS/CTS"))
    {
        serial_->setFlowControl(QSerialPort::HardwareControl);
    }
    else if(flowControl == QString::fromStdWString(L"XON/XOFF"))
    {
        serial_->setFlowControl(QSerialPort::SoftwareControl);
    }
    else if(flowControl == QString::fromStdWString(L"Неизвестно"))
    {
        serial_->setFlowControl(QSerialPort::UnknownFlowControl);
    }
}

void device::BarcodeScanner::updatePortList()
{
    const auto availablePorts = QSerialPortInfo::availablePorts();
    for(const QSerialPortInfo& port : availablePorts)
    {
        ui_->port_cbox->addItem(port.portName());
    }
}
