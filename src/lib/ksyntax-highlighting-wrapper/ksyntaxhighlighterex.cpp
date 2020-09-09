#include "ksyntaxhighlighterex.h"
#include "ksyntaxhighlightingwrapper_p.h"
#include <QTextDocument>
#include <QAbstractTextDocumentLayout>

KSyntaxHighlighterEx::KSyntaxHighlighterEx(QTextDocument *document, KSyntaxHighlightingWrapperPrivate *higlightWrapperPrivate) :
    KSyntaxHighlighting::SyntaxHighlighter(document),
    m_higlightWrapperPrivate(higlightWrapperPrivate)
{
}

KSyntaxHighlighterEx::~KSyntaxHighlighterEx()
{
}

void KSyntaxHighlighterEx::highlightBlock(const QString &text)
{
    // ksyntax-highlight
    KSyntaxHighlighting::SyntaxHighlighter::highlightBlock(text);
    QTextBlock block = currentBlock();
    //qInfo("highlightBlock line %i", block.firstLineNumber());

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
        int matchAfter = idx+l;
        if (m_higlightWrapperPrivate->wholeWords() &&
            ((idx && text.at(idx-1).isLetterOrNumber()) || (matchAfter < text.length() && text.at(matchAfter).isLetterOrNumber())))
            continue;
        // iterate all text fragments of same char format and modify those in search match
        for(auto it = block.begin(); !(it.atEnd()); ++it) {
            QTextFragment currentFragment = it.fragment();
            if (currentFragment.isValid()) {
                int fragStart = currentFragment.position() - block.position(); // relative to text-/block-start
                int fragAfter = fragStart + currentFragment.length();
                int setStart = idx > fragStart ? idx : fragStart;
                int setAfter = matchAfter < fragAfter ? matchAfter : fragAfter;
                if(setStart < setAfter) {
                    // don't ruin syntax-highlighter's changes
                    QTextCharFormat currFormat = currentFragment.charFormat();
                    currFormat.setBackground(m_searchHighlightBrush);
                    setFormat(setStart, setAfter-setStart, currFormat);
                }
            }
        }
    }
    // make sure block is repainted in QML
    document()->documentLayout()->updateBlock(block);
}

void KSyntaxHighlighterEx::newSearch()
{
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
}

void KSyntaxHighlighterEx::setVisibleArea(const int firstLine, const int lastLine)
{
}

