#ifndef KSYNTAX_HIGHLIGHTING_WRAPPER_H
#define KSYNTAX_HIGHLIGHTING_WRAPPER_H

#include "ksyntaxhighlightingwrapper_export.h"
#include <QObject>

QT_BEGIN_NAMESPACE

// forward
class QQmlEngine;
class QQuickTextDocument;
class QTextDocument;

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

    // KSyntaxHighligher
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

    // Search highlighter
    // search params
    Q_PROPERTY(QString search READ search WRITE setSearch NOTIFY searchParamChanged)
    Q_PROPERTY(bool caseSensitive READ caseSensitive WRITE setCaseSensitive NOTIFY searchParamChanged)
    Q_PROPERTY(bool wholeWords READ wholeWords WRITE setWholeWords NOTIFY searchParamChanged)
    Q_PROPERTY(bool regExpr READ regExpr WRITE setRegExpr NOTIFY searchParamChanged)

    // search methods
    Q_INVOKABLE void findPrevious();
    Q_INVOKABLE void findNext();
    Q_INVOKABLE void setVisibleArea(const int firstLine, const int lastLine);

    // replace
    Q_PROPERTY(QString replace READ replace WRITE setReplace NOTIFY replaceChanged)

    // higlight color
    Q_PROPERTY(QColor highlightColor READ highlightColor WRITE setHighlightColor NOTIFY highlightColorChanged)


    // implemetation for KSyntaxHighligher properties
    void setTextDocument(QTextDocument *textDocument);
    QTextDocument *textDocument() const;

    void setQmlTextDocument(QQuickTextDocument *qmlTextDocument);
    QQuickTextDocument *qmlTextDocument() const;

    const QString definitionName();
    void setDefinitionName(const QString& definitionName);
    int definitionNumber() const;
    void setDefinitionNumber(const int definitionNumber);

    const QStringList definitionNames() const;

    const QString themeName();
    void setThemeName(const QString& themeName);
    const QString themeNameTranslated();
    int themeNumber();
    void setThemeNumber(const int themeNumber);

    const QStringList themeNames() const;
    const QStringList themeNamesTranslated() const;

    // implemetation for KSyntaxHighligher properties
    QString search() const;
    void setSearch(const QString& search);

    bool caseSensitive() const;
    void setCaseSensitive(const bool caseSensitive);

    bool wholeWords() const;
    void setWholeWords(const bool wholeWords);

    bool regExpr() const;
    void setRegExpr(const bool regExpr);

    QString replace() const;
    void setReplace(const QString& replace);

    const QColor highlightColor() const;
    void setHighlightColor(const QColor highlightColor);

signals:
    void documentChanged();
    void definitionChanged();
    void themeChanged();
    void searchParamChanged();
    void replaceChanged();
    void highlightColorChanged();

public slots:

protected:
    const QScopedPointer<KSyntaxHighlightingWrapperPrivate> d_ptr;
    KSyntaxHighlightingWrapper(KSyntaxHighlightingWrapperPrivate &dd, QObject *parent);
    Q_DECLARE_PRIVATE(KSyntaxHighlightingWrapper)
};

QT_END_NAMESPACE

#endif // KSYNTAX_HIGHLIGHTING_WRAPPER_H
