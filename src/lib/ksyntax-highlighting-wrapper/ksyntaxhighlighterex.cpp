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
    if(!m_searchExpression.pattern().isEmpty()) {
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

    /*for(QTextBlock block = document()->findBlockByLineNumber(m_lineHighlightedStart);
            block.isValid() && block.firstLineNumber() < m_lineHighlightedEnd;
            block = block.next()) {
        rehighlightBlock(block);
    }*/
}

void KSyntaxHighlighterEx::setVisibleArea(const int firstLine, const int lastLine)
{
    // ignore odd values during QML startup/flicker
    int lineStart = firstLine;
    if(lineStart < 0) {
        lineStart = 0;
    }
    int lineEnd = lastLine;
    if(lineEnd < lineStart) {
        return;
    }
    // This is called with high frequency. To avoid search highlighter
    // redrawing endlless, we call highlighter with spare lines to
    // redraw and check here if last notification was already matching
    // the area we have now

    int lineEndMax = lineEnd;
    if(lineEndMax < document()->lineCount()) {
        lineEndMax = document()->lineCount();
    }
    // Calc the area (minus edge) we would highlight now
    int lineNewStart = lineStart - m_extraLinesForInaccuray;
    if(lineNewStart < 0) {
        lineNewStart = 0;
    }
    int lineNewEnd = lineEnd + m_extraLinesForInaccuray;
    if(lineNewEnd > lineEndMax) {
        lineNewEnd = lineEndMax;
    }
    // re-highlight necessary?
    if(lineNewStart < m_lineHighlightedStart || lineNewEnd > m_lineHighlightedEnd) {
        // over highlight half frame above and below
        int linesVisble = lineNewEnd - lineNewStart;
        m_lineHighlightedStart = lineStart - linesVisble/2;
        if(m_lineHighlightedStart < 0) {
            m_lineHighlightedStart = 0;
        }
        m_lineHighlightedEnd = lineEnd + linesVisble/2;
        if(m_lineHighlightedEnd > lineEndMax) {
            m_lineHighlightedEnd = lineEndMax;
        }
        qInfo("Rehighlight (%i,%i)", m_lineHighlightedStart, m_lineHighlightedEnd);
        //newSearch();
    }
}

