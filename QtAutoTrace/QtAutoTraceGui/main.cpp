#include <QGuiApplication>
#include <QQmlApplicationEngine>

#include "inputoptions.h"
#include "qtautotracelib.h"

int main(int argc, char *argv[])
{
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);

    QGuiApplication app(argc, argv);
    qmlRegisterType<QtAutoTraceV2::QtAutoTraceLib>("aperture.qtautotracelib",
        1,
        0,
        "QtAutoTraceLib");
    qmlRegisterType<QtAutoTraceV2::InputOptions>("aperture.qtautotracelib",
        1,
        0,
        "InputOptions");

    QQmlApplicationEngine engine;
    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));
    if (engine.rootObjects().isEmpty())
        return -1;

    QtAutoTraceV2::QtAutoTraceLib autotrace;

    return app.exec();
}
