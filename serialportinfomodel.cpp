#include "serialportinfomodel.h"
#include <qserialport.h>

SerialPortInfoModel::SerialPortInfoModel(QObject *parent)
    : QAbstractListModel(parent)
{
    refresh();
}

int SerialPortInfoModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    return m_ports.count();
}

QVariant SerialPortInfoModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid() || index.row() < 0 || index.row() >= m_ports.count())
        return {};

    const QSerialPortInfo &info = m_ports.at(index.row());
    switch (role) {
    case PortNameRole:
        return info.portName();
    case DescriptionRole:
        return info.description();
    default:
        return {};
    }
}

QHash<int, QByteArray> SerialPortInfoModel::roleNames() const
{
    QHash<int, QByteArray> roles;
    roles[PortNameRole] = "portName";
    roles[DescriptionRole] = "description";
    return roles;
}

void SerialPortInfoModel::refresh()
{
    beginResetModel();
    m_ports = QSerialPortInfo::availablePorts();
    endResetModel();
    emit baudRatesChanged();
}

QVariantList SerialPortInfoModel::baudRates() const
{
    QVariantList list;
    const auto rates = QSerialPortInfo::standardBaudRates();
    for (auto rate : rates)
        list.append(rate);
    return list;
}
