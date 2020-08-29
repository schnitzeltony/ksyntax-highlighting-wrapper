#ifndef KSYNTAX_HIGHLIGHTING_WRAPPER_H
#define KSYNTAX_HIGHLIGHTING_WRAPPER_H

#include "ksyntaxhighlightingwrapper_export.h"
#include <QQuickTextDocument>

QT_BEGIN_NAMESPACE

// forward
class QQmlEngine;

// clang: be quiet
QT_WARNING_DISABLE_CLANG("-Wshadow-field")

class KSyntaxHighlightingWrapperPrivate;
class KSYNTAXHIGHLIGHTINGWRAPPER_EXPORT KSyntaxHighlightingWrapper : public QObject
{
  Q_OBJECT
public:
    explicit KSyntaxHighlightingWrapper(QObject *parent = nullptr);
    virtual ~KSyntaxHighlightingWrapper();

    // As name suggests: register for QML usage
    static void registerQml();

    // document
    Q_PROPERTY(QTextDocument *textDocument READ textDocument WRITE setTextDocument NOTIFY documentChanged)
    Q_PROPERTY(QQuickTextDocument *qmlTextDocument READ qmlTextDocument WRITE setQmlTextDocument NOTIFY documentChanged)

    // access current definition
    Q_PROPERTY(QString definitionName READ definitionName WRITE setDefinitionName NOTIFY definitionChanged)
    Q_PROPERTY(int definitionNumber READ definitionNumber WRITE setDefinitionNumber NOTIFY definitionChanged)
    Q_INVOKABLE void setDefinitionForFileName(const QString &fileName);
    Q_INVOKABLE void setDefinitionForMimeType(const QString &mimeType);

    // available definitions
    Q_PROPERTY(QStringList definitionNames CONSTANT READ definitionNames)

    // theme access
    Q_PROPERTY(QString themeName READ themeName WRITE setThemeName NOTIFY themeChanged)
    Q_PROPERTY(QString themeNameTranslated READ themeNameTranslated NOTIFY themeChanged)
    Q_PROPERTY(int themeNumber READ themeNumber WRITE setThemeNumber NOTIFY themeChanged)

    // available themes
    Q_PROPERTY(QStringList themeNames CONSTANT READ themeNames)
    Q_PROPERTY(QStringList themeNamesTranslated CONSTANT READ themeNamesTranslated)


    // implemetation for properties & invokables
    void setTextDocument(QTextDocument *textDocument);
    QTextDocument *textDocument() const;

    void setQmlTextDocument(QQuickTextDocument *qmlTextDocument);
    QQuickTextDocument *qmlTextDocument() const;

    const QString definitionName();
    void setDefinitionName(const QString& definitionName);
    int definitionNumber();
    void setDefinitionNumber(const int definitionNumber);

    const QStringList definitionNames() const;

    const QString themeName();
    void setThemeName(const QString& themeName);
    const QString themeNameTranslated();
    int themeNumber();
    void setThemeNumber(const int themeNumber);

    const QStringList themeNames() const;
    const QStringList themeNamesTranslated() const;

signals:
    void documentChanged();
    void definitionChanged();
    void themeChanged();

public slots:

protected:
    const QScopedPointer<KSyntaxHighlightingWrapperPrivate> d_ptr;
    KSyntaxHighlightingWrapper(KSyntaxHighlightingWrapperPrivate &dd, QObject *parent);
    Q_DECLARE_PRIVATE(KSyntaxHighlightingWrapper)
};

QT_END_NAMESPACE

#endif // KSYNTAX_HIGHLIGHTING_WRAPPER_H