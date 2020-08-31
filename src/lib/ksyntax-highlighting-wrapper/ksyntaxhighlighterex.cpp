#include "ksyntaxhighlighterex.h"
#include "ksyntaxhighlightingwrapper_p.h"
#include <QTextDocument>
#include <QAbstractTextDocumentLayout>

KSyntaxHighlighterEx::KSyntaxHighlighterEx(QTextDocument *document, KSyntaxHighlightingWrapperPrivate *higlightWrapperPrivate) :
    KSyntaxHighlighting::SyntaxHighlighter(document),
    m_higlightWrapperPrivate(higlightWrapperPrivate),
    m_highlightSearchOnly(false)
{
}

KSyntaxHighlighterEx::~KSyntaxHighlighterEx()
{
}

void KSyntaxHighlighterEx::highlightBlock(const QString &text)
{
    // ksyntax-highlight
    KSyntaxHighlighting::SyntaxHighlighter::highlightBlock(text);

    // search-highlight
    // stolen from qt-creator / texteditor.cpp
    int idx = -1;
    int l = 1;
    while (idx < text.length()) {
        const QRegularExpressionMatch match = m_searchExpression.match(text, idx + 1);
        if (!match.hasMatch())
            break;
        idx = match.capturedStart();
        l = match.capturedLength();
        if (l == 0)
            break;
        if (m_higlightWrapperPrivate->wholeWords() &&
            ((idx && text.at(idx-1).isLetterOrNumber()) || (idx + l < text.length() && text.at(idx + l).isLetterOrNumber())))
            continue;
        for(int idxHighlight=idx; idxHighlight<idx+l; ++idxHighlight) {
            // don't ruin what KSyntaxHighlighting has done
            QTextCharFormat currFormat = format(idxHighlight);
            currFormat.setBackground(m_searchHighlightBrush);
            setFormat(idx, l, currFormat);
        }
    }
    // make sure block is repainted in QML
    document()->documentLayout()->updateBlock(currentBlock());
}

void KSyntaxHighlighterEx::newSearch()
{
    m_highlightSearchOnly = true;
    // stolen from qt-creator / texteditor.cpp
    const QString pattern = m_higlightWrapperPrivate->regExpr() ?
                m_higlightWrapperPrivate->search() :
                QRegularExpression::escape(m_higlightWrapperPrivate->search());
    const QRegularExpression::PatternOptions options = m_higlightWrapperPrivate->caseSensitive() ?
                QRegularExpression::NoPatternOption :
                QRegularExpression::CaseInsensitiveOption;

    m_searchExpression.setPattern(pattern);
    m_searchExpression.setPatternOptions(options);

    m_searchHighlightBrush = QBrush(m_higlightWrapperPrivate->highlightColor());

    // TODO this needs optimization!!!
    rehighlight();
    m_highlightSearchOnly = false;
}

