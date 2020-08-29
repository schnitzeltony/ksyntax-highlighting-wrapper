#ifndef TEXT_SEARCH_HELPER_H
#define TEXT_SEARCH_HELPER_H

#include "textareaenhanced_export.h"
#include <QObject>
#include <QColor>

QT_BEGIN_NAMESPACE

// forward
class QQmlEngine;
class QQuickTextDocument;
class QTextDocument;

// clang: be quiet
QT_WARNING_DISABLE_CLANG("-Wshadow-field")

class TextSearchHelperPrivate;
class TEXTAREAENHANCED_EXPORT TextSearchHelper : public QObject
{
  Q_OBJECT
public:
    explicit TextSearchHelper(QObject *parent = nullptr);
    virtual ~TextSearchHelper();

    // As name suggests: register for QML usage
    static void registerQml();

    // document
    Q_PROPERTY(QTextDocument *textDocument READ textDocument WRITE setTextDocument NOTIFY documentChanged)
    Q_PROPERTY(QQuickTextDocument *qmlTextDocument READ qmlTextDocument WRITE setQmlTextDocument NOTIFY documentChanged)

    // search params
    Q_PROPERTY(QString search READ search WRITE setSearch NOTIFY searchParamChanged)
    Q_PROPERTY(bool caseSensitive READ caseSensitive WRITE setCaseSensitive NOTIFY searchParamChanged)
    Q_PROPERTY(bool wholeWords READ wholeWords WRITE setWholeWords NOTIFY searchParamChanged)

    // search methods
    Q_INVOKABLE void findPrevious();
    Q_INVOKABLE void findNext();

    // replace
    Q_PROPERTY(QString replace READ replace WRITE setReplace NOTIFY replaceChanged)

    // higlight color
    Q_PROPERTY(QColor highlightColor READ highlightColor WRITE setHighlightColor NOTIFY highlightColorChanged)

    // implemetation for properties
    void setTextDocument(QTextDocument *textDocument);
    QTextDocument *textDocument() const;

    void setQmlTextDocument(QQuickTextDocument *qmlTextDocument);
    QQuickTextDocument *qmlTextDocument() const;

    QString search() const;
    void setSearch(const QString& search);

    bool caseSensitive() const;
    void setCaseSensitive(const bool caseSensitive);

    bool wholeWords() const;
    void setWholeWords(const bool wholeWords);

    QString replace() const;
    void setReplace(const QString& replace);

    const QColor highlightColor();
    void setHighlightColor(const QColor highlightColor);

signals:
    void documentChanged();
    void searchParamChanged();
    void replaceChanged();
    void highlightColorChanged();

protected:
    const QScopedPointer<TextSearchHelperPrivate> d_ptr;
    TextSearchHelper(TextSearchHelperPrivate &dd, QObject *parent);
    Q_DECLARE_PRIVATE(TextSearchHelper)
};

QT_END_NAMESPACE

#endif // TEXT_SEARCH_HELPER_H
