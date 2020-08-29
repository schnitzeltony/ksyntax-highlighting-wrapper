#ifndef TEXT_SEARCH_HELPER_P_H
#define TEXT_SEARCH_HELPER_P_H

#include "textsearch_helper.h"
#include "textsearch_highlighter.h"

class TextSearchHelperPrivate
{
    Q_DECLARE_PUBLIC(TextSearchHelper)
public:
    virtual ~TextSearchHelperPrivate();
    TextSearchHelperPrivate(TextSearchHelper* pPublic);

    bool setTextDocument(QTextDocument *textDocument);
    QTextDocument *textDocument() const;
    bool setQmlTextDocument(QQuickTextDocument *qmlTextDocument);
    QQuickTextDocument *qmlTextDocument() const;

    QString search() const;
    bool setSearch(const QString& search);

    bool caseSensitive() const;
    bool setCaseSensitive(const bool caseSensitive);

    bool wholeWords() const;
    bool setWholeWords(const bool wholeWords);

    QString replace() const;
    bool setReplace(const QString& replace);

    const QColor highlightColor();
    bool setHighlightColor(const QColor highlightColor);

private:
    QQuickTextDocument *m_quickTextDocument;
    QTextDocument *m_textDocument;
    QString m_search;
    bool m_caseSensitive;
    bool m_wholeWords;
    QColor m_highlightColor;
    QString m_replace;

    TextSearchHighlighter* m_highlighter;
    TextSearchHelper *q_ptr;
};

#endif // TEXT_SEARCH_HELPER_P_H
