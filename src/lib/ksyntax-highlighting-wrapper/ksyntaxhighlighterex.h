#ifndef KSYNTAXHIGHLIGHTEREX_H
#define KSYNTAXHIGHLIGHTEREX_H

#include <syntaxhighlighter.h>

class KSyntaxHighlightingWrapperPrivate;

class KSyntaxHighlighterEx : public KSyntaxHighlighting::SyntaxHighlighter
{
    Q_OBJECT
public:
    explicit KSyntaxHighlighterEx(QTextDocument *document, KSyntaxHighlightingWrapperPrivate* higlightWrapperPrivate);
    ~KSyntaxHighlighterEx() override;
    // make public for KSyntaxHighlightingWrapperPrivate
    void setFormat(int start, int count, const QTextCharFormat &format);
protected:
    void highlightBlock(const QString &text) override;
private:
    KSyntaxHighlightingWrapperPrivate* m_higlightWrapperPrivate;
};

#endif // KSYNTAXHIGHLIGHTEREX_H
