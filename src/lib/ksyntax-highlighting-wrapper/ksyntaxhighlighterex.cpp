#include "ksyntaxhighlighterex.h"
#include "ksyntaxhighlightingwrapper_p.h"
#include <QTextDocument>

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
    if(!m_highlightSearchOnly) {
        KSyntaxHighlighting::SyntaxHighlighter::highlightBlock(text);
    }
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
        setFormat(idx, l, m_textCharFormat);
    }
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

    m_textCharFormat.setBackground(QBrush(m_higlightWrapperPrivate->highlightColor()));

    m_searchExpression.setPattern(pattern);
    m_searchExpression.setPatternOptions(options);

    rehighlight();
    m_highlightSearchOnly = false;
}

