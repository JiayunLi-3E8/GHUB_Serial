#include <QSettings>
#include <QCoreApplication>
#include "autostartmanager.h"

AutoStartManager::AutoStartManager(QObject *parent)
    : QObject{parent}
{}

bool AutoStartManager::isAutoStartEnabled()
{
#ifdef Q_OS_WIN
    QSettings reg("HKEY_CURRENT_USER\\Software\\Microsoft\\Windows\\CurrentVersion\\Run",
                  QSettings::NativeFormat);
    return reg.contains(QCoreApplication::applicationName());
#else
    return false; // Linux 另写
#endif
}

void AutoStartManager::setAutoStartEnabled(bool enable)
{
#ifdef Q_OS_WIN
    QSettings reg("HKEY_CURRENT_USER\\Software\\Microsoft\\Windows\\CurrentVersion\\Run",
                  QSettings::NativeFormat);
    QString appPath = QCoreApplication::applicationFilePath().replace('/', '\\');
    if (enable)
        reg.setValue(QCoreApplication::applicationName(), appPath);
    else
        reg.remove(QCoreApplication::applicationName());
#endif
}
