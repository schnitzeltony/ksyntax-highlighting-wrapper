#ifndef KSYNTAX_HIGHLIGHTING_WRAPPER_P_H
#define KSYNTAX_HIGHLIGHTING_WRAPPER_P_H

#include "ksyntaxhighlightingwrapper.h"
#include <repository.h>
#include <syntaxhighlighter.h>
#include <definition.h>
#include <theme.h>

class KSyntaxHighlightingWrapperPrivate
{
     Q_DECLARE_PUBLIC(KSyntaxHighlightingWrapper)
public:
    KSyntaxHighlightingWrapperPrivate(KSyntaxHighlightingWrapper* pPublic);
    virtual ~KSyntaxHighlightingWrapperPrivate();

    bool setTextDocument(QTextDocument *textDocument);

    bool setDefinition(KSyntaxHighlighting::Definition def);
    KSyntaxHighlighting::Definition definition();

    bool setTheme(KSyntaxHighlighting::Theme theme);
    KSyntaxHighlighting::Theme theme();

    static KSyntaxHighlighting::Repository m_repository;
    KSyntaxHighlighting::SyntaxHighlighter *m_highlighter;

    // Just in case...
    QQuickTextDocument *m_quickTextDocument;

    // m_highlighter has them but m_highlighter can be nullptr
    // so keep these locally
    KSyntaxHighlighting::Definition m_currentDefinition;
    KSyntaxHighlighting::Theme m_currentTheme;

    KSyntaxHighlightingWrapper *q_ptr;
};


#endif // KSYNTAX_HIGHLIGHTING_WRAPPER_P_H
