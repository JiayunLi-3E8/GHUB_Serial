#ifndef GLOBALKEYBOARDHOOK_H
#define GLOBALKEYBOARDHOOK_H

#include <QObject>
#include <QQmlEngine>
#ifdef Q_OS_WIN
#include <Windows.h>
#endif


class GlobalKeyboardHook : public QObject
{
    Q_OBJECT
    QML_ELEMENT
public:
    explicit GlobalKeyboardHook(QObject *parent = nullptr);
    ~GlobalKeyboardHook();

    bool installHook();
    void uninstallHook();

signals:
    void keyPressed(Qt::Key pressedKey);

private:
#ifdef Q_OS_WIN
    static LRESULT CALLBACK KeyboardProc(int nCode, WPARAM wParam, LPARAM lParam);
    static HHOOK g_hHook;
    static GlobalKeyboardHook *instance;
#endif
};

#endif // GLOBALKEYBOARDHOOK_H
