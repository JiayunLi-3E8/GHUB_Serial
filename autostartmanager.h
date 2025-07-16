#ifndef AUTOSTARTMANAGER_H
#define AUTOSTARTMANAGER_H

#include <QObject>
#include <QQmlEngine>

class AutoStartManager : public QObject
{
    Q_OBJECT
    QML_ELEMENT
public:
    explicit AutoStartManager(QObject *parent = nullptr);

    Q_INVOKABLE bool isAutoStartEnabled();
    Q_INVOKABLE void setAutoStartEnabled(bool enable);
};

#endif // AUTOSTARTMANAGER_H
