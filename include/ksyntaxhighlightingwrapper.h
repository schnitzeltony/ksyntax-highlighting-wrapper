#ifndef KSYNTAX_HIGHLIGHTING_WRAPPER_H
#define KSYNTAX_HIGHLIGHTING_WRAPPER_H

#include "ksyntaxhighlightingwrapper_export.h"
#include <QObject>

QT_BEGIN_NAMESPACE
class QTextDocument;
class QQuickTextDocument;
class KSyntaxHighlightingWrapperPrivate;

// clang: be quiet
QT_WARNING_DISABLE_CLANG("-Wshadow-field")

class KSYNTAXHIGHLIGHTINGWRAPPER_EXPORT KSyntaxHighlightingWrapper : public QObject
{
  Q_OBJECT
public:
    explicit KSyntaxHighlightingWrapper(QObject *parent = nullptr);
    virtual ~KSyntaxHighlightingWrapper();

    // connect document
    Q_INVOKABLE void setTextDocument(QObject *textDocument);
    Q_INVOKABLE void setQmlTextDocument(QObject *qmlTextDocument);

    // select definition
    Q_PROPERTY(QString definitionName READ definitionName WRITE setDefinitionName NOTIFY definitionChanged)
    Q_INVOKABLE void definitionForFileName(const QString &fileName);
    Q_INVOKABLE void definitionForMimeType(const QString &mimeType);

    // theme access
    Q_PROPERTY(QString themeName READ themeName WRITE setThemeName NOTIFY themeChanged)
    Q_PROPERTY(QString themeNameTranslated READ themeNameTranslated NOTIFY themeChanged)
    Q_PROPERTY(int themeNumber READ themeNumber WRITE setThemeNumber NOTIFY themeChanged)

    // available themes
    Q_PROPERTY(QStringList themeNames READ themeNames)
    Q_PROPERTY(QStringList themeNamesTranslated READ themeNamesTranslated)

    //void setQmlTextDocument(const QQuickTextDocument* qmlTextDocument);

    const QString definitionName() const;
    void setDefinitionName(const QString& definitionName);

    const QString themeName() const;
    void setThemeName(const QString& themeName);
    const QString themeNameTranslated() const;
    int themeNumber() const;
    void setThemeNumber(const int themeNumber);

    const QStringList themeNames() const;
    const QStringList themeNamesTranslated() const;

signals:
    void definitionChanged();
    void themeChanged();

public slots:

protected:
    const QScopedPointer<KSyntaxHighlightingWrapperPrivate> d_ptr;
    KSyntaxHighlightingWrapper(KSyntaxHighlightingWrapperPrivate &dd, QObject *parent);
    Q_DECLARE_PRIVATE(KSyntaxHighlightingWrapper)
private:
};

QT_END_NAMESPACE

#endif // KSYNTAX_HIGHLIGHTING_WRAPPER_H
