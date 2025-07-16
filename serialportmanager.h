#pragma once

#include <QObject>
#include <QSerialPort>
#include <QQmlEngine>
#include "crc32.h"

class SerialPortManager : public QObject
{
    Q_OBJECT
    QML_ELEMENT
    Q_PROPERTY(QString portName READ portName WRITE setPortName NOTIFY portNameChanged)
    Q_PROPERTY(int baudRate READ baudRate WRITE setBaudRate NOTIFY baudRateChanged)
    Q_PROPERTY(DataBits byteSize READ byteSize WRITE setByteSize NOTIFY byteSizeChanged)
    Q_PROPERTY(Parity parity READ parity WRITE setParity NOTIFY parityChanged)
    Q_PROPERTY(StopBits stopBits READ stopBits WRITE setStopBits NOTIFY stopBitsChanged)
    Q_PROPERTY(bool isOpen READ isOpen NOTIFY isOpenChanged)
    Q_PROPERTY(QString currentPortName READ currentPortName NOTIFY currentPortNameChanged)
public:
    enum DataBits {
        Data5 = QSerialPort::Data5,
        Data6 = QSerialPort::Data6,
        Data7 = QSerialPort::Data7,
        Data8 = QSerialPort::Data8
    };
    Q_ENUM(DataBits)

    enum Parity {
        NoParity = QSerialPort::NoParity,
        EvenParity = QSerialPort::EvenParity,
        OddParity = QSerialPort::OddParity,
        SpaceParity = QSerialPort::SpaceParity,
        MarkParity = QSerialPort::MarkParity
    };
    Q_ENUM(Parity)

    enum StopBits {
        OneStop = QSerialPort::OneStop,
        OneAndHalfStop = QSerialPort::OneAndHalfStop,
        TwoStop = QSerialPort::TwoStop
    };
    Q_ENUM(StopBits)

    explicit SerialPortManager(QObject *parent = nullptr);

    QString portName() const { return m_portName; }
    void setPortName(const QString &port);

    int baudRate() const { return m_baudRate; }
    void setBaudRate(int rate);

    DataBits byteSize() const { return m_byteSize; }
    void setByteSize(DataBits size);

    Parity parity() const { return m_parity; }
    void setParity(Parity p);

    StopBits stopBits() const { return m_stopBits; }
    void setStopBits(StopBits s);

    bool isOpen() const {return serial.isOpen();}

    QString currentPortName() const {return serial.portName();}

    Q_INVOKABLE bool open();
    Q_INVOKABLE void close();
    Q_INVOKABLE void sendCommand(const QString& cmd);

signals:
    void portNameChanged();
    void baudRateChanged();
    void byteSizeChanged();
    void parityChanged();
    void stopBitsChanged();
    void isOpenChanged(bool isOpen);
    void currentPortNameChanged();

private:
    QSerialPort serial;
    Crc32 crc32;

    QString m_portName;
    int m_baudRate = 115200;
    DataBits m_byteSize = Data8;
    Parity m_parity = NoParity;
    StopBits m_stopBits = OneStop;
};
