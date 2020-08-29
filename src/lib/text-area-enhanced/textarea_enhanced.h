#ifndef TEXT_AREA_ENHANCED_H
#define TEXT_AREA_ENHANCED_H

#include <textareaenhanced_export.h>
#include <QObject>

QT_BEGIN_NAMESPACE

// forward
class QQmlEngine;

class TEXTAREAENHANCED_EXPORT TextAreaEnhanced
{
public:
    /**
     * @brief registerQml Register TextAreaEnhaced QML component (add 'import TextAreaEnhaced 1.0' in your QML
     * to use it
     * @param engine the engine to register for - only used for debug
     */
    static void registerQml(QQmlEngine *engine);
};

QT_END_NAMESPACE

#endif // TEXT_AREA_ENHANCED_H
