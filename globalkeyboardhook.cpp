#include "globalkeyboardhook.h"

#ifdef Q_OS_WIN
HHOOK GlobalKeyboardHook::g_hHook = nullptr;
GlobalKeyboardHook *GlobalKeyboardHook::instance = nullptr;

GlobalKeyboardHook::GlobalKeyboardHook(QObject *parent)
    : QObject(parent)
{
    instance = this;
    installHook();
}

GlobalKeyboardHook::~GlobalKeyboardHook()
{
    uninstallHook();
    instance = nullptr;
}

bool GlobalKeyboardHook::installHook()
{
    if (!g_hHook)
    {
        g_hHook = SetWindowsHookEx(WH_KEYBOARD_LL, KeyboardProc, GetModuleHandle(nullptr), 0);
        if (!g_hHook)
        {
            qWarning() << "Failed to install global keyboard hook!";
            return false;
        }
        qDebug() << "Global keyboard hook installed.";
    }
    return true;
}

void GlobalKeyboardHook::uninstallHook()
{
    if (g_hHook)
    {
        UnhookWindowsHookEx(g_hHook);
        g_hHook = nullptr;
        qDebug() << "Global keyboard hook uninstalled.";
    }
}

LRESULT CALLBACK GlobalKeyboardHook::KeyboardProc(int nCode, WPARAM wParam, LPARAM lParam)
{
    if (nCode == HC_ACTION && wParam == WM_KEYDOWN)
    {
        KBDLLHOOKSTRUCT *p = reinterpret_cast<KBDLLHOOKSTRUCT *>(lParam);

        if (p->vkCode >= VK_F13 && p->vkCode <= VK_F24)
        {
            emit instance->keyPressed(p->vkCode);
        }
    }
    return CallNextHookEx(g_hHook, nCode, wParam, lParam);
}
#else
GlobalKeyboardHook::GlobalKeyboardHook(QObject *parent) : QObject(parent) {}
GlobalKeyboardHook::~GlobalKeyboardHook() {}
bool GlobalKeyboardHook::installHook() { return false; }
void GlobalKeyboardHook::uninstallHook() {}
#endif
