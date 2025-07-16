#include "serialportmanager.h"
#include <QByteArray>
#include <QDebug>

SerialPortManager::SerialPortManager(QObject *parent)
    : QObject(parent)
{
}

void SerialPortManager::setPortName(const QString &port) {
    if (port != m_portName) {
        m_portName = port;
        emit portNameChanged();
    }
}

void SerialPortManager::setBaudRate(int rate) {
    if (rate != m_baudRate) {
        m_baudRate = rate;
        emit baudRateChanged();
    }
}

void SerialPortManager::setByteSize(DataBits size) {
    if (size != m_byteSize) {
        m_byteSize = size;
        emit byteSizeChanged();
    }
}

void SerialPortManager::setParity(Parity p) {
    if (p != m_parity) {
        m_parity = p;
        emit parityChanged();
    }
}

void SerialPortManager::setStopBits(StopBits s) {
    if (s != m_stopBits) {
        m_stopBits = s;
        emit stopBitsChanged();
    }
}

bool SerialPortManager::open()
{
    if (!m_portName.isEmpty()) {
        serial.setPortName(m_portName);
        emit currentPortNameChanged();
        serial.setBaudRate(m_baudRate);
        serial.setDataBits(static_cast<QSerialPort::DataBits>(m_byteSize));
        serial.setParity(static_cast<QSerialPort::Parity>(m_parity));
        serial.setStopBits(static_cast<QSerialPort::StopBits>(m_stopBits));

        if (!serial.open(QIODeviceBase::OpenModeFlag::WriteOnly)) {
            qWarning() << "Failed to open serial port:" << serial.errorString();
            emit isOpenChanged(serial.isOpen());
            return false;
        } else {
            emit isOpenChanged(serial.isOpen());
            return true;
        }
    } else {
        qWarning() << "portName is Empty!";
        return false;
    }
}

void SerialPortManager::close()
{
    if (serial.isOpen()) {
        serial.close();
        emit isOpenChanged(serial.isOpen());
    }
}

void SerialPortManager::sendCommand(const QString& cmd)
{
    if (serial.isOpen()) {
        QByteArray data(16, 0);
        QByteArray cmdBytes = cmd.toUtf8();
        int copyLen = qMin(cmdBytes.size(), 12);
        memcpy(data.data(), cmdBytes.data(), copyLen);

        uint32_t crc = crc32.calculate((uint32_t*)data.constData(), 12/4);
        data[12] = crc & 0xFF;
        data[13] = (crc >> 8) & 0xFF;
        data[14] = (crc >> 16) & 0xFF;
        data[15] = (crc >> 24) & 0xFF;

        serial.write(data);
        serial.flush();

        qDebug() << "Sent:" << cmd << "CRC32:" << QString::number(crc, 16);
    }
}
