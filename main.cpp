#ifdef QT_WIDGETS_LIB
#include <QApplication>
#else
#include <QGuiApplication>
#endif
#include <QFontDatabase>
#include <QDebug>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QQuickStyle>

#include "documenthandler.h"

int main(int argc, char *argv[])
{
    QGuiApplication::setApplicationName("Word Processor");
    QGuiApplication::setOrganizationName("QtProject");

#ifdef QT_WIDGETS_LIB
    QApplication app(argc, argv);
#else
    QGuiApplication app(argc, argv);
#endif

    if (QFontDatabase::addApplicationFont(":/fonts/fontello.ttf") == -1)
        qWarning() << "Failed to load fontello.ttf";

    qmlRegisterType<DocumentHandler>("my.wordprocessor", 1, 0, "DocumentHandler");

    QStringList selectors;
#ifdef QT_EXTRA_FILE
    selectors += QT_EXTRA_FILE;
#endif

    QQmlApplicationEngine engine;
    engine.setExtraFileSelectors(selectors);

    engine.load(QUrl("qrc:/qml/wordprocessor.qml"));
    if (engine.rootObjects().isEmpty())
        return -1;

    return app.exec();
}
