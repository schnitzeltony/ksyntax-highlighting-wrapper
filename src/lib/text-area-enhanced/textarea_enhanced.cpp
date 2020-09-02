#include "textarea_enhanced.h"
#include <QQmlEngine>

#if !defined(KSW_TAE_QML_DEBUG_SOURCE_DIR)
static bool wasRegistered = false;
#endif

void TextAreaEnhanced::registerQml(QQmlEngine *engine)
{
#if defined(KSW_TAE_QML_DEBUG_SOURCE_DIR)
    QStringList importPaths = engine->importPathList();
    QString importPath = QStringLiteral(QT_STRINGIFY(KSW_TAE_QML_DEBUG_SOURCE_DIR));
    if(!importPaths.contains(importPath)) {
        engine->addImportPath(importPath);
    }
#else
    if(!wasRegistered) {
        Q_UNUSED(engine)
        qmlRegisterType(QUrl("qrc:/qml/TextAreaEnhanced/TextAreaExCodeArea.qml"), "TextAreaEnhanced", 1, 0, "TextAreaExCodeArea");
        qmlRegisterType(QUrl("qrc:/qml/TextAreaEnhanced/TextAreaExConnector.qml"), "TextAreaEnhanced", 1, 0, "TextAreaExConnector");
        qmlRegisterType(QUrl("qrc:/qml/TextAreaEnhanced/TextAreaExSearchFrame.qml"), "TextAreaEnhanced", 1, 0, "TextAreaExSearchFrame");
        wasRegistered = true;
    }
#endif
}
