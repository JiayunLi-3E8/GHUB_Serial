#pragma once

#include <QAbstractListModel>
#include <QSerialPortInfo>
#include <QQmlEngine>

class SerialPortInfoModel : public QAbstractListModel
{
    Q_OBJECT
    QML_ELEMENT
    Q_PROPERTY(QVariantList baudRates READ baudRates NOTIFY baudRatesChanged)

public:
    explicit SerialPortInfoModel(QObject *parent = nullptr);

    enum Roles {
        PortNameRole = Qt::UserRole + 1,
        DescriptionRole
    };

    // QAbstractListModel overrides
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    QHash<int, QByteArray> roleNames() const override;

    Q_INVOKABLE void refresh();
    QVariantList baudRates() const;

signals:
    void baudRatesChanged();

private:
    QList<QSerialPortInfo> m_ports;
};
