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
        qmlRegisterType(QUrl("qrc:/qml/TextAreaEnhanced/CodeArea.qml"), "TextAreaEnhanced", 1, 0, "CodeArea");
        wasRegistered = true;
    }
#endif
}
