#ifndef TEXT_SEARCH_HIGHLIGHTER_H
#define TEXT_SEARCH_HIGHLIGHTER_H

#include <QSyntaxHighlighter>

class TextSearchHelperPrivate;
class TextSearchHighlighter : public QSyntaxHighlighter
{
public:
    explicit TextSearchHighlighter(TextSearchHelperPrivate* privTextSearchHighlighter, QObject *parent = nullptr);
protected:
    void highlightBlock(const QString &text) override;

private:
    TextSearchHelperPrivate* m_privTextSearchHighlighter;
};

#endif // TEXT_SEARCH_HIGHLIGHTER_H
