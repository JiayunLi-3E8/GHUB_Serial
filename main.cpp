#include <QApplication>
#include <QQmlApplicationEngine>
#include <qicon.h>
#include <QQuickWindow>
#include "kdsingleapplication.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    KDSingleApplication kdapp;

    if (!kdapp.isPrimaryInstance()) {
        kdapp.sendMessage("activate");
        return 0;
    }

    app.setOrganizationName("GHUBSerial");
    app.setOrganizationDomain("ghubserial.com");
    app.setApplicationName("GHUBSerial");
    app.setWindowIcon(QIcon(":/qt/qml/GHUBSerial/res/TrayIcon.ico"));

    QQmlApplicationEngine engine;
    QObject::connect(
        &engine,
        &QQmlApplicationEngine::objectCreationFailed,
        &app,
        []() { QCoreApplication::exit(-1); },
        Qt::QueuedConnection);
    engine.loadFromModule("GHUBSerial", "Main");

    QObject::connect(&kdapp, &KDSingleApplication::messageReceived, &engine,
        [&engine](const QByteArray &msg){
            if (msg == "activate") {
                auto roots = engine.rootObjects();
                QQuickWindow *window = qobject_cast<QQuickWindow*>(roots.first());
                window->show();
                window->raise();
                window->requestActivate();
            }
        });

    return app.exec();
}
