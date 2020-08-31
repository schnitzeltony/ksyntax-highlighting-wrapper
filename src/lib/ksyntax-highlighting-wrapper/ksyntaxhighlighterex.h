#ifndef KSYNTAXHIGHLIGHTEREX_H
#define KSYNTAXHIGHLIGHTEREX_H

#include <syntaxhighlighter.h>
#include <QRegularExpression>

class KSyntaxHighlightingWrapperPrivate;

class KSyntaxHighlighterEx : public KSyntaxHighlighting::SyntaxHighlighter
{
    Q_OBJECT
public:
    explicit KSyntaxHighlighterEx(QTextDocument *document, KSyntaxHighlightingWrapperPrivate* higlightWrapperPrivate);
    ~KSyntaxHighlighterEx() override;
    void newSearch();

protected:
    void highlightBlock(const QString &text) override;

private:
    KSyntaxHighlightingWrapperPrivate* m_higlightWrapperPrivate;
    bool m_highlightSearchOnly;
    QTextCharFormat m_textCharFormat;
    QRegularExpression m_searchExpression;
};

#endif // KSYNTAXHIGHLIGHTEREX_H
