#include <QGuiApplication>
#include <QQmlApplicationEngine>

#include "inputoptions.h"
#include "qtautotracelib.h"

int main(int argc, char *argv[])
{
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);

    QGuiApplication app(argc, argv);
//    auto inputOptions = new QtAutoTraceV2::InputOptions();
    qmlRegisterType<QtAutoTraceV2::QtAutoTraceLib>("aperture.qtautotracelib",
        1,
        0,
        "QtAutoTraceLib");
//    qmlRegisterSingletonType<QtAutoTraceV2::InputOptions>(
//                "aperture.qtautotracelib",
//                1,
//                0,
//                "QtAutoTraceLib",
//                inputOptions);
    qRegisterMetaType<QtAutoTraceV2::InputOptions>();
//    qmlRegisterType<QtAutoTraceV2::InputOptions>("aperture.qtautotracelib",
//        1,
//        0,
//        "QtAutoTraceLib");

    QQmlApplicationEngine engine;
    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));
    if (engine.rootObjects().isEmpty())
        return -1;

    return app.exec();
}
