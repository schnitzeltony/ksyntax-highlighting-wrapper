#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <ksyntaxhighlightingwrapper.h>
#include <textarea_enhanced.h>

int main(int argc, char *argv[])
{
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);

    QGuiApplication app(argc, argv);

    QQmlApplicationEngine engine;

    // Register QML extensions
    KSyntaxHighlightingWrapper::registerQml();
    TextAreaEnhanced::registerQml(&engine);

    const QUrl url(QStringLiteral("qrc:/qml/main.qml"));
    QObject::connect(&engine, &QQmlApplicationEngine::objectCreated,
                     &app, [url](QObject *obj, const QUrl &objUrl) {
        if (!obj && url == objUrl)
            QCoreApplication::exit(-1);
    }, Qt::QueuedConnection);
    engine.load(url);

    return app.exec();
}
