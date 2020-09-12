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

    m_higlightWrapperPrivate->highlightBlockSearch(text, block, true);
    // make sure block is repainted in QML
    document()->documentLayout()->updateBlock(block);
}

void KSyntaxHighlighterEx::setFormat(int start, int count, const QTextCharFormat &format)
{
    return KSyntaxHighlighting::SyntaxHighlighter::setFormat(start, count, format);
}

