#include "textsearch_highlighter.h"
#include "textsearch_helper_p.h"
#include <QRegularExpression>

TextSearchHighlighter::TextSearchHighlighter(TextSearchHelperPrivate* privTextSearchHighlighter, QObject *parent) :
    QSyntaxHighlighter(parent),
    m_privTextSearchHighlighter(privTextSearchHighlighter)
{
}

void TextSearchHighlighter::highlightBlock(const QString &text)
{
    QTextCharFormat textCharFormat;
    textCharFormat.setBackground(QBrush(m_privTextSearchHighlighter->highlightColor()));

    // TODO
    QRegularExpression expression("\\bMy[A-Za-z]+\\b");
    QRegularExpressionMatchIterator i = expression.globalMatch(text);
    while (i.hasNext())
    {
      QRegularExpressionMatch match = i.next();
      setFormat(match.capturedStart(), match.capturedLength(), textCharFormat);
    }
}
