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

void KSyntaxHighlighterEx::rehighlightBlock(const QTextBlock &block)
{
    Q_UNUSED(block)
    /*
    For unknown reasons:
    * rehighlightBlock is TERRIBLY slow in this (QML) environment. We learned
      that already when trying to make our search highlighter faster.
    * When changing definition, KDE's syntax-highligher seems to call
      rehighlightBlock for each char in the document and that takes ages..

    Interesting:
    * When setting up definition / pasting huge document rehighlightBlock is
      not called at all. Pasting first and change definition causes disaster
      mentioned above.
    * Whatever contents we tested so far - turning rehighlightBlock to no-op
      does not seem to cause any difference...

    Additional note:
    * We cannot enable this workaround for a limited time of changing
     definition: KDE's syntax-highlighter uses QMetaObject::invokeMethod with
      Qt::QueuedConnection for a deferred call of rehighlightBlock.
    */
}

