#ifndef KSYNTAX_HIGHLIGHTING_WRAPPER_P_H
#define KSYNTAX_HIGHLIGHTING_WRAPPER_P_H

#include "ksyntaxhighlightingwrapper.h"
#include "ksyntaxhighlighterex.h"
#include <repository.h>
#include <definition.h>
#include <theme.h>

class KSyntaxHighlightingWrapperPrivate
{
    Q_DECLARE_PUBLIC(KSyntaxHighlightingWrapper)
public:
    KSyntaxHighlightingWrapperPrivate(KSyntaxHighlightingWrapper* pPublic);
    virtual ~KSyntaxHighlightingWrapperPrivate();

    // Q(Quick)TextDocument access
    bool setTextDocument(QTextDocument *textDocument);
    QTextDocument *textDocument() const;
    bool setQmlTextDocument(QQuickTextDocument *qmlTextDocument);
    QQuickTextDocument *qmlTextDocument() const;

    // Definition access
    bool setDefinition(KSyntaxHighlighting::Definition def);
    KSyntaxHighlighting::Definition definition();
    bool setDefinitionForFileName(const QString &fileName);
    bool setDefinitionForMimeType(const QString &mimeType);
    const QString definitionName();
    bool setDefinitionName(const QString& definitionName);
    const QStringList definitionNames() const;
    int definitionNumber();
    bool setDefinitionNumber(const int definitionNumber);

    // Theme access
    KSyntaxHighlighting::Theme theme();
    bool setTheme(KSyntaxHighlighting::Theme theme);
    const QString themeName();
    bool setThemeName(const QString& themeName);
    int themeNumber();
    bool setThemeNumber(const int themeNumber);
    const QStringList themeNames() const;
    const QStringList themeNamesTranslated() const;

    // Search param access
    QString search() const;
    bool setSearch(const QString& search);

    bool caseSensitive() const;
    bool setCaseSensitive(const bool caseSensitive);

    bool wholeWords() const;
    bool setWholeWords(const bool wholeWords);

    bool regExpr() const;
    bool setRegExpr(const bool regExpr);

    QString replace() const;
    bool setReplace(const QString& replace);

    QColor highlightColor() const;
    bool setHighlightColor(const QColor highlightColor);

    void setVisibleArea(const int firstLine, const int lastLine);
private:
    KSyntaxHighlighterEx *m_highlighter;

    // We have to keep for getter
    QQuickTextDocument *m_quickTextDocument;
    // For brute force redraw hack
    QTextDocument *m_textDocument;

    // m_highlighter has them but m_highlighter can be nullptr
    // so keep these locally. We keep default from KSyntaxHighlighting:
    // without beeing set they are invalid. Maybe we have to change that..
    KSyntaxHighlighting::Definition m_currentDefinition;
    KSyntaxHighlighting::Theme m_currentTheme;  // default is invalid see theme.h

    // search params
    QString m_search;
    bool m_caseSensitive;
    bool m_wholeWords;
    bool m_regExpr;

    // search params
    QColor m_highlightColor;
    QString m_replace;

    // visible area
    int m_firstLine;
    int m_lastLine;

    KSyntaxHighlightingWrapper *q_ptr;
};


#endif // KSYNTAX_HIGHLIGHTING_WRAPPER_P_H
