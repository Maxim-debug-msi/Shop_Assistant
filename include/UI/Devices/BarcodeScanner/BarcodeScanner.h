//
// Created by Maxim on 21.03.2023.
//

#ifndef SHOP_ASSISTANT_BARCODESCANNER_H
#define SHOP_ASSISTANT_BARCODESCANNER_H

#include <QWidget>
#include <QtSerialPort/QSerialPort>
#include <QtSerialPort/QSerialPortInfo>
#include <QByteArray>
#include "ui_BarcodeScanner.h"

namespace device
{
    class BarcodeScanner final : public QWidget
    {
    Q_OBJECT

    public:
        explicit BarcodeScanner(QWidget* parent = nullptr);
        ~BarcodeScanner() override;

    signals:
        void serialEmit(const std::string&);

    private slots:
        void serialReceive();
        void setPortName(const QString& portName);
        void setBoundRate(const QString& boundRate);
        void setBitCount(const QString& bitCount);
        void setParity(const QString& parity);
        void setStopBits(const QString& stopBits);
        void setFlowControl(const QString& flowControl);
        void updatePortList();

    protected:
        void closeEvent(QCloseEvent* event) override;

    private:
        Ui::BarcodeScannerSettings* ui_;
        QSerialPort* serial_;
    };
}

#endif //SHOP_ASSISTANT_BARCODESCANNER_H

